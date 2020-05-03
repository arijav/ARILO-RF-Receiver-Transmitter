/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlfan_sm.c                                            */
/*                                                                              */
/* DESCRIPTION:      State machine that controls the fan                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include "controlfan_sm.h"
#include "../../../System/HAL/hw_general.h"

#if(ARILOTX_HWREV == ARILOTX_HW2)
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
/* NAME:            init_controlfan_sm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_controlfan_sm(void)
{
	/* Initial States */
	controlfan_sm.statem.state = INIT_FAN;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlfan_sm_main                                          */
/*                                                                              */
/* DESCRIPTION:     Main function from the Fan State Machine                    */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void controlfan_sm_main(eventtype const *event)
{

	/* SM */
	switch (controlfan_sm.statem.state)
	{
		case INIT_FAN:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */
					/* Set 0% PWM Duty Cycle at 25KHz */
					FAN(25000, 0);
				break;
				case GOALIVE_SIG:
					/* Actions */
					controlfan_sm.statem.state = CONTROL_FAN;
					/* Exit State Signal */
					controlfan_sm_main(EXIT_STATE);
				break;
				default:
				break;
			};
		break;
		case CONTROL_FAN:
			switch(event->sig)
			{
				case ENTRY_SIG:
					/* Entry call code */

				break;
				case TSLOT_80ms:
					/* Actions */
					/* Activate fan if temperature goes over 65° */
                    if(TEMP_HOPERF >= 65)
                    {
    					/* Set 50% PWM Duty Cycle at 25KHz */
    					FAN(25000, 50);
                    }
                    else
                    {
    					/* Set 0% PWM Duty Cycle at 25KHz */
    					FAN(25000, 0);
                    }
				break;
				default:
				break;
			};
		break;
		default:
		break;
	}
}

#endif

/*
 * controlfan_sm.c
 *
 *  Created on: 04/03/2015
 *      Author: arigita
 */


