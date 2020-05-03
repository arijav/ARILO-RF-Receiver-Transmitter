/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_Handler_LLSM.c                                          */
/*                                                                              */
/* DESCRIPTION:      RF Frequency Hopping Handler                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "RF_Handler_LLSM.h"
#include "../Modules/MCU/hw_mcu.h"
#include "../Modules/MCU/hw_eeprom.h"
#include "../Modules/MCU/hw_timers.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
uint32_t rfhandler_states[MAX_STATE_RF_HANDLER_LLSM];

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
uint8_t flag_nextchannel_checked;

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_SetRFAddress                                      */
/*                                                                              */
/* DESCRIPTION:     Set desired RF TX Address                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_SetRFAddress(uint16_t address)
{
	uint8_t ret_val = 0;
	uint16_t curr_add;
	curr_add = RFHandler_GetRFAddress();
	if(  (rfhandler_llsm.eeprom_set == 0)
	   &&(address != DEF_RF_ADDR)
	   &&(address != curr_add) )
	{
		/* Save address in EEPROM */
		ret_val = EE_WRITE(BINDING_ADDR_EE, address);
		if(ret_val == FLASH_COMPLETE)
		{
			/* Save flag binded in EEPROM */
			ret_val = EE_WRITE(FLAG_BINDINGSET_EE, FLAG_SET_EEPROM);
		}
		if(ret_val == FLASH_COMPLETE)
		{
			/* Save address in RF_Handler */
			rfhandler_llsm.rf_address = address;
			/* Save address in Transceivers */
			RF_SetAddress(address);
		}
		/* Set EEPROM Protection Flag */
		rfhandler_llsm.eeprom_set = 1;
	}
	else
	{
		/* Save address in RF_Handler */
		rfhandler_llsm.rf_address = address;
		/* Save address in Transceivers */
		RF_SetAddress(address);
	}

};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetRFAddress                                      */
/*                                                                              */
/* DESCRIPTION:     Retrieves the current RF Address from Flash                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint16_t RFHandler_GetRFAddress(void)
{
	uint8_t ret_val;
	uint16_t data;
	/* Check EEPROM */
	ret_val = EE_READ(FLAG_BINDINGSET_EE, &data);
	if(  (ret_val == 0)
	   &&(data == FLAG_SET_EEPROM)  )
	{
		ret_val = EE_READ(BINDING_ADDR_EE, &data);
		if(ret_val != 0) data = DEF_RF_ADDR;
	}
	else
	{
		data = DEF_RF_ADDR;
	}
	/* TODO: TEST */
	//data = DEF_RF_ADDR;
	/* Save address in Transceivers */
	//RF_SetAddress(data);
	return(data);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetRXFlag                                         */
