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
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*****************************    Defines    *******************************/
#define ACCOUNTINPUT    5
#define PININPUTDONE    9

xQueueHandle keyQueue;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/



//void inputtask(void *pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
/*
{
  // State machine i stedet?

  static INT8U position = 0;

  while (1)
  {
    INT8U received_key;
    INT8U string_sets[4];
    INT8U account[4];
    INT8U input[1];

    // Keyboard press received?
    if (keyQueue != 0)
    {
      // Receive a message on the created queue.  Block for 10 ticks if a
      // message is not immediately available.
      if (xQueueReceive(keyQueue, &(received_key), (portTickType ) 10))
      {

        switch(received_key)
        {
        case '1':
          input[0] = "1";
          lcd_writedata_position(6, input[0]);
          strcpy(account,input);
          lcd_writedata_position(7, account[0]);
          break;
          default:
            break;
        }



        INT8U key = received_key + '0';
        if( position < ACCOUNTINPUT )
        {
          lcd_writedata_position(6, key);                  //debug linje
          strcpy(string_sets,received_key);
          lcd_writedata_position(7, string_sets[0]);   //debug linje
          INT8U stringss[1] = "i";
          lcd_writedata_position(8, stringss[0]);   //debug linje
          position++;
        }
        else
        {
          for( int i = 0; i < ACCOUNTINPUT; i++)
          {
            //lcd_writedata_position((i+1), account.accountnr[i]);
          }
          // Send event to free task mutex for input stage
        }

      }
    }
  }
}
*/

/****************************** End Of Module *******************************/










