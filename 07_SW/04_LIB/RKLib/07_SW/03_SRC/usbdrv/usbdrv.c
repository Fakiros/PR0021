/* Name: usbdrv.c
 * Project: AVR USB driver
 * Author: Christian Starkjohann
 * Creation Date: 2004-12-29
 * Tabsize: 4
 * Copyright: (c) 2005 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: usbdrv.c 135 2006-02-20 13:59:06Z cs $
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../usbdrv.h"

/*
General Description:
This module implements the C-part of the USB driver. See usbdrv.h for a
documentation of the entire driver.
*/

/* ------------------------------------------------------------------------- */

/* raw USB registers / interface to assembler code: */
/* USBRDV_RxBuf MUST be in 1 byte addressable range (because USBRDV_InputBuf is only 1 byte) */
uchar       USBRDV_RxBuf[2][USBRDV_BUFSIZE] __attribute__ ((section (USBRDV_BUFFER_SECTION)));/* raw RX buffer: PID, 8 bytes data, 2 bytes CRC */
uchar       USBRDV_DeviceId;        /* assigned during enumeration, defaults to 0 */
uchar       USBRDV_Configuration;   /* currently selected configuration. Administered by driver, but not used */
uchar       USBRDV_InputBuf;        /* ptr to raw buffer used for receiving */
uchar       USBRDV_AppBuf;          /* ptr to raw buffer passed to app for processing */
volatile char USBRDV_RxLen;         /* = 0; number of bytes in USBRDV_AppBuf; 0 means free */
uchar       USBRDV_CurrentTok;      /* last token received */
uchar       USBRDV_RxToken;         /* token for data we received */
uchar       USBRDV_MsgLen = 0xff;   /* remaining number of bytes, no msg to send if -1 (see USBRDV_MsgPtr) */
volatile char USBRDV_TxLen = -1;    /* number of bytes to transmit with next IN token */
uchar       USBRDV_TxBuf[USBRDV_BUFSIZE];/* data to transmit with next IN, free if USBRDV_TxLen == -1 */
#if USBRDV_CFG_HAVE_INTRIN_ENDPOINT
/* uchar       USBRDV_RxEndp;          endpoint which was addressed (1 bit in MSB) [not impl] */
volatile char USBRDV_TxLen1 = -1;   /* TX count for endpoint 1 */
uchar       USBRDV_TxBuf1[USBRDV_BUFSIZE];/* TX data for endpoint 1 */
#endif
uchar       USBRDV_AckBuf[1] = {USBPID_ACK};    /* transmit buffer for ack tokens */
uchar       USBRDV_NakBuf[1] = {USBPID_NAK};    /* transmit buffer for nak tokens */

/* USB status registers / not shared with asm code */
uchar           *USBRDV_MsgPtr;     /* data to transmit next -- ROM or RAM address */
static uchar    USBRDV_MsgFlags;    /* flag values see below */
static uchar    USBRDV_NewDeviceId; /* = 0; device ID which should be set after status phase */
static uchar    USBRDV_IsReset;     /* = 0; USB bus is in reset phase */

#define USBRDV_FLG_TX_PACKET       (1<<0)
/* Leave free 6 bits after TX_PACKET. This way we can increment USBRDV_MsgFlags to toggle TX_PACKET */
#define USBRDV_FLG_MSGPTR_IS_ROM   (1<<6)
#define USBRDV_FLG_USE_DEFAULT_RW  (1<<7)

/*
optimizing hints:
- do not post/pre inc/dec integer values in operations
- assign value of PRG_RDB() to register variables and don't use side effects in arg
- use narrow scope for variables which should be in X/Y/Z register
- assign char sized expressions to variables to force 8 bit arithmetics
*/

/* ------------------------------------------------------------------------- */

