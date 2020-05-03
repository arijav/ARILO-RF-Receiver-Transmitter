/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_timers.c                                                */
/*                                                                              */
/* DESCRIPTION:      STM32 HW Timer init and functions                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "hw_timers.h"
#include "stm32f10x.h"
#include "config_switches.h"
#include "../../Low_Level_State_Machines/RF_Handler/RF_Handler_LLSM.h"
#include "../../Low_Level_State_Machines/SafeLink_Handler/SafeLink_Handler_LLSM.h"
#include "../../Low_Level_State_Machines/Error_Handler/Error_Handler_LLSM.h"
#include "../../../Event_Manager/event_manager.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
/* System Tick every 1ms */
volatile uint32_t system_tick_count = 0;
uint16_t PrescalerValue;
#if(ARILOTX_HWREV == ARILOTX_HW1)
uint16_t CCR4_Val = INIT_CR;
volatile uint32_t timer8_overflow_count = 0;
volatile uint32_t timer3_overflow_count = 0;
#else
uint16_t CCR2_Val = INIT_CR;
uint16_t CCR4_Val = INIT_CR;
#if(ARILOTX_HWREV != ARILOTX_HW1)
volatile uint32_t timer1_overflow_count = 0;
#endif
#endif
/* INT event */
EventINT INT_events;
EventINT *INT_events_p = &INT_events;
#if(ARILOTX_HWREV == ARILOTX_HW1)
volatile uint32_t timer8_capture_count = 0;
#else
volatile uint32_t timer3_capture_count = 0;
#endif
volatile uint8_t cpu_load_isr = 0;
volatile uint8_t cpu_load_events = 0;
volatile uint8_t cpu_load = 0;
volatile uint8_t cpu_load_max = 0;
volatile float32_t isr_time[MAX_TISR];

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
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
extern void Timers_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	#if(ARILOTX_HWREV == ARILOTX_HW1)
	/* ----------------------------------------------------------------------------------------
	TIM8 Configuration: Input Capture/Output compare. Period = 10ms (6MHz / 60000 = 100Hz)
	---------------------------------------------------------------------------------------- */
	/* Compute the prescaler value */
	/* Freq. 6MHz */
	PrescalerValue = (uint16_t) (SystemCoreClock / 6000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 59999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM8, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Input Capture: Channel1 */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

	/* Output Compare Inactive Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 59999;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

	TIM_OC2Init(TIM8, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

	/* TIM enable counter */
	TIM_Cmd(TIM8, ENABLE);

	/* Enable the CC1/CC2 Interrupt Request */
	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);

	TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);

	/* ----------------------------------------------------------------------------------------
	TIM4 Configuration: Output compare. Freq. 8MHz
	---------------------------------------------------------------------------------------- */
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0; /* -1 is already included */
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM enable counter */
	TIM_Cmd(TIM4, ENABLE);

	/* ----------------------------------------------------------------------------------------
	TIM3 Configuration: No output. Freq. 12MHz
	---------------------------------------------------------------------------------------- */
	/* Configure Timer 3 for 4ms Overflow ISR (250Hz = 12MHz prescaler and Period of 47999) */
	PrescalerValue = (uint16_t) (SystemCoreClock / 12000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 47999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* Enable Update Interrupt */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	#else
	/* ----------------------------------------------------------------------------------------
	TIM3 Configuration: Input Capture/Output compare. Period = 10ms (6MHz / 60000 = 100Hz)
	---------------------------------------------------------------------------------------- */
	/* Compute the prescaler value */
	/* Freq. 6MHz */
	PrescalerValue = (uint16_t) (SystemCoreClock / 6000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 59999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Input Capture: Channel1 */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);


	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* Enable the CC1/CC2 Interrupt Request */
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);


	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

	/* ----------------------------------------------------------------------------------------
	TIM2 Configuration: Output compare. Freq. 8MHz
	---------------------------------------------------------------------------------------- */
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0; /* -1 is already included */
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);

	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);

	/* ----------------------------------------------------------------------------------------
	TIM1 Configuration: No output. Freq. 12MHz
	---------------------------------------------------------------------------------------- */
	/* Configure Timer 1 for 4ms Overflow ISR (250Hz = 12MHz prescaler and Period of 47999) */
	PrescalerValue = (uint16_t) (SystemCoreClock / 12000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 47999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* TIM enable counter */
	TIM_Cmd(TIM1, ENABLE);

	/* Enable Update Interrupt */
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	#endif

	/* ----------------------------------------------------------------------------------------
	TIM4 Configuration: Output compare. Freq. 25KHz
	---------------------------------------------------------------------------------------- */
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 25000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0; /* -1 is already included */
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM enable counter */
	TIM_Cmd(TIM4, ENABLE);

	/* Watchdog Timer */

	/* WWDG clock counter = (PCLK1 (72MHz)/4096)/8 = 2197Hz (~455us)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 110; WWDG counter should be refreshed only when the counter
	 is below 110 (and greater than 64) otherwise a reset will be generated */
	WWDG_SetWindowValue(127);

	/* Enable WWDG at INIT and set counter value to 127, WWDG timeout = ~455 us * 64 = 29,7 ms
	 In this case the refresh window is: ~455 * (127-110)= 7,735ms < refresh window < ~455 us * 64 = 29,7 ms
	*/
};

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
void SetChannel(float32_t frequency, uint8_t duty_cycle, uint8_t timer_num, uint8_t output_ch)
{
	uint32_t loc_freq, loc_prescaler, loc_pulse, loc_reload;
	float32_t loc_aux1, loc_aux2;
	uint16_t loc_aux3;
	TIM_TypeDef* timer;

	switch(timer_num)
	{
        case 1:
        	timer = TIM1;
        break;
        case 2:
        	timer = TIM2;
        break;
        case 3:
        	timer = TIM3;
        break;
        case 4:
        	timer = TIM4;
        break;
        default:
        	timer = TIM2;
        break;
    }

    loc_pulse = 0;

    /* Calculate Freq Factor for prescaler */
    loc_aux3 = ((uint16_t)frequency / (uint16_t)1000) + 1;

    /* Configure Timer */
    if(  (     (frequency <= 25000)
             &&(frequency > 0) )
          &&(duty_cycle <= 100) )
    {
        /* Calculate frequency for the Prescaler: Desired Frequency * 65536 / loc_aux3 (maximum 72000000) */
        /* Convert to uint */
        loc_freq = ((uint32_t)frequency) * (65536 / loc_aux3);
        loc_prescaler = (uint16_t) ((SystemCoreClock / loc_freq) - 1) + 1;

        loc_aux1 = (float32_t)(SystemCoreClock / (loc_prescaler + 1));
        loc_aux2 = frequency;

        /* Calculate Reload Value (Period) */
        loc_reload = (uint16_t)(loc_aux1 / loc_aux2) / loc_aux3;

        /* Calculate Duty Cycle */
        loc_pulse = (uint16_t)((float32_t)duty_cycle * (float32_t)loc_reload / (float32_t)100);


        /* Set Timer Config */
        TIM_PrescalerConfig(timer, loc_prescaler, TIM_PSCReloadMode_Immediate);
        TIM_SetAutoreload(timer, loc_reload);
    }
    else
    {
        /* Silence */
        /* Set Timer Config */
        TIM_PrescalerConfig(timer, 0, TIM_PSCReloadMode_Immediate);
        TIM_SetAutoreload(timer, 65535);
    }

    /* Set Output Channel */
    switch(output_ch)
    {
        case 1:
        	TIM_SetCompare1(timer, loc_pulse);
        break;
        case 2:
        	TIM_SetCompare2(timer, loc_pulse);
        break;
        case 3:
        	TIM_SetCompare3(timer, loc_pulse);
        break;
        case 4:
        	TIM_SetCompare4(timer, loc_pulse);
        break;
        default:
        	TIM_SetCompare2(timer, loc_pulse);
        break;
	}
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetTimeUp                                                  */
/*                                                                              */
/* DESCRIPTION:      Calculates the time between flanks uses Timer 8            */
/*                                                                              */
/* PARAMETERS:       Out: Microseconds                                          */
/*                   In : Time first flank                                      */
/*                        Time second flank                                     */
/*                        Tick Cycle difference between flanks                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint32_t GetTimeDiffFlank(uint16_t first_fl, uint16_t second_fl, uint8_t cyclediff)
{
    uint32_t pulse;

    if(cyclediff == 0)
    {
        if(second_fl > first_fl)
        {
            pulse = (int32_t)(second_fl - first_fl);
        }
        else
        {
            pulse = 0;
            /* Report Timer Capture Error */
            ERRORHANDLER_LLSM_IFACE(TIMER_CAPTURE_ERROR ,ERROR_ON);
        }
    }
    else
    {
        pulse = (uint32_t)(second_fl + (MAX_TIMER - first_fl) + (MAX_TIMER * (cyclediff - 1)));
    };
    return (pulse);
}

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
uint16_t GetMicros(void)
{
	uint16_t aux;
	aux = (uint16_t)((float32_t)SysTick->VAL * (float32_t)US_TACKT);
	return aux;
}

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
uint32_t GetTicks(void)
{
	uint32_t aux;
	aux = system_tick_count;
	return aux;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             reset_isr_dur                                              */
/*                                                                              */
/* DESCRIPTION:      Resets ISR durations                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void reset_isr_dur(void)
{
	uint8_t aux;
	for(aux = 0; aux < MAX_TISR; aux++) isr_time[aux] = 0;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             get_isr_dur                                                */
/*                                                                              */
/* DESCRIPTION:      Gets ISR durations                                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
float32_t get_isr_dur(void)
{
	uint8_t aux;
	float32_t duration = 0;
	for(aux = 0; aux < MAX_TISR; aux++) duration += isr_time[aux];
	reset_isr_dur();
	return duration;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             cpu_load_calc                                              */
/*                                                                              */
/* DESCRIPTION:      CPU Load Calculation                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void cpu_load_calc(void)
{
	float32_t aux_time_ev, aux_time_isr;
	/* First add the Task load */
	aux_time_ev = get_event_dur();
	cpu_load_events = 100 * (aux_time_ev / (float32_t)CPU_LOAD_TWIN);
	/* Second add the ISR load */
	aux_time_isr = get_isr_dur();
	cpu_load_isr = 100 * (aux_time_isr / (float32_t)CPU_LOAD_TWIN);
	cpu_load = 100 * ((aux_time_isr + aux_time_ev) / (float32_t)CPU_LOAD_TWIN);
	if(cpu_load > cpu_load_max) cpu_load_max = cpu_load;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             get_cpu_load_calc                                          */
/*                                                                              */
/* DESCRIPTION:      Get CPU Load Calculation                                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t get_cpu_load_calc(void)
{
	return cpu_load;
}

#if(ARILOTX_HWREV == ARILOTX_HW1)
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
uint32_t GetOverflowTimer8(void)
{
	uint32_t aux;
	aux = timer8_overflow_count;
	return aux;
}
#else
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             GetOverflowTimer3()                                        */
/*                                                                              */
/* DESCRIPTION:      Get number of periods of Timer 3 counted                   */
/*                                                                              */
/* PARAMETERS:       Out : Number of ticks                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint32_t GetOverflowTimer3(void)
{
	uint32_t aux;
	aux = timer3_overflow_count;
	return aux;
}
#endif

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(SysTick)                                               */
/*                                                                              */
/* DESCRIPTION:      STM32 SysTick Handler                                      */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
	/* Interrupt each System Interrupt (10ms) */
	/* Time event */
	EventTime time_events;
	EventTime *time_events_p = &time_events;
	uint32_t aux = system_tick_count;
	aux += 1;
	system_tick_count = aux;

	/* Calculate ISR Load */
	float32_t aux_time_start = GetTicks() + (1000 - ((float32_t)GetMicros())) / ((float32_t)1000);
	float32_t aux_time_end;
	/* Event Calls */

	/* 10ms Event */
	if ((aux % 10) == 0)
	{
		time_events_p->event.sig = TSLOT_10ms;
		sendq_Tevent((eventtype *) time_events_p);
	};

	/* 80ms Event */
	if ((aux % 80) == 0)
	{
		time_events_p->event.sig = TSLOT_80ms;
		sendq_Tevent((eventtype *) time_events_p);

		/* Call to Error Handler */
		ErrorHandler_llsm_main();
	};

	/* Call to RF Handler every ms */
	RFHandler_llsm_main();

	/* Report Event Processing Times */
	aux_time_end = GetTicks() + (1000 - ((float32_t)GetMicros())) / ((float32_t)1000);
	if(aux_time_end >= aux_time_start)
	{
		isr_time[SYSTICK_ISR] += aux_time_end - aux_time_start;
	}

	/* Calculate CPU Load */
	if((aux % CPU_LOAD_TWIN) == 0) cpu_load_calc();
}

