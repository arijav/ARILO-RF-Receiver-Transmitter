/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             port_init.h                                                */
/*                                                                              */
/* DESCRIPTION:      STM32 Port HW Init.                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_PORTS_H_
#define HW_PORTS_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
#if(ARILORX_HWREV == ARILORX_HW1)
#define UPPER_FLANK 0
#define LOWER_FLANK 1
#define MAX_TIMESTAMPS 100

/* MAX Time for RPM calculations */
#define MAX_TIME 4294967295u

/* EXT Numbers */
#define EXTI3 0
#define EXTI6 1
#define MAX_EXTI 2

/* Number of timestamps between the two selected for RPM calculation */
#define TIMESTAMPS_RPMCALC_OFFSET 3
#endif

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
#if(ARILORX_HWREV == ARILORX_HW1)
/* Communications State Machine typedef */
typedef struct EXTTimeStampStructTag {
	/* Array */
	float32_t array[MAX_TIMESTAMPS];
	/* Head */
	uint8_t head;
} EXTTimeStampStruct;
#endif

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             port_init                                                  */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Ports Init.                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void port_init(void);

#if(ARILORX_HWREV == ARILORX_HW1)
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTI3Count()                                            */
/*                                                                              */
/* DESCRIPTION:      Get number of EXTI3 interrupts until the call              */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI3 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetEXTI3Count(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTI6Count()                                            */
/*                                                                              */
/* DESCRIPTION:      Get number of EXTI6 interrupts until the call              */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI6 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetEXTI6Count(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTITimeLastPulse()                                     */
/*                                                                              */
/* DESCRIPTION:      Get time stamps of the last pulse                          */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI6 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern float32_t GetEXTITimeLastPulse(uint8_t ext_number);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTITimeStamp()                                         */
/*                                                                              */
/* DESCRIPTION:      Get time stamps difference of the last two EXT Interrupts  */
/*                   (float in ms)                                              */
/*                                                                              */
/* PARAMETERS:       Out : Number of EXTI6 interrupts                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern float32_t GetEXTITimeStampDiff(uint8_t ext_number);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetEXTITimeStamp()                                         */
/*                                                                              */
/* DESCRIPTION:      Get time number of timestamps between the two used for     */
/*                   RPM calculation                                            */
/*                                                                              */
/* PARAMETERS:       Out : Number of Timestamps                                 */
/*                   In :  EXTI Number                                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t GetTimeStampRPMOff(uint8_t ext_number);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   EXTI3(IRQ Handler)                                         */
/*                                                                              */
/* DESCRIPTION:      STM32 EXTI3 routine                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   EXTI9_5(IRQ Handler)                                       */
/*                                                                              */
/* DESCRIPTION:      STM32 EXTI9_5 routine                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
#endif

#endif /* HW_PORTS_H_ */
/*
 * hw_ports.h
 *
 *  Created on: 25.05.2012
 *      Author: X-ArigitaJ
 */