static char USBRDV_DescrDevice[] PROGMEM = {    /* USB device descriptor */
    18,         /* sizeof(USBRDV_DescrDevice): length of descriptor in bytes */
    USBDESCR_DEVICE,    /* descriptor type */
    0x01, 0x01, /* USB version supported */
    USBRDV_CFG_DEVICE_CLASS,
    USBRDV_CFG_DEVICE_SUBCLASS,
    0,          /* protocol */
    8,          /* max packet size */
    USBRDV_CFG_VENDOR_ID,  /* 2 bytes */
    USBRDV_CFG_DEVICE_ID,  /* 2 bytes */
    USBRDV_CFG_DEVICE_VERSION, /* 2 bytes */
#if USBRDV_CFG_VENDOR_NAME_LEN
    1,          /* manufacturer string index */
#else
    0,          /* manufacturer string index */
#endif
#if USBRDV_CFG_DEVICE_NAME_LEN
    2,          /* product string index */
#else
    0,          /* product string index */
#endif
#if USBRDV_CFG_SERIAL_NUMBER_LENGTH
    3,          /* serial number string index */
#else
    0,          /* serial number string index */
#endif
    1,          /* number of configurations */
};

static char USBRDV_DescrConfig[] PROGMEM = {    /* USB configuration descriptor */
    9,          /* sizeof(USBRDV_DescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    (18 + 7 * USBRDV_CFG_HAVE_INTRIN_ENDPOINT
#if USBRDV_CFG_HID_REPORT_DESCRIPTOR_LENGTH
     + 9
#endif
        ), 0,   /* total length of data returned (including inlined descriptors) */
    1,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USBRDV_CFG_IS_SELF_POWERED
    USBATTR_SELFPOWER,  /* attributes */
#else
    USBATTR_BUSPOWER,   /* attributes */
#endif
    USBRDV_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */
/* interface descriptor follows inline: */
    9,          /* sizeof(USBRDV_DescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USBRDV_CFG_HAVE_INTRIN_ENDPOINT,   /* endpoints excl 0: number of endpoint descriptors to follow */
    USBRDV_CFG_INTERFACE_CLASS,
    USBRDV_CFG_INTERFACE_SUBCLASS,
    USBRDV_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */
#if USBRDV_CFG_HID_REPORT_DESCRIPTOR_LENGTH    /* HID descriptor */
    9,          /* sizeof(USBRDV_DescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    USBRDV_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  /* total length of report descriptor */
#endif
#if USBRDV_CFG_HAVE_INTRIN_ENDPOINT    /* endpoint descriptor for endpoint 1 */
    7,          /* sizeof(USBRDV_DescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x81,       /* IN endpoint number 1 */
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USBRDV_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif
};

static char USBRDV_DescrString0[] PROGMEM = {   /* language descriptor */
    4,          /* sizeof(USBRDV_DescrString0): length of descriptor in bytes */
    3,          /* descriptor type */
    0x09, 0x04, /* language index (0x0409 = US-English) */
};

#if USBRDV_CFG_VENDOR_NAME_LEN
static int  USBRDV_DescrString1[] PROGMEM = {
    USBRDV_STRING_DESCRIPTOR_HEADER(USBRDV_CFG_VENDOR_NAME_LEN),
    USBRDV_CFG_VENDOR_NAME
};
#endif
#if USBRDV_CFG_DEVICE_NAME_LEN
static int  USBRDV_DescrString2[] PROGMEM = {
    USBRDV_STRING_DESCRIPTOR_HEADER(USBRDV_CFG_DEVICE_NAME_LEN),
    USBRDV_CFG_DEVICE_NAME
};
#endif

/* We don't use prog_int or prog_int16_t for compatibility with various libc
 * versions. Here's an other compatibility hack:
 */
#ifndef PRG_RDB
#define PRG_RDB(addr)   pgm_read_byte(addr)
#endif

typedef union{
    unsigned    word;
    uchar       *ptr;
    uchar       bytes[2];
}converter_t;
/* We use this union to do type conversions. This is better optimized than
 * type casts in gcc 3.4.3 and much better than using bit shifts to build
 * ints from chars. Byte ordering is not a problem on an 8 bit platform.
 */

