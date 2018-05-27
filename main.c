/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: Last assignment, main module. No main.h file.
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 180511  Jes   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "hardware.h"
#include "FreeRTOS.h"
#include "task.h"
#include "global.h"
#include "keyboard.h"
#include "inputtask.h"
#include "lcd.h"
#include "uart0.h"
#include "semphr.h"
#include "queue.h"

/*****************************    Defines    *******************************/
xSemaphoreHandle xSemaphore;

xQueueHandle xQueue;
extern xQueueHandle keyQueue;    // INT8U med maks 8 elementer.

typedef struct{
  char accountnr[7];
  char pin[5];
} account;

account accounts[6];

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U productchoice;

/*****************************   Functions   *******************************/

/*****************************   Functions   *******************************/

int putChar()
/*****************************************************************************
 *   Input    :  -
 *   Output   :  Result
 *   Function :  putChar for FreeRTOS debug functionality.
 *****************************************************************************/
{
  return (0);
}

static void emp_board_alive( void *pvParameters )
/*****************************************************************************
 *   Input    :  Process Parameters
 *   Output   :  -
 *   Function :  EMP board alive led task
 *****************************************************************************/
{
  while (1)
  {
    if (xSemaphore != NULL)
    {
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY))
      {
        emp_toggle_status_led();
        vTaskDelay(150);
        xSemaphoreGive(xSemaphore);
        taskYIELD();
      }
    }
  }
}

static void timer( void *pvParameters )
/*****************************************************************************
 *   Input    :  Process Parameters
 *   Output   :  -
 *   Function :  EMP board alive led task
 *****************************************************************************/
{
  INT8U time[] = {'0','1','2','3','4','5','6','7','8','9','\n'};
  static int i = 0;
  while (1)
  {
    lcd_writedata_position(0, time[i]);
    i++;
    if( i > 9 )
    {
    i = 0;
    }
    vTaskDelay(1000/portTICK_RATE_MS);        // timing med delay her! Resten er ligegyldigt / debugging. Sat til 1000ms = 1s.
  }
}

static void UART( void *pvParameters )
/*****************************************************************************
 *   Input    :  Process Parameters
 *   Output   :  -
 *   Function :  UART
 *****************************************************************************/
{
  while (1)
  {
    uart0_sendchar(74);
    vTaskDelay(200);

    if (xSemaphore != NULL)
    {
      //if( xSemaphoreTake( xSemaphore, ( portTickType ) 100 ) == pdTRUE )
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY))
      {
        static INT8U i = 40;
        lcd_writedata_position(9, i);
        i++;
        xSemaphoreGive(xSemaphore);
        taskYIELD();
      }
    }
  }
}

static void queue_producer( void *pvParameters )
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function : Eksempel på queue.
 *****************************************************************************/
{
  static INT16U queue_var = 48; // sender '0' til lcd-skærmen.

  /* Create a queue capable of containing 8 INT16U values. */
  xQueue = xQueueCreate(8, sizeof(INT16U));

  if (xQueue == NULL)
  {
    /* Queue was not created and must not be used. */
  }

  if (xQueue != 0)
  {
    /* Send an unsigned long.  Wait for 10 ticks for space to become
     available if necessary. */
  }
    while (1)
    {
      if ( xQueueSendToBack( xQueue,
          ( void * ) &queue_var,
          ( portTickType ) 10 ) != pdPASS)
      {
        /* Failed to post the message, even after 10 ticks. */
      }
      queue_var++;
      vTaskDelay(25);     // udskriver lynhurtigt.
    }
}

