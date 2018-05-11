/*****************************************************************************
 * University of Southern Denmark
 *
 * MODULENAME.: hardware.h
 *
 * PROJECT....: Assignment 2
 *
 * DESCRIPTION:
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * --------------------
 * Mar 6, 2018	jorn    Module created.
 *
 *****************************************************************************/

#ifndef HARDWARE_H_
#define HARDWARE_H_

/***************************** Include files *******************************/
#include "emp_type.h"

/*****************************    Defines    *******************************/
#define LED_OFF           0b000
#define LED_COLOR_GREEN   0b001
#define LED_COLOR_BLUE    0b010
#define LED_COLOR_CYAN    0b011
#define LED_COLOR_RED     0b100
#define LED_COLOR_YELLOW  0b101
#define LED_COLOR_MAGENTA 0b110
#define LED_COLOR_WHITE   0b111

#define EMP_LED_RED       0b011
#define EMP_LED_YELLOW    0b101
#define EMP_LED_GREEN     0b110
#define EMP_LED_ALL       0b000

/********************** External declaration of Variables ******************/
enum sw1_events
{
  BE_NONE, BE_SINGLE, BE_DOUBLE, BE_LONG
};

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/
INT8U is_sw1_pressed(void);
/*****************************************************************************
 *   Input    : -
 *   Output   : INT8U as Bool
 *   Function : Returns !0 if sw1 is pressed
 ******************************************************************************/

INT8U is_sw2_pressed(void);
/*****************************************************************************
 *   Input    : -
 *   Output   : INT8U
 *   Function : Returns !0 if sw2 is pressed
 ******************************************************************************/

void emp_set_led(INT8U led);
/*****************************************************************************
 *   Input    : 3 bit LED pattern
 *   Output   : -
 *   Function : Sets the EMP board leds
 ******************************************************************************/

void emp_clear_leds();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Clear all the EMP board leds
 ******************************************************************************/

void status_led_task();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function :
 ******************************************************************************/

void hardware_init(void);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Task to blink the EMP status led in 200ms interval
 ******************************************************************************/

enum sw1_events sw1_task(void);
/*****************************************************************************
 *   Input    : -
 *   Output   : sw1_event enum (header file)
 *   Function : Task to handel sw1 logic
 ******************************************************************************/

/****************************** End Of Module *******************************/
#endif /* DEBUG_H_ */
