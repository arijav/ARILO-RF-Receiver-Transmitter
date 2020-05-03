/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_commonrf                                                */
/*                                                                              */
/* DESCRIPTION:      Common Functions for Txceiver Modules                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "hw_commonrf.h"

/* TODO: Test */
uint32_t channels_used[MAX_RF_CHANNELS];

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_Init                                                    */
/*                                                                              */
/* DESCRIPTION:      Initialize Transceiver                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RF_Init(uint8_t txceiver)
{
	uint8_t status = 0;
	switch(txceiver)
	{
		case NRF24L01_TRX:
			Init_NRF();
		break;
		case HOPERF_TRX:
			status = Init_Hoperf();
		break;
		default:
		break;
	}
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_DeInit                                                  */
/*                                                                              */
/* DESCRIPTION:      De-Initialize Transceiver                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RF_DeInit(uint8_t txceiver)
{
	uint8_t status = 0;
	switch(txceiver)
	{
		case NRF24L01_TRX:
			DeInit_NRF();
		break;
		case HOPERF_TRX:
			DeInit_Hoperf();
		break;
		default:
		break;
	}
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_CheckTRX                                                */
/*                                                                              */
/* DESCRIPTION:      Function to check transceiver                              */
/*                                                                              */
/* PARAMETERS:       Out: status                                                */
/*                   In:  Transceiver                                           */
/*                   	  Mode                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RF_CheckTRX(uint8_t txceiver, uint8_t mode)
{
	uint8_t status = 0;
	switch(txceiver)
	{
		case NRF24L01_TRX:
			status = CheckTRX_NRF(mode);
		break;
		case HOPERF_TRX:
			// CheckTRX_Hoperf();// TODO
		break;
		default:
		break;
	}
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_TxDataWireless                                          */
/*                                                                              */
/* DESCRIPTION:      Function to send data                                      */
/*                                                                              */
/* PARAMETERS:       In: buffer                                                 */
/*                       channel                                                */
/*                       txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RF_TxDataWireless(uint8_t *buffer, uint8_t txceiver)
{
	switch(txceiver)
	{
		case NRF24L01_TRX:
			TxDataWireless_NRF(buffer);
		break;
		case HOPERF_TRX:
			TxDataWireless_Hoperf(buffer);
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_TxMode                                                  */
/*                                                                              */
/* DESCRIPTION:      Setup TX Mode                                              */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RF_TxMode(uint8_t txceiver)
{
	switch(txceiver)
	{
		case NRF24L01_TRX:
			Tx_Mode_NRF();
		break;
		case HOPERF_TRX:
			Tx_Mode_Hoperf();
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_TxDone                                                  */
/*                                                                              */
/* DESCRIPTION:      Checks if something has been received                      */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                   Out: flag                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RF_TxDone(uint8_t txceiver)
{
	uint8_t tx_done = 0;
	switch(txceiver)
	{
		case NRF24L01_TRX:
			tx_done = Sent_Done_NRF();
		break;
		case HOPERF_TRX:
			tx_done = Sent_Done_Hoperf();
		break;
		default:
		break;
	}
	return(tx_done);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_SetChannel                                              */
/*                                                                              */
/* DESCRIPTION:      Setup Channel                                              */
/*                                                                              */
/* PARAMETERS:       In: channel                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RF_SetChannel(uint8_t channel, uint8_t txceiver)
{
	switch(txceiver)
	{
		case NRF24L01_TRX:
			Set_Channel_NRF(channel);

			/* TODO: Test */
			channels_used[channel]++;
		break;
		case HOPERF_TRX:
			Set_Channel_Hoperf(channel);
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_SetAddress                                              */
/*                                                                              */
/* DESCRIPTION:      Setup Address                                              */
/*                                                                              */
/* PARAMETERS:       In: channel                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RF_SetAddress(uint16_t address)
{
	/* For NRF24L01+ */
	Set_Address_NRF(address);
	/* For Hoperf */
	Set_Address_Hoperf(address);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_RxDataWireless                                          */
/*                                                                              */
/* DESCRIPTION:      Returns pointer to incomming data                          */
/*                                                                              */
/* PARAMETERS:       Out: buffer                                                */
/*                   In:  txceiver                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RF_RxDataWireless(uint8_t *buffer, uint8_t txceiver)
{
	switch(txceiver)
	{
		case NRF24L01_TRX:
			RxDataWireless_NRF(buffer);
		break;
		case HOPERF_TRX:
			RxDataWireless_Hoperf(buffer);
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_RxMode                                                  */
/*                                                                              */
/* DESCRIPTION:      Sets Transceiver in RX_Mode                                */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RF_RxMode(uint8_t txceiver)
{
	switch(txceiver)
	{
		case NRF24L01_TRX:
			Rx_Mode_NRF();
		break;
		case HOPERF_TRX:
			Rx_Mode_Hoperf();
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_RxDone                                                  */
/*                                                                              */
/* DESCRIPTION:      Checks if something has been received                      */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                   Out: flag                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RF_RxDone(uint8_t txceiver)
{
	uint8_t rx_done = 0;
	switch(txceiver)
	{
		case NRF24L01_TRX:
			rx_done = Comm_Ready_NRF();
		break;
		case HOPERF_TRX:
			rx_done = Comm_Ready_Hoperf();
		break;
		default:
		break;
	}
	return(rx_done);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_Ch_Quality                                              */
/*                                                                              */
/* DESCRIPTION:      Checks the Channel Quality                                 */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                   Out: 0: No interference, 1: Interference                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RF_Ch_Quality(uint8_t txceiver)
{
	uint8_t ch_quality = 0;
	switch(txceiver)
	{
		case NRF24L01_TRX:
			ch_quality = Get_Ch_Quality_NRF();
		break;
		case HOPERF_TRX:
			ch_quality = Get_Ch_Quality_Hoperf();
		break;
		default:
		break;
	}
	return(ch_quality);
}

/*
 * hw_commonrf.c
 *
 *  Created on: 27.09.2013
 *      Author: ArigitaJ
 */


