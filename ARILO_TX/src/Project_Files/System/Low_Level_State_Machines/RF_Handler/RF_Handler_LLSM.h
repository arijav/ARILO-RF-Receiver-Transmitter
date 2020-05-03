/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_Handler_LLSM.h                                          */
/*                                                                              */
/* DESCRIPTION:      RF Frequency Hopping Handler                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef RF_HANDLER_LLSM_H_
#define RF_HANDLER_LLSM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../Modules/RF/hw_commonrf.h"
#include "../Modules/MCU/hw_common.h"
#include "../../../config_switches.h"

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_RFHandlerLLSM {                   /* States RF Handler State Machine*/
	RF_HANDLER_DEINIT = 1,                  /* De-initialize and deactivate TRX */
	RF_HANDLER_REINIT,                        /* Re-initialize and activate TRX */
    RF_HANDLER_INIT,              			       /* Wait for Application Init */
    RF_SEND_DATA,                                            /* Send Data State */
    RF_RECEIVE_DATA,                                      /* Receive Data State */
    RF_HANDLER_SHUTDOWN,                                       /* Nothing to do */

    MAX_STATE_RF_HANDLER_LLSM
};

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* Config related defines */

/* FH Speed */
#define FH_SLOW 0
#define FH_FAST 1
/* Banned list active/inactive */
#define BANNED_DISABLED 0
#define BANNED_ENABLED 1

/* Buffer related Defines */
/* Buffer State */
#define RF_BUFF_NOTINITIATED 0
#define RF_BUFF_INITIATED    1
#define TBUFF_SIZE rfhandler_llsm.tbuff_size
#define RBUFF_SIZE rfhandler_llsm.rbuff_size

#if(RF_HANDLER_FHSPEED == FH_SLOW)
/* Time related Defines (units are in 2ms units) */
/* Configurable Switch Time */
#define CHANNEL_TIME (370 / RF_HANDLER_TXPERIOD)
/* Configurable periodic time for switching to Receive Mode */
#define RXSWITCH_PERIOD (40 / RF_HANDLER_TXPERIOD)
/* Time window around the Switch Frequency events where no change to Receive */
/* Mode is allowed (positive and negative around the Channel Time)           */
#define NORX_SWITCH_TIME (30 / RF_HANDLER_TXPERIOD)
/* Maximum Time in Receive Mode */
#define MAX_TIME_RX (16 / RF_HANDLER_RXPERIOD)
/* Default Banned Time (each unit are ms)  */
#define RF_BANNED_TIME 2000 / RF_HANDLER_TXPERIOD
#elif(RF_HANDLER_FHSPEED == FH_FAST)
/* Time related Defines (units are in 2ms units) */
/* Configurable Switch Time */
#define CHANNEL_TIME (100 / RF_HANDLER_TXPERIOD)
/* Configurable periodic time for switching to Receive Mode */
#define RXSWITCH_PERIOD (20 / RF_HANDLER_TXPERIOD)
/* Time window around the Switch Frequency events where no change to Receive */
/* Mode is allowed (positive and negative around the Channel Time)           */
#define NORX_SWITCH_TIME (10 / RF_HANDLER_TXPERIOD)
/* Maximum Time in Receive Mode */
#define MAX_TIME_RX (4 / RF_HANDLER_RXPERIOD)
/* Default Banned Time (each unit are ms)  */
#define RF_BANNED_TIME 2000 / RF_HANDLER_TXPERIOD
#endif

/* Threshold in % of Packets Lost to detect if CH Quality is bad */
#define BAD_RFCHANNEL_THR 0

/* Command TX Flags */
#define RF_TX_TXMITREQUEST_FLAG 0x80
#define RF_TX_LASTTXCH_FLAG     0x40
#define RF_TX_RESERVED_FLAG     0x20
#define RF_TX_ALL_FLAGS         0xFF

/* Command RX Flags */
#define RF_RX_TXMITDONE_FLAG    0x80
#define RF_RX_ALL_FLAGS         0x80

/* Initial Channel */
#define RF_INITCHANNEL       1

/* Link related Flags */
/* Reception Flags */
#define RF_NOTRECEIVED 0
#define RF_RECEIVED    1
#define GOOD_RFCHANNEL 0
#define BAD_RFCHANNEL  1

/* Minimum number of channels available  */
#define MIN_RF_CHANNELS 3

