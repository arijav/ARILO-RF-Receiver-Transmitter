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
#include "../Low_Level_State_Machines/Error_Handler/Error_Handler_LLSM.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
uint32_t slhandler_states[MAX_STATE_SL_HANDLER_LLSM];

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
	uint16_t address;

	address = (uint16_t)DEVICE_ID;
	/* Save address in Transceivers */
	return(address);
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
	return(slhandler_llsm.tel_status);
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
/* NAME:            SafeLinkHandler_LinkQualityTX                               */
/*                                                                              */
/* DESCRIPTION:     Gets the Link Quality                                       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_LinkQualityTX(void)
{
	return(slhandler_llsm.channel_quality[slhandler_llsm.channel]);
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
	uint8_t tx_packets = ((slhandler_llsm.tx_packetctr + 1) % 16);
	slhandler_llsm.tx_packetctr = tx_packets;
	/* Save Command Flags */
	slhandler_llsm.tBuffer[SL_TX_PLOAD_WIDTH - 1] &= 0x3F;
	slhandler_llsm.tBuffer[SL_TX_PLOAD_WIDTH - 1] |= (slhandler_llsm.txcommand << 6);
	/* Save Packet Counter */
	slhandler_llsm.tBuffer[SL_TX_PLOAD_WIDTH - 2] &= 0x0F;
	slhandler_llsm.tBuffer[SL_TX_PLOAD_WIDTH - 2] |= (tx_packets << 4);
	/* Send Command Byte + Payload */
	RF_TxDataWireless(slhandler_llsm.tBuffer, txceiver);
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
	uint8_t rx_buff[SL_RX_PLOAD_WIDTH];
	int16_t payload;

	uint8_t rbuff_pos = 0;
	/* Get Command Byte + PayloadID (pos) + Payload in CURRENT CHANNEL */
	RF_RxDataWireless(rx_buff, txceiver);
	/* First bit is the position in buffer (4bit) + RSSI/PCKT SUCC (4bit) */
	rbuff_pos = (rx_buff[0] & 0xF0) >> 4;
	if(rbuff_pos < slhandler_llsm.rbuff_size)
	{
		/* Packet Ctr. 4 bit */
		slhandler_llsm.rx_packetctr = (rx_buff[0] & 0x0F);
		/* Payload 16 bit */
		payload = rx_buff[1];
		payload = (payload << 8) | (rx_buff[2]);
		slhandler_llsm.rBuffer[rbuff_pos] = payload;
        /* RSSI/PKT SUCC */
        #if(SL_HANDLER_CHQ == Q_SNR)
        slhandler_llsm.rssi = (uint8_t)(((uint32_t)rx_buff[3] + 1) * 100) / 256;
        #elif(SL_HANDLER_CHQ == Q_PKTSUCC)
        slhandler_llsm.channel_quality[slhandler_llsm.channel] = (uint8_t)((((uint32_t)rx_buff[3] + 1) * 100) / 256);
        #endif
	}
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
	slhandler_llsm.timer_rxswitch = 0;
	slhandler_llsm.state = SL_HANDLER_INIT;
	slhandler_llsm.channel = SL_INITCHANNEL;
	slhandler_llsm.rf_address = SafeLinkHandler_GetRFAddress();
	SafeLinkHandler_SetRFAddress(slhandler_llsm.rf_address);
	slhandler_llsm.txcommand = 0;
	slhandler_llsm.rxcommand = 0;
	slhandler_llsm.timer_txretries = 0;
	slhandler_llsm.timer_rxmode = 0;
	slhandler_llsm.tel_status = 0;
	for(aux = 0; aux < MAX_SL_CHANNELS; aux++)
	{
		slhandler_llsm.channel_quality[aux] = 100;
	}
	slhandler_llsm.channel_rxpacketctr = 0;
	slhandler_llsm.channel_rxtotalpacket = 0;
	/* Initialize other data */
	slhandler_llsm.rx_packetctr = 0;
	slhandler_llsm.rx_packetctr_old = 0;
	slhandler_llsm.rx_totpacket = 0;
	slhandler_llsm.rx_packetloss = 0;
	slhandler_llsm.rx_timeouts = 0;
	slhandler_llsm.tx_packetctr = 0;
	slhandler_llsm.flagtxpause = SL_TXPAUSE_NOACT;
	slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetNextChannel                              */
