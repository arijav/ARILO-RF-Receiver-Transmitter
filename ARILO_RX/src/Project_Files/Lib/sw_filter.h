/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             sw_filter.c                                                */
/*                                                                              */
/* DESCRIPTION:      Filter library                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef SW_FILTER_H_
#define SW_FILTER_H_
/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             iir_lowpass_fil                                            */
/*                                                                              */
/* DESCRIPTION:      Inifinite Impulse Response Lowpass filter.                 */
/*                                                                              */
/* PARAMETERS:       In: val_act_raw: Actual unfiltered sample                  */
/*                       val_old_fil: Last filtered sample                      */
/*                       dt: Time in ms between samples                         */
/*                       time_const: Time Constant                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern int32_t iir_lowpass_fil(int32_t val_act_raw, int32_t val_old_fil, uint16_t dt, uint16_t time_const);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             fir_lowpass_fil                                            */
/*                                                                              */
/* DESCRIPTION:      Finite Impulse Response Lowpass filter (average).          */
/*                                                                              */
/* PARAMETERS:       In: array_val: Pointer to array of samples                 */
/*                       array_length: Length of samples array                  */
/*                   NOTE: Array order is position 0 NEWEST value               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint16_t fir_lowpass_fil(uint16_t *array_val, uint8_t array_length);

#endif /* SW_FILTER_H_ */

/*
 * sw_filter.h
 *
 *  Created on: 22.03.2013
 *      Author: ArigitaJ
 */
