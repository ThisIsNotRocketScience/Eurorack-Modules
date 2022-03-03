/*
 * Eeprom.h
 *
 *  Created on: 9 jan. 2017
 *      Author: Stijn
 */

#ifndef SOURCES_EEPROM_H_
#define SOURCES_EEPROM_H_


extern volatile int i2csending ;
extern volatile int i2creceiving ;

void EE24_WriteByte(unsigned short address, unsigned char value);
void EE24_WriteBlock(unsigned short address, unsigned char *data, int len);
unsigned char EE24_ReadByte(unsigned short address);
void EE24_ReadBlock(unsigned short address, unsigned char *out, int len);


#endif /* SOURCES_EEPROM_H_ */
