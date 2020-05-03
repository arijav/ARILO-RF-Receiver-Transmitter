/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             state_machine_fw.h                                         */
/*                                                                              */
/* DESCRIPTION:      State machine framework                                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef STATE_MACHINE_FW_H_
#define STATE_MACHINE_FW_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

enum SMs {                                                    /* State Machines */
    CONTROLCH_SM = 1,                                         /* Control_PPM SM */
    CH_GENERATION_SM,                                     /* CH Generation LLSM */
    COMMUNICATIONS_SM,                                     /* Communications SM */
    TELEMETRYHUB_SM,                                        /* Telemetry HUB SM */
    CMDINPUT_SM,                                                 /* CmdInput SM */

    BROADCAST_SM                                        /* Used to reach all SM */
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/

/* State Machine Tag */
typedef struct StateMachineTag {
	/* State */
	uint8_t state;
}StateMachine;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
/* Debugging, registers number of times in each state */
//uint32_t state_counter_sm[BROADCAST_SM][10];

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             init_statem                                                */
/*                                                                              */
/* DESCRIPTION:      Init. of state machines                                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_statem(void);

/*------------------------------------------------------------------------------*/
/* Header-Files, State Machines (#include)                                      */
/*------------------------------------------------------------------------------*/
#include "../System/Low_Level_State_Machines/CH_Generation/CHGeneration_llsm.h"
#include "../Application/State_Machines/Control_CH/controlCH_sm.h"
#include "../Application/State_Machines/Communications/communications_sm.h"
#include "../Application/State_Machines/Telemetry_HUB/telemetryHUB_sm.h"
#include "../Application/State_Machines/CmdInput/cmdInput_sm.h"
#include "../Application/State_Machines/Safety/safety_sm.h"

#endif /* STATE_MACHINE_FW_H_ */
/*
 * state_machine.h
 *
 *  Created on: 03/06/2012
 *      Author: arigita
 */
