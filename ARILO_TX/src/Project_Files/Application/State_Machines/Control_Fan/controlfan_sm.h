/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlfan_sm.c                                            */
/*                                                                              */
/* DESCRIPTION:      State machine that controls the fan                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CONTROLFAN_SM_H_
#define CONTROLFAN_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../config_switches.h"

#if(ARILOTX_HWREV == ARILOTX_HW2)

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

enum States_ControlFan {                         /* States Control State Machine*/
	INIT_FAN = 1,              			                       /* Initial State */
	CONTROL_FAN,                                   /* Temperature Control State */

    MAX_STATE_FAN_SM
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Control Fan State Machine typedef */
typedef struct ControlFanSMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
} ControlFanSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
/* Control Buzzer State Machine */
ControlFanSM controlfan_sm;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_controlfan_sm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_controlfan_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlfan_sm_main                                          */
/*                                                                              */
/* DESCRIPTION:     Main function from the Fan State Machine                    */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void controlfan_sm_main(eventtype const *event);

#endif

#endif /* CONTROLFAN_SM_H_ */

/*
 * controlfan_sm.h
 *
 *  Created on: 04/03/2015
 *      Author: arigita
 */
