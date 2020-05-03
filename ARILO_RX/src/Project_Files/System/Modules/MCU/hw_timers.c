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
#include "../../Low_Level_State_Machines/CH_Generation/CHGeneration_llsm.h"
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
uint16_t CCR1_Val = INIT_CR, CCR2_Val = INIT_CR, CCR3_Val = INIT_CR, CCR4_Val = INIT_CR;
#if(ARILORX_HWREV == ARILORX_HW1)
volatile uint32_t timer3_overflow_count = 0;
#else
volatile uint32_t timer1_overflow_count = 0;
#endif
/* INT event */
EventINT INT_events;
EventINT *INT_events_p = &INT_events;
volatile uint32_t timer8_capture_count = 0;
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
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* -----------------------------------------------------------------------
	TIM2 Configuration: generate 4 PWM signals with 4 different duty cycles:
	The TIM2CLK frequency is set to SystemCoreClock (Hz), to get TIM2 counter
	clock at 24 MHz the Prescaler is computed as following:
	 - Prescaler = (TIM2CLK / TIM2 counter clock) - 1
	SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
	and Connectivity line devices and to 24 MHz for Low-Density Value line and
	Medium-Density Value line devices

	The TIM2 is running at 36 KHz: TIM2 Frequency = TIM2 counter clock/(ARR + 1)
												  = 24 MHz / 666 = 36 KHz
	TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR)* 100 = 50%
	TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR)* 100 = 37.5%
	TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR)* 100 = 25%
	TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR)* 100 = 12.5%
	----------------------------------------------------------------------- */
	/* Compute the prescaler value */
	/* Freq. 3MHz */
