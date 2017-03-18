/*
 * File:   LCD.h
 * Author: Ib Helmer Nielsen
 *         University College Nordjylland
 * Created on 12. februar 2017, 21:10
*/

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define HLCD    20         // LCD width = 20 characters 
#define VLCD    4          // LCD height = 4 rows

#define LCDDATA 0x00       
#define LCDCMD  0x01

#define SETUP   0x18       // 8 bit interface, 2 line and font 5x7
#define LINE1   0x00       // Start address of line 1
#define LINE2   0x40       // Start address of line 2
#define LINE3   0x10       // Start address of line 3
#define LINE4   0x50       // Start address of line 4

///////////////////////////////////////////////////////////////////////////
// Command set for display                                               //
///////////////////////////////////////////////////////////////////////////
#define CLRDISP 0x01       // Command for clear display
#define HOME    0x02       // Command for return home (upper left corner))
#define ENTRY   0x04       // Command for entry mode set
#define ONOFF   0x08       // Command for ON/OFF control
#define CURSOR  0x10       // Command for controlling cursor
#define FUNC    0x20       // Command for function set
#define CGRAM   0x40       // Switch to CGRAM 
#define DDRAM   0x80       // Switch to DDRAM
   
///////////////////////////////////////////////////////////////////////////
// Bit in registers                                                      //
///////////////////////////////////////////////////////////////////////////
#define SHIFT   0x01       // Bit for shift of display
#define I_D     0x02       // Bit for Increment or decrement
#define BLINK   0x01       // Bit for binking cursor
#define CUR_ON  0x02       // Bit for 
#define D_ONOFF 0x04       // Bit for diplay on/off

///////////////////////////////////////////////////////////////////////////
// Pin location and mapping                                              //
///////////////////////////////////////////////////////////////////////////   
#define RS      0x01       // Change according to location
#define E       0x02       // Change according to location

 ///////////////////////////////////////////////////////////////////////////
// Prototype for function to export                                       //
////////////////////////////////////////////////////////////////////////////  
extern void initLCD( void);
extern void writeLCD( char addr, unsigned char c);    
extern void clrLCD();
extern void offLCD();
extern void onLCD();
extern void gotoLCD(char row, char col);
extern void putsLCD( char *s);
extern void user_charLCD(char nr,const char patterns[]);
extern void printfLCD(const char *prt,...);

///////////////////////////////////////////////////////////////////////////
// Local MACRO                                                           //
///////////////////////////////////////////////////////////////////////////
#define putLCD( D)  writeLCD( LCDDATA, (D))
#define cmdLCD( C)  writeLCD( LCDCMD, (C))

#ifdef	__cplusplus
}
#endif

#endif	/* MCP23S17_H */





