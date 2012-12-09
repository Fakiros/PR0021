#ifndef NVM_H
#define NVM_H
#include <avr/eeprom.h>

/* Read a block of __n bytes from EEPROM address __src to SRAM __dst. */
#define NVM_ReadDirect(__dst, __src, __n) eeprom_read_block( (__dst), (__src), (__n) )	

/* Update a block of __n bytes to EEPROM address __dst from __src */
#define NVM_WriteDirect(__src, __dst, __n) eeprom_update_block	( (__src), (__dst), (__n))	


typedef struct
{
	uint16_t		KeyOnCounter;
	uint32_t		ACLineTime;


}NVM_t;


extern NVM_t NVM;



#endif //NVM_H
