/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_timers.h                                                */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Timer init and functions                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_TIMERS_H_
#define HW_TIMERS_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines & Enums                                                              */
/*------------------------------------------------------------------------------*/
enum time_isr {                                                     /* Time ISR */
	/* Time ISR */
	SYSTICK_ISR = 0,
	FOUR_MS_ISR,

    MAX_TISR                                               /* The last time ISR */
};

/* Relationship between us and CPU Tackt */
#define US_TACKT (float32_t)((float32_t)1000000 / (float32_t)SystemCoreClock)
#define INIT_CR 2942

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Timers_init                                                */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Timers Init                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Timers_init();

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SetChannel                                                 */
/*                                                                              */
/* DESCRIPTION:      Sets PWM in selected channel                               */
/*                                                                              */
/* PARAMETERS:       In : Channel                                               */
/*                        pulse_value in Timer units                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void SetChannel(uint8_t channel, uint16_t pulse_value);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetIndMillis()                                             */
/*                                                                              */
/* DESCRIPTION:      Get number of milliseconds (ind. counter) until the call   */
/*                                                                              */
/* PARAMETERS:       Out : Number of ticks                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetIndMillis(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetTicks()                                                 */
/*                                                                              */
/* DESCRIPTION:      Get number of ticks until the call (1 TICK = 10ms)         */
/*                                                                              */
/* PARAMETERS:       Out : Number of ticks                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetTicks(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetMicros()                                                */
/*                                                                              */
/* DESCRIPTION:      Get number of micros of the systick counter (max 1000)     */
/*                                                                              */
/* PARAMETERS:       Out : Number of ticks                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint16_t GetMicros(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   get_cpu_load_calc                                          */
/*                                                                              */
/* DESCRIPTION:      Get CPU Load Calculation                                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t get_cpu_load_calc(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   DelayUs                                                    */
/*                                                                              */
/* DESCRIPTION:      STM32 uS Delay                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void DelayUs(uint32_t delay_us);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(SysTick)                                               */
/*                                                                              */
/* DESCRIPTION:      STM32 SysTick Handler                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(Timer3 Overflow)                                       */
/*                                                                              */
/* DESCRIPTION:      STM32 Timer 3 Overflow Handler                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   TIM2_IRQHandler (Timer Update)                             */
/*                                                                              */
/* DESCRIPTION:      STM32 TIM2 IRQ routine                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#endif /* HW_TIMERS_H_ */

/*
 * hw_timers.h
 *
 *  Created on: 30.05.2012
 *      Author: X-ArigitaJ
 */
