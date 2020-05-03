/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_ADC.c                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW ADC Init. and functions                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_ADC_H_
#define HW_ADC_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC_VREF           1 /* 1.2V internal reference for calibration */
#define ADC_TEMP           2 /* internal temperature sensor reading */
#if(ARILORX_HWREV == ARILORX_HW1)
#define ADC_CH4            3
#define ADC_CH8            4
#define ADC_CH9            5
#endif

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             ADC_init                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW ADC Init.                                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void ADC_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             ADC_get                                                    */
/*                                                                              */
/* DESCRIPTION:      STM32 HW ADC read value                                    */
/*                                                                              */
/* PARAMETERS:       Out: Value                                                 */
/*                   In: Channel                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint16_t ADC_get(uint8_t value);

#endif /* HW_ADC_H_ */

/*
 * hw_ADC.h
 *
 *  Created on: 25.05.2012
 *      Author: X-ArigitaJ
 */