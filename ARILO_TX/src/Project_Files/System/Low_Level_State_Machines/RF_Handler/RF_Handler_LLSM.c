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
#include "../Modules/MCU/hw_timers.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
uint32_t rfhandler_states[MAX_STATE_RF_HANDLER_LLSM];

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
/* NAME:            RFHandler_SetRFAddress                                      */
/*                                                                              */
/* DESCRIPTION:     Set desired RF TX Address                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_SetRFAddress(uint16_t address)
{
	rfhandler_llsm.rf_address = address;
	/* Save address in Transceivers */
	RF_SetAddress(address);
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
	uint16_t address;

	address = (uint16_t)DEVICE_ID;
	/* Save address in Transceivers */
	return(address);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetRXFlag                                         */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the defined flag                                  */
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
/* NAME:            RFHandler_ModifyTXFlag                                      */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the defined flag                                  */
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
/* NAME:            RFHandler_ModifyRXFlag                                      */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the defined flag                                  */
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
/* NAME:            RFHandler_GetLinkStatus                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the link Status                                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_GetLinkStatus(void)
{
	return(rfhandler_llsm.tel_status);
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
	uint8_t send_buff[TX_PLOAD_WIDTH];
	uint8_t tbuff_pos = rfhandler_llsm.tpos;
	uint8_t tx_packets = (rfhandler_llsm.tx_packetctr + 1) & 0x0F;
	rfhandler_llsm.tx_packetctr = tx_packets;
	rfhandler_llsm.tpos = (rfhandler_llsm.tpos + 1) % TBUFF_SIZE;
	int32_t payload = rfhandler_llsm.tBuffer[tbuff_pos];
	/* Save Command Flags + Next Transmission Channel in first byte (most significant) */
	send_buff[0] = (rfhandler_llsm.txcommand | rfhandler_llsm.next_channel);
	/* Save position in buffer of the payload */
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
		/* First bit is the RX Command */
		rfhandler_llsm.rxcommand = (rx_buff[0] & 0x80);
		/* Next seven bits are the quality of the actual channel (in %) */
		rfhandler_llsm.channel_quality[rfhandler_llsm.channel] = (uint8_t)(((uint32_t)(rx_buff[0] & 0x7F) + 1) * 100) / 128;
		/* Payload */
		payload = ((int32_t)(rx_buff[2]) << 24) | ((int32_t)(rx_buff[3]) << 16) | ((int32_t)(rx_buff[4]) << 8) | (rx_buff[5]);
		rfhandler_llsm.rBuffer[rbuff_pos] = payload;
        rfhandler_llsm.rx_packetctr = (rx_buff[1] >> 4);
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_CheckRFQualityTXC                                 */
/*                                                                              */
/* DESCRIPTION:     Checks channel quality specific to TXCEIVER                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_CheckRFQualityTXC(uint8_t txceiver)
{
	/* Check the quality of the actual channel (transmitter side) */
	uint8_t ch_quality = RF_Ch_Quality(txceiver);
	uint8_t channel = rfhandler_llsm.channel;

	/* The actual channel is noisy */
	if(ch_quality == BAD_RFCHANNEL)
	{
		if(rfhandler_llsm.channel_banlist[channel] == 0)
		{
			/* Put the channel in the Ban List by increasing its ban time */
			rfhandler_llsm.channel_banlist[channel] = RF_BANNED_TIME;
			rfhandler_llsm.num_ban_chs++;
		}
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_CheckRFQualityPCTR                                */
/*                                                                              */
/* DESCRIPTION:     Checks channel quality based on Packet Counters             */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_CheckRFQualityPCTR(void)
{
	uint8_t ch_quality, channel, aux, tellink;
	channel = rfhandler_llsm.channel;

	/* Check the quality of the actual channel (receiver side) */
	if(rfhandler_llsm.rx_packetctr == 0)
	{
		ch_quality = 0;
		rfhandler_llsm.channel_quality[channel] = 0;
	}
	else
	{
		ch_quality = (uint8_t)rfhandler_llsm.channel_quality[channel];
	}

	/* Check if all channels are reported as lower than Packet Threshold (Telemetry Lost) */
	for(aux = 0; aux < MAX_RF_CHANNELS; aux++)
	{
		if(rfhandler_llsm.channel_quality[aux] > BAD_RFCHANNEL_THR) tellink = 1;
	}

	if(tellink == 1)
	{
		/* If Channel is not ok from Quality Point of view */
		if(ch_quality <= BAD_RFCHANNEL_THR)
		{
			if(rfhandler_llsm.channel_banlist[channel] == 0)
			{
				/* Put the channel in the Ban List by increasing its ban time */
				rfhandler_llsm.channel_banlist[channel] = RF_BANNED_TIME;
				rfhandler_llsm.num_ban_chs++;
			}
		}
		/* If Channel is ok from Quality Point of view */
		else
		{
			if(rfhandler_llsm.channel_banlist[channel] != 0)
			{
				rfhandler_llsm.channel_banlist[channel] = 0;
				rfhandler_llsm.num_ban_chs--;
			}
		}
		//rfhandler_llsm.tel_status = 1;
	}
	else
	{
		/* Telemetry Link Lost, report it and do not Ban RX 0 Quality Channels */
		//rfhandler_llsm.tel_status = 0;
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
	rfhandler_llsm.timer_chswitch = 0;
	rfhandler_llsm.timer_rxswitch = 0;
	rfhandler_llsm.state = RF_HANDLER_INIT;
	rfhandler_llsm.channel = RF_INITCHANNEL;
	rfhandler_llsm.next_channel = GET_RANDOM_CHANNEL;
	rfhandler_llsm.tpos = 0;
	rfhandler_llsm.rf_address = RFHandler_GetRFAddress();
	RFHandler_SetRFAddress(rfhandler_llsm.rf_address);
	rfhandler_llsm.txcommand = 0;
	rfhandler_llsm.rxcommand = 0;
	rfhandler_llsm.timer_rxmode = 0;
	rfhandler_llsm.num_ban_chs = 0;
	rfhandler_llsm.tel_status = 0;
	rfhandler_llsm.cnt = 0;
	for(aux = 0; aux < MAX_RF_CHANNELS; aux++)
	{
		rfhandler_llsm.channel_quality[aux] = 100;
		rfhandler_llsm.channel_banlist[aux] = 0;
		rfhandler_llsm.channel_cycle[aux] = 0;
	}
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
/* NAME:            RFHandler_BanListUpdate                                     */
/*                                                                              */
/* DESCRIPTION:     Updates the ban times periodically                          */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_BanListUpdate(void)
{
	uint8_t i;
	for(i = 0; i < MAX_RF_CHANNELS; i++)
	{
		if((rfhandler_llsm.channel_banlist[i]) > 0)
		{
			/* Decrement banned time for each channel */
			rfhandler_llsm.channel_banlist[i]--;

			/* If the channel has just reached 0 the number of banned channels */
			/* should be decreased                                             */
			if(rfhandler_llsm.channel_banlist[i] == 0)
			{
				rfhandler_llsm.num_ban_chs--;
			}
		}
	}
	/* Reset the list if a minimum number of channels are left (under threshold) */
	if(rfhandler_llsm.num_ban_chs >= (MAX_RF_CHANNELS - MIN_RF_CHANNELS))
	{
		for(i = 0; i < MAX_RF_CHANNELS; i++)
		{
			rfhandler_llsm.channel_banlist[i] = 0;
		}
		rfhandler_llsm.num_ban_chs = 0;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_CheckStatusTRX                                    */
/*                                                                              */
/* DESCRIPTION:     Checks Transceiver Status                                   */
/*                                                                              */
/* PARAMETERS:      Out: Status (1 ok, 0 notok)                                 */
/*                  In: Mode to check (TX / RX)                                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_CheckStatusTRX(uint8_t transceiver, uint8_t mode)
{
	return(RF_CheckTRX(transceiver, mode));
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetNextChannel                                    */
/*                                                                              */
/* DESCRIPTION:     Gets next channel                                           */
/*                                                                              */
/* PARAMETERS:      Out: Next Channel                                           */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_GetNextChannel(void)
{
	uint8_t channel = 0;
	uint8_t aux = MAX_RF_CHANNELS;
	channel = GET_RANDOM_CHANNEL;
	/* If this channel has been already used in this cycle */
	while(rfhandler_llsm.channel_cycle[channel] == 1)
	{
		/* Pick up next available channel */
		channel += 1;
		channel %= MAX_RF_CHANNELS;
		aux--;
		if(aux == 0)
		{
			/* Cycle is consumed */
			for(aux = 0; aux < MAX_RF_CHANNELS; aux++) rfhandler_llsm.channel_cycle[aux] = 0;
			break;
		}
	}
	rfhandler_llsm.channel_cycle[channel] = 1;

	#if(RF_HANDLER_BANNEDCH == BANNED_ENABLED)
	/* Avoid Banned Channels, repeat search a maximum number of 25 times */
	for(aux = 0; aux < MAX_RF_CHANNELS; aux++)
	{
		/* If it is a banned channel */
		if(rfhandler_llsm.channel_banlist[channel] != 0)
		{
			/* Pick a channel 4 channels away and try again */
			channel = ((channel + 4) % MAX_RF_CHANNELS);
		}
		else
		{
			/* Channel is approved */
			break;
		}
	}
	#endif
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
	uint8_t loc_state = rfhandler_llsm.state;
	uint8_t txceiver = NRF24L01_TRX;
	uint8_t flag_received;
	rfhandler_states[loc_state]++;
	switch(loc_state)
	{
		case RF_HANDLER_SHUTDOWN:
			/* Nothing to do */
		break;
		case RF_HANDLER_DEINIT:
			/* Entry actions */
			/* Deactivate TRX */
			rfhandler_llsm.rx_packetctr = 0;
			rfhandler_llsm.tel_status = 0;
			RF_DeInit(txceiver);
			rfhandler_llsm.state = RF_HANDLER_SHUTDOWN;
		break;
		case RF_HANDLER_REINIT:
			/* Entry actions */
			/* Deactivate TRX */
			rfhandler_llsm.rx_packetctr = 0;
            rfhandler_llsm.tel_status = 0;
			RF_Init(txceiver);
			RFHandler_llsm_init();
			rfhandler_llsm.state = RF_HANDLER_INIT;
		break;
		case RF_HANDLER_INIT:
			/* Entry actions */
			if(rfhandler_llsm.init_buff == RF_BUFF_INITIATED)
			{
				RF_SetChannel(rfhandler_llsm.channel, txceiver);
				RF_TxMode(txceiver);
				rfhandler_llsm.state = RF_SEND_DATA;
			}
		break;
		case RF_SEND_DATA:
			rfhandler_llsm.cnt++;
			if((rfhandler_llsm.cnt % RF_HANDLER_TXPERIOD) == 0)
			{
				/* Increment Counters */
				rfhandler_llsm.timer_chswitch++;
				rfhandler_llsm.timer_rxswitch++;
				/* Send Data */
				RFHandler_ModifyTXFlag(RF_TX_ALL_FLAGS, 0);

				/* Evaluate if channel needs to be switched */
				if(rfhandler_llsm.timer_chswitch >= CHANNEL_TIME)
				{
					rfhandler_llsm.channel = rfhandler_llsm.next_channel;
					rfhandler_llsm.timer_chswitch = 0;
					rfhandler_llsm.next_channel = RFHandler_GetNextChannel();
					RF_SetChannel(rfhandler_llsm.channel, txceiver);
				}
				/* Set command CH shift flag if needed */
				else if(rfhandler_llsm.timer_chswitch == (CHANNEL_TIME - 1))
				{
					RFHandler_ModifyTXFlag(RF_TX_LASTTXCH_FLAG, 1);
				}
				/* Otherwise evaluate if transition to RX Mode is required */
				else if( (  (rfhandler_llsm.timer_chswitch >= NORX_SWITCH_TIME)
							&&(rfhandler_llsm.timer_chswitch <= (CHANNEL_TIME - NORX_SWITCH_TIME))
						)
						&& (rfhandler_llsm.timer_rxswitch >= RXSWITCH_PERIOD)
						)
				{
						RFHandler_ModifyTXFlag(RF_TX_TXMITREQUEST_FLAG, 1);
						rfhandler_llsm.timer_rxswitch = 0;
						rfhandler_llsm.cnt = 0;
						rfhandler_llsm.state = RF_RECEIVE_DATA;
				}
				/* Send Payload */
				RFHandler_SendPayload(txceiver);
			}
		break;
		case RF_RECEIVE_DATA:
			rfhandler_llsm.cnt++;
			if((rfhandler_llsm.cnt % RF_HANDLER_TXPERIOD) == 0) rfhandler_llsm.timer_chswitch++;
			if((rfhandler_llsm.cnt % RF_HANDLER_RXPERIOD) == 0)
			{
				/* Increase RX Mode Timer */
				rfhandler_llsm.timer_rxmode++;
				/* Check RF Quality specific to TXCEIVER */
				RFHandler_CheckRFQualityTXC(txceiver);
				if(rfhandler_llsm.timer_rxmode > 1)
				{
					/* Check if we have received something */
					flag_received = RF_RxDone(txceiver);
					if(flag_received == 1)
					{
						/* Get and decode message */
						RFHandler_ReceivePayload(txceiver);
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
                        rfhandler_llsm.rx_totpacket++;
					}

					/* If we are close to change the channel or we have consumed the RX Time */
					if(  (rfhandler_llsm.timer_chswitch <= 6)
							 ||(rfhandler_llsm.timer_rxmode >= MAX_TIME_RX)
							 ||(RFHandler_GetRXFlag(RF_RX_TXMITDONE_FLAG))
						   )
					{
						/* Reset Flags */
						RFHandler_ModifyRXFlag(RF_RX_ALL_FLAGS, 0);
						/* Check RF Quality based on Packet Counters */
						RFHandler_CheckRFQualityPCTR();

						/* Report Telemetry Status */
                        if(rfhandler_llsm.rx_totpacket == 0)
                        {
                            rfhandler_llsm.tel_status = 0;
                        }
                        else
                        {
                            rfhandler_llsm.tel_status = 1;
                        }

                        /* Calculate packet loss */
                        if(rfhandler_llsm.rx_totpacket > 0)
                        {
                            rfhandler_llsm.rx_totpacket += rfhandler_llsm.rx_packetlost;
                            rfhandler_llsm.rx_packetloss = 100 * ((float32_t)rfhandler_llsm.rx_packetlost / (float32_t)rfhandler_llsm.rx_totpacket);
                        }
                        else
                        {
                            rfhandler_llsm.rx_packetloss = 100;
                        }
                        rfhandler_llsm.rx_packetlost = 0;
                        rfhandler_llsm.rx_totpacket = 0;

						/* Change to TX Mode */
						RF_TxMode(txceiver);
						/* Reset RX Timer */
						rfhandler_llsm.timer_rxmode = 0;
						rfhandler_llsm.cnt = 0;
						/* Change to RF_SEND_DATA State */
						rfhandler_llsm.state = RF_SEND_DATA;
					}
				}
				else
				{
					/* If just arrived from sending, change to RX Mode */
					RF_RxMode(txceiver);
				}
			}
		break;
		default:
		break;
	};

	/* Update Ban List Timers */
	RFHandler_BanListUpdate();
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


