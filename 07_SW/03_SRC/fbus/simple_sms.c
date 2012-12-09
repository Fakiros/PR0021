/**************************************************************************

  Nokia 61xx SMS send/receive functions

  Version 1.0

  Justin Karneges
  infiniti@affinix.com
  June 2000

  Thanks goes to the gnokii team for figuring out the frame format, among
  many other things!  These guys did all the hard work.  Although this file
  was written from scratch, it was heavily based on their research.

  http://www.gnokii.org/

  Overview:

    This file contains a set of functions for sending and receiving SMS
    (short message service) text messages across a GSM network with a
    Nokia 61xx series phone.  I've only tested it with my phone (a 6190),
    however it should work fine with any GSM phone in the series.  It
    should also work with the 59xx series phones.

    These functions were meant to be as portable as possible (they contain
    no serial communications code), and to perform two simple things: send
    and receive SMS.  That's all.  If you want a full program to control
    all the aspects of the Nokia 61xx, then I suggest checking out gnokii.
    This code was meant to be small, so that it could be used in small
    devices/applications.


  To use:

    Setup the serial port and call n61_init().  Then use n61_smssend() and
    n61_smsrecv() to send/recv SMS messages.  Call n61_update() as often
    as possible (possibly by having a timer ISR call it) so that the driver
    stays in sync with the phone.  All incoming SMS messages are deleted
    from the phone's inbox (you want this).


  ------------------------------------------------------------------------
  Functions:

    +---------------------------------------------------------+
    |  void n61_init(int (*func)(int cmd, unsigned char c));  |
    +---------------------------------------------------------+

      Before calling this function, do everything necessary to have the
      serial port ready.  The port MUST be set as follows:

        Baud rate: 115200
        Parity: None
        Bits: 8
        Stop bits: 1
        DTR: set
        RTS: cleared

      Since this driver does not contain any serial communications
      functionality, you must supply it.  An interrupt driven / threaded
      serial interface MUST be used.  This driver does not poll the port,
      and so it is completely dependant on a background serial driver
      keeping a queue of all incoming data (so that no data is lost).

      To give the serial functionality to this nokia driver, write a simple
      function to perform the four types of requests that this driver will
      need handled.  Pass this function as the argument to init:

        n61_init(myhandler);

      The myhandler() function should look like this:

        int myhandler(int cmd, unsigned char c)
        {
            if(cmd == 0) {
                serial_send(c);          // send c out the port
                return;
            }
            else if(cmd == 1)
                return serial_recv();    // return the next byte in queue
            else if(cmd == 2)
                return serial_isdata();  // return zero if queue is empty
            else if(cmd == 3)
                msleep(1);               // delay for 1 millisecond
        }

      0 means send "c" out the serial port.  1 means return a byte from the
      serial port.  2 means return true/false if there is data waiting.
      Simple enough!

      This driver also requires a millisecond resolution timer, which is
      what the last request is for.  Most platforms include some sort of
      millisecond (or less) delay function.  For MSVC++ there is Sleep(),
      Unix has usleep() and DOS has delay().  If you're not using such a
      platform, then you'll have to time it yourself.  The driver doesn't
      specify how many milliseconds to wait, your function should wait
      just one millisecond.  So just do Sleep(1), usleep(1000), delay(1),
      or whatever your homebrew method is.  Easy!

      Lastly, n61_init() also queries the phone for the SMSC number to be
      used when sending SMS.

      Whew!  If you got past n61_init(), then the rest is easy as cake.

    +--------------------------------------------+
    |  int n61_smssend(char *dest, char *msg);  |
    +--------------------------------------------+

      Sends "msg" to "dest".  Returns 1 if sent, 0 if not.


    +---------------------------------------------+
    |  int n61_smsrecv(char *source, char *msg);  |
    +---------------------------------------------+

      Copies a received message into "msg", stores the source phone number
      in "source" and returns 1.  Returns 0 if no messages are in the queue.
      "msg" will not be larger than 161 bytes (including null byte).
      "source" will not be larger than 17 bytes (including null byte).


    +-----------------------+
    |  int n61_smsqueue();  |
    +-----------------------+

      Returns the number of messages in the incoming queue.


    +---------------------+
    |  int n61_update();  |
    +---------------------+

      This must be called as often as possible.  If it's not called, then
      you won't be able to receive messages.  This might be something good
      to put in a timer interrupt if possible (yes, it's safe).


  That's all you need to know!

  ------------------------------------------------------------------------

  Nokia 6190 message format:

    1 byte = start byte (0x1e)
    1 byte = message destination
    1 byte = message source
    1 byte = message type
    1 byte = ???
    1 byte = message size

    X bytes = message data (X = message size)

    1 byte = filler byte (exists only if needed to make message size even)

    2 byte = result of all 16bit words XOR'ed together

**************************************************************************/

#include <string.h>
#include "..\..\04_LIB\RKLib\07_SW\03_SRC\global.h"
#include "..\..\04_LIB\RKLib\07_SW\03_SRC\uart.h"


#define N61MODE_SYNC	0
#define N61MODE_DEST	1
#define N61MODE_SOURCE	2
#define N61MODE_TYPE	3
#define N61MODE_UNKNOWN	4
#define N61MODE_SIZE	5
#define N61MODE_DATA	6

#define N61STATE_WAIT	0
#define N61STATE_PROC	1
#define N61STATE_GOOD	2
#define N61STATE_BAD	3

#define N61MAXSIZE	120

#define N61VALID_1H	0x0b
#define N61VALID_6H	0x47
#define N61VALID_24H	0xa7
#define N61VALID_72H	0xa9
#define N61VALID_1W	0xad
#define N61VALID_MAX	0xff

