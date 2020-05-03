/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_spi.c                                                   */
/*                                                                              */
/* DESCRIPTION:      SPI support functions for STM32                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_spi.h"

/*------------------------------------------------------------------------------*/
/* Defines & Typedef                                                            */
/*------------------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_init                                                   */
/*                                                                              */
/* DESCRIPTION:      Inits. STM32 SPI as Master                                 */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SPI_init(void)
{
      SPI_InitTypeDef  SPI_InitStructure;

	  /* SPI configuration ------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_Init(SPI1, &SPI_InitStructure);

	  SPI_Init(SPI2, &SPI_InitStructure);

	  /* Enable SPI */
	  SPI_Cmd(SPI1, ENABLE);
	  SPI_Cmd(SPI2, ENABLE);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             WriteByteSPI1                                              */
/*                                                                              */
/* DESCRIPTION:      Sends one byte through SPI1                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline void WriteByteSPI1(uint8_t byte)
{
    /* Send SPI1 data */
    SPI_I2S_SendData(SPI1, (uint16_t)byte);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             ReadByteSPI1                                               */
/*                                                                              */
/* DESCRIPTION:      Reads one byte through SPI1                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline uint8_t ReadByteSPI1(uint8_t addr)
{
	uint16_t temp_spiread = 250;
    /* Send SPI1 data */
    SPI_I2S_SendData(SPI1, (uint16_t)addr);
    /* Wait for SPI1 data reception */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
    	temp_spiread--;
    	if(temp_spiread == 0) break;
    };
    /* Read SPI1 received data */
    if(temp_spiread != 0)
    {
    	addr = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    }
    else addr = 0;
	return addr;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             WriteByteSPI2                                              */
/*                                                                              */
/* DESCRIPTION:      Sends one byte through SPI2                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline void WriteByteSPI2(uint8_t byte)
{
    /* Send SPI2 data */
    SPI_I2S_SendData(SPI2, (uint16_t)byte);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             ReadByteSPI2                                               */
/*                                                                              */
/* DESCRIPTION:      Reads one byte through SPI2                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline uint8_t ReadByteSPI2(uint8_t addr)
{
	uint8_t test_flag_ovr;
	uint8_t test_flag_modf;
	uint8_t test_flag_crcerr;
	uint8_t test_flag_udr;
	uint16_t temp_spiread = 250;
    /* Send SPI2 data */
    SPI_I2S_SendData(SPI2, (uint16_t)addr);
    /* Wait for SPI2 data reception */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
    	temp_spiread--;
    	if(temp_spiread == 0)
    	{
    		break;
    	}
    }
	test_flag_ovr = SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_OVR);
	test_flag_modf = SPI_I2S_GetFlagStatus(SPI2, SPI_FLAG_MODF);
	test_flag_crcerr = SPI_I2S_GetFlagStatus(SPI2, SPI_FLAG_CRCERR);
	test_flag_udr = SPI_I2S_GetFlagStatus(SPI2, I2S_FLAG_UDR);
	/* Read SPI2 received data */
	if(temp_spiread != 0)
	{
		addr = (uint8_t)SPI_I2S_ReceiveData(SPI2);
	}
	else addr = 0;
	return addr;
}

/*
 * hw_spi.c
 *
 *  Created on: 03.07.2012
 *      Author: X-ArigitaJ
 */


