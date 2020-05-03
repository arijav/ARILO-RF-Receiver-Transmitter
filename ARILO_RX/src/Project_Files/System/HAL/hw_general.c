/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_general.c                                               */
/*                                                                              */
/* DESCRIPTION:      HW Setup STM32                                             */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "hw_general.h"
#include "../Modules/RF/hw_commonrf.h"
#include "../Low_Level_State_Machines/RF_Handler/RF_Handler_LLSM.h"
#include "../Low_Level_State_Machines/SafeLink_Handler/SafeLink_Handler_LLSM.h"
#include "../Low_Level_State_Machines/Error_Handler/Error_Handler_LLSM.h"

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_init                                                    */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Init.                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hw_init(void)
{
    /* MCU Peripherals */

	/* Initialize MCU (Clocks) */
	MCU_init();

	/* ADC init */
	ADC_init();

    /* Timers */
	Timers_init();

	/* Try to activate the interruptions just before the startup delay */
    /* Interrupts */
	Interr_init();

	/* Ports init */
	port_init();

	/* SPI Init */
    SPI_init();

    /* I2C Init */
    //i2c_init();

    /* Init USART for Maxsonar communications */
    //uart_init(UART_BAUD_SELECT(9600,16000000L));

    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();

    /* EEPROM Init */
    EE_Init();

    /* NON MCU Modules */

    /* Error Handler init */
    ErrorHandler_llsm_init();

    /* Peripherals */
    /* IMU */
    //MPU6050_I2C_Init();
    //MPU6050_Initialize();

	/* TODO: Test */
	//uint8_t test = TEMP_HOPERF;
	
	/* RF Init Transceivers */
	//(void)RF_Init(NRF24L01_TRX);
	(void)RF_Init(HOPERF_TRX);

    /* Init CH Generation Low Level SM */
    //CHGeneration_llsm_init();

    /* Delay before startup */
    while(GET_MILLIS < NTICK_STARTUP){};

	/* Init RF/SL Handler LLSM */
    //RFHandler_llsm_init();
    SafeLinkHandler_llsm_init();
}

