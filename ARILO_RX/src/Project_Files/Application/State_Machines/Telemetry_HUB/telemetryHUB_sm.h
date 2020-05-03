/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             telemetryHUB_sm.h                                          */
/*                                                                              */
/* DESCRIPTION:      State machine that processes the telemetry information     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef TELEMETRYHUB_SM_H_
#define TELEMETRYHUB_SM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../Event_Manager/event_manager.h"
#include "../../../Event_Manager/state_machine_fw.h"
#include "../../../System/HAL/hw_general.h"
#include "../../../System/HAL/hw_signalprocessor.h"
#include "../../../Lib/sw_filter.h"
#include "../../../config_switches.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

#if(ARILORX_HWREV == ARILORX_HW1)
/* Compiler Switch for RPM Measurement Method */
#define RPM_TIMESTAMPS_METHOD 0
#define RPM_TIMEWINDOW_METHOD 1
#endif

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_TelemetryHUBSM {                 /* States Telemetry. State Machine */
	/* ADC Sub-SM States */
	INIT_TELEMETRY = 1,                                 /* Initial State Telem. */
    #if(ARILORX_HWREV == ARILORX_HW1)
	MEASURE_BATTERY,                                     /* Battery measurement */
	MEASURE_CURRENT,                                     /* Current measurement */
	MEASURE_TEMPERATURE,                             /* Temperature measurement */
	MEASURE_VREF,                               /* Measure internal voltage ref */

	/* Pulse Measurement Sub-SM States */

    #if(RPM_MEAS_METHOD == RPM_TIMEWINDOW_METHOD)
	CALCULATE_TIMEWINDOWS_RANGE,  /* Calculate Array Sizes for RPM Measurements */
	CALCULATE_TIMEWINDOWS_RPM,    /* Calculate Array Sizes for RPM Measurements */
    #elif(RPM_MEAS_METHOD == RPM_TIMESTAMPS_METHOD)
	CALCULATE_RPM,                        /* Calculate RPM for all Hall sensors */
    #endif
	SEND_RPM_MEASUREMENTS,						   /* RPM and Sense calculation */
    #endif

	/* IMU Measurement Sub-SM States */
	PROCESS_IMU_MEASUREMENTS,      /* Receive and send IMU Measurements per I2C */

	/* USART Information Request */
	GET_DISTANCE,                                               /* Get Distance */
	SEND_DISTANCE,                                             /* Send Distance */

    MAX_STATE_TELEMETRYHUB_SM
};

/*------------------------------------------------------------------------------*/
/* More Defines                                                                 */
/*------------------------------------------------------------------------------*/

/* Distance RX Conversion Factor */
#define RX_CONV_FACT 2.54

#if(ARILORX_HWREV == ARILORX_HW1)
/* Number of calibration measurements */
#define NUM_CAL 100

/* Minimum pulse number to calculate RPM since last measurement */
#define MINIMUM_PULSES 100

/* Maximum time window to detect a revolution (if none RPM = 0) */
/* Each unit are 10ms */
#define MAX_TIME_REV 50

/* Time Constants for filtering purposes (in ms) */
#define TC_BATTERY 100
#define TC_CURRENT 250
#define TC_DISTANCE 10
#define TC_TEMPERATURE 100
#define TC_VREF 100

/* Time difference between samples (in ms) */
#define T_DIFF_TEL 10

/* Number of samples for FIR Filtering */
#define N_SAMPLES 5
#define N_SAMPLES_CURR 15

/* Maximum time for each measurement (each unit are 10ms) */
#define MAX_TMEAS 5

/* Defined required for RPM Measurement */
/* Required for Pulse Array */
#define NUM_PULS 0
#define TIME_PULS 1
#define MAX_Y_ARRAY_PULSES 2
#define MAX_X_ARRAY_PULSES 100
/* Required for Tail array */
#define T10MS                0
#define T100MS               1
#define T500MS               2
#define T1S                  3
#define TOTAL_TIMEWINDOWS_X  4
#define TAIL                 0
#define WINDOW_LENGTH        1
#define TOTAL_TIMEWINDOWS_Y  2

/* Period for sending RPM Measurements (each unit are 10ms) */
#define RPM_SEND_PERIOD 10

/* Period between RPM calculations (each unit are 10ms) */
#define RPM_PERIOD 10

/* Relationship between pulses and wheel revolutions */
/* (DEPENDANT OF CONFIGURATION OF HALL SENSOR) */
#define PULSE_REV 5

/* Minimum amount of pulses to calculate RPM */
#define MIN_PULSES 20

/* Maximum period between pulses for RPM Calculation (above that rpm = 0) */
/* Units are 10ms, default max is 1s */
#define MAX_RPM_PERIOD 100

/* Maximum RPM */
#define MAX_RPM 65535
#endif

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Telemetry State Machine typedef */
typedef struct TelemetryHUBSMTag {
    #if(ARILORX_HWREV == ARILORX_HW1)
	/* Derived from basis State Machine, Sub-SM for ADC measurements */
	StateMachine statemADC;
	/* Derived from basis State Machine, Sub-SM for pulse measurements */
	StateMachine statemPulse;
    #endif
	/* Derived from basis State Machine, Sub-SM for IMU measurements */
	StateMachine statemIMU;
	/* Derived from basis State Machine, Sub-SM for Usart Distance measurements */
	StateMachine statemUSART;
	/* Particular information from the Telemetry State Machine */
} TelemetryHUBSM;

#if(ARILORX_HWREV == ARILORX_HW1)
/* Telemetry State Machine typedef */
typedef struct RPMTag {
#if(RPM_MEAS_METHOD == RPM_TIMEWINDOW_METHOD)
	/* Pulse Array */
	uint32_t pulse_array[MAX_X_ARRAY_PULSES][MAX_Y_ARRAY_PULSES];
	/* Head Array */
	uint8_t head;
	/* Tail array for 10ms, 100ms, 500ms and 1s calculations */
	uint8_t tail_array[TOTAL_TIMEWINDOWS_X][TOTAL_TIMEWINDOWS_Y];
	/* RPM Array */
	uint16_t rpm_array[TOTAL_TIMEWINDOWS_X];
#elif(RPM_MEAS_METHOD == RPM_TIMESTAMPS_METHOD)
	/* RPM Array */
	uint16_t rpm;
#endif
} HallRPM;
#endif

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
/* Telemetry State Machine */
TelemetryHUBSM telemetry_sm;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_telemetryHUBm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void init_telemetryHUB_sm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetryHUB_sm_main                                        */
/*                                                                              */
/* DESCRIPTION:     Main function from the Telemetry State Machine              */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void telemetryHUB_sm_main(eventtype const *event);

#endif /* TELEMETRYHUB_SM_H_ */


/*
 * telemetryHUB_sm.h
 *
 *  Created on: 04.03.2013
 *      Author: ArigitaJ
 */
