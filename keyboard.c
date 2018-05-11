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
#define     X3      0x04      // tredje kolone
#define     X2      0x08      // anden kolone
#define     X1      0x10      // første kolone
#define     Y1      0x08      // Første række
#define     Y2      0x04      // Anden række
#define     Y3      0x02      // Tredje række
#define     Y4      0x01      // Fjerde række
#define     INPUT   0x0F      // Der er input
#define     CLEAR   0xE3      // Clear Port A

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

INT8U key_catch( INT8U x, INT8U y )
{
  const INT8U matrix[3][4] = {{'*','7','4','1'},
                              {'0','8','5','2'},
                              {'#','9','6','3'}};

  return( matrix[x-1][y-1] );

}

void readkeyboardtask(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Læser input på keyboard
******************************************************************************/
{
  static enum states
  {
    NONE, NOTUSED0, NOTUSED1, THIRDCOLUMN, SECONDCOLUMN, FIRSTCOLUMN             // States skal have nummer, som passer til port-udgange X1, X2 og X3...
  } state = NONE;

  static INT8U inputPortE = 0;
  INT8U x;
  INT8U y;
  static INT8U i = 0;
  INT8U pressed;            // skal ligges på en queue i stedet

  //if( (~(inputPortA) & (X1+X2+X3)) || inputPortE )    // Kommer der et input?

    switch( state )
    {
      case NONE:
        emp_clear_leds();

        GPIO_PORTA_DATA_R &= 0xE3;
        GPIO_PORTA_DATA_R |= (X3 << i);     // Kolonne 3 høj
        inputPortE = GPIO_PORTE_DATA_R;     // Læs rækker
        if( inputPortE & INPUT )            // Er der input på kolonne 3?
        {
          state = THIRDCOLUMN + i;
        }
        else
        {
            if( i < 3 )
            {
              ++i;
            }
            else
            {
                i = 0;
            }
        }

        /*
        GPIO_PORTA_DATA_R &= 0xE3;          // Sæt alle lav (nødvendigt her?)
        GPIO_PORTA_DATA_R |= X3;            // Kolonne 3 høj
        inputPortE = GPIO_PORTE_DATA_R;     // Læs rækker
        if( inputPortE & INPUT )            // Er der input på kolonne 3?
        {
          state = THIRDCOLUMN;
        }

        GPIO_PORTA_DATA_R &= 0xE3;
        GPIO_PORTA_DATA_R |= X2;            // Kolonne 2 høj
        inputPortE = GPIO_PORTE_DATA_R;     // Læs rækker
        if( inputPortE & INPUT )            // Er der input på kolonne 2?
        {
          state = SECONDCOLUMN;
        }
        */
        break;
      case SECONDCOLUMN:
        inputPortE = GPIO_PORTE_DATA_R;     // Læs rækker / tjek for input.
        x = 2;                              // Anden kolonne

        if( inputPortE & Y1 )               // Række 1?
        {
          y = 4;
          pressed = key_catch(x,y);
          lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y2 )        // Række 2? Osv...
        {
            y = 3;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y3 )
        {
            y = 2;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y4 )
        {
            y = 1;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else
        {
            state = NONE;
        }
        break;
      case THIRDCOLUMN:
        inputPortE = GPIO_PORTE_DATA_R;     // Læs rækker
        x = 3;

        if( inputPortE & Y1 )
        {
            y = 4;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y2 )
        {
            y = 3;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y3 )
        {
            y = 2;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y4 )
        {
            y = 1;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else
        {
            state = NONE;
        }
        break;
      case FIRSTCOLUMN:
        inputPortE = GPIO_PORTE_DATA_R;     // Læs rækker
        x = 1;

        if( inputPortE & Y1 )
        {
            y = 4;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y2 )
        {
            y = 3;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y3 )
        {
            y = 2;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else if( (inputPortE) & Y4 )
        {
            y = 1;
            pressed = key_catch(x,y);
            lcd_writedata_position(7, pressed);
        }
        else
        {
            state = NONE;
        }
        break;
    }
    return;
}


/****************************** End Of Module *******************************/












