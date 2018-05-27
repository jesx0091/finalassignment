/*****************************************************************************
 * University of Southern Denmark
 *
 * MODULENAME.: drehimpulsgeber.h
 *
 * PROJECT....: finalassignment
 *
 * DESCRIPTION:
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * --------------------
 * May 15, 2018  ESC    Module created.
 ******************************************************************************/



#ifndef DREHIMPULSGEBER_H_
#define DREHIMPULSGEBER_H_


/***************************** Include files *******************************/
#include "emp_type.h"

/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/
INT8U is_digi_p2_pressed(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : returns 0 if the integratet button in the digiswitch is pressed and 1 if not
******************************************************************************/

INT8U is_digi_A(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : returns 1 if the A pin of the digiswitch is high
******************************************************************************/
INT8U is_digi_B(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : returns 1 if the B pin of the digiswitch is high
******************************************************************************/

void init_digiswitch();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize the digiswitch
 ******************************************************************************/
void digi_p2_task(void *pvParameters);



#endif /* DREHIMPULSGEBER_H_ */
