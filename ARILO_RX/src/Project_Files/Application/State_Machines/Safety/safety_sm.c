/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             safety_sm.c                                                */
/*                                                                              */
/* DESCRIPTION:      State machine that controls Safety                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "safety_sm.h"

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
/* NAME:            init_safety_sm                                              */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_safety_sm(void)
{
	safety_sm.cnt = 0;
	safety_sm.statem.state = INIT_SAFETY;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            safety_sm_main                                              */
/*                                                                              */
/* DESCRIPTION:     Main function from the Safety State Machine                 */
/*                                                                              */
/* PARAMETERS:      In: Event                                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void safety_sm_main(eventtype const *event)
{
	switch (safety_sm.statem.state)
	{
		case INIT_SAFETY:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case GOALIVE_SIG:
					safety_sm.statem.state = INIT_WD;
					/* Exit State Signal */
					safety_sm_main(EXIT_STATE);
				break;
				default:
				break;
			}
		break;
		case INIT_WD:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_10ms:
					/* safety_sm.cnt++; */
					/* if(safety_sm.cnt == 100) */
					/*{ */
						/* Start Watchdog */
						/* START_WATCHDOG; */
						safety_sm.statem.state = REFRESH_WD;
						/* Exit State Signal */
						safety_sm_main(EXIT_STATE);
					/*} */
				break;
				default:
				break;
			}
		break;
		case REFRESH_WD:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
				break;
				case TSLOT_10ms:
					/* Refresh Watchdog */
					/* REFRESH_WATCHDOG; */
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
 * safety_sm.c
 *
 *  Created on: 22.02.2014
 *      Author: ArigitaJ
 */