/* ------------------------------------------------------------------------- */

#if USBRDV_CFG_HAVE_INTRIN_ENDPOINT
static uchar    USBRDV_TxPacketCnt1;
#if USBRDV_CFG_IMPLEMENT_HALT
static uchar    USBRDV_Halted1;         /* not 0 if endpoint 1 is halted */
#endif

void    USBRDV_SetInterrupt(uchar *data, uchar len)
{
uchar       *p, i;

#if USBRDV_CFG_IMPLEMENT_HALT
    if(USBRDV_Halted1)
        return;
#endif
    if(len > 8) /* interrupt transfers are limited to 8 bytes */
        len = 8;
    i = USBPID_DATA1;
    if(USBRDV_TxPacketCnt1 & 1)
        i = USBPID_DATA0;
    if(USBRDV_TxLen1 < 0){      /* packet buffer was empty */
        USBRDV_TxPacketCnt1++;
    }else{
        USBRDV_TxLen1 = -1;     /* avoid sending incomplete interrupt data */
    }
    p = USBRDV_TxBuf1;
    *p++ = i;
    for(i=len;i--;)
        *p++ = *data++;
    USBRDV_Crc16Append(&USBRDV_TxBuf1[1], len);
    USBRDV_TxLen1 = len + 4;    /* len must be given including sync byte */
#if DEBUG_LEVEL > 1
    DBG2(0x21, USBRDV_TxBuf1, USBRDV_TxLen1-1);
#else
//    DBG1(0x21, USBRDV_TxBuf1 + 1, 2);
#endif
}
#endif


static uchar    USBRDV_Read(uchar *data, uchar len)
{
#if USBRDV_CFG_IMPLEMENT_FN_READ
    if(USBRDV_MsgFlags & USBRDV_FLG_USE_DEFAULT_RW){
#endif
        uchar i = len, *r = USBRDV_MsgPtr;
        if(USBRDV_MsgFlags & USBRDV_FLG_MSGPTR_IS_ROM){    /* ROM data */
            while(i--){
                char c = PRG_RDB(r);    /* assign to char size variable to enforce byte ops */
                *data++ = c;
                r++;
            }
        }else{                  /* RAM data */
            while(i--)
                *data++ = *r++;
        }
        USBRDV_MsgPtr = r;
        return len;
#if USBRDV_CFG_IMPLEMENT_FN_READ
    }else{
        if(len != 0)    /* don't bother app with 0 sized reads */
            return USBRDV_FunctionRead(data, len);
        return 0;
    }
#endif
}

/* Don't make this function static to avoid inlining.
 * The entire function would become too large and exceed the range of
 * relative jumps.
 * 2006-02-25: Either gcc 3.4.3 is better than the gcc used when the comment
 * above was written, or other parts of the code have changed. We now get
 * better results with an inlined function. Test condition: PowerSwitch code.
 */
