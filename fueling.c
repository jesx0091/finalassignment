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
    READY, HOOK_OFF, INITIAL_SHUNT, NORMAL, CLOSING_SHUNT, IDLE
  } state = READY;

  switch (state)
  {
  case READY:

    break;
  case HOOK_OFF:

    break;
  case INITIAL_SHUNT:

    break;
  case NORMAL:

    break;
  case CLOSING_SHUNT:

    break;
  default:
    break;
  }

}

/****************************** End Of Module *******************************/

