/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             telemetry_sm.h                                             */
/*                                                                              */
/* DESCRIPTION:      State machine responsible for the telemetry                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef TELEMETRY_SM_H_
#define TELEMETRY_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../System/HAL/hw_general.h"
#include "../../../System/HAL/hw_radio.h"
#include "../../../System/HAL/hw_signalprocessor.h"
#include "../../../config_switches.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_TelemetrySM {                     /* States Telemetry State Machine */
	INIT_TELEMETRY = 1,           			                   /* Initial State */
    STANDBY_TEL,                                                /* Standby mode */
	SENDING_TEL,                                           /* Sending Telemetry */

    MAX_STATE_TELEMETRY_SM
};

enum Outputs_BasicTelemetrySM {                             /* Outputs Telemetry SM */
    /* Basic Telemetry Information */
	TEL_VOLTAGE1 = 0,                                              /* Voltage Batt. */
    TEL_VOLTAGE2,           			                       /* Alternative Volt. */
    TEL_LINK_QUALITY_DOWN,                                 /* Link Quality DOWNlink */
    TEL_LINK_QUALITY_UP,                                     /* Link Quality UPlink */
    MAX_OUTPUTS_BASICTEL,                           /* Max. Outputs Basic Telemetry */
};

enum ExtendedTelemetryFrameIndex {                /* Extended Telemetry Frame Index */
	TEL_EXT_FRAME1 = 0,
	TEL_EXT_FRAME2,
	TEL_EXT_FRAME3,
	TEL_NUM_EXTFRAMES
};

enum Outputs_ExtendedTelemetryFR1SM {                        /* Outputs Telemetry SM */
    /* Extended Telemetry Information */
    TEL_ACCX = 0,
    TEL_ACCY,
    TEL_ACCZ,
    TEL_ALTITUDE1,
    TEL_ALTITUDE2,
    TEL_TEMPERATURE1,
    TEL_TEMPERATURE2,
    TEL_VOLT,
    TEL_VAMPS1,
    TEL_VAMPS2,
    TEL_CURRENT,
    TEL_RPM,
    MAX_OUTPUTS_EXTTELFR1
};

enum Outputs_ExtendedTelemetryFR2SM {                        /* Outputs Telemetry SM */
    /* Extended Telemetry Information */
    TEL_COURSE1 = 0,
    TEL_COURSE2,
    TEL_LATITUDE1,
    TEL_LATITUDE2,
    TEL_LATITUDE3,
    TEL_LONGITUDE1,
    TEL_LONGITUDE2,
    TEL_LONGITUDE3,
    TEL_GPS_SPEED1,
    TEL_GPS_SPEED2,
    TEL_GPS_ALTITUDE1,
    TEL_GPS_ALTITUDE2,
    TEL_FUEL_LEVEL,
    MAX_OUTPUTS_EXTTELFR2
};

enum Outputs_ExtendedTelemetryFR3SM {                        /* Outputs Telemetry SM */
    /* Extended Telemetry Information */
	TEL_DATE_MONTH = 0,
	TEL_YEAR,
	TEL_HOUR_MINUTE,
	TEL_SECOND,
    MAX_OUTPUTS_EXTTELFR3
};

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Configure use of TX Temp or RX Temp */
#define USE_TX_TEMP 0
#define USE_RX_TEMP 1

/* Period time to send basic telemetry (units are 10ms) */
#define TEL_BASIC_PERIOD 2
#define TEL_USR_PERIOD 3

/* Period in which telemetry info has to be received, otherwise values reset */
/* Units are 10ms */
#define TEL_RESET_PERIOD 50

/* Basis Index Telemetry Measurements in Local Buffer*/
#define TEL_BASIC_INDEX 0
#define TEL_EXT1_INDEX MAX_OUTPUTS_BASICTEL
#define TEL_EXT2_INDEX (TEL_EXT1_INDEX + MAX_OUTPUTS_EXTTELFR1)
#define TEL_EXT3_INDEX (TEL_EXT2_INDEX + MAX_OUTPUTS_EXTTELFR2)
#define TEL_L_BUFFER (TEL_EXT3_INDEX + MAX_OUTPUTS_EXTTELFR3)

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Communications State Machine typedef */
typedef struct TelemetrySMTag {
	/* Derived from basis State Machine */
	StateMachine statem;
	/* Pointer to TX Buffer */
	uint8_t *tel_txbuff;
	/* Pointer to RX Buffer */
	uint8_t *tel_rxbuff;
	/* Current frame length in bytes */
	uint8_t frame_length;
	/* Buffer with Telemetry data */
	int32_t tel_buff[TEL_L_BUFFER];
	/* Telemetry Counter (to decide when to send frames) */
	uint32_t tel_ctr;
	/* Reset counter (resets measurements if no info in time window) */
	uint8_t reset_ctr;
	/* User Data Buff with Bytes already Stuffed */
	uint8_t usr_data_buff[9];
	/* Index of Info to be sent for each Frame as User Data */
	uint8_t frame_telusr[TEL_NUM_EXTFRAMES];
	/* Frame Selector (to decide which frame info comes now) */
	uint8_t frame_ctr;
	/* Index to control in User Data if info from telem. value is pending */
	uint8_t index_usrd_pend;
} TelemetrySM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
TelemetrySM telemetry_sm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_telemetry_sm                                           */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_telemetry_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetry_sm_main                                           */
/*                                                                              */
/* DESCRIPTION:     Main function from the Telemetry State Machine              */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void telemetry_sm_main(eventtype const *event);

#endif /* TELEMETRY_SM_H_ */

/*
 * telemetry_sm.h
 *
 *  Created on: 07.12.2013
 *      Author: X-ArigitaJ
 */
