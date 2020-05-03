/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_commonrf                                                */
/*                                                                              */
/* DESCRIPTION:      Common Data Structures and Defines for Txceiver Modules    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_COMMONRF_H_
#define HW_COMMONRF_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "hw_nRF24L01.h"
#include "hw_hoperf.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/
/* Maximum number of channels to be received (max.) */
#define MAX_CHANNELS 8

/* Enable / Disable Frequency Hopping */
#define FH_DISABLE   1
#define FH_ENABLE    2

/* TX Period */
#define ONE_MS_PERIOD 1
#define TWO_MS_PERIOD 2
#define THREE_MS_PERIOD 3
#define ONE_CYCLE_PERIOD 1

/* Modes */
#define RXMODE 0
#define TXMODE 1

/* Transceiver Type */
#define NRF24L01_TRX 1
#define HOPERF_TRX 2
#define TXR_TYPE NRF24L01_TRX

/* Payload length */
#define TX_ADR_WIDTH    3  /* 3 bytes TX address width     */
#define TX_PLOAD_WIDTH  6  /* 6 bytes TX payload           */
#define RX_ADR_WIDTH    3  /* 3 bytes RX address width     */
#define RX_PLOAD_WIDTH  6  /* 6 bytes RX payload           */

/* SafeLink Payload length */
#define SL_TX_PLOAD_WIDTH  4  /* 4 bytes TX payload        */
#define SL_RX_PLOAD_WIDTH  7  /* 7 bytes RX payload        */

/* Channel Quality */
#define GOOD_CHANNEL 0
#define BAD_CHANNEL  1

/* RF Device has received information (for INT pin polling purposes) */
#define READY 1

/* RF Device has not received information (for INT pin polling purposes) */
#define NOT_READY 0

/* Received Data is consistent */
#define DATA_CONSISTENT 1

/* Received Data is inconsistent */
#define DATA_INCONSISTENT 0

/* No data yet */
#define DATA_NOT_RECEIVED 2

/* Retransmission limit reached */
#define MAXIMUM 1

/* Retransmission limit not yet reached */
#define NOT_YET 0

/* Channels */
#define MAX_RF_CHANNELS 25
#define MAX_SL_CHANNELS 3

/*------------------------------------------------------------------------------*/
/* Typedef                                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_Init                                                    */
/*                                                                              */
/* DESCRIPTION:      Initialize Transceiver                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RF_Init(uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_DeInit                                                  */
/*                                                                              */
/* DESCRIPTION:      De-Initialize Transceiver                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RF_DeInit(uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_CheckTRX                                                */
/*                                                                              */
/* DESCRIPTION:      Function to check transceiver                              */
/*                                                                              */
/* PARAMETERS:       Out: status                                                */
/*                   In:  Transceiver                                           */
/*                   	  Mode                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RF_CheckTRX(uint8_t txceiver, uint8_t mode);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_TxDataWireless                                          */
/*                                                                              */
/* DESCRIPTION:      Function to send data                                      */
/*                                                                              */
/* PARAMETERS:       In: buffer                                                 */
/*                       channel                                                */
/*                       txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RF_TxDataWireless(uint8_t *buffer, uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_TxMode                                                  */
/*                                                                              */
/* DESCRIPTION:      Setup TX Mode                                              */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RF_TxMode(uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_TxDone                                                  */
/*                                                                              */
/* DESCRIPTION:      Checks if something has been received                      */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                   Out: flag                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RF_TxDone(uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_SetChannel                                              */
/*                                                                              */
/* DESCRIPTION:      Setup Channel                                              */
/*                                                                              */
/* PARAMETERS:       In: channel                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RF_SetChannel(uint8_t channel, uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_SetAddress                                              */
/*                                                                              */
/* DESCRIPTION:      Setup Address                                              */
/*                                                                              */
/* PARAMETERS:       In: channel                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RF_SetAddress(uint16_t address);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_RxDataWireless                                          */
/*                                                                              */
/* DESCRIPTION:      Returns pointer to incomming data                          */
/*                                                                              */
/* PARAMETERS:       Out: buffer                                                */
/*                   In:  txceiver                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RF_RxDataWireless(uint8_t *buffer, uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_RxMode                                                  */
/*                                                                              */
/* DESCRIPTION:      Sets Transceiver in RX_Mode                                */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RF_RxMode(uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_RxDone                                                  */
/*                                                                              */
/* DESCRIPTION:      Checks if something has been received                      */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                   Out: flag                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RF_RxDone(uint8_t txceiver);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RF_Ch_Quality                                              */
/*                                                                              */
/* DESCRIPTION:      Checks the Channel Quality                                 */
/*                                                                              */
/* PARAMETERS:       In: txceiver                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t RF_Ch_Quality(uint8_t txceiver);

#endif /* HW_COMMONRF_H_ */

/*
 * hw_commonrf.h
 *
 *  Created on: 20.09.2013
 *      Author: ArigitaJ
 */
