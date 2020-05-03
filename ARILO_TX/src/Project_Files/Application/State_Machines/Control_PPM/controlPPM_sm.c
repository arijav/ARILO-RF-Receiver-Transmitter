/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             controlPPM_sm.c                                            */
/*                                                                              */
/* DESCRIPTION:      State machine that processes control events                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include "controlPPM_sm.h"
#include "config_switches.h"
#include "../../../System/HAL/hw_general.h"

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
/* NAME:            init_controlPPM_sm                                          */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_controlPPM_sm(void)
{
    uint8_t aux;
    /* Initial States */
    control_sm.statem.state = WAITING;
    /* Init Max Ch. Length */
    control_sm.received_ch = 0;
    control_sm.received_ch_old = 0;
    control_sm.timer_val = 0;
    control_sm.timer_cycle = 0;
    control_sm.flag_ppm_corrupted = PPM_OK;
    for(aux = 0; aux < MAX_CHANNELS; aux++)
    {
        control_sm.channel[aux] = 0;
    }
    #if(CTR_SERVO_TEST == SERVO_TEST_ON)
    control_sm.test_value = (2 * TEST_MIN_US);
    control_sm.test_dir = TEST_DIR_UP;
    control_sm.tested_ch = MAX_CHANNELS;
    #endif
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            controlPPM_sm_main                                          */
/*                                                                              */
/* DESCRIPTION:     Main function from the Control State Machine                */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void controlPPM_sm_main(eventtype const *event)
{
    uint32_t loc_timecalc;
    uint16_t loc_timeval;
    uint32_t loc_timecycle;
    int16_t  loc_timecyclediff;
    uint8_t  loc_aux;
    uint8_t  loc_rxch;

    switch (control_sm.statem.state)
    {
        case WAITING:
            switch(event->sig)
            {
                case ENTRY_SIG:
                    /* Entry call code */
                    control_sm.received_ch = 0;
                    control_sm.received_ch_old = 0;
                break;
                case ICP_1:
                    /* Actions */
                    control_sm.timer_val = ((EventINT const *) event)->timer_val;
                    control_sm.timer_cycle = ((EventINT const *) event)->timer_cycle;
                    control_sm.statem.state = SYNCHRONIZING;
                    controlPPM_sm_main(EXIT_STATE);
                break;
                default:
                break;
            };
        break;
        case SYNCHRONIZING:
            switch(event->sig)
            {
                case ENTRY_SIG:
                    /* Entry call code */
                break;
                case ICP_1:
                    /* Actions */
                    loc_timeval = ((EventINT const *) event)->timer_val;
                    loc_timecycle = ((EventINT const *) event)->timer_cycle;
                    loc_timecalc = GET_TIME_DIFF_FLANK(control_sm.timer_val, loc_timeval, loc_timecycle - control_sm.timer_cycle);
                    /* Convert pulse to 0.5us resolution */
                    loc_timecalc = (uint16_t)(loc_timecalc / RX_RESOLUTION_CONV);
                    control_sm.timer_val = loc_timeval;
                    control_sm.timer_cycle = loc_timecycle;

                    /* If pulse width is equal or longer than the minimum SYNC pulse width and shorter than the max */
                    if(   (loc_timecalc >= SYNCRO_PULSE_MIN)
                        &&(loc_timecalc <= SYNCRO_PULSE_MAX)
                      )
                    {
                        control_sm.statem.state = CHANNEL_SURVEY;
                        controlPPM_sm_main(EXIT_STATE);
                    }
                break;
                default:
                break;
            };
        break;
        case CHANNEL_SURVEY:
            switch(event->sig)
            {
                case ENTRY_SIG:
                    /* Entry call code */
                break;
                case ICP_1:
                    /* Actions: */
                    loc_timeval = ((EventINT const *) event)->timer_val;
                    loc_timecycle = ((EventINT const *) event)->timer_cycle;
                    if(loc_timecycle >= control_sm.timer_cycle)
                    {
                        loc_timecyclediff = loc_timecycle - control_sm.timer_cycle;
                    }
                    else
                    {
                        loc_timecyclediff = (UINT32_MAX - control_sm.timer_cycle) + loc_timecycle;
                    }
                    loc_timecalc = GET_TIME_DIFF_FLANK(control_sm.timer_val, loc_timeval, loc_timecyclediff);
                    /* Convert pulse to 0.5us resolution */
                    loc_timecalc = (uint16_t)(loc_timecalc / RX_RESOLUTION_CONV);
                    control_sm.timer_val = loc_timeval;
                    control_sm.timer_cycle = loc_timecycle;

                    loc_rxch = control_sm.received_ch;
                    /* If pulse width is lower than the minimum SYNC pulse width */
                    if((loc_timecalc <= SYNCRO_PULSE_MIN) && (loc_rxch < MAX_CHANNELS))
                    {
                        /* Check if channel is valid */
                        if(  (loc_timecalc > SERVO_MAX_VAL)
                           ||(loc_timecalc < SERVO_MIN_VAL)  )
                        {
                            /* Keep the last channel value which was ok */
                            control_sm.channel[loc_rxch] = control_sm.channel_last_ok[loc_rxch];
                            /* Report channel error */
                            ERR_HANDLER_IF(CONTROL_ERROR_PPM_CHLENGTH ,ERROR_ON);
                            /* Report PPM corrupted */
                            control_sm.flag_ppm_corrupted = PPM_NOK;
                        }
                        else
                        {
                            /* Update channel value */
                            control_sm.channel[loc_rxch] = loc_timecalc;
                        }
                        control_sm.received_ch = (loc_rxch + 1);
                    }
                    else if(  (loc_timecalc > SYNCRO_PULSE_MIN)
                            &&(loc_timecalc <= SYNCRO_PULSE_MAX)  )
                    {
                        /* Change State to */
                        control_sm.statem.state = PPM_TRAIN_PROCESS;
                        controlPPM_sm_main(EXIT_STATE);
                    }
                    else
                    {
                        /* Report PPM Error */
                        ERR_HANDLER_IF(CONTROL_ERROR_PPM_SYNC ,ERROR_ON);
                        /* Change State to */
                        control_sm.statem.state = WAITING;
                        controlPPM_sm_main(EXIT_STATE);
                    }
                break;
                default:
                break;
            };
        break;
        case PPM_TRAIN_PROCESS:
            switch(event->sig)
            {
                /* Entry call code */
                case ENTRY_SIG:
                    /* Entry call code */
                    loc_rxch = control_sm.received_ch;
                    /* If it is the first PPM train */
                    /* AND PPM is not corrupted */
                    if(  (control_sm.received_ch_old == 0)
                       &&(control_sm.flag_ppm_corrupted == PPM_OK)  )
                    {
                        control_sm.received_ch_old = loc_rxch;
                        #if(CTR_SERVO_TEST == SERVO_TEST_OFF)
                        setup_channels();
                        #endif
                        /* Save last working channels into array */
                        for(loc_aux = 0; loc_aux < loc_rxch; loc_aux++)
                        {
                            control_sm.channel_last_ok[loc_aux] = control_sm.channel[loc_aux];
                        }
                    }
                    /* Otherwise if the ch. number has not changed */
                    else if(control_sm.received_ch_old == loc_rxch)
                    {
                        #if(CTR_SERVO_TEST == SERVO_TEST_OFF)
                        setup_channels();
                        #endif
                        /* Save last working channels into array */
                        for(loc_aux = 0; loc_aux < loc_rxch; loc_aux++)
                        {
                            control_sm.channel_last_ok[loc_aux] = control_sm.channel[loc_aux];
                        }
                    }
                    else
                    {
                        control_sm.received_ch_old = loc_rxch;
                        /* Report channel number error */
                        ERR_HANDLER_IF(CONTROL_ERROR_PPM_CHNUMCHANGE, ERROR_ON);
                    }
                    control_sm.received_ch = 0;
                    /* Change State to */
                    control_sm.statem.state = CHANNEL_SURVEY;
                    controlPPM_sm_main(EXIT_STATE);
                break;
                default:
                break;
            };
        break;
        default:
        break;
    }
    #if(CTR_SERVO_TEST == SERVO_TEST_ON)
    /* Test servos from 1000us to 2000us and down to 1000us */
    uint8_t aux;
    if(event->sig == TSLOT_10ms)
    {
        if(control_sm.test_dir == TEST_DIR_UP)
        {
            /* Consider 0.5us channel resolution and correction is done each 10ms */
            control_sm.test_value += ((SERVO_GRADIENT / 100) * 2);
            if(control_sm.test_value > (2 * TEST_MAX_US))
            {
                /* If it reaches the top, go back in downwards direction */
                control_sm.test_value = (2 * TEST_MAX_US) - (control_sm.test_value - (2 * TEST_MAX_US));
                control_sm.test_dir = TEST_DIR_DOWN;
            }
        }
        else
        {
            /* Consider 0.5us channel resolution and correction is done each 10us */
            control_sm.test_value -= ((SERVO_GRADIENT / 100) * 2);
            if(control_sm.test_value < (2 * TEST_MIN_US))
            {
                /* If it reaches the bottom, go back in downwards direction */
                control_sm.test_value = (2 * TEST_MIN_US) + ((2 * TEST_MIN_US) - control_sm.test_value);
                control_sm.test_dir = TEST_DIR_UP;
            }
        }
        for(aux = 0; aux < MAX_CHANNELS; aux ++)
        {
            control_sm.channel[aux] = control_sm.test_value;
        }
        control_sm.tested_ch = MAX_CHANNELS;
        setup_channels();
    }
    #endif
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            setup_channels                                              */
/*                                                                              */
/* DESCRIPTION:     Sends the channels to the Communications State Machine      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void setup_channels(void)
{
    uint8_t i;
    uint8_t n_channels;
    /* Auxiliary Signal */
    SignalSM loc_signal;
    SignalSM *loc_signal_p = &loc_signal;

    #if(CTR_SERVO_TEST == SERVO_TEST_OFF)
    n_channels = control_sm.received_ch;
    #elif(CTR_SERVO_TEST == SERVO_TEST_ON)
    n_channels = control_sm.tested_ch;
    #endif

    /* If channel number is plausible */
    if(n_channels <= MAX_CHANNELS)
    {
        /* Send Channel Info to the Communications SM */
        loc_signal_p->SM_sender = CONTROLPPM_SM;
        loc_signal_p->SM_receiver = COMMUNICATIONS_SM;
        loc_signal_p->event.sig = CTR_NCHANNELS_SIG;
        loc_signal_p->param = (int32_t) n_channels;

        /* Send Signal */
        sendq_Sevent((eventtype *) loc_signal_p);

        for(i = 0; i < n_channels; i++)
        {
            /* Fill appropriate Data */
            loc_signal_p->event.sig = CTR_CH0_SIG + i;
            loc_signal_p->param = (int32_t) control_sm.channel[i];
            /* Send Signal */
            sendq_Sevent((eventtype *) loc_signal_p);
        }
    }
    else
    {
        /* Report Ch. Num. Error */
        ERR_HANDLER_IF(CONTROL_ERROR_PPM_CHNUM ,ERROR_ON);
    }
};

/*
 * controlPPM_sm.c
 *
 *  Created on: 03/06/2012
 *      Author: arigita
 */


