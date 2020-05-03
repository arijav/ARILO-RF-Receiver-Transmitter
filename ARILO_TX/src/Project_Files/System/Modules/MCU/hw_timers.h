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
#define MAX_TIMER 60000
#define INIT_CR 0
#define TIM4_PERIOD 35999

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
/* PARAMETERS:       In : frequency (Hz)                                        */
/*                        duty_cycle (%)                                        */
/*                        timer_num                                             */
/*                        output_ch                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SetChannel(float32_t frequency, uint8_t duty_cycle, uint8_t timer_num, uint8_t output_ch);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetTimeDiffFlank                                           */
/*                                                                              */
/* DESCRIPTION:      Calculates the time between flanks uses one Timer          */
/*                                                                              */
/* PARAMETERS:       Out: Time in Timer1 units (counts)                         */
/*                   In : Time first flank                                      */
/*                        Time second flank                                     */
/*                        Timer 1 Cycle difference between flanks               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetTimeDiffFlank(uint16_t first_fl, uint16_t second_fl, uint8_t cyclediff);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetMicros()                                                */
/*                                                                              */
/* DESCRIPTION:      Get number of uS counted from Systick Timer                */
/*                                                                              */
/* PARAMETERS:       Out : Number of us in Systick Timer (overflow 10ms)        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint16_t GetMicros(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetTicks()                                                 */
/*                                                                              */
/* DESCRIPTION:      Get number of ticks until the call (1 TICK = 1ms)          */
/*                                                                              */
/* PARAMETERS:       Out : Number of ticks                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetTicks(void);

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
/* NAME:             GetOverflowTimer8()                                        */
/*                                                                              */
/* DESCRIPTION:      Get number of periods of Timer 8 counted                   */
/*                                                                              */
/* PARAMETERS:       Out : Number of ticks                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetOverflowTimer8(void);

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


#endif /* HW_TIMERS_H_ */

/*
 * hw_timers.h
 *
 *  Created on: 30.05.2012
 *      Author: X-ArigitaJ
 */
