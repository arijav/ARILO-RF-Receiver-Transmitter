/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_mcu.h                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW MCU Init. and functions                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_MCU_H_
#define HW_MCU_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
#define SYST_TICK_VAL (SysTick->VAL)

/* Random Channel (0-24) */
#define GET_RANDOM_CHANNEL hw_pseudorng();

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             MCU_ReadDevId                                              */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Device ID Read.(first 3Byte from 12)              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t MCU_ReadDevId(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             MCU_init                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW MCU Init.                                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void MCU_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_pseudorng                                               */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Pseudo Random Number Generator                    */
/*                                                                              */
/* PARAMETERS:       Out: Value                                                 */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t hw_pseudorng(void);

#endif /* HW_MCU_H_ */

/*
 * hw_mcu.h
 *
 *  Created on: 09.09.2013
 *      Author: ArigitaJ
 */