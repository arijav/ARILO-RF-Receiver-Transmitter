/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlPPM_sm.c                                            */
/*                                                                              */
/* DESCRIPTION:      State machine that processes control events                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CONTROLPPM_SM_H_
#define CONTROLPPM_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../System/HAL/hw_radio.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

#define SERVO_TEST_OFF 0
#define SERVO_TEST_ON  1

/* Allowed maximum and minimum servo values in 0.5ms resolution */
#define SERVO_MAX_VAL  6000
#define SERVO_MIN_VAL  1000

/* State of the PPM train */
#define PPM_OK  0
#define PPM_NOK 1

#if(CTR_SERVO_TEST == SERVO_TEST_ON)
/* Servo gradient in us/s */
#define SERVO_GRADIENT 1400//666
#define TEST_DIR_DOWN  0
#define TEST_DIR_UP    1
#define TEST_MIN_US    1000
#define TEST_MAX_US    2000
#endif

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

enum States_ControlSM {                          /* States Control State Machine*/
    WAITING = 1,                           /* Stand By, waiting for first pulse */
	SYNCHRONIZING,                                      /* Sync Pulse detection */
    CHANNEL_SURVEY,                                           /* Channel Survey */
    PPM_TRAIN_PROCESS,             /* Channels properly received and processing */

    MAX_STATE_CONTROL_SM
};

enum Channels_ControlSM {
	ESC = 0,												     /* ESC Channel */
	SERVO1,
	SERVO2,
	SERVO3,
	SERVO4,
	SERVO5,
	SERVO6,
	SERVO7,
	SERVO8,
	SERVO9,
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Control State Machine typedef */
typedef struct ControlSMTag {
    /* Derived from basis State Machine */
    StateMachine statem;
    /* Received channels in the current PPM train */
    uint8_t received_ch;
    /* Received channels in the last PPM train */
    uint8_t received_ch_old;
    /* Flag that indicates if PPM train is corrupted */
    uint8_t flag_ppm_corrupted;
    /* Value Timer counter for pulse width calculation */
    uint16_t timer_val;
    /* Length in Timer counts for each channel */
    uint16_t channel[MAX_CHANNELS];
    /* Particular information from the Control State Machine */
    /* Cycle Number Timer counter for pulse width calculation */
    uint32_t timer_cycle;
    /* Length in Timer counts for last working channels */
    uint16_t channel_last_ok[MAX_CHANNELS];
    #if(CTR_SERVO_TEST == SERVO_TEST_ON)
    uint16_t test_value;
    uint8_t test_dir;
    uint8_t tested_ch;
    #endif
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
/* NAME:            init_controlPPM_sm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_controlPPM_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            control_sm_main                                             */
/*                                                                              */
/* DESCRIPTION:     Main function from the Control State Machine                */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void controlPPM_sm_main(eventtype const *event);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            setup_channels                                              */
/*                                                                              */
/* DESCRIPTION:     Sends the channels to the Communications State Machine      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void setup_channels(void);

#endif /* CONTROLPPM_SM_H_ */

/*
 * controlPPM_sm.h
 *
 *  Created on: 03/06/2012
 *      Author: arigita
 */
