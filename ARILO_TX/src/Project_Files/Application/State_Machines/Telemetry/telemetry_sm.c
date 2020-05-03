/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             telemetry_sm.c                                             */
/*                                                                              */
/* DESCRIPTION:      State machine responsible for the telemetry                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "telemetry_sm.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
/* Array with Data IDs for each Extended Telemetry Value */
uint8_t tel_dataid_ext1[MAX_OUTPUTS_EXTTELFR1] = {0x24, 0x25, 0x26, 0x10, 0x21, 0x02,
											      0x05, 0x06, 0x3A, 0x3B, 0x28, 0x03};

uint8_t tel_dataid_ext2[MAX_OUTPUTS_EXTTELFR2] = {0x14, 0x1C, 0x13, 0x1B, 0x23,
											      0x12, 0x1A, 0x22, 0x11, 0x19,
											      0x01, 0x09, 0x04};

uint8_t tel_dataid_ext3[MAX_OUTPUTS_EXTTELFR3] = {0x15, 0x16, 0x17, 0x18};

/* 0 Little Endian, 1 Big Endian */
uint8_t tel_endianness_ext1[MAX_OUTPUTS_EXTTELFR1] = {0, 0, 0, 0, 0, 0,
													  0, 1, 1, 1, 1, 0};

uint8_t tel_endianness_ext2[MAX_OUTPUTS_EXTTELFR2] = {0, 0, 1, 1, 1,
													  1, 1, 1, 0, 0,
													  0, 0, 0};

uint8_t tel_endianness_ext3[MAX_OUTPUTS_EXTTELFR3] = {0, 0, 0, 0};

/* Array with Frame Basis Index */
uint8_t frame_basisidx[TEL_NUM_EXTFRAMES] = {TEL_EXT1_INDEX,
											 TEL_EXT2_INDEX,
											 TEL_EXT3_INDEX};

/* Array with Frame lengths */
uint8_t frame_length[TEL_NUM_EXTFRAMES] = {MAX_OUTPUTS_EXTTELFR1,
		                                   MAX_OUTPUTS_EXTTELFR2,
		                                   MAX_OUTPUTS_EXTTELFR3};

/* Array with pointers to DataIDs per Frame */
uint8_t *frame_dataID[TEL_NUM_EXTFRAMES] = {tel_dataid_ext1,
		                                    tel_dataid_ext2,
		                                    tel_dataid_ext3};