static void USBRDV_ProcessRx(uchar *data, uchar len)
{
USBRDV_Request_t    *rq = (void *)data;
uchar           replyLen = 0, flags = USBRDV_FLG_USE_DEFAULT_RW;
/* We use if() cascades because the compare is done byte-wise while switch()
 * is int-based. The if() cascades are therefore more efficient.
 */
#if DEBUG_LEVEL > 1
//    DBG2(0x10 + (USBRDV_RxToken == (uchar)USBPID_SETUP), data, len);
#else
//    DBG1(0x10 + (USBRDV_RxToken == (uchar)USBPID_SETUP), data, 2);
#endif
    if(USBRDV_RxToken == (uchar)USBPID_SETUP){
        if(len == 8){   /* Setup size must be always 8 bytes. Ignore otherwise. */
            uchar type = rq->bmRequestType & USBRQ_TYPE_MASK;
            if(type == USBRQ_TYPE_STANDARD){
                uchar *replyData = USBRDV_TxBuf + 9; /* there is 3 bytes free space at the end of the buffer */
                replyData[0] = 0;   /* common to USBRQ_GET_STATUS and USBRQ_GET_INTERFACE */
                if(rq->bRequest == USBRQ_GET_STATUS){           /* 0 */
                    uchar __attribute__((__unused__)) recipient = rq->bmRequestType & USBRQ_RCPT_MASK;  /* assign arith ops to variables to enforce byte size */
#if USBRDV_CFG_IS_SELF_POWERED
                    if(recipient == USBRQ_RCPT_DEVICE)
                        replyData[0] =  USBRDV_CFG_IS_SELF_POWERED;
#endif
#if USBRDV_CFG_HAVE_INTRIN_ENDPOINT && USBRDV_CFG_IMPLEMENT_HALT
                    if(USBRDV_Halted1 && recipient == USBRQ_RCPT_ENDPOINT && rq->wIndex.bytes[0] == 0x81)   /* request status for endpoint 1 */
                        replyData[0] = 1;
#endif
                    replyData[1] = 0;
                    replyLen = 2;
                }else if(rq->bRequest == USBRQ_SET_ADDRESS){    /* 5 */
                    USBRDV_NewDeviceId = rq->wValue.bytes[0];
                }else if(rq->bRequest == USBRQ_GET_DESCRIPTOR){ /* 6 */
                    flags = USBRDV_FLG_MSGPTR_IS_ROM | USBRDV_FLG_USE_DEFAULT_RW;
                    if(rq->wValue.bytes[1] == 1){   /* descriptor type requested */
                        replyLen = sizeof(USBRDV_DescrDevice);
                        replyData = (uchar *)USBRDV_DescrDevice;
                    }else if(rq->wValue.bytes[1] == 2){
                        replyLen = sizeof(USBRDV_DescrConfig);
                        replyData = (uchar *)USBRDV_DescrConfig;
                    }else if(rq->wValue.bytes[1] == 3){ /* string descriptor */
                        if(rq->wValue.bytes[0] == 0){   /* descriptor index */
                            replyLen = sizeof(USBRDV_DescrString0);
                            replyData = (uchar *)USBRDV_DescrString0;
#if USBRDV_CFG_VENDOR_NAME_LEN
                        }else if(rq->wValue.bytes[0] == 1){
                            replyLen = sizeof(USBRDV_DescrString1);
                            replyData = (uchar *)USBRDV_DescrString1;
#endif
#if USBRDV_CFG_DEVICE_NAME_LEN
                        }else if(rq->wValue.bytes[0] == 2){
                            replyLen = sizeof(USBRDV_DescrString2);
                            replyData = (uchar *)USBRDV_DescrString2;
#endif
#if USBRDV_CFG_SERIAL_NUMBER_LENGTH
                        }else if(rq->wValue.bytes[0] == 3){
                            replyLen = 2 * USBRDV_CFG_SERIAL_NUMBER_LENGTH + 2;
                            replyData = (uchar *)USBRDV_CfgSerialNumberStringDescriptor;
#endif
                        }
                    }
#if USBRDV_CFG_HID_REPORT_DESCRIPTOR_LENGTH
                    else if(rq->wValue.bytes[1] == USBDESCR_HID){           /* 0x21 */
                        replyLen = 9;
                        replyData = (uchar *)USBRDV_DescrConfig + 18;
                    }else if(rq->wValue.bytes[1] == USBDESCR_HID_REPORT){   /* 0x22 */
                        replyLen = USBRDV_CFG_HID_REPORT_DESCRIPTOR_LENGTH;
                        replyData = (uchar *)USBRDV_HidReportDescriptor;
                    }
#endif
                }else if(rq->bRequest == USBRQ_GET_CONFIGURATION){  /* 8 */
                    replyLen = 1;
                    replyData = &USBRDV_Configuration;  /* send current configuration value */
                }else if(rq->bRequest == USBRQ_SET_CONFIGURATION){  /* 9 */
                    USBRDV_Configuration = rq->wValue.bytes[0];
#if USBRDV_CFG_IMPLEMENT_HALT
                    USBRDV_Halted1 = 0;
#endif
                }else if(rq->bRequest == USBRQ_GET_INTERFACE){      /* 10 */
                    replyLen = 1;
#if USBRDV_CFG_HAVE_INTRIN_ENDPOINT
#if USBRDV_CFG_IMPLEMENT_HALT
                }else if(rq->bRequest == USBRQ_CLEAR_FEATURE || rq->bRequest == USBRQ_SET_FEATURE){   /* 1|3 */
                    if(rq->wValue.bytes[0] == 0 && rq->wIndex.bytes[0] == 0x81){   /* feature 0 == HALT for endpoint == 1 */
                        USBRDV_Halted1 = rq->bRequest - 1;
                        if(USBRDV_Halted1){
                            USBRDV_TxBuf1[0] = USBPID_STALL;
                            USBRDV_TxLen1 = 2;      /* length including sync byte */
                        }
                        USBRDV_TxPacketCnt1 = 0;    /* reset data toggling for interrupt endpoint */
                    }
#endif
                }else if(rq->bRequest == USBRQ_SET_INTERFACE){      /* 11 */
                    USBRDV_TxPacketCnt1 = 0;        /* reset data toggling for interrupt endpoint */
#if USBRDV_CFG_IMPLEMENT_HALT
                    USBRDV_Halted1 = 0;
#endif
#endif
                }else{
                    /* the following requests can be ignored, send default reply */
                    /* 1: CLEAR_FEATURE, 3: SET_FEATURE, 7: SET_DESCRIPTOR */
                    /* 12: SYNCH_FRAME */
                }
                USBRDV_MsgPtr = replyData;
                if(!rq->wLength.bytes[1] && replyLen > rq->wLength.bytes[0])  /* max length is in */
                    replyLen = rq->wLength.bytes[0];
            }else{  /* not a standard request -- must be vendor or class request */
                replyLen = USBRDV_FunctionSetup(data);
#if USBRDV_CFG_IMPLEMENT_FN_READ || USBRDV_CFG_IMPLEMENT_FN_WRITE
                if(replyLen == 0xff){   /* use user-supplied read/write function */
                    if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_DEVICE_TO_HOST){
                        replyLen = rq->wLength.bytes[0];    /* IN transfers only */
                    }
                    flags = 0;  /* we have no valid msg, use user supplied read/write functions */
                }
#endif
            }
        }
        /* make sure that data packets which are sent as ACK to an OUT transfer are always zero sized */
    }else{  /* DATA packet from out request */
#if USBRDV_CFG_IMPLEMENT_FN_WRITE
        if(!(USBRDV_MsgFlags & USBRDV_FLG_USE_DEFAULT_RW)){
            uchar rval = USBRDV_FunctionWrite(data, len);
            replyLen = -1;
            if(rval == 0xff){       /* an error occurred */
                /* USBRDV_MsgLen = 0xff; cancel potentially pending ACK [has been done by ASM module when OUT token arrived] */
                USBRDV_TxBuf[0] = USBPID_STALL;
                USBRDV_TxLen = 2;       /* length including sync byte */
            }else if(rval != 0){    /* This was the final package */
                replyLen = 0;       /* answer with a zero-sized data packet */
            }
            flags = 0;    /* start with a DATA1 package, stay with user supplied write() function */
        }
#else
        replyLen = 0;      /* send zero-sized block as ACK */
#endif
    }
    USBRDV_MsgFlags = flags;
    USBRDV_MsgLen = replyLen;
}

