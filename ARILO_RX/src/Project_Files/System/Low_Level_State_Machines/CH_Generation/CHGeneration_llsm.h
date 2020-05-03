/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             CHGeneration_llsm.c                                        */
/*                                                                              */
/* DESCRIPTION:      State machine that generates PWMs for 8 Channels in Servo  */
/*                   format. This SM does not use the Event Manager and         */
/*                   is triggered directly by a Hardware Interruption           */
/*                   for performance reasons                                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CHGENERATION_LLWM_H_
#define CHGENERATION_LLWM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Maximum number of physical channels */
#define MAX_CH_SR 8

/* Timer 1 Modes, Channel = period of 4.096ms, Pause = period of 30.517ms */
#define CHANNEL_MODE 0

#define PAUSE_MODE 1

/* Type of channels/pause, 0 safe, 1 normal */
#define SAFE_TYPE 0

#define NORMAL_TYPE 1

/* Maximum number of cycles (of 20ms) without commands */
#define NO_COMM_MAX_PERIOD 50

/* Control channels released for Low Level State Machine Chanel Generator */
#define CTR_NOT_RELEASED 0
#define CTR_RELEASED     1

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

enum States_CHGeneration_llsm {     /* States CH Generation LLSM Low Level State Machine */
	INIT_CHANNELS_SAFEPOS = 1,
	CHANNELS_ACT,

    MAX_STATE_CHGENERATION_LLSM
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* CH Generation State Machine typedef */
typedef struct CHGenerationTag {
	/* Channel command values */
	uint16_t channel[MAX_CH_SR];
	/* Length in Timer counts for each channel (intermediate channel buffer) */
	uint16_t buff_channel[MAX_CH_SR];
	/* Failsafe channel command values */
	uint16_t fsafe_channel[MAX_CH_SR];
	/* Counter, periods without commands from the ControlCH State Machine */
	uint16_t counter_no_cmds;
	/* state */
	uint8_t state;
	/* Channel to be processed */
	uint8_t ch_process;
	/* Number of channels to be processed acc. to command */
	uint8_t num_channels;
	/* Number of channels to be processed acc. to command */
	uint8_t num_buffchannels;
	/* Flag released channels */
	uint8_t released;
	/* Emergency Flag */
	uint8_t emergency;
	/* Flag to detect if eeprom has been set since startup */
	uint8_t eeprom_set;
} CHGeneration;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
/* Control State Machine */
volatile CHGeneration CHGeneration_llsm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_init                                      */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_SetChReleasedFlag                         */
/*                                                                              */
/* DESCRIPTION:     Sets ch released flags                                      */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_SetChReleasedFlag(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_UpdateBuffChannel                         */
/*                                                                              */
/* DESCRIPTION:     Updates buffered channels                                   */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_UpdateBuffChannel(uint8_t channel, uint16_t value);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_BuffSetChNumber                           */
/*                                                                              */
/* DESCRIPTION:     Sets ch Number Buff Channels                                */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_BuffSetChNumber(uint8_t nchannels);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_SetFailsafe                               */
/*                                                                              */
/* DESCRIPTION:     Sets failsafe ch.                                           */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_SetFailsafe(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_SetFailsafe                               */
/*                                                                              */
/* DESCRIPTION:     Sets failsafe ch.                                           */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_SetFailsafe(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_EmergencyFailsafe                         */
/*                                                                              */
/* DESCRIPTION:     Release Failsafe Channels.                                  */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_EmergencyFailsafe(uint8_t flag);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_main                                      */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_main(void);

#endif /* CHGENERATION_LLWM_H_ */

/*
 * CHGeneration_llsm.h
 *
 *  Created on: 03/06/2012
 *      Author: arigita
 */
