/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: trafficlight.c
 *
 * PROJECT....: Assignment 2
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 180307  Jes   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "trafficlight.h"
#include "hardware.h"
/*****************************    Defines    *******************************/
#define TIM_200MS            200
#define TIM_800MS            800
#define TIM_1S              1000
#define TIM_4S              4000

/*****************************     Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

INT8U norwegian_mode()
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Handle the logic of the Norwegian mode.
 ******************************************************************************/
{
  static enum states
  {
    YELLOW_ON, YELLOW_OFF
  } state = YELLOW_OFF;

  static INT16U timer = TIM_200MS;

  INT8U return_led;

  switch (state)
  {
    case YELLOW_ON:
      return_led = TL_LED_YELLOW;
      if (!--timer)
      {
        state = YELLOW_OFF;
        timer = TIM_800MS;
      }
      break;
    case YELLOW_OFF:
      return_led = TL_LED_NONE;
      if (!--timer)
      {
        state = YELLOW_ON;
        timer = TIM_200MS;
      }
      break;
  }

  return (return_led);
}

INT8U emergency_mode()
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Handle the logic of the Emergency mode.
 ******************************************************************************/
{
  static enum states
  {
    EMERGENCY
  } state = EMERGENCY;

  INT8U return_led;

  switch (state)
  {
    case EMERGENCY:
      return_led = TL_LED_RED;
      break;
  }

  return (return_led);
}

INT8U auto_mode()
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Auto-mode (normal operation) controls changes / color code
 *              for the led's of the traffic light.
 ******************************************************************************/
{
  static enum states
  {
    RED, GREEN, YELLOW, RED_YELLOW
  } state = RED;

  static INT16U timer = TIM_4S;

  INT8U return_led;

  switch (state)
  {
    case RED:
      return_led = TL_LED_RED;
      if (!--timer)
      {
        state = RED_YELLOW;
        timer = TIM_1S;
      }
      break;
    case RED_YELLOW:
      return_led = TL_LED_R_Y;
      if (!--timer)
      {
        state = GREEN;
        timer = TIM_4S;
      }
      break;
    case GREEN:
      return_led = TL_LED_GREEN;
      if (!--timer)
      {
        state = YELLOW;
        timer = TIM_1S;
      }
      break;
    case YELLOW:
      return_led = TL_LED_YELLOW;
      if (!--timer)
      {
        state = RED;
        timer = TIM_4S;
      }
      break;
  }
  return (return_led);
}

INT8U trafficlight(enum sw1_events sw1_event)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
  static enum tl_states
  {
    AUTO_MODE, NORWEGIAN_MODE, EMERGENCY_MODE
  } tl_state = AUTO_MODE;

  INT8U return_led;
  switch (tl_state)
  {
    case AUTO_MODE:
      return_led = auto_mode();
      switch (sw1_event)
      {
        case BE_SINGLE:
          tl_state = NORWEGIAN_MODE;
          break;
        case BE_DOUBLE:
          tl_state = EMERGENCY_MODE;
          break;
        default:
          break;
      }
      break;

    case NORWEGIAN_MODE:
      return_led = norwegian_mode();
      switch (sw1_event)
      {
        case BE_DOUBLE:
          tl_state = EMERGENCY_MODE;
          break;
        case BE_LONG:
          tl_state = AUTO_MODE;
          break;
        default:
          break;
      }
      break;

    case EMERGENCY_MODE:
      return_led = emergency_mode();
      switch (sw1_event)
      {
        case BE_SINGLE:
          tl_state = NORWEGIAN_MODE;
          break;
        case BE_LONG:
          tl_state = AUTO_MODE;
          break;
        default:
          break;
      }
      break;
  }

  return (return_led);
}
/****************************** End Of Module *******************************/

