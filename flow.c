/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: flow.c
 *
 * PROJECT....: lastassignment
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * May 15, 2018  ESC    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "hardware.h"
#include "lcd.h"
#include "semphr.h"
#include "queue.h"
#include "global.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


extern void flow_state_mashine(void)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
  static enum states
  {
    READY, INITIAL_SHUNT, REFUELING, CLOSING_SHUNT
  } state = READY;
  static INT32U last_ticks;
  switch (state)
  {
  case READY:
    if (is_sw1_pressed())
    { // her skal nok lige laves lidt mere om, da denne ikke rigtigt gør som den skal, vi skal konstant tjekke om sw1 er trykket
      last_ticks = xTaskGetTickCount();
      state = INITIAL_SHUNT;
    }
    break;
  case INITIAL_SHUNT:
    if (is_sw1_pressed())
    {
      if (xTaskGetTickCount() - last_ticks >= 44 / portTICK_RATE_MS) // 44ms er ikke helt korrekt, det burde være 44,44 men jeg ved ikk om den kan det.
      {
        // her skal der så genereres den impuls

        last_ticks = xTaskGetTickCount();
      }    break;
      // her skal også laves lidt logik til om der har været mere end 0.1 L

    }
    else
    {
      state = READY;
    }
    break;
  case REFUELING:
    if (is_sw1_pressed())
    {
      if (xTaskGetTickCount() - last_ticks >= 4 / portTICK_RATE_MS) // 4ms er ikke helt korrekt, det burde være 4,44 men jeg ved ikk om den kan det.
      {
        // her skal der så genereres den impuls

        last_ticks = xTaskGetTickCount();
      }
    }
    else
    {
      state = READY;
    }
    break;
  case CLOSING_SHUNT:
    if (is_sw1_pressed())
        {
          if (xTaskGetTickCount() - last_ticks >= 44 / portTICK_RATE_MS) // 44ms er ikke helt korrekt, det burde være 44,44 men jeg ved ikk om den kan det.
          {
            // her skal der så genereres den impuls

            last_ticks = xTaskGetTickCount();
          }    break;
          // her skal også laves lidt logik til om der har løber det antal L som der er betalt for.

        }
        else
        {
          state = READY;
        }
    break;
  default:
    break;
  }

}

/****************************** End Of Module *******************************/

