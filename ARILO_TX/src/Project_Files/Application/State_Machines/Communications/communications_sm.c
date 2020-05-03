/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             communications_sm.c                                        */
/*                                                                              */
/* DESCRIPTION:      State machine that controls communic. (contr. + telem.)    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "communications_sm.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

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
/* NAME:            Comm_SLinkEncode                                            */
/*                                                                              */
/* DESCRIPTION:     Compress the Channel Lengths sent by the SafeLink           */
/*                                                                              */
/* PARAMETERS:      In:  data in 1/2us                                          */
/*                       Bit coding (8/4)                                       */
/*                  Out: channel data 8bit (ON in us from 507us (0) until 2500) */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Comm_SLinkEncode(uint16_t ch_length, uint8_t bit_coding)
{
	int32_t channel_data;
	if(ch_length == 0)
	{
		channel_data = 0;
	}
	else
	{
		/* Compress (b is in us not in 1/2us   */
        /* a = ((b - 500)* 255 / (2500 - 500)) */
        if(bit_coding == 8)
        {
		    channel_data = (uint8_t)((float32_t)((ch_length / 2) - 500) * (float32_t)0.1275);
        }
		/* Compress (b is in us not in 1/2us   */
        /* a = ((b - 500)* 15 / (2500 - 500)) */
        else
        {
        	channel_data = (uint8_t)((float32_t)((ch_length / 2) - 500) * (float32_t)0.0075);
        }
	}
	return(channel_data);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            Comm_UpdateChannelInfo                                      */
/*                                                                              */
/* DESCRIPTION:     Updates the channel Info in TX Buffer                       */
/*                                                                              */
/* PARAMETERS:      In: field to be updated                                     */
/*                      data to be updated in the field                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Comm_UpdateChannelInfo(uint8_t field, int32_t data)
{
	switch (field)
	{
		case CTR_CH0_SIG:
			/* 16 MSB Buffer TX position [0] */
			communications_sm.tBuffer[0] = (communications_sm.tBuffer[0] & 0xFFFF);
			communications_sm.tBuffer[0] |= ((data) << 16);

			communications_sm.sl_tBuffer[0] = Comm_SLinkEncode(data,8);
		break;
		case CTR_CH1_SIG:
			/* 16 LSB Buffer TX position [0] */
			communications_sm.tBuffer[0] = (communications_sm.tBuffer[0] & 0xFFFF0000);
			communications_sm.tBuffer[0] |= data;

			communications_sm.sl_tBuffer[1] = Comm_SLinkEncode(data,8);
		break;
		case CTR_CH2_SIG:
			/* 16 MSB Buffer TX position [1] */
			communications_sm.tBuffer[1] = (communications_sm.tBuffer[1] & 0xFFFF);
			communications_sm.tBuffer[1] |= ((data) << 16);

			communications_sm.sl_tBuffer[2] = Comm_SLinkEncode(data,8);
		break;
		case CTR_CH3_SIG:
			/* 16 LSB Buffer TX position [1] */
			communications_sm.tBuffer[1] = (communications_sm.tBuffer[1] & 0xFFFF0000);
			communications_sm.tBuffer[1] |= data;

			communications_sm.sl_tBuffer[3] = Comm_SLinkEncode(data,8);
		break;
		case CTR_CH4_SIG:
			/* 16 MSB Buffer TX position [2] */
			communications_sm.tBuffer[2] = (communications_sm.tBuffer[2] & 0xFFFF);
			communications_sm.tBuffer[2] |= ((data) << 16);

			communications_sm.sl_tBuffer[4] = Comm_SLinkEncode(data,8);
		break;
		case CTR_CH5_SIG:
			/* 16 LSB Buffer TX position [2] */
			communications_sm.tBuffer[2] = (communications_sm.tBuffer[2] & 0xFFFF0000);
			communications_sm.tBuffer[2] |= data;

			communications_sm.sl_tBuffer[5] = Comm_SLinkEncode(data,4);
		break;
		case CTR_CH6_SIG:
			/* 16 MSB Buffer TX position [3] */
			communications_sm.tBuffer[3] = (communications_sm.tBuffer[3] & 0xFFFF);
			communications_sm.tBuffer[3] |= ((data) << 16);

			if(data >= SERVO_MAXP)
			{
				communications_sm.sl_tBuffer[6] |= (1 << CH_6_BIT);
			}
			else
			{
				communications_sm.sl_tBuffer[6] &= ~(1 << CH_6_BIT);
			}
		break;
		case CTR_CH7_SIG:
			/* 16 LSB Buffer TX position [3] */
			communications_sm.tBuffer[3] = (communications_sm.tBuffer[3] & 0xFFFF0000);
			communications_sm.tBuffer[3] |= data;

			if(data >= SERVO_MAXP)
			{
				communications_sm.sl_tBuffer[6] |= (1 << CH_7_BIT);
			}
			else
			{
				communications_sm.sl_tBuffer[6] &= ~(1 << CH_7_BIT);
			}
		break;
		case CTR_CH8_SIG:
			/* 16 MSB Buffer TX position [4] */
			communications_sm.tBuffer[4] = (communications_sm.tBuffer[4] & 0xFFFF);
			communications_sm.tBuffer[4] |= ((data) << 16);
		break;
		case CTR_CH9_SIG:
			/* 16 LSB Buffer TX position [4] */
			communications_sm.tBuffer[4] = (communications_sm.tBuffer[4] & 0xFFFF0000);
			communications_sm.tBuffer[4] |= data;
		break;
		case CTR_CH10_SIG:
			/* 16 MSB Buffer TX position [5] */
			communications_sm.tBuffer[5] = (communications_sm.tBuffer[5] & 0xFFFF);
			communications_sm.tBuffer[5] |= ((data) << 16);
		break;
		case CTR_CH11_SIG:
			/* 16 LSB Buffer TX position [5] */
			communications_sm.tBuffer[5] = (communications_sm.tBuffer[5] & 0xFFFF0000);
			communications_sm.tBuffer[5] |= data;
		break;
		case CTR_NCHANNELS_SIG:
			/* 16 MSB Buffer TX position [6] */
			communications_sm.tBuffer[6] = (communications_sm.tBuffer[6] & 0xFFFF);
			communications_sm.tBuffer[6] |= ((data) << 16);

			communications_sm.sl_tBuffer[6] &= 0xC3;
			communications_sm.sl_tBuffer[6] |= ((uint8_t)data << 2);
		break;
		case CTR_COMMANDTX_SIG:
			/* 16 LSB Buffer TX position [6] */
			communications_sm.tBuffer[6] = (communications_sm.tBuffer[6] & 0xFFFF0000);
			communications_sm.tBuffer[6] |= data;
		break;
		case CTR_ADDRESS_SIG:
			/* 32 bit RX position [7] */
			communications_sm.tBuffer[7] = data;
		break;
		default:
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            Comm_GetTelInfo                                             */
/*                                                                              */
/* DESCRIPTION:     Extracts the required telemetry info from RX Buffer         */
/*                                                                              */
/* PARAMETERS:      In:  field to be read                                       */
/*                  Out: data                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
int32_t Comm_GetTelInfo(uint8_t field, uint8_t trx)
{
	int32_t data;
	switch (field)
	{
		case TEL_RPM_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [0] */
				data = (communications_sm.rBuffer[0] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_BATT_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [0] */
				data = (communications_sm.rBuffer[0] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = (communications_sm.sl_rBuffer[0]);
			}
		break;
		case TEL_DIST_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [1] */
				data = (communications_sm.rBuffer[1] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_CURR_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [1] */
				data = (communications_sm.rBuffer[1] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_RSSITX_SIG:
			if(trx == MAIN_TRX)
			{
				/* 32 LSB Buffer RX position [2] */
				data = (communications_sm.rBuffer[2] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = (int32_t)SL_GET_LQUALITY_TX;
			}
		break;
		case TEL_RSSIRX_SIG:
			if(trx == MAIN_TRX)
			{
				data = (int32_t)RF_GET_LQUALITY;
			}
			else if(trx == SAFELINK_TRX)
			{
				data = (int32_t)SL_GET_LQUALITY_RX;
			}
		break;
		case TEL_CPULOAD_SIG:
			if(trx == MAIN_TRX)
			{
				/* 32 LSB Buffer RX position [2] */
				data = (communications_sm.rBuffer[2] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_TEMPRX_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [3] */
				data = (communications_sm.rBuffer[3] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case CTR_COMMANDRX_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [3] */
				data = (communications_sm.rBuffer[3] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_IMUACELX_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [4] */
				data = (communications_sm.rBuffer[4] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_IMUACELY_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [4] */
				data = (communications_sm.rBuffer[4] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_IMUACELZ_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [5] */
				data = (communications_sm.rBuffer[5] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_IMUGYROX_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [5] */
				data = (communications_sm.rBuffer[5] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_IMUGYROY_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [6] */
				data = (communications_sm.rBuffer[6] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		case TEL_IMUGYROZ_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [6] */
				data = (communications_sm.rBuffer[6] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = 0;
			}
		break;
		default:
		break;
	}

	return(data);
	/* TODO: GPS Coordinates in Main and SafeLink */
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_communications_sm                                      */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_communications_sm(void)
{
	uint8_t i;

	/* Set initial State */
	communications_sm.statem.state = INIT_COMM;
	communications_sm.bindcnt = 0;
	communications_sm.status_comm = LINK_DOWN;
	communications_sm.control = 0;
	for(i = 0; i < TX_BUFF_SIZE; i++)
	{
		communications_sm.tBuffer[i] = 0;
	};
	for(i = 0; i < RX_BUFF_SIZE; i++)
	{
		communications_sm.rBuffer[i] = 0;
	};
	communications_sm.main_trx = MAIN_TRX;

	/* Setup Buffers */
	RF_SETUP_EXT_BUFFERS(communications_sm.tBuffer, TX_BUFF_SIZE, communications_sm.rBuffer, RX_BUFF_SIZE);

	/* Setup SafeLink Buffers */
	SL_SETUP_EXT_BUFFERS(communications_sm.sl_tBuffer, SL_TX_BUFF_SIZE, communications_sm.sl_rBuffer, SL_RX_BUFF_SIZE);
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            communications_sm_main                                      */
/*                                                                              */
/* DESCRIPTION:     Main function from the Communications State Machine         */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void communications_sm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;
	int32_t aux;
	uint8_t link_status = LINK_DOWN;
	uint8_t transceiver = MAIN_TRX;

	switch (communications_sm.statem.state)
	{
		case INIT_COMM:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					/* Get initial address */
					communications_sm.address = RF_GET_ADDRESS;
					/* Update Address field in TX Buffer */
					Comm_UpdateChannelInfo(CTR_ADDRESS_SIG, communications_sm.address);
					/* Change state to TX_RX */
					communications_sm.statem.state = COMM_TX_RX;
					communications_sm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case COMM_TX_RX:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_10ms:

					/* Prepare Signals for Telemetry SM */
					loc_signal_p->SM_sender = COMMUNICATIONS_SM;
					loc_signal_p->SM_receiver = TELEMETRY_SM;

					/* Check reliable TRX for Telemetry information */
					#if(TEST_SAFELINK == SL_TEST_DISABLE)
					link_status = RF_GET_TELEM_LINK_STATUS;
					#endif
					if(link_status == LINK_DOWN)
					{
						link_status = SL_GET_TELEM_LINK_STATUS;
						transceiver = SAFELINK_TRX;
					}
					else
					{
						transceiver = MAIN_TRX;
					}

					if(link_status == LINK_UP)
					{
						/* Send Transceiver Signal */
						loc_signal_p->event.sig = TRXACTIVE_SIG;
						loc_signal_p->param = transceiver;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send InfoRF Signal */
						loc_signal_p->event.sig = INFO_RF_SIG;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send RPM Signal */
						loc_signal_p->event.sig = TEL_RPM_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_RPM_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Battery Signal */
						loc_signal_p->event.sig = TEL_BATT_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_BATT_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Distance Signal */
						loc_signal_p->event.sig = TEL_DIST_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_DIST_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Current Signal */
						loc_signal_p->event.sig = TEL_CURR_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_CURR_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Current Signal */
						loc_signal_p->event.sig = TEL_RSSIRX_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_RSSIRX_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Current Signal */
						loc_signal_p->event.sig = TEL_RSSITX_SIG;
						/* Send RSSI on receivers point of view just if there is Link (RSSIRX != 0) */
						if(loc_signal_p->param != 0)
						{
							loc_signal_p->param = Comm_GetTelInfo(TEL_RSSITX_SIG, transceiver);
						}
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send RX CPU Load Signal */
						loc_signal_p->event.sig = TEL_CPULOAD_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_CPULOAD_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Temperature Signal */
						loc_signal_p->event.sig = TEL_TEMPRX_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_TEMPRX_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Command Signal */
						loc_signal_p->event.sig = CTR_COMMANDRX_SIG;
						loc_signal_p->param = Comm_GetTelInfo(CTR_COMMANDRX_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Accel Reading Signals */
						loc_signal_p->event.sig = TEL_IMUACELX_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_IMUACELX_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						loc_signal_p->event.sig = TEL_IMUACELY_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_IMUACELY_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						loc_signal_p->event.sig = TEL_IMUACELZ_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_IMUACELZ_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Gyro Reading Signals */
						loc_signal_p->event.sig = TEL_IMUGYROX_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_IMUGYROX_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						loc_signal_p->event.sig = TEL_IMUGYROY_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_IMUGYROY_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						loc_signal_p->event.sig = TEL_IMUGYROZ_SIG;
						loc_signal_p->param = Comm_GetTelInfo(TEL_IMUGYROZ_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);
					}
					else
					{
						transceiver = NO_TRX;
						/* Send Transceiver Signal */
						loc_signal_p->event.sig = TRXACTIVE_SIG;
						loc_signal_p->param = transceiver;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);
						/* Send InfoNORF Signal */
						loc_signal_p->event.sig = INFO_NO_RF_SIG;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);
					}
					communications_sm.main_trx = transceiver;
				break;
				case CTR_NCHANNELS_SIG:
				case CTR_CH0_SIG:
				case CTR_CH1_SIG:
				case CTR_CH2_SIG:
				case CTR_CH3_SIG:
				case CTR_CH4_SIG:
				case CTR_CH5_SIG:
				case CTR_CH6_SIG:
				case CTR_CH7_SIG:
				case CTR_CH8_SIG:
				case CTR_CH9_SIG:
				case CTR_CH10_SIG:
				case CTR_CH11_SIG:
				case CTR_COMMANDTX_SIG:
					Comm_UpdateChannelInfo((uint8_t) event->sig, (((SignalSM const *) event)->param));
				break;
				case PBUTTOM_BINDREQ_SIG:
					/* TODO: Beep */
					communications_sm.statem.state = COMM_BINDING;
					communications_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case COMM_BINDING:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* Set default Address */
					RF_SET_ADDRESS(DEF_RF_ADDR);

					/* Buzzer Signal, Initiate Binding */
					loc_signal_p->param = 4;
					loc_signal_p->event.sig = BUZZREQ_SIG;
					loc_signal_p->SM_receiver = CONTROLBUZZER_SM;
					/* Send Signal */
					sendq_Sevent((eventtype *) loc_signal_p);
				break;
				case TSLOT_80ms:
					/* Check if RX acknowledges the Binding */
					if(RF_GET_TELEM_LINK_STATUS == LINK_UP)
					{
						aux = CMD_MASK_COMM & Comm_GetTelInfo(CTR_COMMANDRX_SIG, transceiver);
					}
					else
					{
						aux = 0;
					}
					/* RX Acknowledge, or timer is 0 */
					if(  (communications_sm.bindcnt >= BIND_REQUEST_TIME)
					   ||(aux == CMDRX_BIND_ACK) )
					{
						if(aux == CMDRX_BIND_ACK)
						{
							loc_signal_p->param = 2;
						}
						else
						{
							loc_signal_p->param = 3;
						}
						loc_signal_p->event.sig = BUZZREQ_SIG;
						loc_signal_p->SM_receiver = CONTROLBUZZER_SM;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* TODO: Beep if timeout, double beep if bind success */
						communications_sm.bindcnt = 0;
						RF_SET_ADDRESS(communications_sm.address);
						communications_sm.statem.state = COMM_TX_RX;
						communications_sm_main(EXIT_STATE);
					}
					else
					{
						communications_sm.bindcnt++;
					}
				break;
				default:
				break;
			};
		break;
		default:
		break;
	}
};

/*
 * Communications_sm.c
 *
 *  Created on: 26.09.2012
 *      Author: X-ArigitaJ
 */


