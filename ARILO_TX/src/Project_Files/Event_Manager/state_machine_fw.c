/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             state_machine_fw.c                                         */
/*                                                                              */
/* DESCRIPTION:      State machine framework                                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "state_machine_fw.h"
#include "config_switches.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
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
extern void init_statem()
{
	/* Initializes every State Machine */
	init_controlPPM_sm();
	init_communications_sm();
	init_display_sm();
	init_cmdInput_sm();
	init_controlbuzzer_sm();
	init_telemetry_sm();
    #if(ARILOTX_HWREV == ARILOTX_HW2)
	init_controlfan_sm();
    #endif
};

/*
 * state_machine_fw.c
 *
 *  Created on: 05.06.2012
 *      Author: X-ArigitaJ
 */

