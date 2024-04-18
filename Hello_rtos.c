/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "cmsis_os2.h"                  /* ::CMSIS:RTOS2 */
#include "Board_LED.h"                  /* ::Board Support:LED */
#include "Board_Buttons.h"              /* ::Board Support:Buttons */
#include "Board_GLCD.h"                 /* ::Board Support:Graphic LCD */
#include "GLCD_Config.h"                /* Keil.V2M-MPS2::Board Support:Graphic LCD */

#include "RTE_Components.h"             /* Component selection */
#include CMSIS_device_header

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

extern GLCD_FONT     GLCD_Font_16x24;

volatile int32_t delay_val = 500;

osThreadId_t tid_thrLED;                /* Thread id of thread: LED */
osThreadId_t tid_thrBUT;                /* Thread id of thread: BUT */

osMutexId_t mut_GLCD;                   /* Mutex to control GLCD access */



/*----------------------------------------------------------------------------
  display Button pressed on GLCD
 *---------------------------------------------------------------------------*/
void BUT_OnLCD (uint32_t butNum) {

#ifdef __USE_LCD
  osMutexAcquire(mut_GLCD, osWaitForever);
  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
  GLCD_DrawChar((9+butNum)*16, 7*24, 0x80+1);     /* Circle full */
  osMutexRelease(mut_GLCD);
#endif
}

/*----------------------------------------------------------------------------
  display button released GLCD
 *---------------------------------------------------------------------------*/
void BUT_OffLCD (uint32_t butNum) {

#ifdef __USE_LCD
  osMutexAcquire(mut_GLCD, osWaitForever);
  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_LIGHT_GREY);
  GLCD_DrawChar((9+butNum)*16, 7*24, 0x80+0);     /* Circle full */
  osMutexRelease(mut_GLCD);
#endif
}

/*----------------------------------------------------------------------------
  display LED on GLCD
 *---------------------------------------------------------------------------*/
void LED_OnLCD (uint32_t ledNum) {

#ifdef __USE_LCD
  osMutexAcquire(mut_GLCD, osWaitForever);
  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
  GLCD_DrawChar((9+ledNum)*16, 6*24, 0x80+1);     /* Circle full */
  osMutexRelease(mut_GLCD);
#endif
}

/*----------------------------------------------------------------------------
  display LED off GLCD
 *---------------------------------------------------------------------------*/
void LED_OffLCD (uint32_t ledNum) {

#ifdef __USE_LCD
  osMutexAcquire(mut_GLCD, osWaitForever);
  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_LIGHT_GREY);
  GLCD_DrawChar((9+ledNum)*16, 6*24, 0x80+0);     /* Circle empty */
  osMutexRelease(mut_GLCD);
#endif
}

/*----------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
void thrLED(void *argument) {
  uint32_t led_max    = LED_GetCount();
  uint32_t led_num    = 0;

  for (;;) {
    osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);
    LED_On(led_num);                                           /* Turn specified LED on */
    LED_OnLCD(led_num);
	  printf("LED : on\n");

    osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);
    LED_Off(led_num);                                          /* Turn specified LED off */
    LED_OffLCD(led_num);
	  printf("LED : off\n");

    led_num++;                                                 /* Change LED number */
    if (led_num >= led_max) {
      led_num = 0;                                             /* Restart with first LED */
    }
  }

}

/*----------------------------------------------------------------------------
  thrBUT: check button state
 *----------------------------------------------------------------------------*/
void thrBUT(void *argument) {
	uint32_t button_state;
	uint32_t button_num;
  uint32_t button_max;
  uint32_t button_msk;

  button_max = Buttons_GetCount();
  button_msk = (1U << button_max) - 1;

  for (;;) {
    osDelay(delay_val);                                        /* Wait */

    do {
			button_state = Buttons_GetState();

      for (button_num = 0; button_num < button_max; button_num++) {
        if (button_state & (1ul << button_num)) {
          BUT_OnLCD(button_num);
        } else {
          BUT_OffLCD(button_num);
        }
      }

		} while (button_state & button_msk);

    osThreadFlagsSet(tid_thrLED, 0x0001);
  }

}

/*----------------------------------------------------------------------------
  configure SystemCoreClock:
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {

	/* add code */
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {

  mut_GLCD = osMutexNew(NULL);                                 /* create GLCD mutex */

  tid_thrBUT = osThreadNew (thrBUT, NULL, NULL);               /* create BUT thread */
  tid_thrLED = osThreadNew (thrLED, NULL, NULL);               /* create LED thread */

  for (;;) {}
}

/*----------------------------------------------------------------------------
  main: create tasks and start kernel
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t button_msk = (1U << Buttons_GetCount()) - 1;
#ifdef __USE_LCD
  uint32_t ledMax = LED_GetCount() - 1;
  uint32_t keyMax = Buttons_GetCount();
	int32_t i;
#endif

  // System Initialization
  SystemCoreClockConfigure();
  SystemCoreClockUpdate();

  LED_Initialize();                                            /* initalize LEDs */
  Buttons_Initialize();                                        /* initalize Buttons */

#ifdef __USE_LCD
  GLCD_Initialize();                                           /* Initialize the GLCD */

  /* display initial screen */
  GLCD_SetFont(&GLCD_Font_16x24);
  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_ClearScreen();
  GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor(GLCD_COLOR_RED);
  GLCD_DrawString(0*16, 0*24, "   V2M-MPS2 Demo    ");
  GLCD_DrawString(0*16, 1*24, "      Blinky        ");
  GLCD_DrawString(0*16, 2*24, "   DesignStart      ");

  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
  switch ((SCB->CPUID >> 4) & 0xFFF) {
    case 0xC23:
      GLCD_DrawString(0*16, 4*24, "   ARM Cortex-M3    ");
      break;
    case 0xC24:
      GLCD_DrawString(0*16, 4*24, "   ARM Cortex-M4    ");
      break;
    case 0xC27:
      GLCD_DrawString(0*16, 4*24, "   ARM Cortex-M7    ");
      break;
    default:
      GLCD_DrawString(0*16, 4*24, "  unknown Cortex-M  ");
      break;
  }
  GLCD_DrawString(2*16, 6*24, "LED:");
  GLCD_DrawString(2*16, 7*24, "KEY:");

  GLCD_SetForegroundColor (GLCD_COLOR_LIGHT_GREY);
  for (i = 0; i <= ledMax; i++) {
    GLCD_DrawChar((9+i)*16, 6*24, 0x80+0);                     /* draw empty circle */
  }
  for (i = 0; i <  keyMax; i++) {
    GLCD_DrawChar((9+i)*16, 7*24, 0x80+0);                     /* draw empty circle */
  }
#endif

#ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize (0U, 1U);                            /* Initialize and start Event Recorder */
  EventRecorderEnable (EventRecordError, 0xF0U, 0xF8U);        /* RTOS Events */
  EventRecorderEnable (EventRecordAll,   0xF0U, 0xF4U);        /* Task, Queue, Timer, EventGroup, Heap Events */
#endif

  osKernelInitialize ();                                       /* initialize CMSIS-RTOS */

  osThreadNew(app_main, NULL, NULL);                           /* create application main thread */

  if (osKernelGetState() == osKernelReady) {
    osKernelStart();                                           /* start thread execution */
  }
}
