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
#include "../Modules/MCU/hw_ADC.h"
#include "../Modules/MCU/hw_ports.h"
#include "../Modules/RF/hw_hoperf.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* ADC Channel mapping */
#define VTMP_S ADC_TEMP
#if(ARILOTX_HWREV != ARILOTX_HW1)
#define VBAT_S ADC_CH8
#endif
#define VREF_S ADC_VREF

/* State of the measurement */
#define M_COMPLETED 1
#define M_INCOMPLETED 0

/* Reference voltage for the conversions in mV */
/* TODO: Calibrate */
#define REF_VOLTAGE_CALIB (uint16_t) 5180
#define REF_VOLTAGE_INT (uint16_t) 1100

/* Conversion factor for Battery measurements */
#define CONV_FACT_BATT (float)6.6

/* Macros */
/* Returns Measurement in mV referred to Internal Reference */
#define GET_MEASUREMENT(x) (uint16_t)ADC_get(x)

/* Temperature Sensor HOPERF Module */
#define TEMP_HOPERF hoperf_temp_get()

#endif /* HW_SIGNALPROCESSOR_H_ */

/*
 * hw_signalprocessor.h
 *
 *  Created on: 04.03.2013
 *      Author: ArigitaJ
 */