/* Default RF Address */
#define DEF_RF_ADDR 5

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* Communications State Machine typedef */
typedef struct RFHandlerLLSMTag {
	/* Pointer to TX Buffer */
	int32_t *tBuffer;
	/* Pointer to RX Buffer */
	int32_t *rBuffer;
	/* Address */
	uint32_t rf_address;
	/* Packet Loss */
	float32_t rx_packetloss;
	/* Banned channel list (each field represents banned time in ms) */
	uint16_t channel_banlist[MAX_RF_CHANNELS];
    /* Total Number of Packets that we should have received */
    uint16_t rx_totpacket;
    /* Total Number of Packets that we should have received */
    uint16_t rx_packetlost;
	/* Timer for Channel Switching */
	uint16_t timer_chswitch;
	/* Timer for switching to RX Mode */
	uint16_t timer_rxswitch;
	/* Cycle channel list (each flag represents a channel used in the same cycle) */
	uint16_t channel_cycle[MAX_RF_CHANNELS];
    /* Counter to control tx/rx period */
    uint16_t cnt;
	/* State */
	uint8_t state;
	/* Channel Quality based on incoming packets */
	uint8_t channel_quality[MAX_RF_CHANNELS];
	/* Received Packet Ctr. */
    uint8_t rx_packetctr;
    uint8_t rx_packetctr_old;
    /* Transmitted Packet Ctr. */
    uint8_t tx_packetctr;
	/* Time in RX Mode */
	uint8_t timer_rxmode;
    /* Actual Channel */
	uint8_t channel;
	/* Next Channel */
	uint8_t next_channel;
	/* Transmission Buffer Size */
	uint8_t tbuff_size;
	/* Reception Buffer Size */
	uint8_t rbuff_size;
	/* Transmission Buffer Head Position */
	uint8_t tpos;
	/* Init Status of Buffers */
	uint8_t init_buff;
	/* Command (Req. Data Receiver Flag, Ch. Switch Flag, Reserved and 5 Bit next channel) */
	uint8_t txcommand;
	/* Command (Req. Data Receiver Flag, Ch. Switch Flag, Reserved and 5 Bit next channel) */
	uint8_t rxcommand;
	/* Number of banned channels */
	uint8_t num_ban_chs;
	/* Tel Link Status */
	uint8_t tel_status;
} RFHandlerLLSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
volatile RFHandlerLLSM rfhandler_llsm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_SetRFAddress                                      */
/*                                                                              */
/* DESCRIPTION:     Set desired RF TX Address                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RFHandler_SetRFAddress(uint16_t address);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetRFAddress                                      */
/*                                                                              */
/* DESCRIPTION:     Retrieves the current RF Address from Flash                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint16_t RFHandler_GetRFAddress(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetRXFlag                                         */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the defined flag                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t RFHandler_GetRXFlag(uint8_t flag);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_ModifyTXFlag                                      */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the defined flag                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_ModifyTXFlag(uint8_t flag, uint8_t status);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_ModifyRXFlag                                      */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the defined flag                                  */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void RFHandler_ModifyRXFlag(uint8_t flag, uint8_t status);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetLinkStatus                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the link Status                                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RFHandler_GetLinkStatus(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_GetPacketSucc                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the Packet Success                                     */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RFHandler_GetPacketSucc(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_LinkQuality                                       */
/*                                                                              */
/* DESCRIPTION:     Gets the Link Quality                                       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RFHandler_LinkQuality(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_init                                         */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RFHandler_llsm_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_reinit                                       */
/*                                                                              */
/* DESCRIPTION:     Re-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RFHandler_llsm_reinit(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_deinit                                       */
/*                                                                              */
/* DESCRIPTION:     De-Initializes the State Machine                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RFHandler_llsm_deinit(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_llsm_main                                         */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RFHandler_llsm_main(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            RFHandler_SetupExtBuffers                                   */
/*                                                                              */
/* DESCRIPTION:     This function will setup the Application Ext. Buffers       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RFHandler_SetupExtBuffers(int32_t *tbuffer, uint8_t tlength, int32_t *rbuffer, uint8_t rlength);

#endif /* RF_HANDLER_LLSM_H_ */

/*
 * RF_Handler_LLSM.h
 *
 *  Created on: 25.09.2013
 *      Author: ArigitaJ
 */
