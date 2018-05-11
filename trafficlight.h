/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: trafficlight.h
*
* PROJECT....: Assignment 2
*
* DESCRIPTION: Headerfile for trafficlight.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 180307  Jes   Module created.
*
*****************************************************************************/

#ifndef _TRAFFICLIGHT_H
#define _TRAFFICLIGHT_H

/***************************** Include files *******************************/
#include "hardware.h"

/*****************************    Defines    *******************************/
#define TL_LED_GREEN  0b110
#define TL_LED_RED    0b011
#define TL_LED_YELLOW 0b101
#define TL_LED_R_Y    0b001
#define TL_LED_NONE   0b111

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

INT8U trafficlight(enum sw1_events sw1_event);
/*****************************************************************************
*   Input    : sw1_events
*   Output   : INT8U led
*   Function : Traffic light control - state machine which controls the three
*              states: Normal operation, Norwegian state and Emergency state.
******************************************************************************/

/****************************** End Of Module *******************************/
#endif

