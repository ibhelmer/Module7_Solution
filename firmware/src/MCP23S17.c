/* 
 * File:   mcp23017.h
 * Author: Ib Helmer Nielsen
 *         University College Nordjylland
 * Created on 30. januar 2016, 23:07
 */

#include "mcp23017.h"

void portchipsetup()
{
   // Code for setup of interrupt response and bank select can be put here 
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
   // Not implemented 
}