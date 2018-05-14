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
#include "systick.h"

/*****************************    Defines    *******************************/
#define     ESC      0x1B

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern QueueHandle_t xHIDQueue;

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
  /* We have not woken a task at the start of the ISR. */
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  uint8_t out = 0;
  //const uint8_t esc = ESC;    / ved faktisk ikke lige hvad den her bruges til
  static uint32_t last_ticks = 0;

  // if A,B same -> CCW=0x10 else CW=0x01
  out = is_digi_A() == is_digi_B() ? 0x10 : 0x01;

  if (out)
  {
    // her skal der puttes et event i en que

  }

  if (GPIO_PORTA_IEV_R & BIT_5)    // er den sat til
  {
    // GPIO Interrupt Event (GPIOIEV)
    bit_clear( GPIO_PORTA_IEV_R, BIT_5);    // Set falling edge or low level
  }
  else
  {
    // GPIO Interrupt Event (GPIOIEV)
    bit_set( GPIO_PORTA_IEV_R, BIT_5);   // Set rising edge or a High level
  }
  // Clear int. for PA5
  bit_set(GPIO_PORTA_ICR_R, BIT_5);

  // portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
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
  bit_clear( GPIO_PORTA_IS_R, BIT_5);   // Set PA5 edge-sensitive

  //  Interrupt Both Edges (GPIOIBE)
  //bit_clear( GPIO_PORTA_IBE_R, BIT_5 | BIT_6);
  bit_clear( GPIO_PORTA_IBE_R, BIT_5); // do not interrupt both edges

  // GPIO Interrupt Event (GPIOIEV)
  bit_set( GPIO_PORTA_IEV_R, BIT_5);    // Set rising edge or a High level

  // GPIO Interrupt Mask (GPIOIM)
  //bit_set( GPIO_PORTA_IM_R, BIT_5 | BIT_6);     // Unmask interrupt for PA5 & PA6
  bit_set( GPIO_PORTA_IM_R, BIT_5);     // Unmask interrupt for PA5

  // Set priority on INT0
  bit_clear(NVIC_PRI0_R, NVIC_PRI0_INT0_M);
  bit_set(NVIC_PRI0_R, 0b101 << 5);               // Set interrupt priority 5

  // Enable NVIC interrupt 0
  bit_set(NVIC_EN0_R, 1);
}

