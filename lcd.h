/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _EMP_H
  #define _EMP_H

/***************************** Include files *******************************/
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

extern void test2(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void delayms( INT32U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void lcd_init(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void writeinstruction( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void write4bits( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void lcd_write( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void lcd_clock( void );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void writedata( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void lcd_writedata_position( INT8U, INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

/****************************** End Of Module *******************************/
#endif

