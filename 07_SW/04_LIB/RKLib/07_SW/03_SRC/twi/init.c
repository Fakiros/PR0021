#include "../twi.h"

volatile unsigned char TWI_address;
volatile unsigned char *TWI_data;
volatile unsigned char TWI_ddr;
volatile unsigned char TWI_bytes;
volatile unsigned char TWI_stop;
volatile unsigned char TWI_stat;
volatile unsigned char TWI_retry_cnt;

void TWI_init(void)
{
#if defined(TWPS0)
  TWSR = 0;
#endif
  TWBR = (F_CPU / 100000UL - 16)/2;
}

SIGNAL(SIG_2WIRE_SERIAL)
{
  unsigned char TWI_status = TWSR & TW_STATUS_MASK;      // Mask out prescaler bits to get TWI status
  
  switch(TWI_status) {
  case TW_START:                        // Start condition
  case TW_REP_START:                      // Repeated start condition
    if(TWI_retry_cnt > 2) {
      TWCR |= (1<<TWINT)|(1<<TWSTO);          // Generate stop condition
      TWI_stat &= ~(1<<TWI_BUSY);                // Bus no longer busy
      return;                        // If 3 times NACK abort
    }
    TWDR = (TWI_address<<1) + TWI_ddr;            // Transmit SLA + Read or Write
    TWCR &= ~(1<<TWSTA);                  // TWSTA must be cleared by software! This also clears TWINT!!!
    break;

  case TW_MT_SLA_ACK:                      // Slave acknowledged address,
    TWI_retry_cnt = 0;                      // so clear retry counter
    TWDR = *TWI_data;                    // Transmit data,
    TWI_data++;                        // increment pointer
    TWCR |= (1<<TWINT);                  // and clear TWINT to continue
    break;

  case TW_MT_SLA_NACK:                    // Slave didn't acknowledge address,
  case TW_MR_SLA_NACK:
    TWI_retry_cnt++;                      // this may mean that the slave is disconnected
    TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWSTO);      // retry 3 times
    break;

  case TW_MT_DATA_ACK:                    // Slave Acknowledged data,
    if(--TWI_bytes > 0) {                  // If there is more data to send,
      TWDR = *TWI_data;                  // Send it,
      TWI_data++;                      // increment pointer
      TWCR |= (1<<TWINT);                // and clear TWINT to continue
    }
    else {
      TWCR |= (1<<TWSTO)|(1<<TWINT);          // Generate the stop condition if desired
      TWI_stat &= ~(1<<TWI_BUSY);                // Bus no longer busy
    }
    break;

  case TW_MT_DATA_NACK:                    // Slave didn't acknowledge data
    TWCR |= (1<<TWINT)|(1<<TWSTO);            // Send stop condition
    TWI_stat &= ~(1<<TWI_BUSY);                  // Bus no longer busy
    break;

  case TW_MT_ARB_LOST:                     // Single master this can't be!!!
    break;

  case TW_MR_SLA_ACK:                                   // Slave acknowledged address
    if(--TWI_bytes > 0) TWCR |= (1<<TWEA)|(1<<TWINT);  // If there is more than one byte to read acknowledge
      else TWCR |= (1<<TWINT);              // else do not acknowledge
    break;

  case TW_MR_DATA_ACK:                     // Master acknowledged data
    *TWI_data = TWDR;                    // Read received data byte
    TWI_data++;                        // Increment pointer
    if(--TWI_bytes > 0) TWCR |= (1<<TWEA)|(1<<TWINT);  // Get next databyte and acknowledge  
      else TWCR &= ~(1<<TWEA);              // Enable Acknowledge must be cleared by software, this also clears TWINT!!!
    break;

  case TW_MR_DATA_NACK:                    // Master didn't acknowledge data -> end of read process
    *TWI_data = TWDR;                    // Read received data byte
    TWCR |= (1<<TWSTO)|(1<<TWINT);             // Generate stop condition
    TWI_stat &= ~(1<<TWI_BUSY);                  // Bus no longer busy    
  }
}









