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

#define HOLD              0x01
#define CLICK             0x02
#define D_CLICK           0x03
#define RELEASED          0x04
#define DIGI_CCW          0x05
#define DIGI_CW           0x06

#define DIGI_SW           0x01
#define DIGI_R            0x02
#define SW1               0x03
#define SW2               0x04
/********************** External declaration of Variables ******************/
/*enum events
{
  BE_NONE, BE_SINGLE, BE_DOUBLE, BE_LONG
};*/
typedef struct _msg {
  uint8_t   ch;
  uint8_t   function;
  uint8_t   event;
} _msg_t;

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

void hardware_ticks(void *pvParameters);

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

void sw1_task(void *pvParameters);
/*****************************************************************************
 *   Input    : -
 *   Output   : sw1_event enum (header file)
 *   Function : Task to handel sw1 logic
 ******************************************************************************/

void sw2_task(void *pvParameters);

/****************************** End Of Module *******************************/
#endif /* DEBUG_H_ */