static void queue_consumer(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function : queue consumer
 *****************************************************************************/
{
  while (1)
  {
    struct _msg received_msg;
    if (Queue != 0)
    {
      // Receive a message on the created queue.  Block for 10 ticks if a
      // message is not immediately available.
      if (xQueueReceive(Queue, &(received_msg), (portTickType ) 10))
      {
        // pcRxedMessage now points to the struct AMessage variable posted
        // by vATask.
        if(received_msg.function == DIGI_R)
        {
          switch(received_msg.event)
          {
          case DIGI_CCW:
            lcd_writedata_position(15, 'V');
            break;
          case DIGI_CW:
            lcd_writedata_position(15, 'H');
            break;
          }
        } else if(received_msg.function == SW1)
        {
          switch(received_msg.event)
          {
          case DIGI_CCW:
            lcd_writedata_position(15, 'V');
            break;
          case DIGI_CW:
            lcd_writedata_position(15, 'H');
            break;
          }
        }

      }
    }
    vTaskDelay(300);      // læser for langsomt, bare for eksemplets skyld.
  }
}

static void debug_testfunc(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function : Testfunktion. Alt må overskrives her.
 *****************************************************************************/
{
  while (1)
  {
  }
}

void inputtask(void *pvParameters)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
  // State machine i stedet?

  static INT8U countaccountnr = 5;
  static INT8U countpinnr = 3;
  static INT8U useraccount = 0;
  static INT8U firsttime = 1;
  static INT8U go_on = 0;
  INT8U tolcd;
  INT8U received_key;

  static enum states
  {
    INIT, INPUTCASH, INPUTACCOUNTNR, INPUTPINNR, PRODUCT, DONE
  } state = INIT;

  while (1)
  {
    if( state == INIT)
    {
      if (keyQueue != 0)
      {
        state = INPUTACCOUNTNR;
      }

    }
    // Keyboard press received?
    if (keyQueue != 0)
    {
      firsttime = 0;
      // Receive a message on the created queue.  Block for 10 ticks if a
      // message is not immediately available.
      if (xQueueReceive(keyQueue, &(received_key), (portTickType ) 10))
      {
        if (firsttime == 1)
        {
          char out[] = "AccNr: ";
          for (INT8U e = 0; e < sizeof(out); e++)
          {
            lcd_writedata_position((e), out[e]);
          }
        }

        if (state == INPUTACCOUNTNR)
        {
          if (countaccountnr == 0)
          {
            go_on = 1;
          }
          switch (received_key)
          {
          case '1':
            strcat(accounts[useraccount].accountnr, "1");
            countaccountnr--;
            break;
          case '2':
            strcat(accounts[useraccount].accountnr, "2");
            countaccountnr--;
            break;
          case '3':
            strcat(accounts[useraccount].accountnr, "3");
            countaccountnr--;
            break;
          case '4':
            strcat(accounts[useraccount].accountnr, "4");
            countaccountnr--;
            break;
          case '5':
            strcat(accounts[useraccount].accountnr, "5");
            countaccountnr--;
            break;
          case '6':
            strcat(accounts[useraccount].accountnr, "6");
            countaccountnr--;
            break;
          case '7':
            strcat(accounts[useraccount].accountnr, "7");
            countaccountnr--;
            break;
          case '8':
            strcat(accounts[useraccount].accountnr, "8");
            countaccountnr--;
            break;
          case '9':
            strcat(accounts[useraccount].accountnr, "9");
            countaccountnr--;
            break;
          default:
            break;
          }
          if (go_on == 1)
          {
            //vTaskDelay(150/portTICK_RATE_MS);
            char out[] = ".PIN:            ";
            for (INT8U e = 0; e < sizeof(out); e++)
            {
              lcd_writedata_position(e, out[e]);
            }
            state = INPUTPINNR;
            go_on = 0;
          }
          else
          {
            char out[] = "AccNr: ";
            for (INT8U e = 0; e < sizeof(out); e++)
            {
              lcd_writedata_position(e, out[e]);
            }
            for (INT8U i = 0; i < (sizeof(accounts[useraccount].accountnr) - 1);
                i++)
            {
              tolcd = accounts[useraccount].accountnr[i];
              lcd_writedata_position((i + 7), tolcd);
            }
          }
        }

        else if (state == INPUTPINNR)
        {
          if (countpinnr == 0)
          {
            go_on = 1;
          }

          switch (received_key)
          {
          case '1':
            strcat(accounts[useraccount].pin, "1");
            countpinnr--;
            break;
          case '2':
            strcat(accounts[useraccount].pin, "2");
            countpinnr--;
            break;
          case '3':
            strcat(accounts[useraccount].pin, "3");
            countpinnr--;
            break;
          case '4':
            strcat(accounts[useraccount].pin, "4");
            countpinnr--;
            break;
          case '5':
            strcat(accounts[useraccount].pin, "5");
            countpinnr--;
            break;
          case '6':
            strcat(accounts[useraccount].pin, "6");
            countpinnr--;
            break;
          case '7':
            strcat(accounts[useraccount].pin, "7");
            countpinnr--;
            break;
          case '8':
            strcat(accounts[useraccount].pin, "8");
            countpinnr--;
            break;
          case '9':
            strcat(accounts[useraccount].pin, "9");
            countpinnr--;
            break;
          default:
            break;
          }
          if (go_on == 1)
          {
            char out[] = ".1: 92, 2: 95      ";
            for (INT8U e = 0; e < sizeof(out); e++)
            {
              lcd_writedata_position(e, out[e]);
            }
            state = PRODUCT;
          }
          else
          {
            char out[] = ".PIN: ";
            for (INT8U e = 0; e < sizeof(out); e++)
            {
              lcd_writedata_position((e), out[e]);
            }
            for (INT8U i = 0; i < (sizeof(accounts[useraccount].pin) - 1); i++)
            {
              tolcd = accounts[useraccount].pin[i];
              lcd_writedata_position((i + 5), tolcd);
            }
            char out2[] = "         ";
            for (INT8U d = 0; d < sizeof(out2); d++)
            {
              lcd_writedata_position((d + 9), out2[d]);
            }
          }
        }

        else if (state == PRODUCT)
        {
          char out[] = ".1: 92, 2: 95      ";
          for (INT8U e = 0; e < sizeof(out); e++)
          {
            lcd_writedata_position(e, out[e]);
          }
          switch (received_key)
          {
          case '1':
            productchoice = 1;

            char out[] = ".92 valgt        ";
            for (INT8U e = 0; e < sizeof(out); e++)
            {
              lcd_writedata_position(e, out[e]);
            }
            vTaskDelay(1000 / portTICK_RATE_MS);
            state = DONE;
            break;
          case '2':
            productchoice = 2;
            char outt[] = ".95 valgt        ";
            for (INT8U e = 0; e < sizeof(outt); e++)
            {
              lcd_writedata_position(e, outt[e]);
            }
            vTaskDelay(1000 / portTICK_RATE_MS);
            state = DONE;
            break;
          case '3':
            productchoice = 3;
            state = DONE;
            break;
          default:
            break;
          }
        }

        else if (state == DONE)
        {
          countaccountnr = 5;
          countpinnr = 3;
          useraccount++;
          firsttime = 1;
          go_on = 0;
          state = INPUTACCOUNTNR;
        }
      }
    }
  }
}

static void setupHardware( void )
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here
  //enum sw1_events sw1_event;
  //INT8U lol[2] = { '1', '\0' };     // Forsøg med at sende parameter til initialisering af parity.

  uart0_init(BAUDRATE, DATABITS, STOPBITS, 'n');
  hardware_init();      // Init the hardware
  emp_clear_leds();     // Turn off EMP leds // RETTET. Fjernet ~.

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
}

