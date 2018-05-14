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
#include "task.h"

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

  /* We have not woken a task at the start of the ISR. */
  //portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE; // test ???
  uint8_t out = 0;
  static uint32_t last_tick_count = 0;

  /*
   switch (last_int)
   {
   case FALING:
   if (is_digi_A())
   {
   if (is_digi_A() == is_digi_B())
   {
   out = CCW;
   lcd_writedata_position(15, 'k');
   }
   else
   {
   out = CW;
   lcd_writedata_position(14, 'k');
   }
   }
   last_int = RISING;
   break;
   case RISING:
   if (!(is_digi_A()))
   {
   if (is_digi_A() == is_digi_B())
   {
   out = CCW;
   lcd_writedata_position(13, 'k');
   }
   else
   {
   out = CW;
   lcd_writedata_position(12, 'k');
   }
   }
   last_int = FALING;
   break;
   } */


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
        lcd_writedata_position(11, 'V');
        break;
      case CW:
        lcd_writedata_position(11, 'H');
        break;
      default:
        break;
      }

    }
    last_tick_count = xTaskGetTickCount();
  }
  // GPIO Interrupt Event (GPIOIEV)
  // bit_flip( GPIO_PORTA_IEV_R, BIT_5);   // flips rising and falling edge trigger

  // Clear int. for PA5
  bit_set(GPIO_PORTA_ICR_R, BIT_5);

  //portEND_SWITCHING_ISR(xHigherPriorityTaskWoken); // test ???
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

