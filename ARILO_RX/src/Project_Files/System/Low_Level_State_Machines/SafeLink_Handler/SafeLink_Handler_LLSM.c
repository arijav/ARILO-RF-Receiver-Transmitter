/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SafeLink_Handler_LLSM.c                                    */
/*                                                                              */
/* DESCRIPTION:      SafeLink Frequency Hopping Handler                         */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "SafeLink_Handler_LLSM.h"
#include "../Modules/MCU/hw_mcu.h"
#include "../Modules/MCU/hw_timers.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
uint32_t slhandler_states[MAX_STATE_SL_HANDLER_LLSM];
uint8_t slhandler_states_old;

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
/* NAME:            SafeLinkHandler_SetRFAddress                                */
/*                                                                              */
/* DESCRIPTION:     Set desired RF TX Address                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_SetRFAddress(uint16_t address)
{
	/* Save address in RF_Handler */
	slhandler_llsm.rf_address = address;
	/* Save address in Transceivers */
	RF_SetAddress(address);
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRFAddress                                */
/*                                                                              */
/* DESCRIPTION:     Retrieves the current RF Address from Flash                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint16_t SafeLinkHandler_GetRFAddress(void)
{
	uint8_t ret_val;
	uint16_t data;
	/* Check EEPROM */
	ret_val = EE_READ(FLAG_BINDINGSET_EE, &data);
	if(  (ret_val == 0)
	   &&(data == FLAG_SET_EEPROM)  )
	{
		ret_val = EE_READ(BINDING_ADDR_EE, &data);
		if(ret_val != 0) data = DEF_SL_ADDR;
	}
	else
	{
		data = DEF_SL_ADDR;
	}
	return(data);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRXFlag                                   */