/* ------------------------------------------------------------------------- */

static void USBRDV_BuildTxBlock(void)
{
uchar       wantLen, len, txLen, token;

    wantLen = USBRDV_MsgLen;
    if(wantLen > 8)
        wantLen = 8;
    USBRDV_MsgLen -= wantLen;
    token = USBPID_DATA1;
    if(USBRDV_MsgFlags & USBRDV_FLG_TX_PACKET)
        token = USBPID_DATA0;
    USBRDV_MsgFlags++;
    len = USBRDV_Read(USBRDV_TxBuf + 1, wantLen);
    if(len <= 8){           /* valid data packet */
        USBRDV_Crc16Append(USBRDV_TxBuf + 1, len);
        txLen = len + 4;    /* length including sync byte */
        if(len < 8)         /* a partial package identifies end of message */
            USBRDV_MsgLen = 0xff;
    }else{
        token = USBPID_STALL;
        txLen = 2;          /* length including sync byte */
        USBRDV_MsgLen = 0xff;
    }
    USBRDV_TxBuf[0] = token;
    USBRDV_TxLen = txLen;
#if DEBUG_LEVEL > 1
    DBG2(0x20, USBRDV_TxBuf, txLen-1);
#else
//    DBG1(0x20, USBRDV_TxBuf + 1, 2);
#endif
}

