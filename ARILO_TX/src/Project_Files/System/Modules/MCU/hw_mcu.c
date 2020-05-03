/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_mcu.c                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW MCU Init. and functions                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "config_switches.h"
#include "hw_mcu.h"
#include "hw_adc.h"
#include "hw_timers.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SetSysClockTo72                                            */
/*                                                                              */
/* DESCRIPTION:      STM32 HW System Clock Init.                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SetSysClockTo72(void)
{
  ErrorStatus HSEStartUpStatus;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
     /* Enable Prefetch Buffer */
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

     /* Flash 2 wait state */
     FLASH_SetLatency(FLASH_Latency_2);

     /* HCLK = SYSCLK */
     RCC_HCLKConfig(RCC_SYSCLK_Div1);

     /* PCLK2 = HCLK */
     RCC_PCLK2Config(RCC_HCLK_Div1);

     /* PCLK1 = HCLK/2 */
     RCC_PCLK1Config(RCC_HCLK_Div2);

	 #if(ARILOTX_HWREV == ARILOTX_HW1)
     /* PLLCLK = 8MHz * 9 = 72 MHz */
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
     #else
     /* PLLCLK = 12MHz * 6 = 72 MHz */
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
     #endif

     /* Enable PLL */
     RCC_PLLCmd(ENABLE);

     /* Wait till PLL is ready */
     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {;}

     /* Select PLL as system clock source */
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

     /* Wait till PLL is used as system clock source */
     while(RCC_GetSYSCLKSource() != 0x08)
        {;}
  }
}


/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             MCU_ReadDevId                                              */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Device ID Read.(first 3Byte from 12)              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint32_t MCU_ReadDevId(void)
{
	uint32_t *UniqueID = (uint32_t *)0x1FFFF7E8;
	return((UniqueID[0] & 0xFFFFFF));
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             MCU_init                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW MCU Init.                                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void MCU_init(void)
{
	/* Initialize System */
	SystemInit();

	/* Initialize Clocks */

	/* Initialize System Clock */
	SetSysClockTo72();

	/* For Ports and related aspects */
	/* GPIOA, GPIOB and GPIOC clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	/* For ADC */
    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); /* Should never be bigger than 14MHz */

    /* For DMA */
    /* Init Clock ADC 1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* For SPI */
	/* SPI1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* SPI2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	/* For Timers */

	#if(ARILOTX_HWREV == ARILOTX_HW1)
	/* TIM8 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    #else
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	#endif

    #if(ARILOTX_HWREV == ARILOTX_HW2)
	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    #endif

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* For Watchdog */
	/* Check if the system has resumed from WWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{
	/* WWDGRST flag set */
	/* Clear reset flags */
	RCC_ClearFlag();
	}
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	/* For USART */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

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
uint8_t hw_pseudorng(void)
{
	/* Based on ADC VREF, TEMP and SYSTICK Values */
	/* 5 Bit value is needed, it will be constructed on a Byte basis with the:   */
	/* - Last two bits of TEMP value                                             */
	/* - Last two bits of VREF value                                             */
	/* - XOR with Systick value                                                  */
	/* The result Byte will be converted to 5 bit with the module operation % 25 */
	uint8_t rng;
	rng = ((ADC_get(ADC_TEMP))<<2) & (0xC);
	rng |= (ADC_get(ADC_VREF) & (0x3));
	rng ^= (uint8_t)SYST_TICK_VAL;
	rng %= 25;

	return(rng);
}

/*
 * hw_mcu.c
 *
 *  Created on: 09.09.2013
 *      Author: ArigitaJ
 */


