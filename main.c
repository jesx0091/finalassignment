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
#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "hardware.h"
#include "trafficlight.h"
#include "lcd.h"
#include "uart0.h"
#include "semphr.h"
#include "queue.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

#define BAUDRATE      19200
#define DATABITS      8
#define STOPBITS      1

xSemaphoreHandle xSemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

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

static void readkeyboard( void *pvParameters )
/*****************************************************************************
 *   Input    :  Process Parameters
 *   Output   :  -
 *   Function :  EMP board alive led task
 *****************************************************************************/
{
  while (1)
  {
    readkeyboardtask();
    vTaskDelay(5);
  }
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

static void write( void *pvParameters )
/*****************************************************************************
 *   Input    :  Process Parameters
 *   Output   :  -
 *   Function :  EMP board alive led task
 *****************************************************************************/
{
  while (1)
  {
    //lcd_display_time();   // Sets ':'
    lcd_seconds();
    delayms(400);        // Vil af en eller anden grund ikke køre med vTaskDelay
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
  return_value &= xTaskCreate(readkeyboard, "Read Keyboard",
                              USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);

  return_value &= xTaskCreate(emp_board_alive, "EMP Board Alive",
                              USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);

  return_value &= xTaskCreate(write, "Time", USERTASK_STACK_SIZE, NULL,
                              MED_PRIO, NULL);

  return_value &= xTaskCreate(UART, "UART", USERTASK_STACK_SIZE, NULL, MED_PRIO,
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
