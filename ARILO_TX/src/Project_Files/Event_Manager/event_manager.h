/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             event_manager.h                                            */
/*                                                                              */
/* DESCRIPTION:      Event/Signal definitions                                   */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* Exit signal for the states, it is called entry_ev because its the entry */
/* event of the next state                                                 */
#define EXIT_STATE &entry_ev

/* Max number of events in INT event queue */
#define N_MAX_EVINT 6

/* Max number of events in time event queue */
#define N_MAX_EVTIME 30

/* Max number of signals in signal queue */
#define N_MAX_SIGNAL 40

/* Total number of queued events */
#define N_QUEUED_EVENTS (EventINTQueueCtr1.queue.qeventnumber + EventTimeQueueCtr1.queue.qeventnumber + SignalSMQueueCtr1.queue.qeventnumber)

/* Signal flanks (lower , upper) */
#define UP_FL 0
#define LOW_FL 1

/* CPU Load Calculation Window (in ms) */
#define CPU_LOAD_TWIN 100

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/

enum Events_and_Signals {                                                /* Events and signals used */
    /* Events */
    ICP_1 = 1,                                          /* Interruption by state-change in pin ICP1 */
    INT_0,                                                                    /* Button 0 interrupt */
    INT_1,                                                                    /* Button 1 interrupt */
    TSLOT_10ms,
    TSLOT_80ms,

    /* Signals */
    /* Telemetry info signals */
    TEL_BATT_SIG,                                                                         /* battery */
    TEL_CURR_SIG,                                                                         /* current */
    TEL_RSSITX_SIG,                                                                          /* RSSI */
    TEL_RSSIRX_SIG,                                                                          /* RSSI */
    TEL_CPULOAD_SIG,                                                                     /* CPU Load */
    TEL_DIST_SIG,                                                                        /* distance */
    TEL_TEMPRX_SIG,                                                                 /* temperature RX */
    TEL_TEMPTX_SIG,                                                                 /* temperature TX */
    TEL_RPM_SIG,                                                                              /* RPM */
    TEL_REVSENSE_SIG,                                                    /* Sense of the revolutions */
    TEL_IMUACELX_SIG,                                                        /* Accelerometer X Axis */
    TEL_IMUACELY_SIG,                                                        /* Accelerometer Y Axis */
    TEL_IMUACELZ_SIG,                                                        /* Accelerometer Z Axis */
    TEL_IMUGYROX_SIG,                                                                 /* Gyro X Axis */
    TEL_IMUGYROY_SIG,                                                                 /* Gyro Y Axis */
    TEL_IMUGYROZ_SIG,                                                                 /* Gyro Z Axis */
    CTR_COMMANDRX_SIG,                                                                 /* RX Command */

    /* Control signals: Channels */
    CTR_NCHANNELS_SIG,
    CTR_CH0_SIG,
    CTR_CH1_SIG,
    CTR_CH2_SIG,
    CTR_CH3_SIG,
    CTR_CH4_SIG,
    CTR_CH5_SIG,
    CTR_CH6_SIG,
    CTR_CH7_SIG,
    CTR_CH8_SIG,
    CTR_CH9_SIG,
    CTR_CH10_SIG,
    CTR_CH11_SIG,
    CTR_COMMANDTX_SIG,
    CTR_ADDRESS_SIG,
	
    /* Push-Buttom Request Signals */
    PBUTTOM_BINDREQ_SIG,
    PBUTTOM_FSSETREQ_SIG,

    /* Buzzer Request Signals */
    BUZZREQ_SIG,

    /* RF info signals */
    INFO_NO_RF_SIG,                                            /* Comm. Module is receiving no signal */
    INFO_RF_SIG,                                                 /* Comm. Module is receiving signals */
    REQ_INFO_RF_SIG,                             /* Request to comm. module, are we receiving signal? */
    TRXACTIVE_SIG,                               /* Indicates which TRX is the main one at the moment */

    /* Emergency situation, autopilot required */
    EMERGENCY_SIG,
    EMERGENCY_BYPASS_SIG,           /* Signal that indicates S.M. that no emergency signal should be sent */

