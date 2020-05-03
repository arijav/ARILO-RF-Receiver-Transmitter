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
#include "../Modules/MCU/hw_usart.h"
#include "../Modules/MCU/hw_interr.h"
#include "../Modules/MCU/hw_eeprom.h"
#include "../Modules/MCU/hw_spi.h"
#include "../Modules/MCU/hw_common.h"
#include "../Modules/MPU6050/hw_MPU6050.h"
#include "../Low_Level_State_Machines/Error_Handler/Error_Handler_LLSM.h"
#include "hw_radio.h"
#include "../Low_Level_State_Machines/CH_Generation/CHGeneration_llsm.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Flags */
#define FLAG_UNSET 0
#define FLAG_SET   1

/* Unique Device ID */
#define DEVICE_ID MCU_ReadDevId()

/* Random Number */
#define GET_RANDOM_NUMBER hw_pseudorng()

/* Millis */
//#define GET_MILLIS GetTicks()

/* Interrupts */
#define DISABLE_IRQ __disable_irq()
#define ENABLE_IRQ __enable_irq()

/* Watchdog */
#define START_WATCHDOG   WWDG_Enable(127)
#define REFRESH_WATCHDOG WWDG_SetCounter(127)

/* Number of ticks (1 Tick = 1ms) after HW initialization until startup */
#define NTICK_STARTUP 201

/* Resolution conversion between channels received (0,5us per unit) */
/* and internal channel generation (0,3us aprox. per unit)          */
#define TX_RX_RESOLUTION_CONV (float32_t)1.5

/* IMU Defines */
#define MPU6050_I2C                  I2C2
#define MPU6050_I2C_RCC_Periph       RCC_APB1Periph_I2C2
#define MPU6050_I2C_Port             GPIOB
#define MPU6050_I2C_SCL_Pin          GPIO_Pin_10
#define MPU6050_I2C_SDA_Pin          GPIO_Pin_11
#define MPU6050_I2C_RCC_Port         RCC_APB2Periph_GPIOB
#define MPU6050_I2C_Speed            100000 // 100kHz standard

#if(ARILORX_HWREV == ARILORX_HW1)
/* Checks the status of the binding pin (PC9), pull-up */
#define PUSHBUTTOM_ON !(GPIOC->IDR & 0x200)
/* PB12 in V2 onwards */
#else
#define PUSHBUTTOM_ON !(GPIOB->IDR & 0x1000)
#endif


/* CH Generation Aspects */
#define SET_CH_NUMBER(x) CHGeneration_llsm_BuffSetChNumber(x)
#define SET_CH_FAILSAFE CHGeneration_llsm_SetFailsafe()
#define EMERGENCY_FAILSAFE CHGeneration_llsm_EmergencyFailsafe()
#define UPDATE_BUFF_CHANNELS(x,y) CHGeneration_llsm_UpdateBuffChannel(x,y)
#define RELEASE_BUFF_CHANNELS CHGeneration_llsm_SetChReleasedFlag()
#define RELEASE_BUFF_CHANNELS CHGeneration_llsm_SetChReleasedFlag()

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
