/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_radio.h                                                 */
/*                                                                              */
/* DESCRIPTION:      Definitions and support functions for radio control and    */
/* 					 telemetry                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_RADIO_H_
#define HW_RADIO_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "../Low_Level_State_Machines/RF_Handler/RF_Handler_LLSM.h"
#include "../Low_Level_State_Machines/SafeLink_Handler/SafeLink_Handler_LLSM.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Init RF Handler */
#define RF_REINIT(x) RFHandler_llsm_reinit(x)

/* De-Init RF Handler */
#define RF_DEINIT(x) RFHandler_llsm_deinit(x)

/* Set RF Address */
#define RF_SET_ADDRESS(x) RFHandler_SetRFAddress(x)

/* Get RF Address from Flash */
#define RF_GET_ADDRESS RFHandler_GetRFAddress()

/* Get RF RX Link Quality */
#define RF_GET_LQUALITY RFHandler_LinkQuality()

/* Get Link Status (Telemetry Reception) */
#define RF_GET_TELEM_LINK_STATUS RFHandler_GetLinkStatus()

/* Get SafeLink Status (Telemetry Reception) */
#define SL_GET_RSSI SafeLinkHandler_GetRSSI()

/* Get Packet SL RX Link Quality */
#define SL_GET_LQUALITY_RX SafeLinkHandler_LinkQualityRX()

/* Get Packet SL TX Link Quality */
#define SL_GET_LQUALITY_TX SafeLinkHandler_LinkQualityTX()

/* Get SafeLink Status (Telemetry Reception) */
#define SL_GET_TELEM_LINK_STATUS SafeLinkHandler_GetLinkStatus()

/* Setup external Buffers */
#define RF_SETUP_EXT_BUFFERS(x, y, w, z) RFHandler_SetupExtBuffers(x, y, w, z)
#define SL_SETUP_EXT_BUFFERS(x, y, w, z) SafeLinkHandler_SetupExtBuffers(x, y, w, z)

#endif /* HW_RADIO_H_ */

/*
 * hw_radio.h
 *
 *  Created on: 03.07.2012
 *      Author: X-ArigitaJ
 */
