/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
uint8_t deviceAddress;
/* I2C Driver TX buffer  */
uint8_t         TXbuffer[] = "AA";
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* callback to Master indicating a change in Buffer Status of transmit 
    OR receive buffer */

void I2CMasterOpStatusCb ( DRV_I2C_BUFFER_EVENT event,
                           DRV_I2C_BUFFER_HANDLE bufferHandle,
                           uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static uint32_t ReadCoreTimer(void);

static uint32_t ReadCoreTimer()
{
    volatile uint32_t timer;

    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));

    return(timer);
}

void DelayMs(unsigned long int msDelay );

void DelayMs(unsigned long int msDelay );

DRV_I2C_BUFFER_EVENT i2cOpStatus;

DRV_I2C_BUFFER_EVENT APP_Check_Transfer_Status(DRV_HANDLE drvOpenHandle, 
                                               DRV_I2C_BUFFER_HANDLE drvBufferHandle);

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Define
// *****************************************************************************
// *****************************************************************************
#define GetSystemClock() (SYS_CLK_FREQ)
#define us_SCALE   (GetSystemClock()/2000000)
#define ms_SCALE   (GetSystemClock()/2000)

/* Address of slave devices */
#define PORTEXT_SLAVE_ADDRESS              0x21 << 1  // Address shifted left


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
/* State Machine for Master Write */
bool APP_Write_Tasks(void);
typedef enum{
    
        SEND_COMMAND = 0,
        STATUS_CHECK,
        TxRx_STATUS_CHECK,
        TxRx_COMPLETED

}APP_I2C_STATES;

static APP_I2C_STATES appWriteState = SEND_COMMAND;

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            DRV_TMR0_Start();
            /* Open the I2C Driver */
            appData.drvI2CHandle = DRV_I2C_Open( DRV_I2C_INDEX_0,DRV_IO_INTENT_WRITE );
            DRV_I2C_BufferEventHandlerSet(appData.drvI2CHandle, I2CMasterOpStatusCb, i2cOpStatus );                           
            if (appData.drvI2CHandle == DRV_HANDLE_INVALID)
            {
               
            //Client cannot open instance
            }
            else
            {
            
            }
            appData.state = APP_STATE_IDLE;
            
            break;
        }

        case APP_STATE_SEND:
        {
            LED1On();
         //   DelayMs(300);
         //   LED2Toggle();
            if (APP_Write_Tasks())
            {
               appData.state = APP_STATE_IDLE;
            }
            break;
        }
        case APP_STATE_IDLE:
        {
           break;
        }        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
bool APP_Write_Tasks(void)
{
    switch (appWriteState)
    {
        case SEND_COMMAND:
        {    

            deviceAddress = PORTEXT_SLAVE_ADDRESS;

            /* Write Transaction - 1 to Port Extender */

            if ( (appData.appI2CWriteBufferHandle == (DRV_I2C_BUFFER_HANDLE) NULL) || 
                    (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_COMPLETE) || 
                        (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_ERROR) )
            {
                appData.appI2CWriteBufferHandle = DRV_I2C_Transmit (  appData.drvI2CHandle,
                                                                        deviceAddress,
                                                                        &TXbuffer[0], 
                                                                        (sizeof(TXbuffer)-1), 
                                                                        NULL);
            }
            
            appWriteState = STATUS_CHECK;
            
            break;
        }   
        case STATUS_CHECK:
        {
            if ( (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_COMPLETE ) ||
                    (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_ERROR) )
                    
                appWriteState = TxRx_STATUS_CHECK; 
            else
                appWriteState = STATUS_CHECK;
            
            break;
        }        
      
        case TxRx_STATUS_CHECK:
        {
            DelayMs(300);
            
           // LED1Toggle(); 
            LED2Toggle(); 
                                                
            /* to run the application only once,  
             * set next state to TxRx_COMPLETED */
//            appWriteState = TxRx_COMPLETED;
            
            /* to run the application in a continuous loop,  
             * set next state to TxRx_TO_EXTERNAL_SLAVE_1 */
            appWriteState = TxRx_COMPLETED;
            
            
            break;
        }
        case TxRx_COMPLETED:
        {
            return true;
            break;
        }
    }
    
    return false;
}

