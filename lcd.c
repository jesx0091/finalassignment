/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

// LCD instructions
#define lcd_clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_home            0b00000010          // return cursor to first position on first line
#define lcd_entrymode       0b00000110          // shift cursor from left to right on read/write
#define lcd_displayoff      0b00001000          // turn display off
#define lcd_displayon       0b00001100          // display on, cursor off, don't blink character
#define lcd_functionreset   0b00110000          // reset the LCD
#define lcd_functionset4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define upper_bits          0b11110000
#define lower_bits          0b00001111
#define LCD_SETCURSOR       0b10000000          // set cursor position

// LCD bits
#define clear               0b00000000          // clear all
#define lcd_RS              0b00000100          // RS-bit
#define lcd_E               0b00001000          // E-bit

// Data / character patterns
#define _0                  0b00110000
#define MAX_SEC             0x3A
#define MAX_SECOND          9

// Timers
#define TIM_1S              1000
#define TIM_10S             10000
#define TIM_1M              60000
#define TIM_10M             600000
#define TIM_1H              3600000

// LCD DDRAM setcursor func
#define LCD_FIRSTLINE       15
#define LCD_SECONDLINE      16
#define DDRAM_ADR_OFFSET    24
#define LCD_REALSECLINE     40


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U    dummy1;
INT16S   dummy2;
//INT32U   DELAY100MS = 160000;
INT32U   DELAY100MS = 320;
//INT32U   DELAY10MS = 16000;
INT32U   DELAY10MS = 160;
//INT32U   DELAY1MS = 1600;
INT32U   DELAY1MS = 16;
INT8U    first4bits;
INT8U    last4bits;

extern INT16U ticks; // sysTick

/*****************************   Functions   *******************************/
void delayms(INT32U DELAY)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  while( DELAY )
  {
    DELAY--;
  }
  return;
}

void write4bits( INT8U byte )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Skriver de fire MSB ud til PORTC (Kun fire MSB bliver læst, alle bliver sendt)
******************************************************************************/
{
  // Sæt data
  GPIO_PORTC_DATA_R &= clear;
  GPIO_PORTC_DATA_R |= byte;

  // Skriv
  //GPIO_PORTD_DATA_R |= lcd_RS;
  GPIO_PORTD_DATA_R |= lcd_E;
  delayms(DELAY1MS);
  GPIO_PORTD_DATA_R &= ~(lcd_E);
  //GPIO_PORTD_DATA_R &= ~(lcd_RS);
  delayms(DELAY1MS);
  return;
}

void writeinstruction( INT8U byte )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  /*
  first4bits = byte;
  first4bits &= upper_bits;

  last4bits = byte;
  last4bits &= lower_bits;
  */

  GPIO_PORTD_DATA_R &= ~(lcd_RS);       // Sæt RS
  GPIO_PORTD_DATA_R &= ~(lcd_E);        // Sæt E lav
  delayms(DELAY1MS);

  write4bits( byte );             // Send første fire bits
  write4bits( byte << 4 );         // Send sidste fire bits

  return;
}

void writedata( INT8U byte )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  /*
  first4bits = byte;
  first4bits &= upper_bits;

  last4bits = byte;
  last4bits &= lower_bits;
  */

  GPIO_PORTD_DATA_R |= lcd_RS;       // Sæt RS høj
  GPIO_PORTD_DATA_R &= ~(lcd_E);        // Sæt E lav
  delayms(DELAY1MS);

  write4bits( byte );             // Send første fire bits
  write4bits( byte << 4 );         // Send sidste fire bits

  return;
}

void lcd_init(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  // Lav som state machine?

  delayms( DELAY10MS );
  GPIO_PORTD_DATA_R &= ~(lcd_RS);       // Sæt RS lav
  GPIO_PORTD_DATA_R &= ~(lcd_E);        // Sæt E lav

  write4bits(lcd_functionreset);        // Step 1-4
  delayms(DELAY100MS);
  write4bits(lcd_functionreset);
  delayms(DELAY10MS);
  write4bits(lcd_functionreset);
  delayms(DELAY1MS);

  write4bits(0b00100000);      // sæt til 4-bit mode
  delayms(DELAY1MS);

  writeinstruction(0b00101000);
  delayms(DELAY1MS);

  writeinstruction(0b00001000);
  delayms(DELAY10MS);

  writeinstruction(0b00001100);
  delayms(DELAY1MS);

  writeinstruction(0b00000110);
  delayms(DELAY10MS);

  writeinstruction(0b00000001);
  delayms(DELAY1MS);

  return;
}

