/*
** LCD.h
**
 
*/

#define HLCD    16      // LCD width = 16 characters 
#define VLCD    4       // LCD height = 2 rows
#define LCDDATA 0x00
#define LCDCMD  0x01

#define SETUP   0x18      // 8 bit interface, 2 line and font 5x7
#define LINE1   0x00
#define LINE2   0x40
#define LINE3   0x10
#define LINE4   0x50

#define CLRDISP 0x01

#define HOME    0x02

#define ENTRY   0x04
#define SHIFT   0x01
#define I_D     0x02

#define ONOFF   0x08
#define BLINK   0x01
#define CUR_ON  0x02
#define D_ONOFF 0x04

#define CURSOR  0x10

#define FUNC    0x20

#define CCRAM   0x40
#define DDRAM   0x80

#define RS      0x02
#define E       0x01

extern void initLCD( void);
extern void writeLCD( char addr, unsigned char c);    
extern void clrLCD();
extern void offLCD();
extern void onLCD();
extern void gotoLCD(char row, char col);
extern void putsLCD( char *s);

#define putLCD( D)  writeLCD( LCDDATA, (D))
#define cmdLCD( C)  writeLCD( LCDCMD, (C))