/*                                                                              */
/* DESCRIPTION:     Gets random channel                                         */
/*                                                                              */
/* PARAMETERS:      Out: Random Channel                                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SafeLinkHandler_GetNextChannel(void)
{
	uint8_t channel;
	if(slhandler_llsm.channel == 0)
	{
		channel = slhandler_llsm.channel = 2;
	}
	else
	{
		channel = slhandler_llsm.channel - 1;
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
	uint8_t loc_state = slhandler_llsm.state;
	uint8_t txceiver = HOPERF_TRX;
	uint8_t flag_trx = 0;
	slhandler_states[loc_state]++;
	switch(loc_state)
	{
		case SL_HANDLER_INIT:
			/* Entry actions */
			if(slhandler_llsm.init_buff == SL_BUFF_INITIATED)
			{
				slhandler_llsm.flagtxpause = SL_TXPAUSE_ACT;
				slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
				slhandler_llsm.state = SL_SEND_DATA;
			}
		break;
		case SL_SEND_DATA:
			flag_trx = RF_TxDone(txceiver);
			if(flag_trx == 1)
			{
				/* We have to switch to RX Mode */
				if(slhandler_llsm.txcommand == SL_TX_TXMITREQUEST_FLAG)
		    	{
					/* Set pause back */
					slhandler_llsm.flagtxpause = SL_TXPAUSE_NOACT;
					slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
					slhandler_llsm.timer_txretries = 0;
					SafeLinkHandler_ModifyTXFlag(SL_TX_ALL_FLAGS, 0);
					slhandler_llsm.timer_rxswitch = 0;
					RF_RxMode(txceiver);
					slhandler_llsm.state = SL_RECEIVE_DATA;
		    	}
				else
				{
					/* Init Pause period between TX TX or RX TX */
					slhandler_llsm.flagtxpause = SL_TXPAUSE_ACT;
					slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
				}
			}
			else
			{
				/* Check if we are on TX pause period */
				if(slhandler_llsm.flagtxpause == SL_TXPAUSE_ACT)
				{
					slhandler_llsm.ctrtxpause--;
					if(slhandler_llsm.ctrtxpause == 0)
					{
				    	/* Set pause back */
						slhandler_llsm.flagtxpause = SL_TXPAUSE_NOACT;
				    	slhandler_llsm.ctrtxpause = SL_TIME_TRANS;
						slhandler_llsm.timer_txretries = 0;

						/* Increment Counters */
						slhandler_llsm.timer_rxswitch++;

						/* Send Data */
						SafeLinkHandler_ModifyTXFlag(SL_TX_ALL_FLAGS, 0);

						/* Switch Channel */
						slhandler_llsm.channel = (slhandler_llsm.channel + 1) % MAX_SL_CHANNELS;
						#if(SL_HANDLER_FH == FH_ENABLE)
						RF_SetChannel(slhandler_llsm.channel, txceiver);
						#endif

						/* Otherwise evaluate if transition to RX Mode is required */
						if(slhandler_llsm.timer_rxswitch >= SL_RXSWITCH_PERIOD)
						{
								SafeLinkHandler_ModifyTXFlag(SL_TX_TXMITREQUEST_FLAG, 1);
						}
						/* Send Payload */
						SafeLinkHandler_SendPayload(txceiver);
					}
				}
				else
				{
					slhandler_llsm.timer_txretries++;
					if(slhandler_llsm.timer_txretries == SL_MAX_TX_RETRIES)
					{
						/* Report error */
						ERRORHANDLER_LLSM_IFACE(SFLINK_ERROR_RESET ,ERROR_ON);
						/* Reset Transceiver */
						RF_Init(txceiver);
						slhandler_llsm.timer_txretries = 0;
						slhandler_llsm.state = SL_HANDLER_INIT;
					}
				}
			}
		break;
		case SL_RECEIVE_DATA:
			/* Increase RX Mode Timer */
			slhandler_llsm.timer_rxmode++;
			/* Check if we have received something */
			flag_trx = RF_RxDone(txceiver);
			if(flag_trx == 1)
			{
				/* Get and decode message */
				SafeLinkHandler_ReceivePayload(txceiver);
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
				
				/* Enter RX Mode again */
				RF_RxMode(txceiver);
			}
			else
			{
				RF_RxMode(txceiver);
			}
			/* If RX Time has been consumed or a packet has been received */
			if(  (flag_trx == 1)
					 ||(slhandler_llsm.timer_rxmode >= SL_MAX_TIME_RX)
				   )
			{
				/* Report Telemetry Status */
				if(slhandler_llsm.rx_totpacket == 0)
				{
					slhandler_llsm.tel_status = 0;
				}
				else
				{
					slhandler_llsm.tel_status = 1;
				}

				/* Calculate packet loss */
				slhandler_llsm.rx_totpacket += slhandler_llsm.rx_packetlost;
				slhandler_llsm.rx_packetloss = 100 * ((float32_t)slhandler_llsm.rx_packetlost / (float32_t)slhandler_llsm.rx_totpacket);

				if(slhandler_llsm.timer_rxmode >= SL_MAX_TIME_RX)
				{
					slhandler_llsm.rx_timeouts++;
					/* In case of timeout over a defined boundary reset values and set Packet Loss to 100 */
					if(slhandler_llsm.rx_timeouts > SL_MAX_TIMEOUTS_NO_RX)
					{
						slhandler_llsm.rx_packetloss = 100;

						/* Reset values */
						slhandler_llsm.rx_totpacket = 0;
						slhandler_llsm.rx_packetlost = 0;
						slhandler_llsm.rx_timeouts = 0;
					}
				}

				/* Reset Flags */
				SafeLinkHandler_ModifyRXFlag(SL_RX_ALL_FLAGS, 0);

				/* Reset RX Timer */
				slhandler_llsm.timer_rxmode = 0;

				/* Init TX pause period */
				slhandler_llsm.flagtxpause = SL_TXPAUSE_ACT;
		    	slhandler_llsm.ctrtxpause = SL_TIME_TRANS;

				/* Change to SL_SEND_DATA State */
				slhandler_llsm.state = SL_SEND_DATA;
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
void SafeLinkHandler_SetupExtBuffers(uint8_t *tbuffer, uint8_t tlength, int16_t *rbuffer, uint8_t rlength)
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


