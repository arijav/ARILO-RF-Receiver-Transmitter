/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             cmdInput_sm.c                                              */
/*                                                                              */
/* DESCRIPTION:      State machine that controls Push Buttoms.                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "cmdInput_sm.h"

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
/* NAME:            init_cmdInput_sm                                            */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_cmdInput_sm(void)
{
	cmdInput_sm.statem.state = INIT_CMDINPUT;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            cmdInput_sm_main                                            */
/*                                                                              */
/* DESCRIPTION:     Main function from the CmdInput State Machine               */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void cmdInput_sm_main(eventtype const *event)
{
	/* Auxiliary Signal */
	SignalSM loc_signal;
	SignalSM *loc_signal_p = &loc_signal;

	switch (cmdInput_sm.statem.state)
	{
		case INIT_CMDINPUT:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					if(PUSHBUTTOM_ON)
					{
						cmdInput_sm.statem.state = BINDING_REQ;
						/* Exit State Signal */
						cmdInput_sm_main(EXIT_STATE);
					}
					else
					{
						cmdInput_sm.statem.state = NO_REQ;
						/* Exit State Signal */
						cmdInput_sm_main(EXIT_STATE);
					}
				break;
				default:
				break;
			}
		break;
		case BINDING_REQ:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					cmdInput_sm.cnt = BIND_REQ_TIME;
				break;
				case TSLOT_80ms:
					if(PUSHBUTTOM_ON)
					{
						if(cmdInput_sm.cnt > 0)
						{
							cmdInput_sm.cnt--;
						}
						else
						{
							/* Send Binding Req. Signal to Communications State Machine */
							/* Fill appropriate Data */
							loc_signal_p->SM_sender = CMDINPUT_SM;
							loc_signal_p->SM_receiver = COMMUNICATIONS_SM;
							loc_signal_p->event.sig = PBUTTOM_BINDREQ_SIG;
							/* Send Signal */
							sendq_Sevent((eventtype *) loc_signal_p);

							/* Go to NO_REQ */
							cmdInput_sm.statem.state = NO_REQ;
							/* Exit State Signal */
							cmdInput_sm_main(EXIT_STATE);
						};
					}
					else
					{
						/* Go to NO_REQ */
						cmdInput_sm.statem.state = NO_REQ;
						/* Exit State Signal */
						cmdInput_sm_main(EXIT_STATE);
					}
				break;
				default:
				break;
			};
		break;
		case NO_REQ:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					cmdInput_sm.cnt = STANDBY_TIME_PUSH;
				break;
				case TSLOT_80ms:
					if(cmdInput_sm.cnt != 0)
					{
						/* During no req time push buttom should not be ON, */
						/* otherwise we reset waiting time                 */
						if(PUSHBUTTOM_ON)
						{
							cmdInput_sm.cnt = STANDBY_TIME_PUSH;
						}
						else
						{
							cmdInput_sm.cnt--;
						}
					}
					else if(PUSHBUTTOM_ON)
					{
						/* Go to binding */
						cmdInput_sm.statem.state = BINDING_REQ;
						/* Exit State Signal */
						cmdInput_sm_main(EXIT_STATE);
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
 * cmdInput_sm.c
 *
 *  Created on: 15.11.2013
 *      Author: X-ArigitaJ
 */


