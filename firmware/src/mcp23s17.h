/* 
 * File:   mcp23s17.h
 * Author: IbHelmer
 *
 * Created on 12. februar 2016, 21:10
 */

#ifndef MCP23S17_H
#define	MCP23S17_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <xc.h>
//#include "app.h"
    
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

#define PORT_A      0x01
#define PORT_B      0x02

    
extern void portchipsetup();
extern void portdir(unsigned char port, unsigned char dir );
extern void portwrite(unsigned char port, unsigned char dat);

#ifdef	__cplusplus
}
#endif

#endif	/* MCP23S17_H */