int main( void )
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : The super loop.
 ******************************************************************************/
{
  portBASE_TYPE return_value = pdTRUE;

  setupHardware();

  xSemaphore = xSemaphoreCreateMutex();

  // Start the tasks.
  // ----------------
  return_value &= xTaskCreate(readkeyboardtask, "Read Keyboard",
                              USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);

  return_value &= xTaskCreate(emp_board_alive, "EMP Board Alive",
                              USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);

  //return_value &= xTaskCreate(timer, "Time", USERTASK_STACK_SIZE, NULL,
  //                            MED_PRIO, NULL);

  //return_value &= xTaskCreate(UART, "UART", USERTASK_STACK_SIZE, NULL, MED_PRIO,
  //                            NULL);

  //return_value &= xTaskCreate(queue_producer, "Queue eksempel1", USERTASK_STACK_SIZE, NULL, MED_PRIO,
  //                            NULL);

  //return_value &= xTaskCreate(queue_consumer, "Queue eksempel2", USERTASK_STACK_SIZE, NULL, MED_PRIO,
  //                            NULL);

//  return_value &= xTaskCreate(debug_testfunc, "Debugging", USERTASK_STACK_SIZE, NULL, MED_PRIO,
//                              NULL);

  return_value &= xTaskCreate(inputtask, "Input", USERTASK_STACK_SIZE, NULL, MED_PRIO,
                              NULL);


  lcd_init();           // Init the LCD-screen

  if (return_value != pdTRUE)
  {
    GPIO_PORTD_DATA_R &= 0xBF;  // Turn on status LED.
    while (1)
      ;  // cold not create one or more tasks.
  }

  // Start the scheduler.
  // --------------------

  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
