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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "lcd.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
APP_DATA appData;
uint8_t I2C_Dev_Adr;
/* I2C Driver TX buffer  */
uint8_t      TXbuffer[] = {0,0};   

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


DRV_I2C_BUFFER_EVENT i2cOpStatus;
DRV_I2C_BUFFER_EVENT APP_Check_Transfer_Status(DRV_HANDLE drvOpenHandle, 
                                               DRV_I2C_BUFFER_HANDLE drvBufferHandle);

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Define
// *****************************************************************************
// *****************************************************************************
#define GetSystemClock() (SYS_CLK_FREQ)


/* Address of slave devices */
#define PORTEXT_SLAVE_ADDRESS              0x21 << 1  // Address shifted left


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
/* State Machine for Master Write */
bool I2C_Write_Tasks(void);


typedef enum{
    
        SEND_COMMAND = 0,
        STATUS_CHECK,
        READY

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
            
            /* Open the I2C Driver */
            appData.drvI2CHandle = DRV_I2C_Open( DRV_I2C_INDEX_0,DRV_IO_INTENT_WRITE );
           
            DRV_I2C_BufferEventHandlerSet(appData.drvI2CHandle, I2CMasterOpStatusCb, i2cOpStatus );                           
            
            if (appData.drvI2CHandle == DRV_HANDLE_INVALID)
            {  
               //Error response here !!!
            }
            else
            {
              
            }
            appWriteState = SEND_COMMAND;
            initLCD();
            LED1Toggle(); // Init was ok
            DRV_TMR0_Start();
            appData.state = APP_STATE_IDLE;
            break;
        }
        case APP_STATE_SEND:
        {
            LED2Toggle(); // Ping every 1. sec
            putsLCD("Hello World");
            appData.state = APP_STATE_IDLE;                  
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

void writeI2C(unsigned char reg, unsigned char val )
{
    TXbuffer[0]=reg;
    TXbuffer[1]=val;
    while(!I2C_Write_Tasks()) {}; // Not well coded !!!!
    appWriteState = SEND_COMMAND;  
}

bool I2C_Write_Tasks(void)
{
   
    switch (appWriteState)
    {
        case SEND_COMMAND:
        {    
            I2C_Dev_Adr = PORTEXT_SLAVE_ADDRESS; // Set portextender address
            /* Write Transaction - 1 to Port Extender */
            if ( (appData.appI2CWriteBufferHandle == (DRV_I2C_BUFFER_HANDLE) NULL) || 
                 (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_COMPLETE) || 
                 (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_ERROR) )
            {
                appData.appI2CWriteBufferHandle = DRV_I2C_Transmit (  appData.drvI2CHandle,
                                                                      I2C_Dev_Adr,
                                                                      &TXbuffer[0], 
                                                                      (sizeof(TXbuffer)), 
                                                                      NULL);

            }
            appWriteState = STATUS_CHECK;
            break;
        }   
        case STATUS_CHECK:
        {
            if ( (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_COMPLETE ) ||
                 (APP_Check_Transfer_Status(appData.drvI2CHandle, appData.appI2CWriteBufferHandle) == DRV_I2C_BUFFER_EVENT_ERROR) )            
            { 
               appWriteState = READY; 
               
            }
            else
            {
                appWriteState = STATUS_CHECK;  
            }
            break;
        }
        case READY:
        {   
            DelayMs(10);
            LED2Toggle();
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


/*******************************************************************************
 End of File
 */
