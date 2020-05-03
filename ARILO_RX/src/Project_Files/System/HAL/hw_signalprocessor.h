/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_signalprocessor.h                                       */
/*                                                                              */
/* DESCRIPTION:      Controls the input measurements                            */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_SIGNALPROCESSOR_H_
#define HW_SIGNALPROCESSOR_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "hw_general.h"
#include "config_switches.h"
#include "../Modules/MCU/hw_ADC.h"
#include "../Modules/MCU/hw_ports.h"
#include "../Modules/MCU/hw_usart.h"
#include "../Modules/RF/hw_hoperf.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* ADC Channel mapping */
#define ACS758_S ADC_CH9
#define TEMP_S ADC_CH4
#define VBAT_S ADC_CH8
#define VREF_S ADC_VREF
#define VTMP_S ADC_TEMP

/* State of the measurement */
#define M_COMPLETED 1
#define M_INCOMPLETED 0

/* Reference voltage for the conversions in mV */
/* TODO: Calibrate */
#define REF_VOLTAGE_CALIB (uint16_t) 5180
#define REF_VOLTAGE_INT (uint16_t) 1100

/* Conversion factor for Battery measurements */
#define CONV_FACT_BATT (float)6.6

/* For Current calculations */
/* mAmper per mV */
#define CURR_A_MV (uint16_t) 25
/* Offset for 0 Amper (it should be arrount 0,12 * Vcc (in this case arround 621,6). Empirical value is: */
#define CURR_OFFSET (uint16_t) 623

/* For Temperature calculations */
/* mV per Degree */
#define TEMP_MV_D (int16_t) 10
/* Offset to 0 degrees */
#define TEMP_OFFSET (int16_t) 500

/* Maximum counter number for RPM measurements */
#define MAX_RPM_COUNT 65536

/* Macros */
#if(ARILORX_HWREV == ARILORX_HW1)
/* Returns Measurement in mV referred to Internal Reference */
#define GET_MEASUREMENT(x) (uint16_t)ADC_get(x)
/* Returns the number of pulses from Hall 1 Sensor */
#define GET_HALL1_PULSES GetEXTI6Count()
/* Returns the number of pulses from Hall 2 Sensor */
#define GET_HALL2_PULSES GetEXTI3Count()
#define HALL1    EXTI6
#define HALL2    EXTI3
#define NUM_HALL MAX_EXTI
#define GET_HALL_TIMEDIFF(x) GetEXTITimeStampDiff(x)
#define GET_HALL_TIMELASTPULSE(x) GetEXTITimeLastPulse(x)
/* Get number of pulses between the samples to calculate RPM */
#define PULSES_RPM_CALC(x) GetTimeStampRPMOff(x)

/* MAX Pulses */
#define MAX_PULSES 4294967295u
#endif

/* Distance Measurement */
#define DIST_BUFFSIZE USART_RXBUFFERSIZE
#define DISTANCE_AVAILABLE    USARTRX_AVAILABLE
#define DISTANCE_NOTAVAILABLE USARTRX_NOTAVAILABLE
#define GET_DISTANCE_BUFF(x) 1//usart_getbuffer(x)

/* Temperature Sensor HOPERF Module */
#define TEMP_HOPERF hoperf_temp_get()

#endif /* HW_SIGNALPROCESSOR_H_ */

/*
 * hw_signalprocessor.h
 *
 *  Created on: 04.03.2013
 *      Author: ArigitaJ
 */
