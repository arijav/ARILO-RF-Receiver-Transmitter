/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_usart.c                                                 */
/*                                                                              */
/* DESCRIPTION:      USART support functions for STM32                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_usart.h"

/*------------------------------------------------------------------------------*/
/* Defines & Typedef                                                            */
/*------------------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define USART1_DR_Base 0x40013804

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
uint8_t RxBuffer[USART_RXBUFFERSIZE];
uint8_t NbrOfDataToRead = USART_RXBUFFERSIZE;
uint8_t i = 0;
volatile TestStatus TransferStatus = FAILED;

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             usart_init                                                 */
/*                                                                              */
/* DESCRIPTION:      Inits. USART                                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void usart_init(void)
{
	/* Configure USART for DMA access on RX */
	/* USART1_Rx_DMA_Channel (triggered by USART1 Rx event) Config */
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART_RXBUFFERSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;

	/* Configure USARTy */
	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART1 DMA RX request */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             usart_available                                            */
/*                                                                              */
/* DESCRIPTION:      Returns USART availability                                 */
/*                                                                              */
/* PARAMETERS:       Out: Availability Flag                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t usart_available(void)
{
	uint8_t available = USARTRX_NOTAVAILABLE;
	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) available = USARTRX_AVAILABLE;
	return available;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             usart_getbuffer                                            */
/*                                                                              */
/* DESCRIPTION:      Returns Pointer to RX Buffer                               */
/*                                                                              */
/* PARAMETERS:       Out: Availability Flag                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t usart_getbuffer(uint8_t *rx_buffer)
{
	(void)rx_buffer;
	uint8_t status = usart_available();
	if(status == USARTRX_AVAILABLE)
	{
		rx_buffer = &(RxBuffer[0]);
	}

	return status;
}

/*
 * hw_uart.c
 *
 *  Created on: 06.09.2013
 *      Author: ArigitaJ
 */


