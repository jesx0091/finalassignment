/*****************************************************************************
 * University of Southern Denmark
 *
 * MODULENAME.: hardware.c
 *
 * PROJECT....: Assignment 2
 *
 * DESCRIPTION: 
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * --------------------
 * Mar 6, 2018  jorn    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include "systick_frt.h"
#include "hardware.h"
#include "lcd.h"
#include "global.h"
#include "drehimpulsgeber.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*****************************    Defines    *******************************/
#define TIM_2_SEC           2000
#define TIM_100_MSEC         100
#define TIM_200_MSEC         200
#define     DELAYMULTIPRESS 40  // The delay time before press is considered multipress.
#define STATUS_LED_TIMER    TIM_200_MSEC
xQueueHandle Queue;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
volatile INT32U hdw_ticks = 0;
/*****************************   Functions   *******************************/

void sendmsgtoqueue(struct _msg msg)
{
  if ( xQueueSendToBack( Queue,
      ( void * ) &msg,
      ( portTickType ) 10 ) != pdPASS)
  {
    /* Failed to post the message, even after 10 ticks. */
  }
  vTaskDelay(DELAYMULTIPRESS);
  return;
}

INT8U is_sw1_pressed(void)
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  return ((GPIO_PORTF_DATA_R & 0x10) ? 0 : 1);
}

INT8U is_sw2_pressed(void)
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  return ((GPIO_PORTF_DATA_R & 0x01) ? 0 : 1);
}


void hardware_ticks(void *pvParameters)
{

  while (1)
  {
    vTaskDelay(10 / portTICK_RATE_MS);
    hdw_ticks ++;

  }
}
INT32U get_hdw_ticks(void)
{
  return(hdw_ticks);
}
void sw1_task(void *pvParameters)
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  if (Queue == NULL)
   {
   // Queue was not created and must not be used.
   Queue = xQueueCreate(10, sizeof(struct _msg));
   }
  uint32_t timer = get_hdw_ticks();
  uint32_t last_timer = 0;
  uint8_t event = 0;

  struct _msg msg;

  static enum states
  {
    IDLE, FIRST_PRESS, LONG_PRESS
  } state = IDLE;

  while (1)
  {
    timer = get_hdw_ticks();
    event = 0;

    switch (state)
    {
    case IDLE:
      if (is_sw1_pressed())       // if button pushed
      {
        state = FIRST_PRESS;
        last_timer = timer;
      }
      break;
    case FIRST_PRESS:
      if (timer >= (last_timer + 200))          // if timeout
      {
        state = LONG_PRESS;
        event = HOLD;
      }
      else
      {
        if (!is_sw1_pressed())    // if button released
        {
          //state = FIRST_RELEASE;
          state = IDLE;
          //last_timer = timer;
          event = CLICK;
        }
      }
      break;

    case LONG_PRESS:
      if (!is_sw1_pressed())                 // if button released
      {
        event = RELEASED;
        state = IDLE;
      }
      break;
    default:
      break;
    }

    if (event)
    {

      msg.ch = 0;
      msg.event = event;
      msg.function = SW1;
      xQueueSendToBack(Queue, &msg, 0);

    }
    vTaskDelay(10);
  }
}
void sw2_task(void *pvParameters)
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  if (Queue == NULL)
   {
   // Queue was not created and must not be used.
   Queue = xQueueCreate(10, sizeof(struct _msg));
   }
  uint32_t timer = get_hdw_ticks();
  uint32_t last_timer = 0;
  uint8_t event = 0;

  struct _msg msg;

  static enum states
  {
    IDLE, FIRST_PRESS, LONG_PRESS
  } state = IDLE;

  while (1)
  {
    timer = get_hdw_ticks();
    event = 0;

    switch (state)
    {
    case IDLE:
      if (is_sw2_pressed())       // if button pushed
      {
        state = FIRST_PRESS;
        last_timer = timer;
      }
      break;
    case FIRST_PRESS:
      if (timer >= (last_timer + 200))          // if timeout
      {
        state = LONG_PRESS;
        event = HOLD;
      }
      else
      {
        if (!is_sw2_pressed())    // if button released
        {
          //state = FIRST_RELEASE;
          state = IDLE;
          //last_timer = timer;
          event = CLICK;
        }
      }
      break;

    case LONG_PRESS:
      if (!is_sw2_pressed())                 // if button released
      {
        event = RELEASED;
        state = IDLE;
      }
      break;
    default:
      break;
    }

    if (event)
    {

      msg.ch = 0;
      msg.event = event;
      msg.function = SW2;
      xQueueSendToBack(Queue, &msg, 0);

    }
    vTaskDelay(10);
  }
}
void emp_set_led(INT8U led)
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  // Set bit pattern bit2-bit0 on for led_color on PF1-PF3
  if (led & 0x01)
    GPIO_PORTF_DATA_R |= 0x08;
  else
    GPIO_PORTF_DATA_R &= ~0x08;

  if (led & 0x02)
    GPIO_PORTF_DATA_R |= 0x04;
  else
    GPIO_PORTF_DATA_R &= ~0x04;

  if (led & 0x04)
    GPIO_PORTF_DATA_R |= 0x02;
  else
    GPIO_PORTF_DATA_R &= ~0x02;
}

