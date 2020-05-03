/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             port_init.c                                                */
/*                                                                              */
/* DESCRIPTION:      STM32 Port HW Init.                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "config_switches.h"
#include "hw_ports.h"
#include "hw_common.h"
#include "hw_timers.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
#if(ARILORX_HWREV == ARILORX_HW1)
volatile uint32_t EXTI3_count = 0;
volatile uint32_t EXTI6_count = 0;
volatile EXTTimeStampStruct EXT_Time_Stamps[MAX_EXTI];
volatile uint8_t Timestamps_RPM_offs[MAX_EXTI];
#endif

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             port_init                                                  */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Ports Init.                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void port_init(void)
{
    #if(ARILORX_HWREV == ARILORX_HW1)
	uint8_t aux1, aux2;
	EXTI_InitTypeDef EXTI_InitStructure;
    #endif
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Port configuration */

	/* Configure TIM -------------------------------------------------------------- */
	/* GPIOA Configuration:TIM2 Channel1, 2, 3 and 4 as alternate function push-pull */
	/* TIM2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIOB Configuration:TIM4 Channel1, 2, 3 and 4 as alternate function push-pull */
	/* TIM4 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    #if(ARILORX_HWREV != ARILORX_HW1)
	/* GPIOC Configuration:TIM8 Channel3 and 4 as alternate function push-pull */
	/* TIM8 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Disable JTAG-DP, enable SWD but PA15 (JTDI) free */
	uint32_t reg = AFIO->MAPR;
	reg &= ~(1<<24 | 1<<26);
	reg |= (1<<25);
	AFIO->MAPR = reg;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	/* GPIOB Configuration:TIM3 Channel1 and 2 as alternate function push-pull */
	/* TIM3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	/* TODO: PB4 switch to just SWD.!!!! */
    #endif

	/* Configure SPI1 pins: SCK, MISO, MOSI and NSS ---------------------------------*/
	/* Configure SCK and MOSI pins as Alternate Function Push Pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure MISO pin as Pull-UP  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure NRF24L01+ extra pins: IRQ, CE */
	/* IRQ as Input Floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* CE */
    #if(ARILORX_HWREV == ARILORX_HW1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    #else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    #endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* CSN */
    #if(ARILORX_HWREV == ARILORX_HW1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    #else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    #endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure SPI2 pins: SCK, MISO, MOSI and NSS ---------------------------------*/
	/* Configure SCK and MOSI pins as Alternate Function Push Pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure MISO pin as Pull-UP  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure NSS pin as Pull-UP  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    #if(ARILORX_HWREV == ARILORX_HW1)
	/* Configure RFM22 extra pins: IRQ, CSN */
	/* IRQ as Input Floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* CSN */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    #elif(ARILORX_HWREV == ARILORX_HW2)
	/* IRQ as Input Floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CSN */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    #elif(ARILORX_HWREV == ARILORX_HW3)
    #endif

	/* Configure ADC Pin ------------------------------------------------------------ */
    #if(ARILORX_HWREV == ARILORX_HW1)
	/* PB0 (VBAT; ADC_12_IN8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* PB1 (ACS758; ADC_12_IN9) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* PA4 (TEMP; ADC_12_IN4) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    #endif

    #if(ARILORX_HWREV != ARILORX_HW1)
	/* Configure LED Pins ----------------------------------------------------------- */
	/* Red LED */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Green LED */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    #endif

	/* Configure USART Pins --------------------------------------------------------- */
    /* RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure INT Pins ----------------------------------------------------------- */
    #if(ARILORX_HWREV == ARILORX_HW1)
	/* Configure PC6 as Input Floating (Hall 1 sensor) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PB3 as Input Floating (Hall 2 sensor) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure PC9 as Input Floating (IN0) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect EXTI6 Line to PC6 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);

    /* Configure EXTI6 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Connect EXTI3 Line to PB3 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);

    /* Configure EXTI3 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Initialize EXT Structures */
    /* Startup head values */
    for(aux1 = 0; aux1 < MAX_EXTI; aux1++)
    {
    	EXT_Time_Stamps[aux1].head = 0;
    	/* Clear arrays */
		for(aux2 = 0; aux2 < MAX_TIMESTAMPS; aux2++)
		{
			EXT_Time_Stamps[aux1].array[aux2] = 0;
		};
		Timestamps_RPM_offs[aux1] = TIMESTAMPS_RPMCALC_OFFSET;
    };
    #else
    /* Configure PB12 as Input Floating (IN1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PA11 as Input Floating (MPU_INT) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    #endif
};

#if(ARILORX_HWREV == ARILORX_HW1)
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTI3Count()                                            */
/*                                                                              */
/* DESCRIPTION:      Get number of EXTI3 interrupts until the call              */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI3 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint32_t GetEXTI3Count(void)
{
	return EXTI3_count;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTI6Count()                                            */
/*                                                                              */
/* DESCRIPTION:      Get number of EXTI6 interrupts until the call              */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI6 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint32_t GetEXTI6Count(void)
{
	return EXTI6_count;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTITimeLastPulse()                                     */
/*                                                                              */
/* DESCRIPTION:      Get time stamps of the last pulse                          */
/*                                                                              */
/* PARAMETERS:       Out : Time of last EXTI Interrupt                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
float32_t GetEXTITimeLastPulse(uint8_t ext_number)
{
	return EXT_Time_Stamps[ext_number].array[EXT_Time_Stamps[ext_number].head];
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTITimeStamp()                                         */
/*                                                                              */
/* DESCRIPTION:      Get time stamps difference of two EXT Interrupts           */
/*                   (float in ms)                                              */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI6 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
float32_t GetEXTITimeStampDiff(uint8_t ext_number)
{
	float32_t tdiff = 0;
	uint8_t aux1, aux2, aux3 = 0;
	aux1 = EXT_Time_Stamps[ext_number].head;
	aux3 = Timestamps_RPM_offs[ext_number];
	/* The timestamps to be compared are Timestamps_RPM_offs pulses away from each other */
	if(aux1 > (aux3 - 1))
	{
		aux2 = aux1 - aux3;
	}
	else
	{
		aux2 = (MAX_TIMESTAMPS - aux3) + aux1;
	}
	if(EXT_Time_Stamps[ext_number].array[aux1] > EXT_Time_Stamps[ext_number].array[aux2])
	{
		tdiff =  EXT_Time_Stamps[ext_number].array[aux1] - EXT_Time_Stamps[ext_number].array[aux2];
	}
	else
	{
		tdiff = ((float32_t)MAX_TIME - EXT_Time_Stamps[ext_number].array[aux2]) + EXT_Time_Stamps[ext_number].array[aux1];
	}
	return tdiff;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTITimeStamp()                                         */
/*                                                                              */
/* DESCRIPTION:      Get time number of timestamps between the two used for     */
/*                   RPM calculation                                            */
/*                                                                              */
/* PARAMETERS:       Out : Number of Timestamps                                 */
/*                   In :  EXTI Number                                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t GetTimeStampRPMOff(uint8_t ext_number)
{
	return Timestamps_RPM_offs[ext_number];
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   EXTI3(IRQ Handler)                                         */
/*                                                                              */
/* DESCRIPTION:      STM32 EXTI3 routine                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		float32_t micros = (float32_t)GET_MICROS;
		float32_t millis = (float32_t)GET_MILLIS;
		uint8_t head = (uint8_t)(EXT_Time_Stamps[EXTI3].head) + 1;
		head = head % MAX_TIMESTAMPS;
		/* Increment counter */
		EXTI3_count++;
		/* Insert time stamp in array */
		EXT_Time_Stamps[EXTI3].head = head;
		EXT_Time_Stamps[EXTI3].array[head] = (micros / (float32_t)1000) + millis;
		/* Clear the  EXTI line 3 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   EXTI9_5(IRQ Handler)                                       */
/*                                                                              */
/* DESCRIPTION:      STM32 EXTI9_5 routine                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		float32_t micros = (float32_t)GET_MICROS;
		float32_t millis = (float32_t)GET_MILLIS;
		uint8_t head = (uint8_t)(EXT_Time_Stamps[EXTI6].head) + 1;
		head = head % MAX_TIMESTAMPS;
		/* Increment counter */
		EXTI6_count++;
		/* Insert time stamp in array */
		EXT_Time_Stamps[EXTI6].head = head;
		EXT_Time_Stamps[EXTI6].array[head] = (micros / (float32_t)1000) + millis;
		/* Clear the  EXTI line 6 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}
#endif

/*
 * hw_ports.c
 *
 *  Created on: 25.05.2012
 *      Author: X-ArigitaJ
 */


