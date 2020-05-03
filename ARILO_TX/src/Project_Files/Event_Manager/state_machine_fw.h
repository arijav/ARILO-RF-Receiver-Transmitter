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
    CONTROLPPM_SM = 1,                                        /* Control_PPM SM */
    COMMUNICATIONS_SM,                                     /* Communications SM */
    DISPLAY_SM,                                                   /* Display SM */
    TELEMETRY_SM,                                               /* Telemetry SM */
    CMDINPUT_SM,                                                 /* CmdInput SM */
    CONTROLBUZZER_SM,                                      /* Control Buzzer SM */

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
#include "../Application/State_Machines/Control_PPM/controlPPM_sm.h"
#include "../Application/State_Machines/Communications/communications_sm.h"
#include "../Application/State_Machines/Display/display_sm.h"
#include "../Application/State_Machines/CmdInput/cmdInput_sm.h"
#include "../Application/State_Machines/Control_Buzzer/controlbuzzer_sm.h"
#include "../Application/State_Machines/Control_Fan/controlfan_sm.h"
#include "../Application/State_Machines/Telemetry/telemetry_sm.h"

#endif /* STATE_MACHINE_FW_H_ */
/*
 * state_machine.h
 *
 *  Created on: 03/06/2012
 *      Author: arigita
 */