void emp_toggle_status_led()
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : -
 ******************************************************************************/
{
  GPIO_PORTD_DATA_R ^= 0x40;
}

void emp_clear_leds()
{
  emp_set_led( EMP_LED_ALL);
}

void status_led_task()
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  static INT16U led_timer = STATUS_LED_TIMER;

  if (!--led_timer)
  {
    led_timer = STATUS_LED_TIMER;
    emp_toggle_status_led();
  }
}

void init_tiva_board()
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize the Tiva board
 ******************************************************************************/
{
  // Set GPIO'S on Run Mode Clock Gating Control Register on PORTF, PORTE, PORTD, PORTC & PORTA
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5 | SYSCTL_RCGCGPIO_R4
      | SYSCTL_RCGCGPIO_R3 | SYSCTL_RCGCGPIO_R2 | SYSCTL_RCGCGPIO_R0;

  // Unlock PORTF
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0xFF;

  // GPIO Direction (GPIODR)
  GPIO_PORTF_DIR_R = 0x0E;    // 0b00001110
  // PORTD - Set the direction as output (PD6, PD2 & PD3).
  GPIO_PORTD_DIR_R = 0x4C;    // 0b01001100
  // PORTC - Set the direction as output (PC4-PC7)
  GPIO_PORTC_DIR_R = 0xF0;    // 0b11110000
  // PORTA - output
  GPIO_PORTA_DIR_R = 0x1E;
  // PORTE - input
  GPIO_PORTE_DIR_R = 0x00;

  // GPIO Digital Enable (GPIODEN)
  GPIO_PORTF_DEN_R = 0x1F;    // 0b00011111
  // Enable the GPIO pins for digital function (PD6, PD2 & PD3).
  GPIO_PORTD_DEN_R = 0x4C;    // 0b01001100
  // Enable the GPIO pins for digital function (PD4-PD7)
  GPIO_PORTC_DEN_R = 0xF0;    // 0b11110000
  GPIO_PORTA_DEN_R = 0xFF;    // 0b11111111    (PA0-PA7)
  GPIO_PORTE_DEN_R = 0x1F;    // 0b00001111    (PE0-PE3)

  // GPIO PullUp Resistor (GPIOPUR)
  GPIO_PORTF_PUR_R = 0x11;    // 0b00010001
  GPIO_PORTA_PUR_R = 0x1E;    // pullup til keyboard
}

void hardware_init()
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{

  // disable global interrupt
  disable_global_int();

  // Initialize the Tiva board
  init_tiva_board();
  init_digiswitch();
  init_systick();

  // Enable global interrupt
  enable_global_int();

}

/****************************** End Of Module *******************************/
