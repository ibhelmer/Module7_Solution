/* ************************************************************************** */
/** Descriptive File Name

  @Company
 UCN/Ib Helmer Nielsen

  @File Name
    delay.h

  @Summary
    Blocking delay functions, using the core timer for better accuracy.

  @Description
    The header file for blocking delay function, DelayMs and DelayUs.
**/
/* ************************************************************************** */

#ifndef _DELAY_FILE_H    /* Guard against multiple inclusion */
#define _DELAY_FILE_H
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include "system_config.h"
#include "system_definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define GetSystemClock() (SYS_CLK_FREQ)
#define us_SCALE   (GetSystemClock()/2000000)
#define ms_SCALE   (GetSystemClock()/2000)  
   
   static uint32_t ReadCoreTimer(void);
   static uint32_t ReadCoreTimer()
   {
      volatile uint32_t timer;
      // get the count reg
      asm volatile("mfc0   %0, $9" : "=r"(timer));
      return(timer);
   }
   // *****************************************************************************
   // *****************************************************************************
   // Section: Data Types
   // *****************************************************************************
   // *****************************************************************************


   // *****************************************************************************
   // *****************************************************************************
   // Section: Interface Functions
   // *****************************************************************************
   // *****************************************************************************

   extern void DelayUs(unsigned long int usDelay );
   extern void DelayMs(unsigned long int msDelay );
  /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
