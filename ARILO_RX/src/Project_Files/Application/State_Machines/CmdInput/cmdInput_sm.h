/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             cmdInput_sm.h                                              */
/*                                                                              */
/* DESCRIPTION:      State machine that controls Push Buttoms.                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CMDINPUT_SM_H_
#define CMDINPUT_SM_H_

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
enum States_CmdInputSM {                       /* States CmdInput. State Machine*/
	INIT_CMDINPUT = 1,           			                   /* Initial State */
    BINDING_REQ,                                             /* Binding Request */
    FAILSAFESET_REQ,                                    /* Failsafe Set Request */
    NO_REQ,                                                       /* No Request */

    MAX_STATE_CMDINPUT_SM
};

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Time to recognize Binding Request (unit = 80ms) */
#define BIND_REQ_TIME 24
/* Time to recognize Failsafe Request (unit = 80ms) */
#define FAILSAFESET_REQ_TIME 48
/* Minimum time between two button push (unit = 80ms) */
#define STANDBY_TIME_PUSH 13

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* CmdInput State Machine typedef */
typedef struct CmdInputSMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
    /* CmdInput counter */
    uint16_t cnt;
} CmdInputSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
CmdInputSM cmdInput_sm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_cmdInput_sm                                            */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_cmdInput_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            cmdInput_sm_main                                            */
/*                                                                              */
/* DESCRIPTION:     Main function from the CmdInput State Machine               */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void cmdInput_sm_main(eventtype const *event);

#endif /* CMDINPUT_SM_H_ */

/*
 * cmdInput_sm.h
 *
 *  Created on: 15.11.2013
 *      Author: X-ArigitaJ
 */
