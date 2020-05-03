/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_nRF24L01.c                                              */
/*                                                                              */
/* DESCRIPTION:      Control Transceiver NRF24L01+                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
#include "hw_commonrf.h"
#include "../MCU/hw_spi.h"
#include "../MCU/hw_timers.h"
#include "../MCU/hw_common.h"
#include "../../../config_switches.h"

/*-----------------------------------------------------------------------------
   Global Defines
------------------------------------------------------------------------------*/

uint8_t TX_ADDRESS[TX_ADR_WIDTH]  = {0x43,0x34,0x01}; /* Define a static TX address */
uint8_t RX_ADDRESS[RX_ADR_WIDTH]  = {0x34,0x43,0x10}; /* Define a static RX address */

uint8_t nrf_channel_list[MAX_RF_CHANNELS] =
{
	0x00,
	0x05,
	0x0A,
	0x0F,
	0x14,
	0x19,
	0x1E,
	0x23,
	0x28,
	0x2D,
	0x32,
	0x37,
	0x3C,
	0x41,
	0x46,
	0x4B,
	0x50,
	0x55,
	0x5A,
	0x5F,
	0x64,
	0x69,
	0x6E,
	0x73,
	0x78
};


/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Init_NRF                                                   */
/*                                                                              */
/* DESCRIPTION:      Initializes the required register of the NRF24L01+         */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Init_NRF(void)
{
	nRF24L01_CEL;
	/* Power Down */
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x3B); /* PowerUP in RX Mode */
	SPI_RW_Reg(NRF_WRITE_REG + SETUP_AW, 0x01); /* Setup 3 Byte Address */
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); /* Setup Initial TX Address */
  	SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  /* Enable Pipe0 */
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); /* Setup RX Pipe 0 Address */
  	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); /* Setup RX Pipe 0 Data width */
	#if(RFM_ENHANCED_SHOCKBURST == ENHS_ENABLED)
  	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x01); /* 1 retransmission */
   	SPI_RW_Reg(NRF_WRITE_REG + FEATURE, 0x01); /* EN_DYN_ACK Enabled */
   	SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x00);      /* Auto ACK on Pipe 0 */
	#elif(RFM_ENHANCED_SHOCKBURST == ENHS_DISABLED)
  	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x00); /* 1 retransmission */
	#endif
   	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 0x78);      /* Set Initial Channel */
   	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x26);	  /* TX_PWR:0dBm, Datarate:250Kbps, LNA:HCURR */
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,0xff); /* clear RX_DR or TX_DS or MAX_RT interrupt flag */
	SPI_RW_Reg(FLUSH_TX,0); /* Flush TX Fifo */
	SPI_RW_Reg(FLUSH_RX,0); /* Flush RX Fifo */
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             DeInit_NRF                                                 */
/*                                                                              */
/* DESCRIPTION:      De-Initializes the NRF24L01+                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void DeInit_NRF(void)
{
	nRF24L01_CEL;
	/* Power Down */
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x39); /* PowerDown */
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             CheckTRX_NRF                                               */
/*                                                                              */
/* DESCRIPTION:      Checks the status of the NRF24L01+                         */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t CheckTRX_NRF(uint8_t mode)
{
	uint8_t status, ret_val, aux;
	status = SPI_Read(NRF_READ_REG + CONFIG);
	switch(mode)
	{
		case TXMODE:
			aux = 0x3a;
		break;
		case RXMODE:
			aux = 0x3b;
		break;
		default:
			aux = 0x3b;
		break;
	}
	if(status == aux)
	{
		ret_val = 1;
	}
	else
	{
		ret_val = 0;
	}
	aux = SPI_Read(NRF_READ_REG + FIFO_STATUS);
	return(ret_val);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             TxDataWireless_NRF                                         */
/*                                                                              */
/* DESCRIPTION:      Transmission of the predefined Structure Type Info data    */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TxDataWireless_NRF(uint8_t *buff)
{
	nRF24L01_CEL;
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,0xff); /* clear RX_DR or TX_DS or MAX_RT interrupt flag */
	#if(RFM_FLUSH_TX == FLUSH_ENABLED)
	SPI_RW_Reg(FLUSH_TX,0); /* Flush TX Fifo */
	#endif
	#if(RFM_ENHANCED_SHOCKBURST == ENHS_ENABLED)
	SPI_RW_Reg(W_TX_PAYLOAD_NO_ACK,0); /* Disable ACK Specific Packet */
	#endif
	SPI_Write_Buf(WR_TX_PLOAD, buff, TX_PLOAD_WIDTH); /* Writes data to TX payload */
	nRF24L01_CEH;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Tx_Mode_NRF                                                */
/*                                                                              */
/* DESCRIPTION:      This function initializes one nRF24L01 device to           */
/*                   TX mode, set TX address, set RX address for auto.ack,      */
/*                   fill TX payload, select RF channel, datarate & TX pwr.     */
/*                   PWR_UP is set, CRC(1 bytes) is enabled, & PRIM:TX.         */
/*                                                                              */
/*                   ToDo: One high pulse(>10us) on CE will now send this       */
/*                   packet and expect an acknowledgment from the RX device.    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Tx_Mode_NRF(void)
{
	nRF24L01_CEL;
	#if(RFM_FLUSH_TX == FLUSH_ENABLED)
	SPI_RW_Reg(FLUSH_TX,0); /* Flush TX Fifo */
	#endif
 	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x3A);     /* Set PWR_UP bit, enable CRC(1 bytes) & Prim:TX. MAX_RT & TX_DS enabled, MAX_RT not reflected on IRQ pin.... */
  	nRF24L01_CEH;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Channel_NRF                                            */
