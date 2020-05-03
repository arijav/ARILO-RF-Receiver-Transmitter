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
#include "hw_eeprom.h"
#include "hw_timers.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Ports */
/* For NRF24L01+ */
#if(ARILORX_HWREV == ARILORX_HW1)
#define nRF24L01_CEH        GPIOC->BSRR = 0x02 /* PC1 */
#define nRF24L01_CEL        GPIOC->BRR = 0x02  /* PC1 */
#define nRF24L01_IRQP       GPIOC->IDR & 0x10  /* PC4 */
#define nRF24L01_CSNH       GPIOC->BSRR = 0x04 /* PC2 */
#define nRF24L01_CSNL       GPIOC->BRR = 0x04  /* PC2 */
#else
#define nRF24L01_CEH        GPIOC->BSRR = 0x04 /* PC2 */
#define nRF24L01_CEL        GPIOC->BRR = 0x04  /* PC2 */
#define nRF24L01_IRQP       GPIOC->IDR & 0x10  /* PC4 */
#define nRF24L01_CSNH       GPIOC->BSRR = 0x02 /* PC1 */
#define nRF24L01_CSNL       GPIOC->BRR = 0x02  /* PC1 */
#endif

/* CSN: SPI1_NSS PA4 */
/* MISO: SPI1_MISO PA6 */
/* MOSI: SPI1_MOSI PA7 */
/* SCK: SPI1_SCK PA5 */

#if(ARILORX_HWREV == ARILORX_HW1)
#define DEBUG_H		GPIOB->BSRR = 0x20 /* PB4 */
#define DEBUG_L		GPIOB->BRR = 0x20  /* PB4 */
#endif

/* For RFM22 */
#if(ARILORX_HWREV == ARILORX_HW1)
#define HOPERF_CSNH         GPIOC->BSRR = 0x80  /* PC7 */
#define HOPERF_CSNL         GPIOC->BRR = 0x80   /* PC7 */
#define HOPERF_IRQP         GPIOA->IDR & 0x1000 /* PA12 */
#elif(ARILORX_HWREV == ARILORX_HW2)
#define HOPERF_CSNH         GPIOC->BSRR = 0x40  /* PC6 */
#define HOPERF_CSNL         GPIOC->BRR = 0x40   /* PC6 */
#define HOPERF_IRQP         GPIOB->IDR & 0x01   /* PB0 */
#elif(ARILORX_HWREV == ARILORX_HW3)
/* TODO */
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

/* EEPROM Emulation Layout and related variables */
/* Virtual Address 0: CH0 Failsafe */
#define CH0_FAILSAFE_EE VirtAddVarTab[0]
/* Virtual Address 1: CH1 Failsafe */
#define CH1_FAILSAFE_EE VirtAddVarTab[1]
/* Virtual Address 2: CH2 Failsafe */
#define CH2_FAILSAFE_EE VirtAddVarTab[2]
/* Virtual Address 3: CH3 Failsafe */
#define CH3_FAILSAFE_EE VirtAddVarTab[3]
/* Virtual Address 4: CH4 Failsafe */
#define CH4_FAILSAFE_EE VirtAddVarTab[4]
/* Virtual Address 5: CH5 Failsafe */
#define CH5_FAILSAFE_EE VirtAddVarTab[5]
/* Virtual Address 6: CH6 Failsafe */
#define CH6_FAILSAFE_EE VirtAddVarTab[6]
/* Virtual Address 7: CH7 Failsafe */
#define CH7_FAILSAFE_EE VirtAddVarTab[7]
/* Virtual Address 8: CH8 Failsafe */
#define CH8_FAILSAFE_EE VirtAddVarTab[8]
/* Virtual Address 9: CH9 Failsafe */
#define CH9_FAILSAFE_EE VirtAddVarTab[9]
/* Virtual Address 10: CH10 Failsafe */
#define CH10_FAILSAFE_EE VirtAddVarTab[10]
/* Virtual Address 11: CH11 Failsafe */
#define CH11_FAILSAFE_EE VirtAddVarTab[11]
/* Virtual Address 12: Flag Code Failsafe Active */
#define FLAG_FAILSAFESET_EE VirtAddVarTab[12]
/* Virtual Address 13: Binding Address */
#define BINDING_ADDR_EE VirtAddVarTab[13]
/* Virtual Address 14: Flag Code Binding Address Set */
#define FLAG_BINDINGSET_EE VirtAddVarTab[14]

/* Define Flag Set in EEPROM */
#define FLAG_SET_EEPROM 4692

#define CH_FAILSAFECHANNELS_EE(x) VirtAddVarTab[x]

/* EEPROM Public calls */
#define EE_READ(x,y) EE_ReadVariable(x,y)
#define EE_WRITE(x,y) EE_WriteVariable(x,y)

/* Set Channel, x = Channel, Y = Value */
#define SET_CH(x,y) SetChannel(x,y);

/* PWM */
#define SERVO_CENTER 4414
#define MOTOR_CENTER 4414

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
