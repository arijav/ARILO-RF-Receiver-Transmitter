/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Error_Handler_LLSM.h                                       */
/*                                                                              */
/* DESCRIPTION:      Error Handler                                              */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef ERROR_HANDLER_LLSM_H_
#define ERROR_HANDLER_LLSM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../../../config_switches.h"


/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* Config related defines */
#define ERROR_OFF 0
#define ERROR_ON  1

/* Error Handler Interface */
#define ERRORHANDLER_LLSM_IFACE(x,y) ErrorHandler_llsm_Iface(x,y)

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_ErrorHandlerLLSM {             /* States Erro Handler State Machine */
    ERROR_HANDLER_DEINIT = 1,               /* De-initialize and deactivate TRX */
    ERROR_HANDLER_REINIT,                     /* Re-initialize and activate TRX */
    ERROR_HANDLER_INIT,                            /* Wait for Application Init */
    ERROR_HANDLER_PROCESS_ERRORS,                             /* Process errors */

    MAX_STATE_ERROR_HANDLER_LLSM
};


enum Errors {                                                         /* Errors */
    /* Application SW errors */
    /* CmdInput Errors */
    /* Communications Errors */
    /* Telemetry Errors */
    /* Low Level SW errors */
    /* Timer Errors */
    /* RF Handler errors */
    /* SafeLink Handler errors */
    SFLINK_ERROR_RESET,
    /* Hoperf Module Errors */
    HPERF_ERROR_FIFO_OF_UF,
    HPERF_ERROR_FIFO_AL_EMPTY,
    HPERF_ERROR_FIFO_AL_FULL,
    HPERF_ERROR_CRC,
    HPERF_ERROR_POWER_ON_RESET,
    HPERF_ERROR_LOW_BAT,
    /* NRF24L01+ Module Errors */
    /* Event Manager Errors */
    EVMNG_ERROR_SIG_UNKNOWN,
    EVMNG_ERROR_FALSEQUEUE_SIG,
    EVMNG_ERROR_QUEUE_OVERFLOW,
    MAX_ERROR                                                 /* The last error */
};

enum Errors_Fields {                                    /* Errors Matrix Fields */
    ERROR_STATE,
    ERROR_OCC_CNT,
    MAX_ERROR_FIELDS                                 /* Max Error Matrix fields */
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Communications State Machine typedef */
typedef struct ErrorHandlerLLSMTag {
    /* State */
    uint8_t state;
    /* Error Matrix: Error State, Occurrence Counter */
    uint8_t Error_Matrix[MAX_ERROR][MAX_ERROR_FIELDS];
} ErrorHandlerLLSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
volatile ErrorHandlerLLSM errorhandler_llsm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
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
extern void ErrorHandler_llsm_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_reinit                                    */
/*                                                                              */
/* DESCRIPTION:     Re-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void ErrorHandler_llsm_reinit(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_deinit                                    */
/*                                                                              */
/* DESCRIPTION:     De-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void ErrorHandler_llsm_deinit(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_main                                      */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void ErrorHandler_llsm_main(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            ErrorHandler_llsm_Iface                                     */
/*                                                                              */
/* DESCRIPTION:     Interface function from the Error Handler                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void ErrorHandler_llsm_Iface(uint8_t error, uint8_t state);

#endif /* ERROR_HANDLER_LLSM_H_ */

/*
 * Error_Handler_LLSM.h
 *
 *  Created on: 02.05.2015
 *      Author: ArigitaJ
 */