struct N61_MESSAGE
{
	int dest;
	int source;
	int type;
	int unknown;
	int size;
	unsigned char dat[256];
	int cs[2];
};

struct N61_MESSAGE n61_tmp, n61_buf;
int n61_seqnum = 0;
int n61_prevseq = 0;
int (*n61_serial)(int cmd, unsigned char c);
int n61_mode = 0;
int n61_readsize = 0;
int n61_readbase = 0;
int n61_state;
int n61_multiple;
char n61_smsc[32];
int n61_msgqueuesize;
char n61_msgqueue[161];
char n61_msgqueue2[17];
int n61_ack, n61_gotmsg, n61_waittype;
int n61_blocked;


// ** translate table taken directly from gnokii **

unsigned char PROGMEM n61_transtable[] = {

	/* ETSI GSM 03.38, version 6.0.1, section 6.2.1; Default alphabet */
	/* Characters in hex position 10, [12 to 1a] and 24 are not present on
		latin1 charset, so we cannot reproduce on the screen, however they are
		greek symbol not present even on my Nokia */

	'@',  0xa3, '$',  0xa5, 0xe8, 0xe9, 0xf9, 0xec, 
	0xf2, 0xc7, '\n', 0xd8, 0xf8, '\r', 0xc5, 0xe5,
	'?',  '_',  '?',  '?',  '?',  '?',  '?',  '?',
	'?',  '?',  '?',  '?',  0xc6, 0xe6, 0xdf, 0xc9,
	' ',  '!',  '\"', '#',  0xa4,  '%',  '&',  '\'',
	'(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
	'8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	0xa1, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
	'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
	'X',  'Y',  'Z',  0xc4, 0xd6, 0xd1, 0xdc, 0xa7,
	0xbf, 'a',  'b',  'c',  'd',  'e',  'f',  'g',
	'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
	'x',  'y',  'z',  0xe4, 0xf6, 0xf1, 0xfc, 0xe0
};
//*/
uint8_t simple_bcd(char *s)
{
	uint8_t  x, n, hi, lo;

	uint8_t len = 1;

	x = 0;
	while(s[x]) 
	{
		x++;
		if(s[x])
			x++;
		len++
	}
	UART_putchar (len);
	
	if(s[0] == '+') {
		UART_putchar (0x91); 
		++s;
	}
	else
		UART_putchar (0x81);

	x = 0;
	while(s[x]) 
	{
		lo = s[x++] - '0';
		if(s[x])
			hi = s[x++] - '0';
		else
			hi = 0x0f;

		n = (hi << 4) + lo;
		UART_putchar (n); 
	}
	return len;
}

uint8_t simple_bcd2(char *dest, char *s)
{
	uint8_t size, x, y, n, hi, lo;

	size = strlen(s);
	UART_putchar (size);
	
	
	if(s[0] == '+') {
		UART_putchar (0x91);
		++s;
	}
	else
		UART_putchar (0x81);

	x = 0;
	y = 2;
	while(s[x]) {
		lo = s[x++] - '0';
		if(s[x])
			hi = s[x++] - '0';
		else
			hi = 0x0f;

		n = (hi << 4) + lo;
		UART_putchar (n);
	}
	return size;
}

void simple_pack7(char *dest, char *s)
{
	int len;
	char send;
	unsigned char c;
	unsigned short *p, w;
	int shift;
	int n, x;

	len = strlen(s);
	x = (len * 8) / 7;
	for(n = 0; n < x; ++n)
		dest[n] = 0;

	shift = 0;
	w = 0;
	for(n = 0; n < len; ++n) {
		w = n61_gettrans(s[n]) & 0x7f;
		w <<= shift;

		send |= w;

		shift += 7;
		if(shift >= 8) 
		{
			shift &= 7;
			UART_putchar (send);
			send = 0;
		}
	}
}


int simple_smssendfull(char *smsc, char *dest, char *msg)
{
	unsigned char buf[256];
	int n;

	// standard frame data header
	UART_putchar (0x00);
	UART_putchar (0x01);
	UART_putchar (0x00);
	
	// send sms ?

	UART_putchar (0x01);
	UART_putchar (0x02);
	UART_putchar (0x00);

	n = simple_bcd(smsc);
	n = 12 - n;
	/*fill out rest of SMSC by zeros*/
	while(n > 0)
	{
		UART_putchar (0x00);
		n--;
	}

	// TPDU ?
	UART_putchar (0x11);

	// misc
	UART_putchar (0x00);	// message ref
	UART_putchar (0x00);	// protocol ID
	UART_putchar (0xf1);	// data coding scheme (non-flash)

	// message size
	UART_putchar (strlen(msg);

	// destination
	n = simple_bcd2(buf + 23, dest);
		n = 12 - n;
	/*fill out rest of SMSC by zeros*/
	while(n > 0)
	{
		UART_putchar (0x00);
		n--;
	}

	// validity period
	UART_putchar (N61VALID_24H);

	// filler
	UART_putchar (0x00);
	UART_putchar (0x00);
	UART_putchar (0x00);
	UART_putchar (0x00);
	UART_putchar (0x00);
	UART_putchar (0x00);

	// the string
	simple_pack7(buf + 42, msg);

	// try till we get some response
	while(1) {
		n61_state = N61STATE_PROC;

		n61_gotmsg = 0;
		n61_waittype = 1;
		n61_sendmsg(0x02, 42 + strlen(msg), buf);

		n61_wait(5000);
		if(n61_state != N61STATE_PROC)
			break;
	}

	if(n61_state == N61STATE_GOOD)
		return 1;

	return 0;
}