static inline uchar isNotSE0(void)
{
uchar   rval;
/* We want to do
 *     return (USBIN & USBMASK);
 * here, but the compiler does int-expansion acrobatics.
 * We can avoid this by assigning to a char-sized variable.
 */
    rval = USBIN & USBMASK;
    return rval;
}

/* ------------------------------------------------------------------------- */

void    USBRDV_Poll(void)
{
uchar   len;

    if((len = USBRDV_RxLen) > 0){
/* We could check CRC16 here -- but ACK has already been sent anyway. If you
 * need data integrity checks with this driver, check the CRC in your app
 * code and report errors back to the host. Since the ACK was already sent,
 * retries must be handled on application level.
 * unsigned crc = USBRDV_Crc16((uchar *)(unsigned)(USBRDV_AppBuf + 1), USBRDV_RxLen - 3);
 */
        len -= 3;       /* remove PID and CRC */
        if(len < 128){  /* no overflow */
            converter_t appBuf;
            appBuf.ptr = (uchar *)USBRDV_RxBuf;
            appBuf.bytes[0] = USBRDV_AppBuf;
            appBuf.bytes[0]++;
            USBRDV_ProcessRx(appBuf.ptr, len);
        }
        USBRDV_RxLen = 0;   /* mark rx buffer as available */
    }
    if(USBRDV_TxLen < 0){   /* TX system is idle */
        if(USBRDV_MsgLen != 0xff){
            USBRDV_BuildTxBlock();
        }else if(USBRDV_NewDeviceId){
            USBRDV_DeviceId = USBRDV_NewDeviceId;
//            DBG1(1, &USBRDV_NewDeviceId, 1);
            USBRDV_NewDeviceId = 0;
        }
    }
    if(isNotSE0()){ /* SE0 state */
        USBRDV_IsReset = 0;
    }else{
        /* check whether SE0 lasts for more than 2.5us (3.75 bit times) */
        if(!USBRDV_IsReset){
            uchar i;
            for(i=100;i;i--){
                if(isNotSE0())
                    goto notUsbReset;
            }
            USBRDV_IsReset = 1;
            USBRDV_DeviceId = 0;
            USBRDV_NewDeviceId = 0;
#if USBRDV_CFG_IMPLEMENT_HALT
            USBRDV_Halted1 = 0;
#endif
//            DBG1(0xff, 0, 0);
notUsbReset:;
        }
    }
}

/* ------------------------------------------------------------------------- */

void    USBRDV_init(void)
{
    USBRDV_InputBuf = (uchar)USBRDV_RxBuf[0];
    USBRDV_AppBuf = (uchar)USBRDV_RxBuf[1];
#if USBRDV_INTR_CFG_SET != 0
    USBRDV_INTR_CFG |= USBRDV_INTR_CFG_SET;
#endif
#if USBRDV_INTR_CFG_CLR != 0
    USBRDV_INTR_CFG &= ~(USBRDV_INTR_CFG_CLR);
#endif
    USBRDV_INTR_ENABLE |= (1 << USBRDV_INTR_ENABLE_BIT);
}

/* ------------------------------------------------------------------------- */