#if(ARILOTX_HWREV == ARILOTX_HW1)
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(Timer3 Overflow)                                       */
/*                                                                              */
/* DESCRIPTION:      STM32 Timer 3 Overflow Handler                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TIM3_IRQHandler(void)
#else
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(Timer1 Overflow)                                       */
/*                                                                              */
/* DESCRIPTION:      STM32 Timer 1 Overflow Handler                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TIM1_UP_IRQHandler(void)
#endif
{
	/* Calculate ISR Load */
	float32_t aux_time_start = GetTicks() + (1000 - ((float32_t)GetMicros())) / ((float32_t)1000);
	float32_t aux_time_end;

	#if(ARILOTX_HWREV == ARILOTX_HW1)
	if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
	{
		TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
		timer3_overflow_count++;
	#else
	if(TIM1->SR & TIM_SR_UIF) // if UIF flag is set
	{
		TIM1->SR &= ~TIM_SR_UIF; // clear UIF flag
		timer1_overflow_count++;
	#endif

		/* Call to SafeLink Handler every 4ms */
		SafeLinkHandler_llsm_main();
	}

	/* Report Event Processing Times */
	aux_time_end = GetTicks() + (1000 - ((float32_t)GetMicros())) / ((float32_t)1000);
	if(aux_time_end >= aux_time_start)
	{
		isr_time[FOUR_MS_ISR] += aux_time_end - aux_time_start;
	}
}