    /* Debug Signals */
    DEBUG_SIG_1,
    DEBUG_SIG_2,
    DEBUG_SIG_3,

    /* Startup Signal */
    GOALIVE_SIG,

    ENTRY_SIG,												                          /* Entry signal */
    MAX_SIG                                                                        /* The last signal */
};

enum EvQueues {                                            /* Event Queues used */
    INTQ = 0,                                                /* INT event Queue */
    TIMEQ,                                             /* Time slot event Queue */
    SIGNALQ,                                              /* Signal event Queue */

    MAX_QUEUE                                                /* The last signal */
};

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/

/************************* Event Structures *****************************/

/* Basis event structure */
typedef struct eventtypeTag {
	uint8_t sig;
}eventtype;

/* Change state event structure in Interrupt pins */
typedef struct EventINTTag {
	/* Event signal */
	eventtype event;
	/* 0 = upper-flank; 1 = lower-flank */
	uint8_t flank;
	/* Value Timer counter for pulse width calculation */
	uint16_t timer_val;
	/* Cycle Number Timer counter for pulse width calculation */
	uint32_t timer_cycle;
}EventINT;

/* Periodic time event structure */
typedef struct EventTimeTag {
	/* Event signal */
	eventtype event;
}EventTime;

/* Signals between SM�s */
typedef struct SignalSMTag {
	/* Event signal */
	eventtype event;
	/* SM that sends the signal */
	uint8_t SM_sender;
	/* SM that receives the signal */
	uint8_t SM_receiver;
	/* Parameters */
	int32_t param;
}SignalSM;

/******************************* Event Queues ***********************************/

/* Basis Queue Control Structure */
typedef struct EventQueueCtrTag {
	/* Queue front */
	uint8_t qfront;
	/* Queue end */
	uint8_t qend;
	/* Events in queue */
	uint8_t qeventnumber;
} EventQueueCtr;

/* Change state pin event control structure queue */
typedef struct EventINTQueueTagCtr {
	/* Derived from basis Queue Structure */
	EventQueueCtr queue;
} EventINTQueueCtr;

/* Time event queue control structure */
typedef struct EventTimeQueueTagCtr {
	/* Derived from basis Queue Structure */
	EventQueueCtr queue;
} EventTimeQueueCtr;

/* Signals between SMs control structure queue */
typedef struct SignalSMQueueTagCtr {
	/* Derived from basis Queue Structure */
	EventQueueCtr queue;
} SignalSMQueueCtr;

/* Queue array declarations */
EventINT queue_pinINT[N_MAX_EVINT];
EventTime queue_Time[N_MAX_EVTIME];
SignalSM queue_signalSM[N_MAX_SIGNAL];

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
eventtype entry_ev;
volatile float32_t event_times_max[MAX_SIG];
volatile float32_t event_times_min[MAX_SIG];
volatile float32_t event_times_act[MAX_SIG];
volatile float32_t event_times_acc[MAX_SIG];

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
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
extern void init_evm(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             getq_event                                                 */
/*                                                                              */
/* DESCRIPTION:      Get next event from the first active queue                 */
/*                                                                              */
/* PARAMETERS:       Out: Event                                                 */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void getq_event(eventtype **event);

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
void dispatch_event(eventtype const *event);

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
void discard_event(void);//eventtype **event);

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
extern void sendq_Ievent(eventtype const *event);

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
extern void sendq_Tevent(eventtype const *event);

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
extern void sendq_Sevent(eventtype const *event);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            get_event_dur                                               */
/*                                                                              */
/* DESCRIPTION:     Gets accumulated event durations                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern float32_t get_event_dur(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            reset_event_dur                                             */
/*                                                                              */
/* DESCRIPTION:     Resets accumulated event durations                          */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void reset_event_dur(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            event_processor_main                                        */
/*                                                                              */
/* DESCRIPTION:     Main function from the event manager. Processes every event */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void event_processor_main(void);

#endif /* EVENT_MANAGER_H_ */

 /*
 * event_manager.h
 *
 *  Created on: 24.05.2012
 *      Author: X-ArigitaJ
 */
