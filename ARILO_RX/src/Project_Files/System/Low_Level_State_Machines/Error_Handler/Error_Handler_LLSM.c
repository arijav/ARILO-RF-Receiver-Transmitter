/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Error_Handler_LLSM.c                                       */
/*                                                                              */
/* DESCRIPTION:      Error Handler                                              */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "stm32f10x.h"
#include "Error_Handler_LLSM.h"


/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/
/* TODO: Test */
uint32_t errorhandler_states[MAX_STATE_ERROR_HANDLER_LLSM];

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
/* NAME:            ErrorHandler_llsm_init                                      */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void ErrorHandler_llsm_init(void)
{
    uint8_t i,j;
    for(i = 0; i < MAX_ERROR; i++)
    {
        for(j = 0; j < MAX_ERROR_FIELDS; j++)
        {
            errorhandler_llsm.Error_Matrix[i][j] = 0;
        }
    }
    /* Initialize Data Structure to Defaults */
    errorhandler_llsm.state = ERROR_HANDLER_INIT;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_reinit                                    */
/*                                                                              */
/* DESCRIPTION:     Re-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void ErrorHandler_llsm_reinit(void)
{
	errorhandler_llsm.state = ERROR_HANDLER_REINIT;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_deinit                                    */
/*                                                                              */
/* DESCRIPTION:     De-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void ErrorHandler_llsm_deinit(void)
{
    errorhandler_llsm.state = ERROR_HANDLER_DEINIT;
}


/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_main                                      */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void ErrorHandler_llsm_main(void)
{
    uint8_t loc_state = errorhandler_llsm.state;
    errorhandler_states[loc_state]++;
    switch(loc_state)
    {
        case ERROR_HANDLER_DEINIT:
             /* Entry actions */
        break;
        case ERROR_HANDLER_REINIT:
            /* Entry actions */
            errorhandler_llsm.state = ERROR_HANDLER_INIT;
        break;
        case ERROR_HANDLER_INIT:
            /* Entry actions */
            errorhandler_llsm.state = ERROR_HANDLER_PROCESS_ERRORS;
        break;
        case ERROR_HANDLER_PROCESS_ERRORS:
            /* Entry actions */
        break;
        default:
        break;
    };
}


/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_Iface                                     */
/*                                                                              */
/* DESCRIPTION:     Interface function from the Error Handler                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void ErrorHandler_llsm_Iface(uint8_t error, uint8_t state)
{
    /* Deactivate Interrupts */
    __disable_irq();
    if(  (error < MAX_ERROR)
       &&(errorhandler_llsm.state == ERROR_HANDLER_PROCESS_ERRORS)  )
    {
        if(state == ERROR_ON)
        {
            errorhandler_llsm.Error_Matrix[error][ERROR_STATE] = ERROR_ON;
            if(errorhandler_llsm.Error_Matrix[error][ERROR_OCC_CNT] < 255) errorhandler_llsm.Error_Matrix[error][ERROR_OCC_CNT] += 1;
        }
        else
        {
            errorhandler_llsm.Error_Matrix[error][ERROR_STATE] = ERROR_OFF;
        }
    }
    /* Reactivate Interrupts */
    __enable_irq();
}

/*
 * Error_Handler_LLSM.c
 *
 *  Created on: 02.05.2015
 *      Author: ArigitaJ
 */


