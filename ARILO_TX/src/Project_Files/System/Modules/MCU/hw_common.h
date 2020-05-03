/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_common.h                                              */
/*                                                                              */
/* DESCRIPTION:      Common Data Structures and Defines for MCU Modules         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_COMMON_H_
#define HW_COMMON_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "config_switches.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Ports */
/* For NRF24L01+ */
#if(ARILOTX_HWREV == ARILOTX_HW1)
#define nRF24L01_CEH        GPIOC->BSRR = 0x02 /* PC1 */
#define nRF24L01_CEL        GPIOC->BRR = 0x02  /* PC1 */
#define nRF24L01_IRQP       GPIOC->IDR & 0x10  /* PC4 */
#define nRF24L01_CSNH       GPIOC->BSRR = 0x04 /* PC2 */
#define nRF24L01_CSNL       GPIOC->BRR = 0x04  /* PC2 */
#else
#define nRF24L01_CEH        GPIOB->BSRR = 0x02 /* PB1 */
#define nRF24L01_CEL        GPIOB->BRR = 0x02  /* PB1 */
#define nRF24L01_IRQP       GPIOB->IDR & 0x08  /* PB3 */
#define nRF24L01_CSNH       GPIOA->BSRR = 0x10 /* PA4 */
#define nRF24L01_CSNL       GPIOA->BRR = 0x10  /* PA4 */
#endif
/* CSN: SPI1_NSS PA4 */
/* MISO: SPI1_MISO PA6 */
/* MOSI: SPI1_MOSI PA7 */
/* SCK: SPI1_SCK PA5 */

#if(ARILOTX_HWREV == ARILOTX_HW1)
#define HOPERF_CSNH         GPIOC->BSRR = 0x80  /* PC7 */
#define HOPERF_CSNL         GPIOC->BRR = 0x80   /* PC7 */
#define HOPERF_IRQP         GPIOA->IDR & 0x1000 /* PA12 */
/* For RFM23BP */
#else
#define HOPERF_CSNH         GPIOA->BSRR = 0x8000  /* PA15 */
#define HOPERF_CSNL         GPIOA->BRR = 0x8000   /* PA15 */
#define HOPERF_IRQP         GPIOA->IDR & 0x100 /* PA8 */
#endif
/* CSN: SPI2_NSS PB12 */
/* MISO: SPI2_MISO PB14 */
/* MOSI: SPI2_MOSI PB15 */
/* SCK: SPI2_SCK PB13 */

/* Timer */
#define GET_MILLIS GetTicks()
#define GET_MICROS GetMicros()
#define DELAY_US(x) DelayUs(x)
#define GET_TIME_DIFF_FLANK(x,y,z) GetTimeDiffFlank(x,y,z)

/* Unique Device ID */
#define DEVICE_ID MCU_ReadDevId()

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

#endif /* HW_COMMON_H_ */

/*
 * hw_common.h
 *
 *  Created on: 25.09.2013
 *      Author: ArigitaJ
 */
