/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SafeLink_Handler_LLSM.h                                    */
/*                                                                              */
/* DESCRIPTION:      SafeLink Frequency Hopping Handler                         */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef SAFELINK_HANDLER_LLSM_H_
#define SAFELINK_HANDLER_LLSM_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"
#include "../Modules/RF/hw_commonrf.h"
#include "../Modules/MCU/hw_common.h"
#include "../../../config_switches.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* Config related defines */

/* Channel Quality mode */
/* Packet based */
#define Q_PKTSUCC 0
/* SNR based */
#define Q_SNR     1

/* Enable / Disable TRX Reset */
#define SL_TRX_RESET_DISABLED 0
#define SL_TRX_RESET_ENABLED 1

/* Transition cycles between reception and transmission */
#define SL_TIME_TRANS 2

/* State of the tx pause */
#define SL_TXPAUSE_NOACT 0
#define SL_TXPAUSE_ACT   1

/*------------------------------------------------------------------------------*/
/* Enum                                                                         */
/*------------------------------------------------------------------------------*/
enum States_SLHandlerLLSM {                  /* States SL Handler State Machine */
    SL_HANDLER_INIT = 1,           			       /* Wait for Application Init */
    SL_SCAN_CHANNELS,                                        /* Find RX Channel */
    SL_SEND_DATA,                                            /* Send Data State */
    SL_RECEIVE_DATA,                                      /* Receive Data State */

    MAX_STATE_SL_HANDLER_LLSM
};

/*------------------------------------------------------------------------------*/
/* More Defines                                                                 */
/*------------------------------------------------------------------------------*/

/* Buffer related Defines */
/* Buffer State */
#define SL_BUFF_NOTINITIATED 0
#define SL_BUFF_INITIATED    1
#define SL_TBUFF_SIZE slhandler_llsm.tbuff_size
#define SL_RBUFF_SIZE slhandler_llsm.rbuff_size

/* Time related Defines (units are in ms) */
/* Configurable Time Window to detect RX lost */
#define SL_LOSTRX_TIME 50
/* Configurable Time Window to detect RX lost when scanning for new channel */
#define SL_LOSTRX_TIME_SCAN 50
/* Maximum Transmitted frames */
#define SL_MAX_TIME_TX 50
/* Transition time between RX and TX */
#define SL_TIME_TRANS 2

/* Command RX Flags */
#define SL_RX_TXMITREQUEST_FLAG 0x02
#define SL_RX_ALL_FLAGS         0x03

/* Command TX Flags */
#define SL_TX_ALL_FLAGS         0x00

/* Initial Channel */
#define SL_INITCHANNEL       1

/* Default RF Address */
#define DEF_SL_ADDR 0x3E

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/
/* RFHandler State Machine typedef */
typedef struct SLHandlerLLSMTag {
	/* Pointer to TX Buffer */
	int16_t *tBuffer;
	/* Pointer to RX Buffer */
	uint8_t *rBuffer;
	/* Address */
	uint16_t rf_address;
	/* Packet Loss */
	float32_t rx_packetloss;
	/* Total Number of Packets that we should have received */
	uint16_t rx_totpacket;
    /* Total Number of Packets that we should have received */
    uint16_t rx_packetlost;
	/* Timer for detecting lost signal */
	uint16_t timer_rxlost;
	/* Channel Quality based on incoming packets */
	uint8_t channel_quality[MAX_SL_CHANNELS];
	/* Received packet counter */
    uint8_t rx_packetctr;
    uint8_t rx_packetctr_old;
	/* State */
	uint8_t state;
	/* Timer to control maximum time in TX Mode */
	uint8_t timer_txmode;
	/* Particular information from the Communications State Machine */
    /* Actual Channel */
	uint8_t channel;
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
	/* Tel Link Status */
	uint8_t ctr_status;
	/* Tx Packet Counter */
	uint8_t tx_packetctr;
	/* Ctr to control TX pause */
	uint8_t ctrtxpause;
	/* Flag to indicate TX pause */
	uint8_t flagtxpause;
} SLHandlerLLSM;

/*------------------------------------------------------------------------------*/
/* Global Variables Definitions                                                 */
/*------------------------------------------------------------------------------*/
volatile SLHandlerLLSM slhandler_llsm ALIGN_4;

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_SetRFAddress                                */
/*                                                                              */
/* DESCRIPTION:     Set desired RF TX Address                                   */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void SafeLinkHandler_SetRFAddress(uint16_t address);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRFAddress                                */
/*                                                                              */
/* DESCRIPTION:     Retrieves the current RF Address from Flash                 */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint16_t SafeLinkHandler_GetRFAddress(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRXFlag                                   */
/*                                                                              */
/* DESCRIPTION:     Gets the satus of a received flag                           */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t SafeLinkHandler_GetRXFlag(uint8_t flag);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_ModifyRXFlag                                */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the received defined flag                         */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_ModifyRXFlag(uint8_t flag, uint8_t status);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_ModifyTXFlag                                */
/*                                                                              */
/* DESCRIPTION:     Set/Unset the transmitted defined flag                      */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void SafeLinkHandler_ModifyTXFlag(uint8_t flag, uint8_t status);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetLinkStatus                               */
/*                                                                              */
/* DESCRIPTION:     Gets the link Status                                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t SafeLinkHandler_GetLinkStatus(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetRSSI                                     */
/*                                                                              */
/* DESCRIPTION:     Gets the RX RSSI                                            */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t SafeLinkHandler_GetRSSI(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_GetPacketSucc                               */
/*                                                                              */
/* DESCRIPTION:     Gets the Packet Success                                     */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t SafeLinkHandler_GetPacketSucc(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_LinkQualityRX                               */
/*                                                                              */
/* DESCRIPTION:     Gets the Link Quality                                       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t SafeLinkHandler_LinkQualityRX(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_llsm_init                                   */
/*                                                                              */
/* DESCRIPTION:     Initializes the State Machine                               */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void SafeLinkHandler_llsm_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_llsm_main                                   */
/*                                                                              */
/* DESCRIPTION:     Main function from the State Machine                        */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void SafeLinkHandler_llsm_main(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:            SafeLinkHandler_SetupExtBuffers                             */
/*                                                                              */
/* DESCRIPTION:     This function will setup the Application Ext. Buffers       */
/*                                                                              */
/* VERSION:         1.0                                                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void SafeLinkHandler_SetupExtBuffers(int16_t *tbuffer, uint8_t tlength, uint8_t *rbuffer, uint8_t rlength);

#endif /* SAFELINK_HANDLER_LLSM_H_ */

/*
 * SafeLink_Handler_LLSM.h
 *
 *  Created on: 06.01.2014
 *      Author: ArigitaJ
 */