/* Array with pointers to Little/Big Endian per Frame */
uint8_t *frame_Endianness[TEL_NUM_EXTFRAMES] = {tel_endianness_ext1,
		                                        tel_endianness_ext2,
		                                        tel_endianness_ext3};

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            init_telemetry_sm                                           */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_telemetry_sm(void)
{
	uint8_t loc_aux;
	telemetry_sm.tel_ctr = 0;
	telemetry_sm.reset_ctr = TEL_RESET_PERIOD;
	telemetry_sm.frame_length = 0;
	for(loc_aux = 0; loc_aux < TEL_L_BUFFER; loc_aux++)
	{
		telemetry_sm.tel_buff[loc_aux] = 0x00;
	}
	for(loc_aux = 0; loc_aux < TEL_NUM_EXTFRAMES; loc_aux++)
	{
		telemetry_sm.frame_telusr[loc_aux] = 0;
	}
	telemetry_sm.frame_ctr = 0;
	telemetry_sm.index_usrd_pend = 0;
	telemetry_sm.statem.state = INIT_TELEMETRY;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetry_update                                            */
/*                                                                              */
/* DESCRIPTION:     Displays the required information of the main screen        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetry_update(uint8_t source, int32_t info)
{
	float32_t loc_aux = info;
	switch(source)
	{
		case TEL_BATT_SIG:
			loc_aux = (loc_aux * 255) / ((float32_t)13200);
			if(loc_aux > 255) loc_aux = 255;
			telemetry_sm.tel_buff[TEL_BASIC_INDEX + TEL_VOLTAGE1] = (int32_t)loc_aux;
		break;
		case TEL_RPM_SIG:
			telemetry_sm.tel_buff[TEL_EXT1_INDEX + TEL_RPM] = (int32_t)loc_aux;
		break;
		case TEL_DIST_SIG:
			/* Come in CM, should be shown in M */
			loc_aux = loc_aux / 100;
			telemetry_sm.tel_buff[TEL_EXT1_INDEX + TEL_ALTITUDE1] = (int32_t)loc_aux;
			telemetry_sm.tel_buff[TEL_EXT1_INDEX + TEL_ALTITUDE2] = (loc_aux - (int32_t)loc_aux) * 100;
		break;
		case TEL_CURR_SIG:
			/* Output on Basic Frame through A2 Voltage, range 0 till 100A*/
			loc_aux = (loc_aux * 255) / 100000;
			if(loc_aux > 255) loc_aux = 255;
			telemetry_sm.tel_buff[TEL_BASIC_INDEX + TEL_VOLTAGE2] = (int32_t)loc_aux;
			/* TODO: Output on Extended FA-100 Sensor Frame as well */
		break;
		case TEL_RSSITX_SIG:
			telemetry_sm.tel_buff[TEL_BASIC_INDEX + TEL_LINK_QUALITY_UP] = (2 * (int32_t)info);
		break;
		case TEL_RSSIRX_SIG:
			telemetry_sm.tel_buff[TEL_BASIC_INDEX + TEL_LINK_QUALITY_DOWN] = (int32_t)info;
		break;
		case TEL_TEMPTX_SIG:
			telemetry_sm.tel_buff[TEL_EXT1_INDEX + TEL_TEMPERATURE2] = (int32_t)info;
		break;
		case TEL_TEMPRX_SIG:
			telemetry_sm.tel_buff[TEL_EXT1_INDEX + TEL_TEMPERATURE1] = (int32_t)info;
		break;
		default:
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetry_sm_basicframe                                     */
/*                                                                              */
/* DESCRIPTION:     Reconstructs the Telemetry Basic Frame                      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetry_basicframe(void)
{
	/* Start index after header */
	uint8_t loc_index = 2;
	/* Aux variables */
	uint8_t loc_aux1, loc_aux2;
	/* Clear current Buffer */
	for(loc_aux1 = 0; loc_aux1 < 15; loc_aux1++)
	{
		telemetry_sm.tel_txbuff[loc_aux1] = 0;
	}
	/* Insert primary output headers */
	telemetry_sm.tel_txbuff[0] = 0x7E;
	telemetry_sm.tel_txbuff[1] = 0xFE;
	/* Insert Basic Telemetry Data with byte stuffing */
	for(loc_aux1 = 0; loc_aux1 < MAX_OUTPUTS_BASICTEL; loc_aux1++)
	{
		loc_aux2 = (uint8_t)telemetry_sm.tel_buff[TEL_BASIC_INDEX + loc_aux1];
		if(  (loc_aux2 != 0x7E)
		   &&(loc_aux2 != 0x7D)	)
		{
			telemetry_sm.tel_txbuff[loc_index] = loc_aux2;
			loc_index++;
		}
		else
		{
			telemetry_sm.tel_txbuff[loc_index] = 0x7D;
			telemetry_sm.tel_txbuff[loc_index + 1] = loc_aux2 ^ 0x20;
			loc_index += 2;
		}
	}
	/* Insert 4 empty Bytes */
	for(loc_aux1 = 0; loc_aux1 < 4; loc_aux1++)
	{
		telemetry_sm.tel_txbuff[loc_index + loc_aux1] = 0;
	}
	loc_index += 4;
	/* Insert Tail Byte */
	telemetry_sm.tel_txbuff[loc_index] = 0x7E;
	loc_index++;
	telemetry_sm.frame_length = loc_index;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetry_userframe                                         */
/*                                                                              */
/* DESCRIPTION:     Reconstructs the Telemetry User Frames (max. 6 byte payload)*/
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetry_userframe(void)
{
	uint8_t loc_frctr, loc_aux, loc_index = 0;
	uint8_t loc_firstbyte, loc_secondbyte, loc_basisidx, loc_indexinframe, loc_framelength;
	uint8_t *loc_IDs, *loc_Endianness;
	int32_t loc_telval;
	/* Clear current Buffer */
	for(loc_aux = 0; loc_aux < 15; loc_aux++)
	{
		telemetry_sm.tel_txbuff[loc_aux] = 0;
	}
	/* Check if Number of Pending Messages == 0 */
	if(telemetry_sm.index_usrd_pend == 0)
	{
		/* Clear Current Usr. Data Array */
		for(loc_index = 0; loc_index < 9; loc_index ++)
		{
			telemetry_sm.usr_data_buff[loc_index] = 0;
		}
		/* Get next frame */
		loc_frctr = telemetry_sm.frame_ctr;
		loc_indexinframe = telemetry_sm.frame_telusr[loc_frctr];
		/* Get next value */
		loc_framelength = frame_length[loc_frctr];
		/* Increment future frame to be sent if current one has been sent */
		if(loc_indexinframe == (loc_framelength - 1))
		{
			telemetry_sm.frame_ctr = ((loc_frctr + 1)%TEL_NUM_EXTFRAMES);
		}
		/* Increment future User Data in Frame */
		telemetry_sm.frame_telusr[loc_frctr] = (loc_indexinframe + 1)%loc_framelength;
		/* Get DataID of the User Data to be sent now */
		loc_IDs = frame_dataID[loc_frctr];
		/* Get Basis Index in Telemetry Buffer of the User Data to be sent now */
		loc_basisidx = frame_basisidx[loc_frctr];
		/* Get Endianness in Telemetry Buffer of the User Data to be sent now */
		loc_Endianness = frame_Endianness[loc_frctr];
		/* Byte Stuffing 5E/5D */
		/* Byte Stuffing 7E/7D */
		/* Include Header + DataID */
		loc_index = 0;
		telemetry_sm.usr_data_buff[loc_index] = 0x5E;
		telemetry_sm.usr_data_buff[loc_index + 1] = loc_IDs[loc_indexinframe];
		loc_index = 2;
		/* Include Data Payload (Endianness check required) */
		loc_telval = telemetry_sm.tel_buff[loc_basisidx + loc_indexinframe];
		if(loc_Endianness[loc_indexinframe] == 0)
		{
			/* Little Endian */
			loc_firstbyte = (uint8_t)(((uint16_t)loc_telval) & 0xFF);
			loc_secondbyte = (uint8_t)((((uint16_t)loc_telval) & 0xFF00)>>8);
		}
		else
		{
			/* Big Endian */
			loc_firstbyte = (uint8_t)((((uint16_t)loc_telval) & 0xFF00)>>8);
			loc_secondbyte = (uint8_t)(((uint16_t)loc_telval) & 0xFF);
		}
		/* Perform Byte Stuffing if needed */
		/* First Byte */
		if(  (loc_firstbyte != 0x5E)
		   &&(loc_firstbyte != 0x5D)
		   &&(loc_firstbyte != 0x7E)
		   &&(loc_firstbyte != 0x7D))
		{
			telemetry_sm.usr_data_buff[loc_index] = loc_firstbyte;
			loc_index++;
		}
		else if(  (loc_firstbyte == 0x7E)
		        ||(loc_firstbyte == 0x7D) )
		{
			telemetry_sm.usr_data_buff[loc_index] = 0x7D;
			telemetry_sm.usr_data_buff[loc_index + 1] = 0x5D;
			telemetry_sm.usr_data_buff[loc_index + 2] = (loc_firstbyte ^ 0x20) ^ 0x60;
			loc_index += 3;
		}
		else
		{
			telemetry_sm.usr_data_buff[loc_index] = 0x5D;
			telemetry_sm.usr_data_buff[loc_index + 1] = loc_firstbyte ^ 0x60;
			loc_index += 2;
		}
		/* Second Byte */
		if(  (loc_secondbyte != 0x5E)
		   &&(loc_secondbyte != 0x5D)
		   &&(loc_secondbyte != 0x7E)
		   &&(loc_secondbyte != 0x7D))
		{
			telemetry_sm.usr_data_buff[loc_index] = loc_secondbyte;
			loc_index++;
		}
		else if(  (loc_secondbyte == 0x7E)
		        ||(loc_secondbyte == 0x7D) )
		{
			telemetry_sm.usr_data_buff[loc_index] = 0x7D;
			telemetry_sm.usr_data_buff[loc_index + 1] = 0x5D;
			telemetry_sm.usr_data_buff[loc_index + 2] = (loc_secondbyte ^ 0x20) ^ 0x60;
			loc_index += 3;
		}
		else
		{
			telemetry_sm.usr_data_buff[loc_index] = 0x5D;
			telemetry_sm.usr_data_buff[loc_index + 1] = loc_secondbyte ^ 0x60;
			loc_index += 2;
		}
		/* Include Tail */
		telemetry_sm.usr_data_buff[loc_index] = 0x5E;
		loc_index++;
		/* Calculate Number of Pending Messages and Construct Buffer */
		telemetry_sm.index_usrd_pend = (loc_index - 1) / 6;

		/* Construct Telemetry Buffer */
		telemetry_sm.tel_txbuff[0] = 0x7E;
		telemetry_sm.tel_txbuff[1] = 0xFD;
		telemetry_sm.tel_txbuff[2] = loc_index;
		telemetry_sm.tel_txbuff[3] = (loc_indexinframe % 32);
		for(loc_aux = 0; loc_aux < loc_index; loc_aux++)
		{
			telemetry_sm.tel_txbuff[4 + loc_aux] = telemetry_sm.usr_data_buff[loc_aux];
		}
		loc_aux += 4;
		telemetry_sm.tel_txbuff[loc_aux] = 0x7E;
		telemetry_sm.frame_length = (loc_aux + 1);
	}
	/* If Number of Pending Messages != 0 */
	else
	{
		telemetry_sm.index_usrd_pend--;

		telemetry_sm.tel_txbuff[0] = 0x7E;
		telemetry_sm.tel_txbuff[1] = 0xFD;
		telemetry_sm.tel_txbuff[2] = 0x3;
		/* Feed Buffer with  6 Bytes and reduce in 1 Number of pending messages */
		for(loc_index = 6; loc_index < 9; loc_index++)
		{
			telemetry_sm.tel_txbuff[loc_index - 2] = telemetry_sm.usr_data_buff[loc_index];
		}
		telemetry_sm.tel_txbuff[7] = 0x7E;
		telemetry_sm.frame_length = 8;
		telemetry_sm.frame_telusr[telemetry_sm.frame_ctr] = (telemetry_sm.frame_telusr[telemetry_sm.frame_ctr] + 1) % frame_length[telemetry_sm.frame_ctr];
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            telemetry_sm_main                                           */
/*                                                                              */
/* DESCRIPTION:     Main function from the Telemetry State Machine              */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void telemetry_sm_main(eventtype const *event)
{
	/* Auxiliary variables */
	uint8_t loc_aux;

	switch (telemetry_sm.statem.state)
	{
		case INIT_TELEMETRY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					/* Setup Buff. pointers */
					telemetry_sm.tel_txbuff = TEL_SETUP_TXBUFF;
					telemetry_sm.statem.state = SENDING_TEL;
					telemetry_sm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case STANDBY_TEL:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_10ms:

				break;
				default:
				break;
			};
		break;
		case SENDING_TEL:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					telemetry_sm.tel_ctr = 0;
					telemetry_sm.index_usrd_pend = 0;
				break;
				case TRXACTIVE_SIG:
					telemetry_sm.tel_buff[TEL_EXT2_INDEX + TEL_FUEL_LEVEL] = (((SignalSM const *) event)->param);
				break;
				case INFO_RF_SIG:
					telemetry_sm.reset_ctr = TEL_RESET_PERIOD;
				break;
				case INFO_NO_RF_SIG:
					if(telemetry_sm.reset_ctr > 0)
					{
						telemetry_sm.reset_ctr--;
					}
					else
					{
						for(loc_aux = 0; loc_aux < TEL_L_BUFFER; loc_aux++)
						{
							if(loc_aux != (TEL_EXT1_INDEX + TEL_TEMPERATURE2)) telemetry_sm.tel_buff[loc_aux] = 0x00;
						}
					}
				break;
				case TEL_BATT_SIG:
				case TEL_RPM_SIG:
				case TEL_DIST_SIG:
				case TEL_CURR_SIG:
				case TEL_RSSITX_SIG:
				case TEL_RSSIRX_SIG:
				case TEL_CPULOAD_SIG:
				case TEL_TEMPRX_SIG:
				case TEL_TEMPTX_SIG:
					telemetry_update((uint8_t) event->sig, (((SignalSM const *) event)->param));
				break;
				case TSLOT_10ms:
					/* Rewrite with internal TX sensor Temperature if needed */
                    #if(TEL_USE_TXTEMP == USE_TX_TEMP)
					telemetry_update((uint8_t)TEL_TEMPTX_SIG, (int32_t)TEMP_HOPERF);
                    #endif

					/* Unlock the Telemetry TX Buffer to be modified */
					TEL_MODIFY_TXBUFF;
					/* Send Basic Frame as well as User Data Frames (Mode 2, max 6 Byte payload) */
					if(  (telemetry_sm.tel_ctr % TEL_USR_PERIOD) == 0
					   ||(telemetry_sm.index_usrd_pend != 0) )
					{
						/* CPU Load sent on Temp 2 value */
						//telemetry_sm.tel_buff[TEL_EXT1_INDEX + TEL_TEMPERATURE2] = CPU_LOAD;

						telemetry_userframe();
						/* Sets Telemetry Frame Length */
						TEL_SET_FRLENGTH(telemetry_sm.frame_length);
						/* Releases the Telemetry TX Buffer */
						TEL_RELEASE_TXBUFF;
					}
					else if((telemetry_sm.tel_ctr % TEL_BASIC_PERIOD) == 0)
					{
						/* Reconstruct Basic Telemetry Frame */
						telemetry_basicframe();
						/* Sets Telemetry Frame Length */
						TEL_SET_FRLENGTH(telemetry_sm.frame_length);
						/* Releases the Telemetry TX Buffer */
						TEL_RELEASE_TXBUFF;
					}

					/* Increment telemetry counter */
					telemetry_sm.tel_ctr++;
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
 * telemetry_sm.c
 *
 *  Created on: 07.12.2013
 *      Author: X-ArigitaJ
 */