#if(ARILOTX_HWREV == ARILOTX_HW1)
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(Timer 8 capture)                                       */
/*                                                                              */
/* DESCRIPTION:      Timer 8 capture Interrupt Handler                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TIM8_CC_IRQHandler(void)
{
    /* capture timer */
    if(TIM8->SR & TIM_IT_CC2)
    {
        /* Clear pending bit */
        TIM8->SR = (unsigned short)~TIM_FLAG_CC2;
        /* Increment overflow counter */
        timer8_overflow_count++;
    }
    /* capture timer */
    if(TIM8->SR & TIM_IT_CC1)
    {
        /* Clear pending bit */
        TIM8->SR = (unsigned short)~TIM_FLAG_CC1;
        /* Interrupt each change of flank ICP1 pin */
        INT_events_p->event.sig = ICP_1;
        INT_events_p->timer_val = (uint16_t)(TIM8->CCR1);
        INT_events_p->timer_cycle = timer8_overflow_count;
        INT_events_p->flank = (uint8_t) LOW_FL;
        sendq_Ievent((eventtype *) INT_events_p);
        /* Increment counter */
        timer8_capture_count++;
    }
}
#else
/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   ISR(Timer 3 interrupt)                                     */
/*                                                                              */
/* DESCRIPTION:      STM32 Timer 3 Interrupt Handler                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TIM3_IRQHandler(void)
{
    /* capture timer */
    if(TIM3->SR & TIM_IT_CC1)
    {
        /* Clear pending bit */
        TIM3->SR = (unsigned short)~TIM_FLAG_CC1;
        /* Interrupt each change of flank ICP1 pin */
        INT_events_p->event.sig = ICP_1;
        INT_events_p->timer_val = (uint16_t)(TIM3->CCR1);
        INT_events_p->timer_cycle = timer3_overflow_count;
        INT_events_p->flank = (uint8_t) LOW_FL;
        sendq_Ievent((eventtype *) INT_events_p);
        /* Increment counter */
        timer3_capture_count++;
    }
    /* capture timer */
    if(TIM3->SR & TIM_IT_Update)
    {
        /* Clear pending bit */
        TIM3->SR = (unsigned short)~TIM_IT_Update;
        /* Increment overflow counter */
        timer3_overflow_count++;
    }
}
#endif

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   DelayUs                                                    */
/*                                                                              */
/* DESCRIPTION:      STM32 uS Delay                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void DelayUs(uint32_t delay_us)
{
	uint32_t nb_loop;
	nb_loop = (((SystemCoreClock / 1000000)/4)*delay_us)+1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
	__asm volatile(
	"1: " "\n\t"
	" nop " "\n\t"
	" subs.w %0, %0, #1 " "\n\t"
	" bne 1b " "\n\t"
	: "=r" (nb_loop)
	: "0"(nb_loop)
	: "r3"
	);
}