void lcd_setcursor(INT8U CURSORPOSITION)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Set cursor with respect to adress offset.
******************************************************************************/
{
  writeinstruction(0x02);               // cursor in top left corner

  if( CURSORPOSITION <= LCD_FIRSTLINE )
  {
    CURSORPOSITION += LCD_SETCURSOR;    // Move cursor to position on first line
    writeinstruction(CURSORPOSITION);
  }
  /*
  if( CURSORPOSITION >= LCD_SECONDLINE )    // Adds the offset
  {
    CURSORPOSITION += DDRAM_ADR_OFFSET;
  }
  if( CURSORPOSITION >= LCD_REALSECLINE ) // Move cursor to position on second line
  {
    CURSORPOSITION += LCD_SETCURSOR;
    writeinstruction(CURSORPOSITION);
  }
  */
  return;
}

void lcd_writedata_position(INT8U cursor, INT8U byte)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  lcd_setcursor(cursor);
  writedata(byte);
  return;
}

void lcd_seconds(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  static enum states
  {
    PAUSE, INIT, SECOND
  } state = INIT;

  static INT32U TIMER1 = TIM_10S;
  static INT32U TIMER2 = TIM_1S;
  static INT32U TIMER3 = TIM_10M;
  static INT32U TIMER4 = TIM_1M;

  static INT8U SECONDS1 = _0;
  static INT8U SECONDS2 = _0;
  static INT8U MINUTES1 = _0;
  static INT8U MINUTES2 = _0;

  if (!PAUSE)
  {
    switch (state)
    {
    case INIT:
      lcd_writedata_position(0, MINUTES1);
      lcd_writedata_position(1, MINUTES2);
      lcd_writedata_position(3, SECONDS1);
      lcd_writedata_position(4, SECONDS2);
      TIMER1--;
      TIMER2--;
      TIMER3--;
      TIMER4--;
      state = SECOND;
      break;
    case SECOND:
      TIMER1--;
      TIMER2--;
      TIMER3--;
      TIMER4--;
      if (!TIMER1)
      {
        TIMER1 = TIM_10S;
        SECONDS1++;
        if (SECONDS1 >= 0x36)
        {
          SECONDS1 = _0;
        }
        lcd_writedata_position(3, SECONDS1);
      }
      if (!TIMER2)
      {
        TIMER2 = TIM_1S;
        SECONDS2++;
        if (SECONDS2 >= MAX_SEC)
        {
          SECONDS2 = _0;
        }
        lcd_writedata_position(4, SECONDS2);
      }
      if (!TIMER3)
      {
        TIMER3 = TIM_10M;
        MINUTES1++;
        if (MINUTES1 >= 0x36)
        {
          MINUTES1 = _0;
        }
        lcd_writedata_position(0, MINUTES1);
      }
      if (!TIMER4)
      {
        TIMER4 = TIM_1M;
        MINUTES2++;
        if (MINUTES2 >= MAX_SEC)
        {
          MINUTES2 = _0;
        }
        lcd_writedata_position(1, MINUTES2);
      }
      break;
    default:
      break;
    }
  }
  return;
}

void lcd_display_time( void )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  lcd_writedata_position(2, 0b00111010);
  return;
}

void lcd_minutes(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  static enum states
  {
    PAUSE, INIT, MINUTE
  } state = INIT;

  static INT32U TIMER1 = TIM_10S;
  static INT32U TIMER2 = TIM_1S;

  static INT8U MINUTES1 = _0;
  static INT8U MINUTES2 = _0;

  if (!PAUSE)
  {
    switch (state)
    {
    case INIT:
      lcd_writedata_position(0b00000100, MINUTES1);
      lcd_writedata_position(0b00000101, MINUTES2);
      TIMER1--;
      TIMER2--;
      state = MINUTE;
      break;
    case MINUTE:
      TIMER1--;
      TIMER2--;
      if (!TIMER1)
      {
        TIMER1 = TIM_10S;
        MINUTES1++;
        if (MINUTES1 >= 0x36)
        {
          MINUTES1 = _0;
        }
        lcd_writedata_position(0b00000001, MINUTES1);
      }
      if (!TIMER2)
      {
        TIMER2 = TIM_1S;
        MINUTES2++;
        if (MINUTES2 >= MAX_SEC)
        {
          MINUTES2 = _0;
        }
        lcd_writedata_position(0b00000010, MINUTES2);
      }
      break;
    default:
      break;
    }
  }
  return;
}

void lcd_write(INT8U timestamp_event)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  if(timestamp_event)
  {
    writedata(timestamp_event);
  }
  return;
}

/****************************** End Of Module *******************************/