//****************************************************************************/
//  Function: APP_Check_Transfer_Status
//
//  Returns the status of Buffer operation from the driver; The application
//  can probe this function to see if the status of a particular I2C
//  transfer is in its execution
//****************************************************************************/
DRV_I2C_BUFFER_EVENT APP_Check_Transfer_Status(DRV_HANDLE drvOpenHandle, DRV_I2C_BUFFER_HANDLE drvBufferHandle)
{
    return (DRV_I2C_TransferStatusGet  (appData.drvI2CHandle, drvBufferHandle));
}

//****************************************************************************/
//  Function: Master Callback Function 
//
//  Callback from DRV_I2C_Tasks when I2C is configured in Master mode     
//****************************************************************************/

void I2CMasterOpStatusCb ( DRV_I2C_BUFFER_EVENT event,
                           DRV_I2C_BUFFER_HANDLE bufferHandle,
                           uintptr_t context)
{
    static uint32_t successCount = 0;
    
    switch (event)
    {
        case DRV_I2C_BUFFER_EVENT_COMPLETE:
            successCount++;
            Nop();
            break;
        case DRV_I2C_BUFFER_EVENT_ERROR:
            successCount--;
            break;
        default:
            break;
    }
}

/***********************************************************
 *   Millisecond Delay function using the Count register
 *   in coprocessor 0 in the MIPS core.
 *   When running 200 MHz, CoreTimer frequency is 100 MHz
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 10ns
 *   1 ms = N x CoreTimer_period;
 *   To count 1ms, N = 100000 counts of CoreTimer
 *   1 ms = 10 ns * 100000 = 10e6 ns = 1 ms
 *   When running 80 MHz, CoreTimer frequency is 40 MHz 
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 25ns
 *   To count 1ms, N = 40000 counts of CoreTimer
 *   1ms = 25 ns * 40000 = 10e6 ns = 1 ms
 *   ms_SCALE = (GetSystemClock()/2000) @ 200 MHz = 200e6/2e3 = 100e3 = 100000
 *   ms_SCLAE = (GetSystemClock()/2000) @ = 80e6/2e3 = 40e3 = 40000 
 */
 
void DelayMs(unsigned long int msDelay )
{
      register unsigned int startCntms = ReadCoreTimer();
      register unsigned int waitCntms = msDelay * ms_SCALE;
 
      while( ReadCoreTimer() - startCntms < waitCntms );
}

/***********************************************************
 *   Microsecond Delay function using the Count register
 *   in coprocessor 0 in the MIPS core.
 *   When running 200 MHz, CoreTimer frequency is 100 MHz
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 10ns
 *   1 us = N x CoreTimer_period;
 *   To count 1us, N = 100 counts of CoreTimer
 *   1 us = 10 ns * 100 = 1000 ns  = 1us
 *   When running 80 MHz, CoreTimer frequency is 40 MHz 
 *   CoreTimer increments every 2 SYS_CLK, CoreTimer period = 25ns
 *   To count 1us, N = 40 counts of CoreTimer
 *   1us = 25 ns * 40 = 1000 ns = 1 us
 *   us_SCALE = (GetSystemClock()/2000) @ 200 MHz = 200e6/2e6 = 100 
 *   us_SCLAE = (GetSystemClock()/2000) @ 80 MHz = 80e6/2e6 = 40 
 */
 
void DelayUs(unsigned long int usDelay )
{
      register unsigned int startCnt = ReadCoreTimer();
      register unsigned int waitCnt = usDelay * us_SCALE;
 
      while( ReadCoreTimer() - startCnt < waitCnt );
}
 

/*******************************************************************************
 End of File
 */
