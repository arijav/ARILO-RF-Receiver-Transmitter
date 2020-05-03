/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_general.h                                               */
/*                                                                              */
/* DESCRIPTION:      HW Setup STM32                                             */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_GENERAL_MANAGER_H_
#define HW_GENERAL_MANAGER_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "stm32f10x.h"
#include "config_switches.h"
#include "../Modules/MCU/hw_mcu.h"
#include "../Modules/MCU/hw_ADC.h"
#include "../Modules/MCU/hw_timers.h"
#include "../Modules/MCU/hw_ports.h"
#include "../Modules/MCU/hw_interr.h"
#include "../Modules/MCU/hw_spi.h"
#include "../Modules/MCU/hw_common.h"
#include "../Modules/MCU/hw_usart.h"
#include "../Low_Level_State_Machines/Error_Handler/Error_Handler_LLSM.h"
#include "hw_radio.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Flags */
#define FLAG_UNSET 0
#define FLAG_SET   1

/* Random Number */
#define GET_RANDOM_NUMBER hw_pseudorng()

/* Interrupts */
#define DISABLE_IRQ __disable_irq()
#define ENABLE_IRQ __enable_irq()

/* Minimum number of Timer counts of the SYNC Pulse prior to PPM */
/* Unit 0.5us */
/* train (min. 4ms) */
#define SYNCRO_PULSE_MIN	8000
/* Max Sync Pulse aprox. 20ms */
#define SYNCRO_PULSE_MAX	40000

/* Number of ticks (1 Tick = 1ms) after HW initialization until startup */
#define NTICK_STARTUP 50

/* PWM (10ms = 60000) */
#define SERVO_CENTER INIT_CR //1471us

/* Conversion from Timer Ticks to Channels resolution (each unit 0.5us) */
/* Timer Ticks: each unit 0.166666666us; CH Resolution, each unit 0.5us */
/* Conv Factor = 0.5/0.1666666 = 3                                      */
#define RX_RESOLUTION_CONV 3

/* Checks the status of the binding pin (PC9), pull-up */
#if(ARILOTX_HWREV == ARILOTX_HW1)
#define PUSHBUTTOM_ON !(GPIOC->IDR & 0x200)
#else
#define PUSHBUTTOM_ON !(GPIOB->IDR & 0x20)
#endif

#if(ARILOTX_HWREV != ARILOTX_HW1)
/* Switch ON/OFF Red LED */
#define REDLED_ON        GPIOB->BSRR = 0x100 /* PB8 */
#define REDLED_OFF       GPIOB->BRR = 0x100  /* PB8 */
/* Switch ON/OFF Green LED */
#if(ARILOTX_HWREV == ARILOTX_HW2)
#define GREENLED_ON      GPIOB->BSRR = 0x2000 /* PC13 */
#define GREENLED_OFF     GPIOB->BRR = 0x2000  /* PC13 */
#elif(ARILOTX_HWREV == ARILOTX_HW3)
#define GREENLED_ON      GPIOB->BSRR = 0x200 /* PB9 */
#define GREENLED_OFF     GPIOB->BRR = 0x200  /* PB9 */
#endif
#endif

#if(ARILOTX_HWREV == ARILOTX_HW1)
#define TEST_PIN_ON	    GPIOB->BSRR = 0x20 /* PB4 */
#define TEST_PIN_OFF    GPIOB->BRR = 0x20  /* PB4 */
#define TEST_PIN_STATUS !(GPIOA->IDR & 0x20)
#elif(ARILOTX_HWREV == ARILOTX_HW2)
#define TEST_PIN_ON   GPIOA->BSRR = 0x800  /* PA11 */
#define TEST_PIN_OFF  GPIOA->BRR = 0x800   /* PA11 */
#define TEST_PIN_STATUS !(GPIOA->IDR & 0x800)
#else
#define TEST_PIN_ON
#define TEST_PIN_OFF
#define TEST_PIN_STATUS
#endif

/* Buzzer */
#if(ARILOTX_HWREV == ARILOTX_HW1)
#define BUZZER(x,y) SetChannel(x,y,4,4)
#else
#define BUZZER(x,y) SetChannel(x,y,2,2)
#endif

/* Fan control */
#if(ARILOTX_HWREV == ARILOTX_HW2)
#define FAN(x,y) SetChannel(x,y,4,4)
#endif

/* Telemetry TX/RX Buffer */
#define TEL_SETUP_TXBUFF usart_getTXbuffer()
//#define TEL_SETUP_RXBUFF(x) usart_getRXbuffer(x)
#define TEL_TXBUFF_LENGTH USART_TXBUFFERSIZE
#define TEL_RXBUFF_LENGTH USART_RXBUFFERSIZE

/* Activate / Deactivate 9x Telemetry Sending */
#define TEL_MODIFY_TXBUFF DMA_Cmd(DMA1_Channel4, DISABLE);
#define TEL_RELEASE_TXBUFF DMA_Cmd(DMA1_Channel4, ENABLE);

/* Set 9x Telemetry Frame Length */
#define TEL_SET_FRLENGTH(x) DMA_SetCurrDataCounter(DMA1_Channel4, x);

/* CPU Load */
#define CPU_LOAD get_cpu_load_calc()

/* Iface to Error Manager */
#define ERR_HANDLER_IF(x,y) ERRORHANDLER_LLSM_IFACE(x,y)

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_init                                                    */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Init.                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void hw_init(void);

#endif /* HARDWARE_GENERAL_H_ */

 /*
 * hardware_general.h
 *
 *  Created on: 24.05.2012
 *      Author: X-ArigitaJ
 */
