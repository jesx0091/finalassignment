/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: flow.c
 *
 * PROJECT....: lastassignment
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * May 15, 2018  ESC    Module created.
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
#include "lcd.h"
#include "semphr.h"
#include "queue.h"
#include "global.h"

/*****************************    Defines    *******************************/
#define F_ON        1
#define F_OFF       2
#define PULSES_P_L  1125
#define PRICE_P_L   10
#define X           1

extern xQueueHandle Queue;
xQueueHandle pulse_queue;
xQueueHandle flow_queue;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
typedef struct flow_msg
{
  uint8_t s_event;
  uint8_t p_event;
} flow_msg_t;
extern INT16U cash;
/*****************************   Functions   *******************************/

void fueling_task(void *pvParameters)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{

  flow_queue = xQueueCreate(10, sizeof(struct flow_msg));
  uint8_t pulse;
  INT32U pulses = 0;
  FP32 l_fueled = 0;
  FP32 spent = 0;
  struct flow_msg f_msg;
  struct _msg r_msg;
  static enum states
  {
    READY, HOOK_OFF, INITIAL_SHUNT, NORMAL, CLOSING_SHUNT, IDLE
  } state = READY;
  uint32_t timer = get_hdw_ticks();
  uint32_t last_timer = 0;

  while (1)
  {
    if (Queue != 0)
      xQueueReceive(Queue, &(r_msg), (portTickType ) 10);
    vTaskDelay(100);
    if (pulse_queue != 0)
    {
      xQueueReceive(pulse_queue, &(pulse), (portTickType ) 10);
      pulses++;
      l_fueled = pulses / PULSES_P_L;
      spent = l_fueled * PRICE_P_L;
    }

    f_msg.p_event = 0;
    f_msg.s_event = 0;

    timer = get_hdw_ticks();

    switch (state)
    {
    case READY:
      if (r_msg.function == SW2 && r_msg.event == CLICK)
        state = HOOK_OFF;
      break;
    case HOOK_OFF:
      if (r_msg.function == SW2 && r_msg.event == CLICK)
      {
        // realease semaphore
        state = READY;
      }
      if (r_msg.function == SW1 && r_msg.event == HOLD)
      {
        //shunt & pump = true
        f_msg.p_event = F_ON;
        f_msg.s_event = F_ON;
        state = INITIAL_SHUNT;
      }
      break;
    case INITIAL_SHUNT:
      if (r_msg.function == SW1 && r_msg.event == RELEASED)
      {
        // start 15 s timer
        last_timer = get_hdw_ticks();
        // shunt & pump = false
        f_msg.p_event = F_OFF;
        f_msg.s_event = F_OFF;
        state = IDLE;
      }
      if (l_fueled >= 1)
      {
        state = NORMAL;

        // shunt = false
        f_msg.s_event = F_OFF;
      }
      break;
    case NORMAL:
      if (r_msg.function == SW1 && r_msg.event == RELEASED)
      {
        // start 15 s timer
        last_timer = get_hdw_ticks();
        // pump = false
        f_msg.p_event = F_OFF;
        state = IDLE;
      }
      if (cash && cash - spent < X)
      {
        state = CLOSING_SHUNT;
        // shunt = true
        f_msg.s_event = F_ON;
      }
      break;
    case CLOSING_SHUNT:
      if (r_msg.function == SW1 && r_msg.event == RELEASED)
      {
        // start 15 s timer
        last_timer = get_hdw_ticks();
        // shunt & pump = false
        f_msg.p_event = F_OFF;
        f_msg.s_event = F_OFF;
        state = IDLE;
      }
      if (cash - spent == 0)
      {
        state = READY;
        // shunt & pump = false
        f_msg.p_event = F_OFF;
        f_msg.s_event = F_OFF;
        // release semaphore

        // ret state machine
      }
      break;
    case IDLE:
      if (r_msg.function == SW1 && r_msg.event == HOLD)
      {
        //shunt & pump = true
        f_msg.p_event = F_ON;
        f_msg.s_event = F_ON;
        state = INITIAL_SHUNT;
      }
      if (r_msg.function == SW2 && r_msg.event == CLICK)
      {
        // realease semaphore
        state = READY;
      }
      if(timer >= (last_timer + 1500));
      break;
    default:
      break;
    }

    if (f_msg.p_event || f_msg.s_event)
      xQueueSendToBack(flow_queue, &f_msg, 0);

  }
}
void flowmeter(void *pvParameters)
{
  pulse_queue = xQueueCreate(100, sizeof(uint8_t));
  portTickType xLastWakeTime;
  const portTickType shunt_freq = 44.4 / portTICK_RATE_MS;
  const portTickType normal_freq = 4.4 / portTICK_RATE_MS;
  static enum states
  {
    IDLE, SHUNT, NORMAL
  } state = IDLE;
  struct flow_msg r_msg;
  uint8_t pulse;
  while (1)
  {
    if (flow_queue != 0)
      xQueueReceive(flow_queue, &(r_msg), (portTickType ) 10);
    pulse = 0;

    switch (state)
    {
    case IDLE:
      if (r_msg.p_event == F_ON)
      {
        if (r_msg.s_event == F_ON)
        {
          xLastWakeTime = xTaskGetTickCount();
          state = SHUNT;
        }
        else
        {
          xLastWakeTime = xTaskGetTickCount();
          state = NORMAL;
        }
      }
      break;
    case SHUNT:
      vTaskDelayUntil(&xLastWakeTime, shunt_freq);
      // send puls på puls queue
      pulse = 1;
      if (r_msg.p_event == F_OFF)
      {
        state = IDLE;
      }
      else if (r_msg.s_event == F_OFF)
      {
        state = NORMAL;
      }
      break;
    case NORMAL:
      vTaskDelayUntil(&xLastWakeTime, normal_freq);
      // send puls på puls queue
      pulse = 1;
      if (r_msg.p_event == F_OFF)
      {
        state = IDLE;
      }
      else if (r_msg.s_event == F_ON)
      {
        state = NORMAL;
      }
      break;
    }
    if (pulse)
      xQueueSendToBack(pulse_queue, &pulse, 0);

  }
}

/****************************** End Of Module *******************************/

