/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             events_manager.c                                           */
/*                                                                              */
/* DESCRIPTION:      Event/Signal listing which serve as input to the           */
/*                   State Machines                                             */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include <stdint.h>
#include "event_manager.h"
#include "state_machine_fw.h"
#include "../System/HAL/hw_general.h"

/*------------------------------------------------------------------------------*/
/* Global Variables                                                             */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
/* Event queues */
volatile EventINTQueueCtr EventINTQueueCtr1 ALIGN_4;
volatile EventTimeQueueCtr EventTimeQueueCtr1;
volatile SignalSMQueueCtr SignalSMQueueCtr1;
volatile uint8_t queue_next = 0;

/*------------------------------------------------------------------------------*/
/* Local Function Prototypes                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Functions                                                                    */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             init_evm                                                   */
/*                                                                              */
/* DESCRIPTION:      Init. of ev manager                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void init_evm(void)
{
	/* Signal to launch the State Machines */
	SignalSM init_signal;
	SignalSM *init_signal_p = &init_signal;
	uint8_t aux;

	/* Init. Queues */
	EventINTQueueCtr1.queue.qfront = 0;
	EventINTQueueCtr1.queue.qend = 0;
	EventINTQueueCtr1.queue.qeventnumber = 0;
	EventTimeQueueCtr1.queue.qfront = 0;
	EventTimeQueueCtr1.queue.qend = 0;
	EventTimeQueueCtr1.queue.qeventnumber = 0;
	SignalSMQueueCtr1.queue.qfront = 0;
	SignalSMQueueCtr1.queue.qend = 0;
	SignalSMQueueCtr1.queue.qeventnumber = 0;

	/* Init Time Arrays */
	for(aux = 0; aux < MAX_SIG; aux++) event_times_min[aux] = UINT32_MAX;

	/* Init. Auxiliary Entry Event */
	entry_ev.sig = ENTRY_SIG;

	/* Launch the State Machines */
	init_signal_p->event.sig = GOALIVE_SIG;
	init_signal_p->SM_receiver = BROADCAST_SM;
	/* Send Signal */
	sendq_Sevent((eventtype *) init_signal_p);
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             getq_event                                                 */
/*                                                                              */
/* DESCRIPTION:      Get next event from the first active queue                 */
/*                                                                              */
/* PARAMETERS:       Out: Pointer to event in the queue                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void getq_event(eventtype **event)
{
	uint8_t event_catch = 0;
	eventtype *aux_ev = 0;
	while (event_catch == 0)
	{
		queue_next = ((queue_next + 1) % MAX_QUEUE);

		switch(queue_next)
		{
			case INTQ:
				if (EventINTQueueCtr1.queue.qeventnumber != 0)
				{
					aux_ev = (eventtype *)(&queue_pinINT[EventINTQueueCtr1.queue.qfront]);
					event_catch = 1;
				}
			break;
			case TIMEQ:
				if (EventTimeQueueCtr1.queue.qeventnumber != 0)
				{
					aux_ev = (eventtype *)(&queue_Time[EventTimeQueueCtr1.queue.qfront]);
					event_catch = 1;
				}
			break;
			case SIGNALQ:
				if (SignalSMQueueCtr1.queue.qeventnumber != 0)
				{
					aux_ev = (eventtype *)(&queue_signalSM[SignalSMQueueCtr1.queue.qfront]);
					event_catch = 1;
				}
			break;
			default:
				/* Error, unknown queue */
			break;
		}
	};
	*event = aux_ev;
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             dispatch_event                                             */
/*                                                                              */
/* DESCRIPTION:      Dispatch event to state machines                           */
/*                                                                              */
/* PARAMETERS:       In: Pointer to event                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void dispatch_event(eventtype const *event)
{
	/* Time Measurement */
	float32_t aux_time_start = GET_MILLIS + (1000 - ((float32_t)GET_MICROS)) / ((float32_t)1000);
	float32_t aux_time_end;
	switch(event->sig)
	{
		case ICP_1:
			/* Invoke State Machines that take this event */
			
		break;
		case INT_0:
			/* Invoke State Machines that take this event */
			
		break;
		case INT_1:
			/* Invoke State Machines that take this event */
			
		break;
		case TSLOT_10ms:
			/* Invoke State Machines that take this event */
			//safety_sm_main(event);
			//telemetryHUB_sm_main(event);
			communications_sm_main(event);
			//controlCH_sm_main(event);
		break;
		case TSLOT_80ms:
			/* Invoke State Machines that take this event */
			//telemetryHUB_sm_main(event);
			communications_sm_main(event);
			//cmdInput_sm_main(event);
		break;
	    /* Signals */
		case TEL_TEMP_SIG:
		case TEL_BATT_SIG:
		case TEL_RPM_SIG:
		case TEL_DIST_SIG:
		case TEL_CURR_SIG:
		case TEL_REVSENSE_SIG:
		case TEL_IMUACELX_SIG:
		case TEL_IMUACELY_SIG:
		case TEL_IMUACELZ_SIG:
		case TEL_IMUGYROX_SIG:
		case TEL_IMUGYROY_SIG:
		case TEL_IMUGYROZ_SIG:
		case CTR_NCHANNELS_SIG:
		case CTR_CH0_SIG:
		case CTR_CH1_SIG:
		case CTR_CH2_SIG:
		case CTR_CH3_SIG:
		case CTR_CH4_SIG:
		case CTR_CH5_SIG:
		case CTR_CH6_SIG:
		case CTR_CH7_SIG:
		case CTR_CH8_SIG:
		case CTR_CH9_SIG:
		case CTR_CH10_SIG:
		case CTR_CH11_SIG:
		case PBUTTOM_BINDREQ_SIG:
		case PBUTTOM_FSSETREQ_SIG:
		case CTR_COMMANDTX_SIG:
		case CTR_COMMANDRX_SIG:
		case CTR_ADDRESS_SIG:
		case INFO_NO_RF_SIG:
		case INFO_RF_SIG:
		case REQ_INFO_RF_SIG:
		case EMERGENCY_SIG:
		case DEBUG_SIG_1:
		case DEBUG_SIG_2:
		case DEBUG_SIG_3:
		case GOALIVE_SIG:
			/* Invoke State Machines that take this event */
			switch(((SignalSM const *)event)->SM_receiver)
			{
				case CONTROLCH_SM:
					//controlCH_sm_main(event);
				break;
				case COMMUNICATIONS_SM:
					communications_sm_main(event);
			    break;
				case TELEMETRYHUB_SM:
					//telemetryHUB_sm_main(event);
				break;
				case CMDINPUT_SM:
					//cmdInput_sm_main(event);
				break;
				case BROADCAST_SM:
					//safety_sm_main(event);
					//controlCH_sm_main(event);
					communications_sm_main(event);
					//telemetryHUB_sm_main(event);
					//cmdInput_sm_main(event);
				break;
				default:
				break;
			}
		break;
		default:
			/* Error, event unknown */
			ERR_HANDLER_IF(EVMNG_ERROR_SIG_UNKNOWN ,ERROR_ON);
		break;
	};
	/* Report Event Processing Times */
	aux_time_end = GET_MILLIS + (1000 - ((float32_t)GET_MICROS)) / ((float32_t)1000);
	if(aux_time_end >= aux_time_start)
	{
		event_times_act[event->sig] = aux_time_end - aux_time_start;
	}
	event_times_acc[event->sig] = event_times_acc[event->sig] + event_times_act[event->sig];
	if(event_times_act[event->sig] > event_times_max[event->sig]) event_times_max[event->sig] = event_times_act[event->sig];
	if(event_times_act[event->sig] < event_times_min[event->sig]) event_times_min[event->sig] = event_times_act[event->sig];
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             discard_event                                              */
/*                                                                              */
/* DESCRIPTION:      Discard processed event from queue                         */
/*                                                                              */
/* PARAMETERS:       In: Pointer to event                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
/* TODO: Receive pointer to event to be discarded and process it for diagnostics purposes  */
void discard_event(void)//eventtype **event)
{
	uint8_t aux_q;

	switch(queue_next)
	{
	case INTQ:
		aux_q = EventINTQueueCtr1.queue.qeventnumber;
		if(aux_q > 0)
		{
			/* Deactivate Interrupts */
			DISABLE_IRQ;
			EventINTQueueCtr1.queue.qfront = ((EventINTQueueCtr1.queue.qfront) + 1)%N_MAX_EVINT;
			EventINTQueueCtr1.queue.qeventnumber--;
			/* Reactivate Interrupts */
			ENABLE_IRQ;
		}
	break;
	case TIMEQ:
		aux_q = EventTimeQueueCtr1.queue.qeventnumber;
		if(aux_q > 0)
		{
			/* Deactivate Interrupts */
			DISABLE_IRQ;
			EventTimeQueueCtr1.queue.qfront = ((EventTimeQueueCtr1.queue.qfront) + 1)%N_MAX_EVTIME;
			EventTimeQueueCtr1.queue.qeventnumber--;
			/* Reactivate Interrupts */
			ENABLE_IRQ;
		}
	break;
	case SIGNALQ:
		aux_q = SignalSMQueueCtr1.queue.qeventnumber;
		if(aux_q > 0)
		{
			SignalSMQueueCtr1.queue.qfront = ((SignalSMQueueCtr1.queue.qfront) + 1)%N_MAX_SIGNAL;
			SignalSMQueueCtr1.queue.qeventnumber--;
		}
	break;
	default:
		/* Error, unknown queue */
	break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             sendq_Ievent                                               */
/*                                                                              */
/* DESCRIPTION:      Send INT event to its queue                                */
/*                                                                              */
/* PARAMETERS:       In: Event Pointer to Signal                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void sendq_Ievent(eventtype const *event)
{
	uint8_t aux_q = 0;
	switch(event->sig)
	{
		case ICP_1:
		case INT_0:
		case INT_1:
			/* Insert in INT Event Queue */
			if(EventINTQueueCtr1.queue.qeventnumber < N_MAX_EVINT)
			{
				aux_q = EventINTQueueCtr1.queue.qend;
				queue_pinINT[aux_q] = *((EventINT *) event);
				EventINTQueueCtr1.queue.qend = ((aux_q + 1) % N_MAX_EVINT);
				EventINTQueueCtr1.queue.qeventnumber++;
			}
		break;
		default:
			/* Error, signal does not belong to this queue */
			ERR_HANDLER_IF(EVMNG_ERROR_FALSEQUEUE_SIG ,ERROR_ON);
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             sendq_Tevent                                               */
/*                                                                              */
/* DESCRIPTION:      Send Time event to its queue                               */
/*                                                                              */
/* PARAMETERS:       In: Event Pointer to Signal                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void sendq_Tevent(eventtype const *event)
{
	uint8_t aux_q = 0;
	switch(event->sig)
	{
		case TSLOT_10ms:
		case TSLOT_80ms:
			/* Insert in Time Queue */
			if(EventTimeQueueCtr1.queue.qeventnumber < N_MAX_EVTIME)
			{
				aux_q = EventTimeQueueCtr1.queue.qend;
				queue_Time[aux_q] = *((EventTime *) event);
				EventTimeQueueCtr1.queue.qend = ((aux_q + 1) % N_MAX_EVTIME);
				EventTimeQueueCtr1.queue.qeventnumber++;
			}
			else
			{
				/* Error, Queue Overflow */
				ERR_HANDLER_IF(EVMNG_ERROR_QUEUE_OVERFLOW ,ERROR_ON);
			}
		break;
		default:
			/* Error, signal does not belong to this queue */
			ERR_HANDLER_IF(EVMNG_ERROR_FALSEQUEUE_SIG ,ERROR_ON);
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             sendq_Sevent                                               */
/*                                                                              */
/* DESCRIPTION:      Sends Signal event to its queue                            */
/*                                                                              */
/* PARAMETERS:       In: Event Pointer to Signal                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void sendq_Sevent(eventtype const *event)
{
	uint8_t aux_q = 0;
	switch(event->sig)
	{
		/* Signals */
		case TEL_TEMP_SIG:
		case TEL_BATT_SIG:
		case TEL_RPM_SIG:
		case TEL_DIST_SIG:
		case TEL_CURR_SIG:
		case TEL_REVSENSE_SIG:
		case TEL_IMUACELX_SIG:
		case TEL_IMUACELY_SIG:
		case TEL_IMUACELZ_SIG:
		case TEL_IMUGYROX_SIG:
		case TEL_IMUGYROY_SIG:
		case TEL_IMUGYROZ_SIG:
		case CTR_NCHANNELS_SIG:
		case CTR_CH0_SIG:
		case CTR_CH1_SIG:
		case CTR_CH2_SIG:
		case CTR_CH3_SIG:
		case CTR_CH4_SIG:
		case CTR_CH5_SIG:
		case CTR_CH6_SIG:
		case CTR_CH7_SIG:
		case CTR_CH8_SIG:
		case CTR_CH9_SIG:
		case CTR_CH10_SIG:
		case CTR_CH11_SIG:
		case PBUTTOM_BINDREQ_SIG:
		case PBUTTOM_FSSETREQ_SIG:
		case CTR_COMMANDTX_SIG:
		case CTR_COMMANDRX_SIG:
		case CTR_ADDRESS_SIG:
		case INFO_NO_RF_SIG:
		case INFO_RF_SIG:
		case REQ_INFO_RF_SIG:
		case EMERGENCY_SIG:
		case DEBUG_SIG_1:
		case DEBUG_SIG_2:
		case DEBUG_SIG_3:
		case GOALIVE_SIG:
			/* Insert in Signal Queue */
			if(SignalSMQueueCtr1.queue.qeventnumber < N_MAX_SIGNAL)
			{
				/* Insert in Input Queue */
				aux_q = SignalSMQueueCtr1.queue.qend;
				queue_signalSM[aux_q] = *((SignalSM *) event);
				SignalSMQueueCtr1.queue.qend = (aux_q + 1) % N_MAX_SIGNAL;
				SignalSMQueueCtr1.queue.qeventnumber++;
			}
			else
			{
				/* Error, Queue Overflow */
				ERR_HANDLER_IF(EVMNG_ERROR_QUEUE_OVERFLOW ,ERROR_ON);
			}
		break;
		default:
			/* Error, signal does not belong to this queue */
			ERR_HANDLER_IF(EVMNG_ERROR_FALSEQUEUE_SIG ,ERROR_ON);
		break;
	};
};

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            get_event_dur                                               */
/*                                                                              */
/* DESCRIPTION:     Gets accumulated event durations                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
float32_t get_event_dur(void)
{
	uint8_t aux;
	float32_t duration = 0;
	for(aux = 0; aux < MAX_SIG; aux++) duration += event_times_acc[aux];
	reset_event_dur();
	return duration;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            reset_event_dur                                             */
/*                                                                              */
/* DESCRIPTION:     Resets accumulated event durations                          */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void reset_event_dur(void)
{
	uint8_t aux;
	for(aux = 0; aux < MAX_SIG; aux++) event_times_acc[aux] = 0;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            event_processor_main                                        */
/*                                                                              */
/* DESCRIPTION:     Main function from the event manager. Processes every event */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void event_processor_main(void)
{
	eventtype *event_pp = 0;
	init_evm();
	while(1)
	{
		while(N_QUEUED_EVENTS != 0)
		{
			getq_event(&event_pp);
			dispatch_event(event_pp);
			discard_event();//&event_pp);
		};
		/* Enter in IDLE (Low Power) Mode */
		/* TODO: Implement low power mode function */
	};
};

/*
 * events.c
 *
 *  Created on: 24.05.2012
 *      Author: X-ArigitaJ
 */