/*                                                                              */
/* DESCRIPTION:     Gets the satus of a received flag                           */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline uint8_t RFHandler_GetRXFlag(uint8_t flag)
{
	uint8_t ret_val = 0;
	if(rfhandler_llsm.rxcommand & flag) ret_val = 1;
	return ret_val;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_ModifyRXFlag                                      */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the received defined flag                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline void RFHandler_ModifyRXFlag(uint8_t flag, uint8_t status)
{
	if(status == 1)
	{
		rfhandler_llsm.rxcommand |= flag;
	}
	else
	{
		rfhandler_llsm.rxcommand &= ~flag;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_ModifyTXFlag                                      */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the transmitted defined flag                      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline void RFHandler_ModifyTXFlag(uint8_t flag, uint8_t status)
{
	if(status == 1)
	{
		rfhandler_llsm.txcommand |= flag;
	}
	else
	{
		rfhandler_llsm.txcommand &= ~flag;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetLinkStatus                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the link Status                                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_GetLinkStatus(void)
{
	return(rfhandler_llsm.ctr_status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetPacketSucc                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the Packet Success                                     */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_GetPacketSucc(void)
{
	return((uint8_t)(100 - rfhandler_llsm.rx_packetloss));
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_LinkQuality                                       */
/*                                                                              */
/* DESCRIPTION:     Gets the Link Quality                                       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_LinkQuality(void)
{
	return(RFHandler_GetPacketSucc());
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RF_SendPayload                                              */
/*                                                                              */
/* DESCRIPTION:     Sends Payload through Transceiver                           */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_SendPayload(uint8_t txceiver)
{
    uint8_t temp;
	uint8_t send_buff[TX_PLOAD_WIDTH];
	uint8_t tbuff_pos = rfhandler_llsm.tpos;
    uint8_t tx_packets = (rfhandler_llsm.tx_packetctr + 1) & 0x0F;
	rfhandler_llsm.tx_packetctr = tx_packets;
	rfhandler_llsm.tpos = (tbuff_pos + 1) % TBUFF_SIZE;
	int32_t payload = rfhandler_llsm.tBuffer[tbuff_pos];
	/* Save Command Flags + Signal Quality of Actual Channel */
	temp = (uint8_t)((((uint32_t)rfhandler_llsm.channel_quality[rfhandler_llsm.channel] * 128) / 100) - 1);
	send_buff[0] = (rfhandler_llsm.txcommand | temp);
	/* Save position in buffer of the payload (4 LSB) + Packet Counter (4 HSB) */
    send_buff[1] = (tbuff_pos | (tx_packets << 4));
	/* Save 4 Byte Payload */
	send_buff[2] = (payload >> 24);
	send_buff[3] = (payload >> 16);
	send_buff[4] = (payload >> 8);
	send_buff[5] = payload;
	/* Send Command Byte + PayloadID (pos) + Payload in CURRENT CHANNEL */
	RF_TxDataWireless(send_buff, txceiver);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_ReceivePayload                                    */
/*                                                                              */
/* DESCRIPTION:     Receives Payload through Transceiver                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_ReceivePayload(uint8_t txceiver)
{
	uint8_t rx_buff[RX_PLOAD_WIDTH];

	uint8_t rbuff_pos = 0;
	int32_t payload = 0;
	/* Get Command Byte + PayloadID (pos) + Payload in CURRENT CHANNEL */
	RF_RxDataWireless(rx_buff, txceiver);
	/* Position of payload in RX Buffer */
    rbuff_pos = (rx_buff[1] & 0xF);
    if(rbuff_pos < rfhandler_llsm.rbuff_size)
	{
		/* First three bits are the RX Command */
		rfhandler_llsm.rxcommand = (rx_buff[0] & 0xE0);
		/* Next five bits are the next transmission channel */
		rfhandler_llsm.next_channel = (uint8_t)(rx_buff[0] & 0x1F);
		/* Payload */
		payload = ((int32_t)(rx_buff[2]) << 24) | ((int32_t)(rx_buff[3]) << 16) | ((int32_t)(rx_buff[4]) << 8) | (rx_buff[5]);
		rfhandler_llsm.rBuffer[rbuff_pos] = payload;
        rfhandler_llsm.rx_packetctr = (rx_buff[1] >> 4);
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_init                                         */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_llsm_init(void)
{
	uint8_t aux;
	/* Initialize Data Structure to Defaults */

	rfhandler_llsm.timer_rxlost = LOSTRX_TIME;
	rfhandler_llsm.state = RF_HANDLER_INIT;
	rfhandler_llsm.channel = RF_INITCHANNEL;
	rfhandler_llsm.next_channel = GET_RANDOM_CHANNEL;
	rfhandler_llsm.tpos = 0;
	rfhandler_llsm.rf_address = RFHandler_GetRFAddress();
	RFHandler_SetRFAddress(rfhandler_llsm.rf_address);
	rfhandler_llsm.txcommand = 0;
	rfhandler_llsm.rxcommand = 0;
	rfhandler_llsm.timer_txmode = 0;
	rfhandler_llsm.num_ban_chs = 0;
	rfhandler_llsm.ctr_status = 0;
    rfhandler_llsm.cnt = 0;
    for(aux = 0; aux < MAX_RF_CHANNELS; aux++)
	{
		rfhandler_llsm.channel_quality[aux] = 100;
	}
	rfhandler_llsm.eeprom_set = 0;
	/* Initialize other data */
	flag_nextchannel_checked = 0;
    rfhandler_llsm.rx_packetctr = 0;
    rfhandler_llsm.rx_packetctr_old = 0;
    rfhandler_llsm.rx_packetlost = 0;
    rfhandler_llsm.rx_totpacket = 0;
    rfhandler_llsm.rx_packetloss = 0;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_reinit                                       */
/*                                                                              */
/* DESCRIPTION:     Re-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_llsm_reinit(void)
{
	rfhandler_llsm.state = RF_HANDLER_REINIT;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_deinit                                       */
/*                                                                              */
/* DESCRIPTION:     De-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_llsm_deinit(void)
{
	rfhandler_llsm.state = RF_HANDLER_DEINIT;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetRandomChannel                                  */
/*                                                                              */
/* DESCRIPTION:     Gets random channel                                         */
/*                                                                              */
/* PARAMETERS:      Out: Random Channel                                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_GetRandomChannel(void)
{
	uint8_t channel = 0;
	channel = (rfhandler_llsm.channel + 1)%MAX_RF_CHANNELS;
	return(channel);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_main                                         */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_llsm_main(void)
{
	uint8_t loc_ch;
	uint8_t loc_state = rfhandler_llsm.state;
	uint8_t txceiver = NRF24L01_TRX;
	uint8_t flag_received;
	rfhandler_states[rfhandler_llsm.state]++;
	switch(loc_state)
	{
		case RF_HANDLER_SHUTDOWN:
			/* Nothing to do */
		break;
		case RF_HANDLER_DEINIT:
			/* Entry actions */
			/* Deactivate TRX */
			rfhandler_llsm.rx_packetctr = 0;
     		rfhandler_llsm.rx_packetctr_old = 0;
			rfhandler_llsm.ctr_status = 0;
			RF_DeInit(txceiver);
			rfhandler_llsm.state = RF_HANDLER_SHUTDOWN;
		break;
		case RF_HANDLER_REINIT:
			/* Entry actions */
			/* Deactivate TRX */
			rfhandler_llsm.rx_packetctr = 0;
    		rfhandler_llsm.rx_packetctr_old = 0;
            rfhandler_llsm.ctr_status = 0;
			RF_Init(txceiver);
			RFHandler_llsm_init();
			rfhandler_llsm.state = RF_HANDLER_INIT;
		break;
		case RF_HANDLER_INIT:
			/* Entry actions */
			if(rfhandler_llsm.init_buff == RF_BUFF_INITIATED)
			{
				rfhandler_llsm.channel = RFHandler_GetRandomChannel();
				RF_SetChannel(rfhandler_llsm.channel, txceiver);
				RF_RxMode(txceiver);
				rfhandler_llsm.ctr_status = 0;
				rfhandler_llsm.timer_rxlost = LOSTRX_TIME_SCAN;
				rfhandler_llsm.timer_rxlostlong = LOSTRX_TIME_RESET;
				rfhandler_llsm.state = RF_SCAN_CHANNELS;
			}
		break;
		case RF_SCAN_CHANNELS:
			/* Entry actions */
			rfhandler_llsm.timer_rxlost--;
			if(rfhandler_llsm.timer_rxlostlong > 0) rfhandler_llsm.timer_rxlostlong--;
			flag_received = RF_RxDone(txceiver);
			if(flag_received == 1)
			{
				rfhandler_llsm.ctr_status = 1;
				rfhandler_llsm.timer_rxlost = LOSTRX_TIME;
				rfhandler_llsm.timer_rxlostlong = LOSTRX_TIME_RESET;
                rfhandler_llsm.rx_packetctr_old = rfhandler_llsm.rx_packetctr;
                if(rfhandler_llsm.rx_totpacket < UINT16_MAX) rfhandler_llsm.rx_totpacket++;
                rfhandler_llsm.rx_packetctr_old = rfhandler_llsm.rx_packetctr;
				/* Get and decode message */
				RFHandler_ReceivePayload(txceiver);
				if(RFHandler_GetRXFlag(RF_RX_TXMITREQUEST_FLAG) == 1)
				{
					rfhandler_llsm.state = RF_SEND_DATA;
					RF_TxMode(txceiver);
					RFHandler_ModifyRXFlag(RF_RX_ALL_FLAGS, 0);
				}
				else
				{
					rfhandler_llsm.state = RF_RECEIVE_DATA;
				}
			}
			#if(RF_HANDLER_TRX_RESET == TRX_RESET_ENABLED)
			else if(rfhandler_llsm.timer_rxlostlong == 0)
			{
				rfhandler_llsm.timer_rxlost = LOSTRX_TIME_SCAN;
				rfhandler_llsm.timer_rxlostlong = TXCEIVER_TIME_RESET;
				if(rfhandler_llsm.rx_totpacket < UINT16_MAX) rfhandler_llsm.rx_totpacket++;
				rfhandler_llsm.state = RF_RESET;
			}
			#endif
			else if(rfhandler_llsm.timer_rxlost == 0)
			{
				rfhandler_llsm.timer_rxlost = LOSTRX_TIME_SCAN;
				rfhandler_llsm.channel = RFHandler_GetRandomChannel();
				RF_SetChannel(rfhandler_llsm.channel, txceiver);
			}
		break;
		case RF_RECEIVE_DATA:
			rfhandler_llsm.cnt++;
		    if((rfhandler_llsm.cnt % RF_HANDLER_RXPERIOD) == 0)
		    {
				/* If just arrived from sending, change to RX Mode */
				if(rfhandler_llsm.timer_txmode >= MAX_TIME_TX)
				{
					rfhandler_llsm.timer_txmode = 0;
					RFHandler_ModifyTXFlag(RF_TX_ALL_FLAGS, 0);
					/* Change to receive mode and set txceiver in Receive Mode */
					RF_RxMode(txceiver);
				}
				else
				{
					/* Assign channel to local var. */
					loc_ch = rfhandler_llsm.channel;
					/* Receive Data */
					rfhandler_llsm.timer_rxlost--;
					/* Check if received */
					flag_received = RF_RxDone(txceiver);
					if(flag_received == 1)
					{
						/* Get and decode message */
						RFHandler_ReceivePayload(txceiver);
						rfhandler_llsm.timer_rxlost = LOSTRX_TIME;
						/* Reset Flags */
						flag_nextchannel_checked = 0;
                        rfhandler_llsm.rx_totpacket++;
                        /* Calculate number of lost packets */
                        if(rfhandler_llsm.rx_packetctr != ((rfhandler_llsm.rx_packetctr_old + 1) & 0x0F))
                        {
                            if(rfhandler_llsm.rx_packetctr > rfhandler_llsm.rx_packetctr_old)
                            {
                                rfhandler_llsm.rx_packetlost += (rfhandler_llsm.rx_packetctr - rfhandler_llsm.rx_packetctr_old);
                            }
                            else
                            {
                                rfhandler_llsm.rx_packetlost += (16 - rfhandler_llsm.rx_packetctr_old + rfhandler_llsm.rx_packetctr);
                            }
                        }
                        rfhandler_llsm.rx_packetctr_old = rfhandler_llsm.rx_packetctr;
						/* If change of channel is requested */
						if(RFHandler_GetRXFlag(RF_RX_LASTTXCH_FLAG) == 1)
						{
                            /* Calculate packet loss */
                            rfhandler_llsm.rx_totpacket += rfhandler_llsm.rx_packetlost;
                            rfhandler_llsm.rx_packetloss = 100 * ((float32_t)rfhandler_llsm.rx_packetlost / (float32_t)rfhandler_llsm.rx_totpacket);
                            rfhandler_llsm.rx_packetlost = 0;
                            rfhandler_llsm.rx_totpacket = 0;
                            rfhandler_llsm.channel_quality[loc_ch] = 100 - rfhandler_llsm.rx_packetloss;
                            rfhandler_llsm.channel =  rfhandler_llsm.next_channel;
                            RF_SetChannel(rfhandler_llsm.channel, txceiver);
                            RFHandler_ModifyRXFlag(RF_RX_ALL_FLAGS, 0);
						}
						else if(RFHandler_GetRXFlag(RF_RX_TXMITREQUEST_FLAG) == 1)
						{
							rfhandler_llsm.cnt = 0;
							rfhandler_llsm.state = RF_SEND_DATA;
							RF_TxMode(txceiver);
							RFHandler_ModifyRXFlag(RF_RX_ALL_FLAGS, 0);
						}
					}
					else
					{
						if(rfhandler_llsm.timer_rxlost == 0)
						{
							rfhandler_llsm.rx_packetlost = 100;
							rfhandler_llsm.channel_quality[loc_ch] = 0;
							if(flag_nextchannel_checked == 0)
							{
								flag_nextchannel_checked = 1;
								rfhandler_llsm.timer_rxlost = NEXTCHANNEL_TIME;
								rfhandler_llsm.channel = rfhandler_llsm.next_channel;
								RF_SetChannel(rfhandler_llsm.channel, txceiver);
							}
							else
							{
								rfhandler_llsm.cnt = 0;
								flag_nextchannel_checked = 0;
								rfhandler_llsm.ctr_status = 0;
								rfhandler_llsm.timer_rxlost = LOSTRX_TIME_SCAN;
								RF_RxMode(txceiver);
								rfhandler_llsm.state = RF_SCAN_CHANNELS;
							}
						}
					}
				}
			}
		break;
		case RF_SEND_DATA:
			rfhandler_llsm.cnt++;
			if((rfhandler_llsm.cnt % RF_HANDLER_TXPERIOD) == 0)
			{
				rfhandler_llsm.timer_txmode++;
				if(rfhandler_llsm.timer_txmode >= MAX_TIME_TX)
				{
					rfhandler_llsm.cnt = 0;
					/* Send Notification to Transmitter that there is nothing else to receive */
					RFHandler_ModifyTXFlag(RF_TX_TXMITDONE_FLAG, 1);
					rfhandler_llsm.state = RF_RECEIVE_DATA;
				}
				RFHandler_SendPayload(txceiver);
			}
		break;
		#if(RF_HANDLER_TRX_RESET == TRX_RESET_ENABLED)
		case RF_RESET:
			/* Reinit Transceiver */
			if(rfhandler_llsm.timer_rxlostlong == TXCEIVER_TIME_RESET)
			{
				rfhandler_llsm.timer_rxlostlong--;
				RF_Init(txceiver);
			}
			else if(rfhandler_llsm.timer_rxlostlong == 0)
			{
				rfhandler_llsm.cnt = 0;
				rfhandler_llsm.timer_rxlostlong = LOSTRX_TIME_RESET;
				rfhandler_llsm.state = RF_HANDLER_INIT;
			}
			else
			{
				rfhandler_llsm.timer_rxlostlong--;
			}
		break;
		#endif
		default:
		break;
	};
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_SetupExtBuffers                                   */
/*                                                                              */
/* DESCRIPTION:     This function will setup the Application Ext. Buffers       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_SetupExtBuffers(int32_t *tbuffer, uint8_t tlength, int32_t *rbuffer, uint8_t rlength)
{
	rfhandler_llsm.tBuffer = tbuffer;
	rfhandler_llsm.rBuffer = rbuffer;
	rfhandler_llsm.tbuff_size = tlength;
	rfhandler_llsm.rbuff_size = rlength;
	rfhandler_llsm.init_buff = RF_BUFF_INITIATED;
}

/*
 * RF_Handler_LLSM.c
 *
 *  Created on: 25.09.2013
 *      Author: ArigitaJ
 */


