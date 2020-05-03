/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             display_sm.h                                               */
/*                                                                              */
/* DESCRIPTION:      State machine that controls the display                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef DISPLAY_SM_H_
#define DISPLAY_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
//#include "../../../System/Modules/LCD/hw_lcd.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_DisplaySM {                          /* States Display State Machine*/
	FIRST_SCREEN = 1,                       /* Process Information First Screen */
	SECOND_SCREEN,                         /* Process Information Second Screen */
	THIRD_SCREEN,                           /* Process Information Third Screen */

    MAX_STATE_DISPLAY_SM
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Display State Machine typedef */
typedef struct DisplaySMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
} DisplaySM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
DisplaySM display_sm;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_display_sm                                             */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_display_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_sm_main                                             */
/*                                                                              */
/* DESCRIPTION:     Main function from the Display State Machine                */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_sm_main(eventtype const *event);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_info _ms                                            */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the main screen        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_info_ms(uint8_t source, int32_t info);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_info _ss                                            */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the second screen      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_info_ss(uint8_t source, int32_t info);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            display_info _ts                                            */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the third screen       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void display_info_ts(uint8_t source, int32_t info);

#endif /* DISPLAY_SM_H_ */

/*
 * display_sm.h
 *
 *  Created on: 22.01.2013
 *      Author: ArigitaJ
 */
