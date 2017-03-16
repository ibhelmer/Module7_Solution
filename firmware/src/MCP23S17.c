/* 
 * File:   mcp23s17.c
 * Author: ihn
 *
 * Created on 30. januar 2016, 23:07
 */

#include "mcp23s17.h"

void portchipsetup()
{

}

void portdir(unsigned char port, unsigned char dir )
{   
    switch(port)
    {   
        case PORT_A :            
                     writeI2C(IODIRA,dir);
                     break;
        case PORT_B : 
                     writeI2C(IODIRB,dir);
                     break;
    }
 }
void portwrite(unsigned char port, unsigned char dat)
{   
    switch(port)
    {    
        case PORT_A :              
                     writeI2C(OLATA,dat);
                     break;
        case PORT_B : 
                     writeI2C(OLATB,dat);
                     break;
    }
}
unsigned char portread()
{
    
}