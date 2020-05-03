/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             safety_sm.h                                                */
/*                                                                              */
/* DESCRIPTION:      State machine that controls safety                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef SAFETY_SM_H_
#define SAFETY_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../System/HAL/hw_general.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_SafetySM {                            /* States Safety State Machine*/
	INIT_SAFETY = 1,           			                       /* Initial State */
	INIT_WD,                                             /* Init Watchdog State */
    REFRESH_WD,                                       /* Refresh Watchdog State */

    MAX_STATE_SAFETY_SM
};

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Safety State Machine typedef */
typedef struct SafetySMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
	/* Counter */
	uint8_t cnt;
} SafetySM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
SafetySM safety_sm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_safety_sm                                              */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_safety_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            safety_sm_main                                              */
/*                                                                              */
/* DESCRIPTION:     Main function from the Safety State Machine                 */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void safety_sm_main(eventtype const *event);

#endif /* SAFETY_SM_H_ */

/*
 * safety_sm.h
 *
 *  Created on: 22.02.2014
 *      Author: ArigitaJ
 */
