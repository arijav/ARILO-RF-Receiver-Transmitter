/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlCH_sm.c                                             */
/*                                                                              */
/* DESCRIPTION:      State machine that processes control events                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "controlCH_sm.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
/* Debugging, registers number of times in each state  */
/* extern uint32_t state_counter_sm[BROADCAST_SM][10]; */

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
/* NAME:            init_controlCH_sm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_controlCH_sm(void)
{
	/* Initial States */
	/* TODO: Modify with the real data for the Control State Machine when implemented */
	control_sm.statem.state = INIT_CONTROLCH;
	control_sm.max_ch = MAX_CHANNELS;

	/* Set initial Control Mode */
	control_sm.ctr_type_status = CTR_MANUAL;

	/* Reset rest of fields */
	control_sm.command = (uint8_t) 0;
	control_sm.emergency_status = STATUS_EMERGENCY_OFF;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlCH_sm_main                                           */
/*                                                                              */
/* DESCRIPTION:     Main function from the Control State Machine                */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void controlCH_sm_main(eventtype const *event)
{
	/* TODO: Test, debugging */
	/* state_counter_sm[(CONTROL_CH - 1)][control_sm.statem.state]++; */
	uint8_t aux;

	switch (control_sm.statem.state)
	{
		case INIT_CONTROLCH:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* TODO: Test */
				break;
				case GOALIVE_SIG:
					/* Change State to */
					control_sm.statem.state = WAITING_STARTUP;
					/* Exit State Signal */
					controlCH_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case WAITING_STARTUP:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* TODO: Test */
				break;
				case INFO_RF_SIG:
					/* Change State to */
					control_sm.statem.state = RECEIVE_CMD_COMM;
					/* Exit State Signal */
					controlCH_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case RECEIVE_CMD_COMM:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case CTR_COMMANDRX_SIG:
					if(1/* TODO: Test CMD_MODE_MANUAL*/)
					{
						/* Change State to */
						control_sm.statem.state = MANUALM_RECEIVE_CHANNELS;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
					else if(CMD_MODE_AUTOPILOT)
					{
						/* Change State to */
						control_sm.statem.state = AUTOPILOTM_RTH;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
				break;
				case INFO_NO_RF_SIG:
					/* Change State to */
					/* TODO: Test */
					/* control_sm.statem.state = AUTOPILOTM_RTH; */
					/* Exit State Signal */
					/* controlCH_sm_main(EXIT_STATE); */
				break;
				case EMERGENCY_SIG:
					if(!(CMD_EMERGENCY_BYPASS))
					{
						/* Change State to */
						control_sm.statem.state = AUTOPILOTM_EMERGENCY;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
				break;
				default:
				break;
			};
		break;
		case MANUALM_RECEIVE_CHANNELS:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
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
					if(((event->sig) - CTR_CH0_SIG) == (uint8_t)(control_sm.max_ch - 1))
					{
						UPDATE_BUFF_CHANNELS((uint8_t)((event->sig) - CTR_CH0_SIG), (uint16_t)(TX_RX_RESOLUTION_CONV * (float)((SignalSM const *) event)->param));
						/* Change State to */
						control_sm.statem.state = MANUALM_PROCESS_CHANNELS;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
					else if(((event->sig) - CTR_CH0_SIG) > (uint8_t)(control_sm.max_ch - 1))
					{
						/* Change State to */
						control_sm.statem.state = RECEIVE_CMD_COMM;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
					else
					{
						UPDATE_BUFF_CHANNELS((uint8_t)((event->sig) - CTR_CH0_SIG), (uint16_t)(TX_RX_RESOLUTION_CONV * (float)((SignalSM const *) event)->param));
					};
				break;
				case CTR_NCHANNELS_SIG:
					aux = (uint8_t) ((SignalSM const *) event)->param;
					CHGeneration_llsm_BuffSetChNumber(aux);
					if(aux <= MAX_CHANNELS)
					{
						control_sm.max_ch = aux;
					}
					else
					{
						control_sm.max_ch = MAX_CHANNELS;
					}

				break;
				case INFO_NO_RF_SIG:
					/* Change State to */
					/* TODO: Test */
					/* control_sm.statem.state = AUTOPILOTM_RTH; */
					/* Exit State Signal */
					/* controlCH_sm_main(EXIT_STATE); */
				break;
				case EMERGENCY_SIG:
					if(!(CMD_EMERGENCY_BYPASS))
					{
						/* Change State to */
						/* control_sm.statem.state = AUTOPILOTM_EMERGENCY; */
						/* Exit State Signal */
						/* controlCH_sm_main(EXIT_STATE); */
					};
				break;
				case PBUTTOM_FSSETREQ_SIG:
					/* Change to set Failsafe State */
					control_sm.statem.state = MANUALM_SET_FAILSAFE;
					controlCH_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case MANUALM_PROCESS_CHANNELS:
			switch(event->sig)
			{
				case ENTRY_SIG:
					if(!(CMD_MODE_ASSISTED))
					{
						RELEASE_BUFF_CHANNELS;
						/* Change State to */
						control_sm.statem.state = MANUALM_RECEIVE_CHANNELS;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
					else
					{
						/* Change State to */
						/* control_sm.statem.state = ASSISTEDM_PROCESS_CHANNELS; */
						/* Exit State Signal */
						/* controlCH_sm_main(EXIT_STATE); */
					}
				break;
				default:
				break;
			};
		break;
		case MANUALM_SET_FAILSAFE:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* Set Failsafe Values */
					SET_CH_FAILSAFE;
					/* Change State to */
					control_sm.statem.state = MANUALM_RECEIVE_CHANNELS;
					/* Exit State Signal */
					controlCH_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case ASSISTEDM_PROCESS_CHANNELS:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/**** TODO: PROCESS CHANNELS ****/
					RELEASE_BUFF_CHANNELS;
					/* Change State to */
					control_sm.statem.state = RECEIVE_CMD_COMM;
					/* Exit State Signal */
					controlCH_sm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case AUTOPILOTM_RTH:
			switch(event->sig)
			{
				case ENTRY_SIG:
					control_sm.ctr_type_status = CTR_AUTOPILOT;
					/**** TODO: PROCESS CHANNELS ****/
					/* release_buff_channels(); */
				break;
				case TSLOT_10ms:
					/**** TODO: PROCESS CHANNELS ****/
					/* release_buff_channels(); */
				break;
				case CTR_COMMANDRX_SIG:
					if(CMD_MODE_MANUAL)
					{
						/* Change State to */
						control_sm.statem.state = MANUALM_RECEIVE_CHANNELS;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
				break;
				default:
				break;
			}
		break;
		case AUTOPILOTM_EMERGENCY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					control_sm.ctr_type_status = CTR_AUTOPILOT;
					/**** TODO: PROCESS CHANNELS ****/
					/* release_buff_channels(); */
				break;
				case TSLOT_10ms:
					/**** TODO: PROCESS CHANNELS and set emergency_status ****/
					/* release_buff_channels(); */
					if ((control_sm.emergency_status == STATUS_EMERGENCY_OFF) || (CMD_EMERGENCY_BYPASS))
					{
						/* Change State to */
						control_sm.statem.state = RECEIVE_CMD_COMM;
						/* Exit State Signal */
						controlCH_sm_main(EXIT_STATE);
					}
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
};

/*
 * controlCH_sm.c
 *
 *  Created on: 27/06/2012
 *      Author: arigita
 */


