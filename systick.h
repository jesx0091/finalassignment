/*****************************************************************************
 * University of Southern Denmark
 *
 * MODULENAME.: systick.c
 *
 * PROJECT....:
 *
 * DESCRIPTION: sysTick module
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * --------------------
 * 16. mar. 2018  jorn    Module adopted from MOH systick
 *
 *****************************************************************************/

#ifndef _SYSTICK_H
#define _SYSTICK_H

/***************************** Include files *******************************/
#include <stdint.h>
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

uint32_t ticks( void );
/*****************************************************************************
 *   Input    : -
 *   Output   : systick value since last touch
 *   Function : Touch the systick timer
 ******************************************************************************/


/****************************** End Of Module *******************************/
#endif