/*                                                                              */
/* DESCRIPTION:      Sets desired channel                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Set_Channel_NRF(uint8_t channel)
{
	#if(RFM_CHSWITCH == CHSWITCH_STANDARD)
	nRF24L01_CEL;
	#endif
	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, nrf_channel_list[channel]);
	#if(RFM_FLUSH_RX == FLUSH_ENABLED)
	SPI_RW_Reg(FLUSH_RX,0); /* Flush RX Fifo */
	#endif
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,0xff); /* clear RX_DR or TX_DS or MAX_RT interrupt flag */
	#if(RFM_CHSWITCH == CHSWITCH_STANDARD)
	nRF24L01_CEH;
	#endif
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Address_NRF                                            */
/*                                                                              */
/* DESCRIPTION:      Sets desired address                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Set_Address_NRF(uint8_t address)
{
	TX_ADDRESS[1] = (uint8_t)(address >> 8);
	TX_ADDRESS[2] = (uint8_t)address;
	RX_ADDRESS[1] = (uint8_t)((~address) >> 8);
	RX_ADDRESS[2] = (uint8_t)(~address);
	#if(RFM_FLUSH_TX == FLUSH_ENABLED)
	SPI_RW_Reg(FLUSH_TX,0); /* Flush TX Fifo */
	#endif
	#if(RFM_FLUSH_RX == FLUSH_ENABLED)
	SPI_RW_Reg(FLUSH_RX,0); /* Flush RX Fifo */
	#endif
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); /* Setup Initial TX Address */
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); /* Setup RX Pipe 0 Address */
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RxDataWireless_NRF                                         */
/*                                                                              */
/* DESCRIPTION:      Reception of the info in buffer                            */
/*                                                                              */
/* PARAMETERS:       In: Buffer                                                 */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RxDataWireless_NRF(uint8_t *buff)
{
	SPI_Read_Buf(RD_RX_PLOAD,buff,RX_PLOAD_WIDTH);/* read nRF24L01 receive payload from RX_FIFO buffer */
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,0xff); /* clear RX_DR or TX_DS or MAX_RT interrupt flag */
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Rx_Mode_NRF                                                */
/*                                                                              */
/* DESCRIPTION:      Activate RX Mode                                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Rx_Mode_NRF(void)
{
	nRF24L01_CEL;
	#if(RFM_FLUSH_RX == FLUSH_ENABLED)
	SPI_RW_Reg(FLUSH_RX,0); /* Flush RX Fifo */
	#endif
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,0xff); /* clear RX_DR or TX_DS or MAX_RT interrupt flag */
  	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x3B);     /* Set PWR_UP bit, enable CRC(1 byte) & Prim:RX. RX_DR enabled, MAX_RT not reflected on IRQ pin.... */
  	nRF24L01_CEH;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Comm_Ready_NRF                                             */
