/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlCH_sm.h                                             */
/*                                                                              */
/* DESCRIPTION:      State machine that processes control events                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CONTROLCH_SM_H_
#define CONTROLCH_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../System/HAL/hw_radio.h"
#include "../../../System/HAL/hw_general.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_ControlCHSM {                      /* States Control. State Machine */
	INIT_CONTROLCH = 1,                                  /* Waiting for startup */
    WAITING_STARTUP,                    /* Waiting to receive commands from TX. */
    RECEIVE_CMD_COMM,           /* Waiting for Command Signal from the Comm. SM */
    MANUALM_RECEIVE_CHANNELS,   /* Waiting for Channels info from the Comm. SM. */
    MANUALM_PROCESS_CHANNELS,             /* Activate current buffered channels */
    MANUALM_SET_FAILSAFE,                                /* Sets Failsafe State */
    ASSISTEDM_PROCESS_CHANNELS,           /* Post-process and activate channels */
    AUTOPILOTM_RTH,                            /* Autopilot Return To Home Mode */
    AUTOPILOTM_EMERGENCY,                           /* Autopilot Emergency Mode */

    MAX_STATE_CONTROLCH_SM
};

enum Bitmasks_Commands {                       /* Commands sent by the Txmitter */
									       /* First 2 Bit reserved for Comm. SM */
    BM_MAN_AUT = 2,                           /* Bit0: Autopilot or Manual Mode */
    BM_ASSISTED_ACT,                                     /* Bit1: Assisted Mode */
    BM_EM_BYPASS,                             /* Bit2: Emergency signals bypass */
    /* Complete with other bit masks (maximum 8) */
};

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Channel mode: safe channel positions or normal channels */
#define CTR_SAFE_MODE   0
#define CTR_NORMAL_MODE 1

/* Type of Control: Manual, Autopilot */
#define CTR_MANUAL     0
#define CTR_AUTOPILOT  1

/* Default Channel that controls the Type of Control */
#define CTR_MODE_DEFAULT_CHANNEL 7 /* Default is channel 7 */

/* Status signals */
#define STATUS_EMERGENCY_OFF     0
#define STATUS_EMERGENCY_ON      1

/* Command masks */
/* Command message structure */
/* Bits 0,1 -> Mode (Manual, Assisted, Autopilot) */

/* Mask to get the commands relevant to the ControlCH SM, bits 2-4 */
#define CMD_MASK_CTR 0x1C

/* Command masks */
/* Command message structure */
/* Bits 2-4 -> Mode (Manual, Assisted, Autopilot) */

/* Macro to detect manual mode (Bit 2, value 0 if active) */
#define CMD_MODE_MANUAL ((1 << BM_MAN_AUT) & ~(control_sm.command))
/* Macro to detect autopilot mode (Bit 2, value 1 if active) */
#define CMD_MODE_AUTOPILOT ((1 << BM_MAN_AUT) & control_sm.command)
/* Macro to detect assisted mode, on (1) or off (0) */
#define CMD_MODE_ASSISTED ((1 << BM_ASSISTED_ACT) & control_sm.command)

/* Macro to bypass the emergency commands (1 active) */
#define CMD_EMERGENCY_BYPASS ((1 << BM_EM_BYPASS) & control_sm.command)


/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Control State Machine typedef */
typedef struct ControlSMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
	/* Particular information from the Control State Machine */
	/* Maximum channel number received (channels to be released) */
	uint8_t max_ch;
	/* Control Type Status: manual or autopilot */
	uint8_t ctr_type_status;
	/* Command received from the transmitter (through comm. sm) */
	uint8_t command;
	/* Length in Timer counts for each channel (channels to be released) */
	uint16_t channel[MAX_CHANNELS];
	/* Emergency Status (STATUS_EMERGENCY_ON active, STATUS_EMERGENCY_OFF inactive) */
	uint8_t emergency_status;
} ControlSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
/* Control State Machine */
ControlSM control_sm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_controlCH_sm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_controlCH_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlCH_sm_main                                           */
/*                                                                              */
/* DESCRIPTION:     Main function from the Control State Machine                */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void controlCH_sm_main(eventtype const *event);

#endif /* CONTROLCH_SM_H_ */

/*
 * controlCH_sm.h
 *
 *  Created on: 03/06/2012
 *      Author: arigita
 */
