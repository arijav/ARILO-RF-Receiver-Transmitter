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
/* NAME:            Comm_UpdateTelemetryInfo                                    */
/*                                                                              */
/* DESCRIPTION:     Updates the Telemetry Info in TX Buffer                     */
/*                                                                              */
/* PARAMETERS:      In: field to be updated                                     */
/*                      data to be updated in the field                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
static void Comm_UpdateTelemetryInfo(uint8_t field, int32_t data)
{
	switch (field)
	{
		case TEL_RPM_SIG:
			/* 16 MSB Buffer TX position [0] */
			communications_sm.tBuffer[0] &= (communications_sm.tBuffer[0] & 0xFFFF);
			communications_sm.tBuffer[0] |= ((data) << 16);
		break;
		case TEL_BATT_SIG:
			/* 16 LSB Buffer TX position [0] */
			communications_sm.tBuffer[0] &= (communications_sm.tBuffer[0] & 0xFFFF0000);
			communications_sm.tBuffer[0] |= data;

			/* Update in SafeLink Buffer as well */
			communications_sm.sl_tBuffer[0] = (int16_t) data;
		break;
		case TEL_DIST_SIG:
			/* 16 MSB Buffer TX position [1] */
			communications_sm.tBuffer[1] &= (communications_sm.tBuffer[1] & 0xFFFF);
			communications_sm.tBuffer[1] |= ((data) << 16);
		break;
		case TEL_CURR_SIG:
			/* 16 LSB Buffer TX position [1] */
			communications_sm.tBuffer[1] &= (communications_sm.tBuffer[1] & 0xFFFF0000);
			communications_sm.tBuffer[1] |= data;
		break;
		case TEL_TEMP_SIG:
			/* 16 MSB Buffer TX position [3] */
			communications_sm.tBuffer[3] &= (communications_sm.tBuffer[3] & 0xFFFF);
			communications_sm.tBuffer[3] |= ((data) << 16);
		break;
		case CTR_COMMANDTX_SIG:
			/* 16 LSB Buffer TX position [3] */
			communications_sm.tBuffer[3] &= (communications_sm.tBuffer[3] & 0xFFFF0000);
			communications_sm.tBuffer[3] |= data;
		break;
		case TEL_IMUACELX_SIG:
			/* 16 MSB Buffer TX position [4] */
			communications_sm.tBuffer[4] &= (communications_sm.tBuffer[4] & 0xFFFF);
			communications_sm.tBuffer[4] |= ((data) << 16);
		break;
		case TEL_IMUACELY_SIG:
			/* 16 LSB Buffer TX position [4] */
			communications_sm.tBuffer[4] &= (communications_sm.tBuffer[4] & 0xFFFF0000);
			communications_sm.tBuffer[4] |= data;
		break;
		case TEL_IMUACELZ_SIG:
			/* 16 MSB Buffer TX position [5] */
			communications_sm.tBuffer[5] &= (communications_sm.tBuffer[5] & 0xFFFF);
			communications_sm.tBuffer[5] |= ((data) << 16);
		break;
		case TEL_IMUGYROX_SIG:
			/* 16 LSB Buffer TX position [5] */
			communications_sm.tBuffer[5] &= (communications_sm.tBuffer[5] & 0xFFFF0000);
			communications_sm.tBuffer[5] |= data;
		break;
		case TEL_IMUGYROY_SIG:
			/* 16 MSB Buffer TX position [6] */
			communications_sm.tBuffer[6] &= (communications_sm.tBuffer[6] & 0xFFFF);
			communications_sm.tBuffer[6] |= ((data) << 16);
		break;
		case TEL_IMUGYROZ_SIG:
			/* 16 LSB Buffer TX position [6] */
			communications_sm.tBuffer[6] &= (communications_sm.tBuffer[6] & 0xFFFF0000);
			communications_sm.tBuffer[6] |= data;
		break;
		default:
		break;
	}
	/* TODO: GPS Coordinates in Main and SafeLink */
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            Comm_SLinkDecode                                            */
/*                                                                              */
/* DESCRIPTION:     Decompress the Channel Lengths sent by the SafeLink         */
/*                                                                              */
/* PARAMETERS:      In:  channel data 8bit/4bit (ON in us from 507us (0)        */
/*                       until 2500)                                            */
/*                       Bit coding (8/4)                                       */
/*                  Out: data in 1/2us                                          */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
static int32_t Comm_SLinkDecode(uint8_t channel_data, uint8_t bit_coding)
{
	int32_t ch_length;
	if(channel_data == 0)
	{
		ch_length = 0;
	}
	else
	{
        /* Decompress in 1/2us (b comes in us)        */
        /* b = ((a * (2500 - 500)) / 255) + 500 */
        if(bit_coding == 8)
        {
            ch_length = (int32_t)((((float32_t)channel_data * (float32_t)7.843) + 500)) * 2;
        }
        /* Decompress in 1/2us (b comes in us)        */
        /* b = ((a * (2500 - 500)) / 15) + 500 */
        else if(bit_coding == 4)
        {
            ch_length = (int32_t)((((float32_t)channel_data * (float32_t)133.333) + 500)) * 2;
        }
        else
        {
        	ch_length = 0;
        }
	}
	return(ch_length);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            Comm_GetCtrInfo                                             */
/*                                                                              */
/* DESCRIPTION:     Extracts the required Control info from RX Buffer           */
/*                                                                              */
/* PARAMETERS:      In:  field to be read                                       */
/*                  Out: data                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
static int32_t Comm_GetCtrInfo(uint8_t field, uint8_t trx)
{
	int32_t data;
	switch (field)
	{
		case CTR_CH0_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [0] */
				data = (communications_sm.rBuffer[0] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = Comm_SLinkDecode(communications_sm.sl_rBuffer[0],8);
			}
		break;
		case CTR_CH1_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [0] */
				data = (communications_sm.rBuffer[0] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = Comm_SLinkDecode(communications_sm.sl_rBuffer[1],8);
			}
		break;
		case CTR_CH2_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [1] */
				data = (communications_sm.rBuffer[1] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = Comm_SLinkDecode(communications_sm.sl_rBuffer[2],8);
			}
		break;
		case CTR_CH3_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [1] */
				data = (communications_sm.rBuffer[1] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = Comm_SLinkDecode(communications_sm.sl_rBuffer[3],8);
			}
		break;
		case CTR_CH4_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [2] */
				data = (communications_sm.rBuffer[2] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = Comm_SLinkDecode(communications_sm.sl_rBuffer[4],8);
			}
		break;
		case CTR_CH5_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [2] */
				data = (communications_sm.rBuffer[2] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = Comm_SLinkDecode(communications_sm.sl_rBuffer[5],4);
			}
		break;
		case CTR_CH6_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [3] */
				data = (communications_sm.rBuffer[3] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				if(communications_sm.sl_rBuffer[6] & (1 << CH_6_BIT))
				{
					data = SERVO_MAXP;
				}
				else
				{
					data = SERVO_MINP;
				}
			}
		break;
		case CTR_CH7_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 LSB Buffer RX position [3] */
				data = (communications_sm.rBuffer[3] & 0x0000FFFF);
			}
			else if(trx == SAFELINK_TRX)
			{
				if(communications_sm.sl_rBuffer[6] & (1 << CH_7_BIT))
				{
					data = SERVO_MAXP;
				}
				else
				{
					data = SERVO_MINP;
				}
			}
		break;
		case CTR_CH8_SIG:
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
		case CTR_CH9_SIG:
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
		case CTR_CH10_SIG:
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
		case CTR_CH11_SIG:
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
		case CTR_NCHANNELS_SIG:
			if(trx == MAIN_TRX)
			{
				/* 16 MSB Buffer RX position [6] */
				data = (communications_sm.rBuffer[6] >> 16);
			}
			else if(trx == SAFELINK_TRX)
			{
				data = ((communications_sm.sl_rBuffer[6] & 0x3C) >> 2);
			}
		break;
		case CTR_COMMANDRX_SIG:
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
		case CTR_ADDRESS_SIG:
			if(trx == MAIN_TRX)
			{
				/* 32 bit RX position [7] */
				data = communications_sm.rBuffer[7];
			}
		break;
		default:
		break;
	}
	return(data);
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
	uint8_t link_status = LINK_DOWN;
	uint8_t transceiver = MAIN_TRX;
	uint8_t aux;
	uint8_t num_ch;

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
					/* Check reliable TRX for Control information */
					#if(TEST_SAFELINK == SL_TEST_DISABLE)
					link_status = RF_GET_CONTROL_LINK_STATUS;
					#endif
					if(link_status == LINK_DOWN)
					{
						link_status = SL_GET_CONTROL_LINK_STATUS;
						communications_sm.main_trx = SAFELINK_TRX;
					}
					else
					{
						communications_sm.main_trx = MAIN_TRX;
					}
					transceiver = communications_sm.main_trx;

					if(link_status == LINK_UP)
					{
						communications_sm.status_comm = LINK_UP;

						/* Update Link Quality (16 LSB) */
						communications_sm.tBuffer[2] = RF_GET_LQUALITY;

						/* Update CPU Load (16 MSB) */
						communications_sm.tBuffer[2] |= (CPU_LOAD << 16);

						loc_signal_p->SM_sender = COMMUNICATIONS_SM;
						loc_signal_p->SM_receiver = CONTROLCH_SM;

						/* Send RF Alive */
						loc_signal_p->event.sig = INFO_RF_SIG;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send RPM Signal */
						loc_signal_p->event.sig = CTR_COMMANDRX_SIG;
						loc_signal_p->param = Comm_GetCtrInfo(CTR_COMMANDRX_SIG, transceiver);
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						/* Send Battery Signal */
						loc_signal_p->event.sig = CTR_NCHANNELS_SIG;
						num_ch = Comm_GetCtrInfo(CTR_NCHANNELS_SIG, transceiver);
						loc_signal_p->param = num_ch;
						/* Send Signal */
						sendq_Sevent((eventtype *) loc_signal_p);

						if(num_ch <= MAX_CHANNELS)
						{
							for(aux = 0; aux < num_ch; aux++)
							{
								loc_signal_p->event.sig = (CTR_CH0_SIG + aux);
								loc_signal_p->param = Comm_GetCtrInfo((CTR_CH0_SIG + aux), transceiver);
								/* Send Signal */
								sendq_Sevent((eventtype *) loc_signal_p);
							}
						}

						/* Save Address, not relevant for other SM */
						communications_sm.address = Comm_GetCtrInfo(CTR_ADDRESS_SIG, transceiver);
					}
				break;
				case TEL_BATT_SIG:
				case TEL_CURR_SIG:
				case TEL_DIST_SIG:
				case TEL_TEMP_SIG:
				case TEL_RPM_SIG:
				case TEL_REVSENSE_SIG:
				case TEL_IMUACELX_SIG:
				case TEL_IMUACELY_SIG:
				case TEL_IMUACELZ_SIG:
				case TEL_IMUGYROX_SIG:
				case TEL_IMUGYROY_SIG:
				case TEL_IMUGYROZ_SIG:
				case CTR_COMMANDTX_SIG:
			    	Comm_UpdateTelemetryInfo((uint8_t) event->sig, (((SignalSM const *) event)->param));
				break;
				case PBUTTOM_BINDREQ_SIG:
					/* Set default Address */
					RF_SET_ADDRESS(DEF_RF_ADDR);
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
					communications_sm.bindcnt = 0;
				break;
				case TSLOT_80ms:
					communications_sm.bindcnt++;
					if(communications_sm.bindcnt >= BIND_RECEIVE_TIME)
					{
						if(RF_GET_CONTROL_LINK_STATUS == LINK_UP)
						{
							Comm_UpdateTelemetryInfo(CTR_COMMANDTX_SIG, CMDRX_BIND_ACK);
							communications_sm.address = Comm_GetCtrInfo(CTR_ADDRESS_SIG, transceiver);
						}
						if(communications_sm.bindcnt >= BIND_ACK_TIME)
						{
							/* Binding completed */
							Comm_UpdateTelemetryInfo(CTR_COMMANDTX_SIG, CMDRX_NO_COMM);
							RF_SET_ADDRESS(communications_sm.address);
							communications_sm.statem.state = COMM_TX_RX;
							communications_sm_main(EXIT_STATE);
						}
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


