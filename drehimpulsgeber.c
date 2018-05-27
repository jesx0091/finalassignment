/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: drehimpulsgeber.c
 *
 * PROJECT....: finalassignment
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * --------------------
 * May 15,  ESC    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "global.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "hardware.h"
#include "systick.h"
extern xQueueHandle Queue;

/*****************************    Defines    *******************************/
#define     ESC      0x1B
#define     CCW      0x10
#define     CW       0x01
#define     FALING   0x00
#define     RISING   0x01
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U last_int = FALING;
/*****************************   Functions   *******************************/

INT8U is_digi_p2_pressed(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : returns 0 if the integratet button in the digiswitch is pressed and 1 if not
 ******************************************************************************/
{
  return ((GPIO_PORTA_DATA_R & BIT_7) ? 0 : 1);
}

INT8U is_digi_A(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : returns 1 if the A pin of the digiswitch is high
 ******************************************************************************/
{
  return ((GPIO_PORTA_DATA_R & BIT_5) ? 0 : 1);
}

INT8U is_digi_B(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : returns 1 if the B pin of the digiswitch is high
 ******************************************************************************/
{
  return ((GPIO_PORTA_DATA_R & BIT_6) ? 0 : 1);
}

void digiswitch_handler(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{

      if (Queue == NULL)
      {
        /* Queue was not created and must not be used. */
        Queue = xQueueCreate(10, sizeof(struct _msg));
      }
  /* We have not woken a task at the start of the ISR. */
  //portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE; // test ???
  uint8_t out = 0;
  static uint32_t last_tick_count = 0;
  struct _msg msg;


  if (xTaskGetTickCount() - last_tick_count >= 1)
  {
    // if A,B same -> CCW=0x10 else CW=0x01
    out = is_digi_A() == is_digi_B() ? CCW : CW;
    if (out)
    {
      // her skal der puttes et event i en que

      // for checking if it works

      switch(out)
      {
      case CCW:
        //lcd_writedata_position(11, 'V');
        msg.event = DIGI_CCW;
        break;
      case CW:
        //lcd_writedata_position(11, 'H');
        msg.event = DIGI_CW;
        break;
      default:
        break;
      }
      msg.ch = 0;
      msg.function = DIGI_R;
      //sendmsgtoqueue(msg);
      xQueueSendToBack(Queue, &msg, 0);
    }
    last_tick_count = xTaskGetTickCount();
  }
  // GPIO Interrupt Event (GPIOIEV)
  // bit_flip( GPIO_PORTA_IEV_R, BIT_5);   // flips rising and falling edge trigger

  // Clear int. for PA5
  bit_set(GPIO_PORTA_ICR_R, BIT_5);

  //portEND_SWITCHING_ISR(xHigherPriorityTaskWoken); // test ???
}
void digi_p2_task(void *pvParameters)
/*****************************************************************************
 *   Header description
 ******************************************************************************/
{
  uint32_t timer = ticks();
  uint32_t last_timer = 0;
  uint8_t event = 0;

  struct _msg msg;

  static enum states
  {
    IDLE, FIRST_PRESS, FIRST_RELEASE, SECOND_PRESS, LONG_PRESS
  } state = IDLE;

  while (1)
  {
    timer = ticks();
    event = 0;

    switch (state)
    {
      case IDLE:
        if (is_digi_p2_pressed())       // if button pushed
        {
          state = FIRST_PRESS;
          last_timer = timer;
        }
        break;
      case FIRST_PRESS:
        if (timer >= (last_timer + 2000))          // if timeout
        {
          state = LONG_PRESS;
          event = HOLD;
        }
        else
        {
          if (!is_digi_p2_pressed())    // if button released
          {
            state = FIRST_RELEASE;
            last_timer = timer;
          }
        }
        break;
      case FIRST_RELEASE:
        if (timer >= (last_timer + 100))          // if timeout
        {
          state = IDLE;
          event = CLICK;
        }
        else
        {
          if (is_digi_p2_pressed())     // if button pressed
          {
            state = SECOND_PRESS;
            last_timer = timer;
          }
        }
        break;
      case SECOND_PRESS:
        if (timer >= (last_timer + 2000))          // if timeout
        {
          state = LONG_PRESS;
          event = HOLD;
        }
        else
        {
          if (!is_digi_p2_pressed())                    // if button released
          {
            state = IDLE;
            event = D_CLICK;
          }
        }
        break;
      case LONG_PRESS:
        if (!is_digi_p2_pressed())                 // if button released
          state = IDLE;
        break;
      default:
        break;
    }

    if (event)
    {

        msg.ch = 0;
        msg.event = event;
        msg.function = DIGI_SW;
        xQueueSendToBack(Queue, &msg, 0);


    }

    vTaskDelay(10);
  }
}
void init_digiswitch()
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize the digiswitch
 ******************************************************************************/
{
  // Interrupt Sense  (GPIOIS)
  //bit_clear( GPIO_PORTA_IS_R, BIT_5 | BIT_6);   // Set PA5 & PA6 edge-sensitive
  bit_clear( GPIO_PORTA_IS_R, BIT_5 );   // Set PA5 edge-sensitive

  //  Interrupt Both Edges (GPIOIBE)
  //bit_clear( GPIO_PORTA_IBE_R, BIT_5 | BIT_6);
  bit_set( GPIO_PORTA_IBE_R, BIT_5);

  // GPIO Interrupt Event (GPIOIEV)
  //bit_set( GPIO_PORTA_IEV_R, BIT_5 | BIT_6);    // Set rising edge or a High level

  // GPIO Interrupt Mask (GPIOIM)
  //bit_set( GPIO_PORTA_IM_R, BIT_5 | BIT_6);     // Unmask interrupt for PA5 & PA6
  bit_set( GPIO_PORTA_IM_R, BIT_5);     // Unmask interrupt for PA5 & PA6

  // Set priority on INT0
  bit_clear(NVIC_PRI0_R, NVIC_PRI0_INT0_M);
  bit_set(NVIC_PRI0_R, 0b101<<5);               // Set interrupt priority 5

  // Enable NVIC interrupt 0
  bit_set(NVIC_EN0_R, 1 );
}