/*                                                                              */
/* DESCRIPTION:      Checks if NRF24L01+ has received information (INT pin LOW) */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_READY, 1: READY                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Comm_Ready_NRF(void)
{
	uint8_t status, reg_status;
	status = NOT_READY;
	/* Polling of IRQ Pin */
	if(!(nRF24L01_IRQP))
	{
		/* Read register STATUS's value */
		reg_status = SPI_Read(STATUS);
		if(reg_status&RX_DR)
		{
			/* Data received ok */
			status = READY; 
		}
		else
		{
			status = NOT_READY;
		}
	}
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Get_Ch_Quality_NRF                                         */
/*                                                                              */
/* DESCRIPTION:      Gets the quality of the current channel                    */
/*                                                                              */
/* PARAMETERS:       Out: 0: No interference, 1: Interference                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Get_Ch_Quality_NRF(void)
{
	uint8_t chquality = 0;
  	SPI_RW_Reg(CD, chquality); /* Select RF channel */
  	return chquality;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Sent_Done_NRF                                              */
/*                                                                              */
/* DESCRIPTION:      Checks if NRF24L01+ has sent the information requested     */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_READY, 1: READY                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Sent_Done_NRF(void)
{
	uint8_t status, reg_status;
	status = NOT_READY;
	/* Polling of IRQ Pin */
	if(!(nRF24L01_IRQP))
	{
		/* read register STATUS's value */
		reg_status = SPI_Read(STATUS);
		if(reg_status&TX_DS)
		{
			/* Data received ok */
			status = READY; 
		}
	}
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Max_Ret_NRF                                                */
/*                                                                              */
/* DESCRIPTION:      Checks if NRF24L01+ has reached the Maximum Retransmit     */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_YET, 1: MAXIMUM                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Max_Ret_NRF(void)
{
	uint8_t status, reg_status;
	status = NOT_READY;
	/* Polling of IRQ Pin */
	if(!(nRF24L01_IRQP))
	{
		/* Read register STATUS's value */
		reg_status = SPI_Read(STATUS);
		if(reg_status&MAX_RT)
		{
			/* Data received ok */
			status = MAXIMUM; 
		}
	}
	return(status);
};



/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_RW                                                     */
/*                                                                              */
/* DESCRIPTION:      Writes one byte to nRF24L01, and return the byte read      */
/*                   from nRF24L01 during write, according to SPI protocol      */
/*                                                                              */
/* PARAMETERS:       In: byte                                                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_RW(uint8_t byte)
{
	/* Hardware SPI */
	byte = ReadByteSPI1(byte);
    return(byte);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_RW_Reg                                                 */
/*                                                                              */
/* DESCRIPTION:      Writes value 'value' to nRF24L01 register 'reg'            */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        value: Value                                          */
/*                   Out: status                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	nRF24L01_CSNL;
	status = SPI_RW(reg);      // select register
  	SPI_RW(value);             // ..and write value to it..
	nRF24L01_CSNH;
  	return(status);            // return nRF24L01 status byte
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_Read                                                   */
/*                                                                              */
/* DESCRIPTION:      Reads one byte from nRF24L01 register 'reg'                */
/*                                                                              */
/* PARAMETERS:       In: reg: Register Number                                   */
/*                   Out: value read                                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_Read(uint8_t reg)
{
	uint8_t reg_val;
	nRF24L01_CSNL;
  	SPI_RW(reg);            // Select register to read from..
  	reg_val = SPI_RW(0);    // ..then read register value
	nRF24L01_CSNH;
  	return(reg_val);        // return register value
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_Read_Buf                                               */
/*                                                                              */
/* DESCRIPTION:      Reads 'bytes' #of bytes from register 'reg'                */
/*                   Typically used to read RX payload, Rx/Tx address           */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        bytes: Number of Bytes to be read                     */
/*                   Out: pBuf: Buffer filled                                   */
/*                        status: status                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status,byte_ctr;
	nRF24L01_CSNL;
  	status = SPI_RW(reg);       		// Select register to write to and read status byte
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0);    // Perform SPI_RW to read byte from nRF24L01
	nRF24L01_CSNH;
  	return(status);                    // return nRF24L01 status byte
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_Write_Buf                                              */
/*                                                                              */
/* DESCRIPTION:      Writes contents of buffer '*pBuf' to nRF24L01              */
/*                   Typically used to write TX payload, Rx/Tx address          */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        pBuf: Buffer to be written                            */
/*                        bytes: Number of Bytes to be read                     */
/*                   Out: status: status                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status,byte_ctr;
    nRF24L01_CSNL;
    status = SPI_RW(reg);    // Select register to write to and read status byte
    for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
        status = SPI_RW(*pBuf++);
    nRF24L01_CSNH;
    return(status);          // return nRF24L01 status byte
}