/*                                                                              */
/* DESCRIPTION:     Gets the satus of a received flag                           */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline uint8_t SafeLinkHandler_GetRXFlag(uint8_t flag)
{
	uint8_t ret_val = 0;
	if(slhandler_llsm.rxcommand & flag) ret_val = 1;
	return ret_val;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_ModifyRXFlag                                */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the received defined flag                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline void SafeLinkHandler_ModifyRXFlag(uint8_t flag, uint8_t status)
{
	if(status == 1)
	{
		slhandler_llsm.rxcommand |= flag;
	}
	else
	{
		slhandler_llsm.rxcommand &= ~flag;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_ModifyTXFlag                                */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the transmitted defined flag                      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
inline void SafeLinkHandler_ModifyTXFlag(uint8_t flag, uint8_t status)
{
	if(status == 1)
	{
		slhandler_llsm.txcommand |= flag;
	}
	else
	{
		slhandler_llsm.txcommand &= ~flag;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetLinkStatus                               */
/*                                                                              */
/* DESCRIPTION:     Gets the link Status                                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_GetLinkStatus(void)
{
	return(slhandler_llsm.ctr_status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRSSI                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the RX RSSI                                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_GetRSSI(void)
{
	return(Get_Ch_Quality_Hoperf());
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetPacketSucc                               */
/*                                                                              */
/* DESCRIPTION:     Gets the Packet Success                                     */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_GetPacketSucc(void)
{
	return((uint8_t)(100 - slhandler_llsm.rx_packetloss));
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_LinkQualityRX                               */
/*                                                                              */
/* DESCRIPTION:     Gets the Link Quality                                       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_LinkQualityRX(void)
{
    #if(SL_HANDLER_CHQ == Q_SNR)
	return(SafeLinkHandler_GetRSSI());
    #elif(SL_HANDLER_CHQ == Q_PKTSUCC)
	return(SafeLinkHandler_GetPacketSucc());
    #endif
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLink_SendPayload                                        */
/*                                                                              */
/* DESCRIPTION:     Sends Payload through Transceiver                           */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_SendPayload(uint8_t txceiver)
{
    uint8_t temp;
	uint8_t send_buff[SL_TX_PLOAD_WIDTH];
	uint8_t tbuff_pos = slhandler_llsm.tpos;
	uint8_t loc_rssi = RF_Ch_Quality(HOPERF_TRX);
    uint8_t tx_packets = (slhandler_llsm.tx_packetctr + 1) % 16;
    slhandler_llsm.tx_packetctr = tx_packets;
	loc_rssi = (((loc_rssi * 16) / 100) - 1) & 0x0F;
	slhandler_llsm.tpos = (tbuff_pos + 1) % slhandler_llsm.tbuff_size;
	int16_t payload = slhandler_llsm.tBuffer[tbuff_pos];
	/* Save Position in Buffer + packet counter */
	send_buff[0] = (((uint8_t)tbuff_pos << 4) | tx_packets);
	/* Save payload */
	send_buff[1] = ((payload & 0xFF00) >> 8);
	send_buff[2] = payload & 0x00FF;
	/* Save RSSI/PKT SUCC */
    #if(SL_HANDLER_CHQ == Q_SNR)
	temp = (uint8_t)((((uint32_t)loc_rssi * 256) / 100) - 1);
    #elif(SL_HANDLER_CHQ == Q_PKTSUCC)
	temp = (uint8_t)((((uint32_t)slhandler_llsm.channel_quality[slhandler_llsm.channel] * 256) / 100) - 1);
    #endif
	send_buff[3] = temp;
	/* Send Command Byte + PayloadID (pos) + Payload in CURRENT CHANNEL */
	RF_TxDataWireless(send_buff, txceiver);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_ReceivePayload                              */
/*                                                                              */
/* DESCRIPTION:     Receives Payload through Transceiver                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_ReceivePayload(uint8_t txceiver)
{
	/* Get Command Byte + PayloadID (pos) + Payload in CURRENT CHANNEL */
	RF_RxDataWireless(slhandler_llsm.rBuffer, txceiver);
	/* First 6 Byte are the channels */
	/* Byte 5 contains the packet counter */
	slhandler_llsm.rx_packetctr = (slhandler_llsm.rBuffer[slhandler_llsm.rbuff_size - 2] >> 4);
	slhandler_llsm.rBuffer[slhandler_llsm.rbuff_size - 2] &= 0x0F;
	/* Last Byte are 2 Channels (MSB) + CH Number + rxcommand */
	slhandler_llsm.rxcommand = ((slhandler_llsm.rBuffer[slhandler_llsm.rbuff_size - 1] & 0xC0) >> 6);
	slhandler_llsm.rBuffer[slhandler_llsm.rbuff_size - 1] &= 0x3F;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_llsm_init                                   */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_llsm_init(void)
{
	uint8_t aux;
	/* Initialize Data Structure to Defaults */
	slhandler_llsm.timer_rxlost = SL_LOSTRX_TIME;
	slhandler_llsm.state = SL_HANDLER_INIT;
	slhandler_llsm.channel = SL_INITCHANNEL;
	slhandler_llsm.tpos = 0;
	slhandler_llsm.rf_address = SafeLinkHandler_GetRFAddress();
	SafeLinkHandler_SetRFAddress(slhandler_llsm.rf_address);
	slhandler_llsm.txcommand = 0;
	slhandler_llsm.rxcommand = 0;
	slhandler_llsm.timer_txmode = 0;
	slhandler_llsm.ctr_status = 0;
	for(aux = 0; aux < MAX_SL_CHANNELS; aux++)
	{
		slhandler_llsm.channel_quality[aux] = 100;
	}
	/* Initialize other data */
	slhandler_llsm.rx_packetctr = 0;
	slhandler_llsm.rx_packetctr_old = 0;
	slhandler_llsm.rx_totpacket = 0;
	slhandler_llsm.rx_packetloss = 0;
	slhandler_llsm.rx_packetlost = 0;
	slhandler_llsm.tx_packetctr = 0;
	slhandler_llsm.flagtxpause = SL_TXPAUSE_NOACT;
	slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRandomChannel                            */
/*                                                                              */
/* DESCRIPTION:     Gets random channel                                         */
/*                                                                              */
/* PARAMETERS:      Out: Random Channel                                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_GetRandomChannel(void)
{
	uint8_t channel;
	if(slhandler_llsm.channel != 0)
	{
		channel = (slhandler_llsm.channel - 1) % MAX_SL_CHANNELS;
	}
	else
	{
		channel = (MAX_SL_CHANNELS - 1);
	}
	return(channel);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_llsm_main                                   */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_llsm_main(void)
{
	uint8_t loc_ch;
	uint8_t loc_state = slhandler_llsm.state;
	uint8_t txceiver = HOPERF_TRX;
	uint8_t flag_trx;
	slhandler_states[slhandler_llsm.state]++;
	switch(loc_state)
	{
		case SL_HANDLER_INIT:
			/* Entry actions */
			if(slhandler_llsm.init_buff == SL_BUFF_INITIATED)
			{
				slhandler_llsm.channel = SafeLinkHandler_GetRandomChannel();
				#if(SL_HANDLER_FH == FH_ENABLE)
				RF_SetChannel(slhandler_llsm.channel, txceiver);
				#endif
				RF_RxMode(txceiver);
				slhandler_llsm.ctr_status = 0;
				slhandler_llsm.timer_rxlost = SL_LOSTRX_TIME_SCAN;
				slhandler_states_old = slhandler_llsm.state;
				slhandler_llsm.state = SL_SCAN_CHANNELS;
			}
		break;
		case SL_SCAN_CHANNELS:
			/* Entry actions */
			slhandler_llsm.timer_rxlost--;
			flag_trx = RF_RxDone(txceiver);
			if(flag_trx == 1)
			{
				slhandler_llsm.ctr_status = 1;
				slhandler_llsm.timer_rxlost = SL_LOSTRX_TIME;
				slhandler_llsm.rx_packetctr_old = slhandler_llsm.rx_packetctr;
				if(slhandler_llsm.rx_totpacket < UINT16_MAX) slhandler_llsm.rx_totpacket++;
				/* Get and decode message */
				SafeLinkHandler_ReceivePayload(txceiver);
				if(SafeLinkHandler_GetRXFlag(SL_RX_TXMITREQUEST_FLAG) == 1)
				{
					slhandler_llsm.flagtxpause = SL_TXPAUSE_ACT;
					slhandler_llsm.state = SL_SEND_DATA;
				}
				else
				{
					/* Change channel */
					slhandler_llsm.channel = (slhandler_llsm.channel + 1) % MAX_SL_CHANNELS;
					#if(SL_HANDLER_FH == FH_ENABLE)
					RF_SetChannel(slhandler_llsm.channel, txceiver);
					#endif
					RF_RxMode(txceiver);
					slhandler_llsm.state = SL_RECEIVE_DATA;
				}
			}
			else if(slhandler_llsm.timer_rxlost == 0)
			{
				RF_RxMode(txceiver);
				slhandler_llsm.timer_rxlost = SL_LOSTRX_TIME_SCAN;
				slhandler_llsm.channel = SafeLinkHandler_GetRandomChannel();
				#if(SL_HANDLER_FH == FH_ENABLE)
				RF_SetChannel(slhandler_llsm.channel, txceiver);
				#endif
			}
		break;
		case SL_RECEIVE_DATA:
			/* Assign channel to local var. */
			loc_ch = slhandler_llsm.channel;
			/* Receive Data */
			slhandler_llsm.timer_rxlost--;
			/* Check if received */
			flag_trx = RF_RxDone(txceiver);
			if(flag_trx == 1)
			{
				/* Get and decode message */
				SafeLinkHandler_ReceivePayload(txceiver);
				slhandler_llsm.timer_rxlost = SL_LOSTRX_TIME;
				/* Increase Total Packet Number */
				if(slhandler_llsm.rx_totpacket < UINT16_MAX) slhandler_llsm.rx_totpacket++;
                /* Calculate number of lost packets */
                if(slhandler_llsm.rx_packetctr != ((slhandler_llsm.rx_packetctr_old + 1) & 0x0F))
                {
                    if(slhandler_llsm.rx_packetctr > slhandler_llsm.rx_packetctr_old)
                    {
                    	slhandler_llsm.rx_packetlost += (slhandler_llsm.rx_packetctr - slhandler_llsm.rx_packetctr_old);
                    }
                    else
                    {
                    	slhandler_llsm.rx_packetlost += (15 - slhandler_llsm.rx_packetctr_old + slhandler_llsm.rx_packetctr);
                    }
                }
                slhandler_llsm.rx_packetctr_old = slhandler_llsm.rx_packetctr;
				if(SafeLinkHandler_GetRXFlag(SL_RX_TXMITREQUEST_FLAG) == 1)
				{
					/* Switch to Send State */
					slhandler_llsm.flagtxpause = SL_TXPAUSE_ACT;
					slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
					slhandler_states_old = slhandler_llsm.state;
					slhandler_llsm.state = SL_SEND_DATA;
				}
				else
				{
					/* Change channel */
					slhandler_llsm.channel = (slhandler_llsm.channel + 1) % MAX_SL_CHANNELS;
					#if(SL_HANDLER_FH == FH_ENABLE)
					RF_SetChannel(slhandler_llsm.channel, txceiver);
					#endif
					RF_RxMode(txceiver);
				}
				if(slhandler_llsm.rx_totpacket == 50)
				{
                    /* Calculate packet loss */
				    slhandler_llsm.rx_totpacket += slhandler_llsm.rx_packetlost;
				    slhandler_llsm.rx_packetloss = 100 * ((float32_t)slhandler_llsm.rx_packetlost / (float32_t)slhandler_llsm.rx_totpacket);
				    slhandler_llsm.rx_packetlost = 0;
				    slhandler_llsm.rx_totpacket = 0;
				    slhandler_llsm.channel_quality[loc_ch] = 100 - slhandler_llsm.rx_packetloss;
				}
			}
			else
			{
				RF_RxMode(txceiver);
				if(slhandler_llsm.timer_rxlost == 0)
				{
					slhandler_llsm.flagtxpause = SL_TXPAUSE_ACT;
					slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
					slhandler_llsm.rx_packetlost = 100;
					slhandler_llsm.channel_quality[loc_ch] = 0;
					slhandler_llsm.ctr_status = 0;
					slhandler_llsm.timer_rxlost = SL_LOSTRX_TIME_SCAN;
					slhandler_llsm.state = SL_SCAN_CHANNELS;
				}
			}
		break;
		case SL_SEND_DATA:
			/* If coming from RX for the first time then send */
			if(slhandler_llsm.flagtxpause == SL_TXPAUSE_ACT)
			{
				if(slhandler_llsm.ctrtxpause > 0)
				{
					/* Do nothing, wait until pause ends */
					slhandler_llsm.ctrtxpause--;
				}
				else
				{
					slhandler_llsm.flagtxpause = SL_TXPAUSE_NOACT;
					slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
					RF_TxMode(txceiver);
					SafeLinkHandler_ModifyRXFlag(SL_RX_ALL_FLAGS, 0);
					SafeLinkHandler_SendPayload(txceiver);
				}
			}
			/* Sent command triggered, waiting for completion */
			else
			{
				flag_trx = RF_TxDone(txceiver);
				slhandler_llsm.timer_txmode++;
				if(  (slhandler_llsm.timer_txmode >= SL_MAX_TIME_TX)
				   ||(flag_trx == 1) )
				{
					slhandler_llsm.timer_txmode = 0;
					SafeLinkHandler_ModifyTXFlag(SL_TX_ALL_FLAGS, 0);
					/* Change channel */
					slhandler_llsm.channel = (slhandler_llsm.channel + 1) % MAX_SL_CHANNELS;
					#if(SL_HANDLER_FH == FH_ENABLE)
					RF_SetChannel(slhandler_llsm.channel, txceiver);
					#endif
					/* Change to receive mode and set txceiver in Receive Mode */
					RF_RxMode(txceiver);
					slhandler_llsm.state = SL_RECEIVE_DATA;
				}
			}
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_SetupExtBuffers                             */
/*                                                                              */
/* DESCRIPTION:     This function will setup the Application Ext. Buffers       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_SetupExtBuffers(int16_t *tbuffer, uint8_t tlength, uint8_t *rbuffer, uint8_t rlength)
{
	slhandler_llsm.tBuffer = tbuffer;
	slhandler_llsm.rBuffer = rbuffer;
	slhandler_llsm.tbuff_size = tlength;
	slhandler_llsm.rbuff_size = rlength;
	slhandler_llsm.init_buff = SL_BUFF_INITIATED;
}

/*
 * SafeLink_Handler_LLSM.c
 *
 *  Created on: 06.01.2014
 *      Author: ArigitaJ
 */