//	PrescalerValue = (uint16_t) (SystemCoreClock / 3000000) - 1;
//	/* Time base configuration */
//	TIM_TimeBaseStructure.TIM_Period = 59999;
//	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	//TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    #if(ARILORX_HWREV != ARILORX_HW1)
	//TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
    #endif

	/* PWM1 Mode configuration: Channel1 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
//
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
//
//    #if(ARILORX_HWREV != ARILORX_HW1)
//    //TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//    //TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
//    #endif
//
//	/* PWM1 Mode configuration: Channel2 */
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
//
//	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
//
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);
//
//    #if(ARILORX_HWREV != ARILORX_HW1)
//	//TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	//TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
//    #endif
//
//	/* PWM1 Mode configuration: Channel3 */
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
//
//	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
//
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);
//
//    #if(ARILORX_HWREV != ARILORX_HW1)
//	/* Additional config. for Timer 8 */
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
//    //TIM_OC3Init(TIM8, &TIM_OCInitStructure);
//    //TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Disable);
//    #endif
//
//	/* PWM1 Mode configuration: Channel4 */
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
//
//	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
//
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);
//
//    #if(ARILORX_HWREV != ARILORX_HW1)
//    TIM_OC4Init(TIM8, &TIM_OCInitStructure);
//    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Disable);
//    #endif
//
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
//	TIM_ARRPreloadConfig(TIM4, ENABLE);
//    #if(ARILORX_HWREV != ARILORX_HW1)
//	//TIM_ARRPreloadConfig(TIM3, ENABLE);
//	TIM_ARRPreloadConfig(TIM8, ENABLE);
//    #endif

	/* TIM enable counter */
	//TIM_Cmd(TIM2, ENABLE);
	//TIM_Cmd(TIM4, ENABLE);
    #if(ARILORX_HWREV != ARILORX_HW1)
	//TIM_Cmd(TIM3, ENABLE);
	//TIM_Cmd(TIM8, ENABLE);
    #endif

	/* Activate TIM2 interrupt when Updating */
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    #if(ARILORX_HWREV == ARILORX_HW1)
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

	/* Watchdog Timer */

	/* WWDG clock counter = (PCLK1 (72MHz)/4096)/8 = 2197Hz (~455us)  */
	//WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 110; WWDG counter should be refreshed only when the counter
	 is below 110 (and greater than 64) otherwise a reset will be generated */
	//WWDG_SetWindowValue(127);

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
/* PARAMETERS:       In : Channel                                               */
/*                        pulse_value in Timer units                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SetChannel(uint8_t channel, uint16_t pulse_value)
{
	switch(channel)
	{
		case 0:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare4(TIM2, pulse_value);
            #else
			//TIM_SetCompare1(TIM3, pulse_value);
            #endif
		break;
		case 1:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare3(TIM2, pulse_value);
            #else
			//TIM_SetCompare2(TIM3, pulse_value);
            #endif
		break;
		case 2:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare2(TIM2, pulse_value);
            #else
			//TIM_SetCompare1(TIM4, pulse_value);
            #endif
		break;
		case 3:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare1(TIM2, pulse_value);
            #else
			//TIM_SetCompare2(TIM4, pulse_value);
            #endif
		break;
		case 4:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare1(TIM4, pulse_value);
            #else
			//TIM_SetCompare3(TIM4, pulse_value);
            #endif
		break;
		case 5:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare2(TIM4, pulse_value);
            #else
			//TIM_SetCompare4(TIM4, pulse_value);
            #endif
		break;
		case 6:
            #if(ARILORX_HWREV == ARILORX_HW1)
			//TIM_SetCompare3(TIM4, pulse_value);
            #else
			//TIM_SetCompare1(TIM2, pulse_value);
            #endif
		break;
		case 7:
            #if(ARILORX_HWREV == ARILORX_HW1)
			TIM_SetCompare4(TIM4, pulse_value);
            #else
			//TIM_SetCompare2(TIM2, pulse_value);
            #endif
		break;
        #if(ARILORX_HWREV != ARILORX_HW1)
		case 8:
			//TIM_SetCompare3(TIM2, pulse_value);
		break;
		case 9:
			//TIM_SetCompare4(TIM8, pulse_value);
		break;
		case 10:
			//TIM_SetCompare4(TIM2, pulse_value);
		break;
		case 11:
			//TIM_SetCompare3(TIM8, pulse_value);
		break;
        #endif
		default:
		break;
	}
};

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
	/* Interrupt each System Interrupt (1ms) */
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

	if((aux & 40) == 0)
	{
		SafeLinkHandler_llsm_main();
	}

	/* 80ms Event */
	if ((aux % 80) == 0)
	{
		time_events_p->event.sig = TSLOT_80ms;
		sendq_Tevent((eventtype *) time_events_p);

		/* Call to Error Handler */
		ErrorHandler_llsm_main();
	};

	/* Call to RF Handler every ms */
	//RFHandler_llsm_main();

	/* Report Event Processing Times */
	aux_time_end = GetTicks() + (1000 - ((float32_t)GetMicros())) / ((float32_t)1000);
	if(aux_time_end >= aux_time_start)
	{
		isr_time[SYSTICK_ISR] += aux_time_end - aux_time_start;
	}

	/* Calculate CPU Load */
	if((aux % CPU_LOAD_TWIN) == 0) cpu_load_calc();
}

#if(ARILORX_HWREV == ARILORX_HW1)
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

    #if(ARILORX_HWREV == ARILORX_HW1)
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
		//SafeLinkHandler_llsm_main();
	}

	/* Report Event Processing Times */
	aux_time_end = GetTicks() + (1000 - ((float32_t)GetMicros())) / ((float32_t)1000);
	if(aux_time_end >= aux_time_start)
	{
		isr_time[FOUR_MS_ISR] += aux_time_end - aux_time_start;
	}
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* INTERRUPT NAME:   TIM2_IRQHandler (Timer Update)                             */
/*                                                                              */
/* DESCRIPTION:      STM32 TIM2 IRQ routine                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		//CHGeneration_llsm_main();
	}
};

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
