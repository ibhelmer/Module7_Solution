/*
** LCD.c
** Interfacing to an LCD display of EPSON type
** 
*/

#include <p32xxxx.h>
#include <stdarg.h>
#include "delay.h"
#include "mcp23017.h"
#include "lcd.h"

void initLCD( void)
{
    portchipsetup();
    portdir(CNTRL_PORT,0x00);
    portdir(DATA_BUS,0x00);
    
    DelayMs(40);                // wait 40mSec
    cmdLCD(0x38);               // Sendt 0x38
    DelayMs(5);                 // wait 5mSec
    cmdLCD(0x38);               // send 0x38
    DelayUs(100);               // wait 100uSec
    cmdLCD(0x38);               // send 0x38
    DelayUs(48);                // wait 48us
    cmdLCD(0x38);
    offLCD();
    cmdLCD(CLRDISP);           // clear display
    DelayMs(2);                 // wait 2mSec
    cmdLCD(6);                 // increment cursor, no shift
    DelayMs(2);                 // wait 2mSec
    onLCD();
    DelayMs(2);
} // initLCD


void writeLCD( char addr, unsigned char c)    
{   portwrite(DATA_BUS,c);
    if (addr==LCDDATA)          // select DDRam or command reg
    {
        portwrite(CNTRL_PORT,RS);
        portwrite(CNTRL_PORT,RS+E);
        DelayUs(1);
        portwrite(CNTRL_PORT,RS);
    }
    else
    {
        portwrite(CNTRL_PORT,0x00);
        portwrite(CNTRL_PORT,E); 
        DelayUs(1);
    }
    portwrite(PORT_A,0x00);     // Just for debugging, changing the databus to 0x00 
    DelayUs(48);                // Wait 48us so LCD is ready
} // writeLCD

void clrLCD()
{
    cmdLCD(CLRDISP);
    DelayMs(2);
}
void offLCD()
{
    cmdLCD(ONOFF);
}

void onLCD()
{
    cmdLCD(ONOFF | D_ONOFF);
}
void gotoLCD(char row, char col)
{
    switch(row)
    {
        case 1 : cmdLCD(DDRAM + LINE1 + col); break;
        case 2 : cmdLCD(DDRAM + LINE2+ col); break;
        case 3 : cmdLCD(DDRAM + LINE3+ col); break;
        case 4 : cmdLCD(DDRAM + LINE4+ col); break;
        default : cmdLCD(DDRAM + LINE1+ col); break;
    }
    
}
/******************************************************************************/
/* Navn       : lcd_user_char		  					   									*/
/* Prototype  : void lcd_user_char(char nr, char patterns[8]); 				   */
/* Programmør : IHN            Dato : 03.02.2002		Version : 1.0  		   */
/* Funktion   :                                                               */
/* 				 Downloader user karakter til CG ram i LCD, efter download 	   */
/*              placeres cursor i HOME                                        */
/* Input      :															  					   */
/* 				 const patterns array med karakter mønster i 5 * 8 matrise     */
/* Output	  :															  					   */
/*					 -																				   */
/*	Kalder	  :																				   */
/*		          cmdLCD, putLCD         												   */
/******************************************************************************/
void user_charLCD(char nr,const char patterns[])
{
	char i;
	cmdLCD(CGRAM+(nr<<3));
	for (i=0; i<8; i++)
	{
		putLCD(patterns[i]);
	}
	cmdLCD(DDRAM);
}



void putsLCD( char *s)
{
    while( *s) putLCD( *s++);   // Send out string char by char
} //putsLCD                        end when \0

/******************************************************************************/
/* Navn       : printfLCD			  					   									*/
/* Prototype  : void lcd_printf(const char *ptr, ...); 							   */
/* Programmør : IHN            Dato : 08.03.2002		Version : 1.0  		   */
/* Funktion   :                                                               */
/* 				 printfLCD is a very limited implementation of printf,         */
/*              below is a list of the formating char that can be used in 	   */
/*              this limited implementation:                                  */
/*					 %d - Decimal, no other formating implemented                  */
/*					 %s - String, do														   	*/
/*              %c - Char, do							                              */
/*					 %t - Time (not standard) set aside 2 position for time   		*/
/* Input      :															  					   */
/* 				 const string pointer, variable number of input parameters     */
/* Output	  :															  					   */
/*					 -																				   */
/*	Call   	  :																				   */
/*		          lcd_write, lcd_prstring 												   */
/******************************************************************************/
void printfLCD(const char *prt,...)
{
	va_list vp;      		// Use to point to unnamed argument in input 
	unsigned char buf[16];
	const char *p;
	unsigned char cval, *sval;
	int ival;
	
	va_start(vp, prt);	// Point to first unnamed argument
	for (p = prt; *p; p++)
	{
		if (*p != '%')
		{
			putLCD(*p);
			continue;
		}
		switch (*++p)
		{
			case 'd' : 	ival = va_arg(vp, int);
						  	sprintf(buf,"%d", ival);
						   putsLCD(buf);
						   break;
			case 'c' :  cval = (char) va_arg(vp,int);
							putLCD(cval);
							break;
			case 's' :  sval = va_arg(vp,char *);
							putsLCD(sval);
							break;
			case 't' :  ival = va_arg(vp,int);
							sprintf(buf,"%2d",ival);
							putsLCD(buf);
							break;							
			default  :	putLCD(*p);
							break;
		}
	}
	va_end(vp);
}

