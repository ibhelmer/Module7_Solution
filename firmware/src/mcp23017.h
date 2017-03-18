/* 
 * File:   mcp23017.h
 * Author: Ib Helmer Nielsen
 *         University College Nordjylland
 * Created on 12. februar 2017, 21:10
 */

#ifndef MCP23017_H
#define	MCP23017_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <xc.h>
   
// MCP23017 register specification    
#define IODIRA      0x00
#define IODIRB      0x01
#define IPOLA       0x02
#define IPOLB       0x03
#define GPINTENA    0x04
#define GPINTENB    0x05
#define DEFVALA     0x06
#define DEFVALB     0x07
#define INTCONA     0x08
#define INTCONB     0x09
#define IOCON       0x0A
#define GPPUA       0x0C
#define GPPUB       0x0D
#define INTFA       0x0E
#define INTFB       0x0F
#define INTCAPA     0x10
#define INTCAPB     0x11
#define GPIOA       0x12
#define GPIOB       0x13
#define OLATA       0x14
#define OLATB       0x15

// Port name and port mapping
#define CNTRL_PORT  PORT_A    // Change according to wiring
#define DATA_BUS    PORT_B    // Change according to wiring
#define PORT_A      0x01
#define PORT_B      0x02

    
extern void portchipsetup();
extern void portdir(unsigned char port, unsigned char dir );
extern void portwrite(unsigned char port, unsigned char dat);
extern void writeI2C(unsigned char reg, unsigned char val );
#ifdef	__cplusplus
}
#endif

#endif	/* MCP23S17_H */

