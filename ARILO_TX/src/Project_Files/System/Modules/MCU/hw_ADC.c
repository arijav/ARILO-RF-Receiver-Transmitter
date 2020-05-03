/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_ADC.c                                                   */
/*                                                                              */
/* DESCRIPTION:      STM32 HW ADC Init. and functions                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "config_switches.h"
#include "hw_ADC.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
/* Declaring ADC struct */
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
#if(ARILOTX_HWREV == ARILOTX_HW1)
__IO uint16_t ADC1ConvertedValue[2];
#else
__IO uint16_t ADC1ConvertedValue[3];
#endif

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
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
void ADC_init(void)
{
    /* Deinit ADC */
    ADC_DeInit(ADC1);

    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Init ADC struct */
    ADC_StructInit(&ADC_InitStructure);

	/* Setting the ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    #if(ARILOTX_HWREV == ARILOTX_HW1)
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    #else
    ADC_InitStructure.ADC_NbrOfChannel = 3;
	#endif
    ADC_Init(ADC1, &ADC_InitStructure);
	#if(ARILOTX_HWREV == ARILOTX_HW1)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 2, ADC_SampleTime_28Cycles5);
	#else
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_28Cycles5);
	#endif
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Calibrate ADC1 */
    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 reset calibration */
    while (ADC_GetCalibrationStatus(ADC1));

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             ADC_get                                                    */
/*                                                                              */
/* DESCRIPTION:      STM32 HW ADC read RAW value                                */
/*                                                                              */
/* PARAMETERS:       Out: Value                                                 */
/*                   In: Channel                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint16_t ADC_get(uint8_t channel)
{
	uint16_t value;
	switch(channel)
	{
		case ADC_VREF:
			value = ADC1ConvertedValue[0];
		break;
		case ADC_TEMP:
			value = ADC1ConvertedValue[1];
		break;
		#if(ARILOTX_HWREV != ARILOTX_HW1)
		case ADC_CH8:
			value = ADC1ConvertedValue[1];
		break;
		#endif
		default:
			value = 0;
		break;
	}
	value = (((uint32_t)3300 * (uint32_t)value) / 4096);
	return value;
};

/*
 * hw_ADC.c
 *
 *  Created on: 25.05.2012
 *      Author: X-ArigitaJ
 */


