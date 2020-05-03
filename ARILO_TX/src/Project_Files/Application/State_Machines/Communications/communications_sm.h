/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             communications_sm.h                                        */
/*                                                                              */
/* DESCRIPTION:      State machine that controls communic. (contr. + telem.)    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef COMMUNICATIONS_SM_H_
#define COMMUNICATIONS_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../config_switches.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../System/HAL/hw_radio.h"
#include "../../../System/HAL/hw_general.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_CommunicationsSM {                     /* States Comm. State Machine*/
    INIT_COMM = 1,                                             /* Initial State */
    COMM_TX_RX,           			            /* Transceiver Mode */
    COMM_BINDING,                                               /* Binding Mode */

    MAX_STATE_COMMUNICATIONS_SM
};

enum Bitmasks_Commands {          /* Commands sent by to Control_CH on Receiver */
									       /* First 2 Bit reserved for Comm. SM */
    BM_MAN_AUT = 2,                           /* Bit0: Autopilot or Manual Mode */
    BM_ASSISTED_ACT,                                     /* Bit1: Assisted Mode */
    BM_EM_BYPASS,                             /* Bit2: Emergency signals bypass */
    /* Complete with other bit masks (maximum 8) */
};

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* ******* Buffers Layout (for info) ********     */

/* Transmission                                   */
/* [0] = CH0 (16 bit) + CH1 (16 Bit)              */
/* [1] = CH2 (16 bit) + CH3 (16 Bit)              */
/* [2] = CH4 (16 bit) + CH5 (16 Bit)              */
/* [3] = CH6 (16 bit) + CH7 (16 Bit)              */
/* [4] = CH8 (16 bit) + CH9 (16 Bit)              */
/* [5] = CH10 (16 bit) + CH11 (16 Bit)            */
/* [6] = NUM_CHANNELS (16 bit) + CONTROL (16 Bit) */
/* [7] = ADDRESS (32bit)                          */

/* Reception                                      */
/* [0] = rpm (16 bit) + battery (16 Bit)          */
/* [1] = distance (16 bit) + current (16 Bit)     */
/* [2] = CPU Load (16 Bit) + RSSI (16 Bit)        */
/* [3] = temperature (16 bit) + CONTROL (16 Bit)  */
/* [4] = ACCEL_X (16 bit) + ACCEL_Y (16 Bit)      */
/* [5] = ACCEL_Z (16 bit) + GYRO_X (16 Bit)       */
/* [6] = GYRO_Y (16 bit) + GYRO_Z (16 Bit)        */

/* *** SafeLink Buffers Layout (for info) ***     */

/* Transmission                                   */
/* [0] = CH0 (8 bit)                              */
/* [1] = CH1 (8 bit)                              */
/* [2] = CH2 (8 bit)                              */
/* [3] = CH3 (8 bit)                              */
/* [4] = CH4 (8 bit)                              */
/* [5] = CH5 (4 bit)                              */
/* [6] = CH6-7 ON/OFF + CHNum (6 bit)             */

/* Reception                                      */
/* [0] = Battery (16 Bit)                         */
/* [1] = GPS Latitude 1 (16 Bit)                  */
/* [2] = GPS Latitude 2 (16 Bit)                  */
/* [3] = GPS Latitude 3 (16 Bit)                  */
/* [4] = GPS Longitude 1 (16 Bit)                 */
/* [5] = GPS Longitude 2 (16 Bit)                 */
/* [6] = GPS Longitude 3 (16 Bit)                 */

/* Transceiver Source of Information */
#define NO_TRX       0
#define MAIN_TRX     1
#define SAFELINK_TRX 2

/* SafeLink required defines */
#define SERVO_MAXP 2000
#define SERVO_MINP 1000
#define CH_6_BIT 0
#define CH_7_BIT 1

/* Buffer TX/RX Size (in int32 units) */
#define TX_BUFF_SIZE 8
#define RX_BUFF_SIZE 7

/* Buffer SafeLink TX Size (in uint8 units) */
#define SL_TX_BUFF_SIZE 7
/* Buffer SafeLink RX Size (in int16 units) */
#define SL_RX_BUFF_SIZE 7

/* SafeLink required defines */
#define SERVO_CENTERP 1500

/* RF Link DOWN / UP */
#define LINK_DOWN 0
#define LINK_UP 1

/* Each unit are 80ms */
#define BIND_REQUEST_TIME 312

/* Mask to get the commands relevant to the Communication SM, bits 0-1 */
#define CMD_MASK_COMM 0x03

/* Commands: First two bits related to Communication SM */

/* Commands to RX */

/* Commands from RX */
#define CMDRX_NO_COMM 0
#define CMDRX_BIND_ACK 1

/* Test SafeLink Defines */
#define SL_TEST_DISABLE 0
#define SL_TEST_ENABLE  1

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Communications State Machine typedef */
typedef struct CommunicationsSMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
    /* Communication Status */
    uint8_t status_comm;
    /* Binding Counter */
    uint16_t bindcnt;
    /* Address currently used for communications */
    uint32_t address;
	/* Transmission Buffer */
	int32_t tBuffer[TX_BUFF_SIZE];
	/* Reception Buffer */
	int32_t rBuffer[RX_BUFF_SIZE];
	/* Transmission SafeLink Buffer */
	uint8_t sl_tBuffer[SL_TX_BUFF_SIZE];
	/* Reception SafeLink Buffer */
	int16_t sl_rBuffer[SL_RX_BUFF_SIZE];
    /* Control Command Received */
    uint8_t control;
    /* Main Transceiver for Telemetry */
    uint8_t main_trx;
} CommunicationsSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
CommunicationsSM communications_sm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_communications_sm                                      */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_communications_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            communications_sm_main                                      */
/*                                                                              */
/* DESCRIPTION:     Main function from the Communications State Machine         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void communications_sm_main(eventtype const *event);

#endif /* COMMUNICATIONS_SM_H_ */

/*
 * communications_sm.h
 *
 *  Created on: 26.09.2012
 *      Author: X-ArigitaJ
 */
