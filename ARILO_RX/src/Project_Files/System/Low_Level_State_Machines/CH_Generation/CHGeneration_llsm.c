/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             CHGeneration_llsm.c                                        */
/*                                                                              */
/* DESCRIPTION:      State machine that generates PWMs for 8 Channels in Servo  */
/*                   format. This SM does not use the Event Manager and         */
/*                   is triggered directly by a Hardware Interruption           */
/*                   for performance reasons                                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "../../Modules/MCU/hw_common.h"
#include "../../Modules/RF/hw_commonrf.h"
#include "CHGeneration_llsm.h"

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
/* NAME:            CHGeneration_llsm_init                                      */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_init(void)
{
	uint8_t i, ret_val;
	uint16_t ee_val;
	CHGeneration_llsm.ch_process = 0;
	CHGeneration_llsm.emergency = 0;
	/* Init. State Machine and positions in case the channel number  */
	/* does not exceed the physical limits                           */
	if(MAX_CHANNELS <= MAX_CH_SR)
	{
		CHGeneration_llsm.num_channels = MAX_CHANNELS;
		ret_val = EE_READ(FLAG_FAILSAFESET_EE, &ee_val);
		for(i = 0; i < MAX_CHANNELS; i++){
			CHGeneration_llsm.buff_channel[i] = SERVO_CENTER; /* Aprox. 1.38ms (4.096ms is 65536) */
			CHGeneration_llsm.channel[i] = SERVO_CENTER; /* Aprox. 1.38ms (4.096ms is 65536) */
			CHGeneration_llsm.fsafe_channel[i] = SERVO_CENTER;//22280; /* Aprox. 1.38ms (4.096ms is 65536) */
		};
		if(  (ret_val == 0)
		   &&(ee_val == FLAG_SET_EEPROM) )
		{
			for(i = 0; i < MAX_CHANNELS; i++)
			{
				if(ret_val == 0)
				{
					ret_val = EE_READ(CH_FAILSAFECHANNELS_EE(i), &ee_val);
					if(ret_val == 0)
					{
						CHGeneration_llsm.buff_channel[i] = ee_val;
						CHGeneration_llsm.channel[i] = ee_val;
						CHGeneration_llsm.fsafe_channel[i] = ee_val;
						SET_CH(i,CHGeneration_llsm.channel[i]);
					}
				}
			}
		}
		for(i = 0; i < MAX_CHANNELS; i++) SET_CH(i,CHGeneration_llsm.channel[i]);

		/* Channels are not released */
		CHGeneration_llsm.released = CTR_NOT_RELEASED;

		/* Initiate Counter to track period without receiving commands from the application layer */
		CHGeneration_llsm.counter_no_cmds = NO_COMM_MAX_PERIOD;

		/* Initial States */
		CHGeneration_llsm.state = INIT_CHANNELS_SAFEPOS;
	}
	else
	{
		/* State Machine Inactive */
		CHGeneration_llsm.state = 0;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_SetChReleasedFlag                                        */
/*                                                                              */
/* DESCRIPTION:     Sets ch released flags                                      */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_SetChReleasedFlag(void)
{
	CHGeneration_llsm.released = 1;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_UpdateBuffChannel                         */
/*                                                                              */
/* DESCRIPTION:     Updates buffered channels                                   */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_UpdateBuffChannel(uint8_t channel, uint16_t value)
{
	if(channel < MAX_CH_SR) CHGeneration_llsm.buff_channel[channel] = value;
}


/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_BuffSetChNumber                           */
/*                                                                              */
/* DESCRIPTION:     Sets ch Number Buff Channels                                */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_BuffSetChNumber(uint8_t nchannels)
{
	if(nchannels <= MAX_CH_SR) CHGeneration_llsm.num_buffchannels = nchannels;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_SetFailsafe                               */
/*                                                                              */
/* DESCRIPTION:     Sets failsafe ch.                                           */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_SetFailsafe(void)
{
	/* Set actual channels as Failsafe */
	/* Save them in EEPROM */
	uint8_t aux, ret_val = FLASH_COMPLETE;
	if(CHGeneration_llsm.eeprom_set == 0)
	{
		/* Save channels as failsafe in EEPROM */
		for(aux = 0; aux < MAX_CH_SR; aux++)
		{
			if(ret_val == FLASH_COMPLETE)
			{
				ret_val = EE_WRITE(CH_FAILSAFECHANNELS_EE(aux), CHGeneration_llsm.channel[aux]);
			}
		}
		if(ret_val == FLASH_COMPLETE)
		{
			ret_val = EE_WRITE(FLAG_FAILSAFESET_EE, FLAG_SET_EEPROM);
			for(aux = 0; aux < MAX_CH_SR; aux++)
			{
				CHGeneration_llsm.fsafe_channel[aux] = CHGeneration_llsm.channel[aux];
			}
		}
	}
	CHGeneration_llsm.eeprom_set = 1;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_EmergencyFailsafe                         */
/*                                                                              */
/* DESCRIPTION:     Release Failsafe Channels.                                  */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void CHGeneration_llsm_EmergencyFailsafe(uint8_t flag)
{
	CHGeneration_llsm.emergency = flag;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            CHGeneration_llsm_main                                      */
/*                                                                              */
/* DESCRIPTION:     Main function from the Control State Machine                */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern inline void CHGeneration_llsm_main(void)
{
	uint8_t i;
	switch(CHGeneration_llsm.state)
	{
		case INIT_CHANNELS_SAFEPOS:
			CHGeneration_llsm.counter_no_cmds = NO_COMM_MAX_PERIOD;
			for(i = 0; i < MAX_CHANNELS; i++)
			{
				/* Set Channel */
				SET_CH(i,CHGeneration_llsm.fsafe_channel[i]);
			};
			if(CHGeneration_llsm.released == CTR_RELEASED)
			{
				/* Change State */
				CHGeneration_llsm.state = CHANNELS_ACT;
			}
		break;
		case CHANNELS_ACT:
			if(CHGeneration_llsm.emergency == 0)
			{
				if(CHGeneration_llsm.released == CTR_RELEASED)
				{
					/* Process channels */
					CHGeneration_llsm.released = CTR_NOT_RELEASED;
					CHGeneration_llsm.counter_no_cmds = NO_COMM_MAX_PERIOD;
					for(i = 0; i < MAX_CHANNELS; i++)
					{
						CHGeneration_llsm.channel[i] = CHGeneration_llsm.buff_channel[i];
						/* Set Channel */
						SET_CH(i,CHGeneration_llsm.channel[i]);
					}
				}
				else if (   (CHGeneration_llsm.released == CTR_NOT_RELEASED)
						 && (CHGeneration_llsm.counter_no_cmds > 0) )
				{
					/* Reduce timer value for going to Safe Position */
					CHGeneration_llsm.counter_no_cmds--;
				}
				else
				{
					/* Go to Safe Position*/
					CHGeneration_llsm.num_channels = MAX_CHANNELS;
					for(i = 0; i < MAX_CHANNELS; i++)
					{
						CHGeneration_llsm.channel[i] = CHGeneration_llsm.fsafe_channel[i];
						/* Set Channel */
						SET_CH(i,CHGeneration_llsm.channel[i]);
					};

					/* Initiate Counter to track period without receiving commands from the application layer */
					CHGeneration_llsm.counter_no_cmds = NO_COMM_MAX_PERIOD;

					/* Change State */
					CHGeneration_llsm.state = INIT_CHANNELS_SAFEPOS;
				};
			}
			else
			{
				/* Change State */
				CHGeneration_llsm.state = INIT_CHANNELS_SAFEPOS;
			}
		break;
		default:
		break;
	};
};

/*
 * CHGeneration_llsm.c
 *
 *  Created on: 27/06/2012
 *      Author: arigita
 */


