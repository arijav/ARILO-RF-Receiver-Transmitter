/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             config_switches.h                                          */
/*                                                                              */
/* DESCRIPTION:      Configuration switches Arilo                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef CONFIG_SWITCHES_H_
#define CONFIG_SWITCHES_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* AriloRX HW revisions                                              */
/* STM32F103RET6 QFP64 512KByte                                      */
/* RFM22 Radio                                                       */
/* 8MHz Oscillator, change HSE_VALUE in precompiler settings!        */
#define ARILORX_HW1 0
/* STM32F103RET6 QFP64 512KByte                                      */
/* Small form factor, no sensor inputs, telemetry ready              */
/* RFM22 Radio                                                       */
/* 12MHz Oscillator, change HSE_VALUE in precompiler settings!       */
#define ARILORX_HW2 1
/* STM32F103RET6 QFP64 512KByte                                      */
/* Small form factor, no sensor inputs, telemetry ready              */
/* RFM95W Radio                                                      */
/* 12MHz Oscillator, change HSE_VALUE in precompiler settings!       */
#define ARILORX_HW3 2

/* Select AriloRX HW revision */
#define ARILORX_HWREV ARILORX_HW1

/* RPM Measurement */
#define RPM_MEAS_METHOD RPM_TIMESTAMPS_METHOD

/* RF Handler */
#define RF_HANDLER_TXPERIOD  TWO_MS_PERIOD
#define RF_HANDLER_RXPERIOD  ONE_MS_PERIOD
#define RF_HANDLER_TRX_RESET TRX_RESET_DISABLED
#define RF_FH_SPEED          FH_SLOW

/* SafeLink Handler */
#define SL_HANDLER_TXPERIOD ONE_CYCLE_PERIOD
#define SL_HANDLER_RXPERIOD ONE_CYCLE_PERIOD
#define SL_HANDLER_FH       FH_ENABLE
#define SL_HANDLER_CHQ      Q_PKTSUCC

/* Transceiver NRF24L01+ */
#define RFM_ENHANCED_SHOCKBURST ENHS_DISABLED
#define RFM_FLUSH_TX FLUSH_ENABLED
#define RFM_FLUSH_RX FLUSH_ENABLED
#define RFM_CHSWITCH CHSWITCH_ONTHEFLY

/* Transceiver HopeRF */
#define HOPERF_AIRRATE   RATE_VERYSLOW

/* Communications */
#define TEST_SAFELINK SL_TEST_DISABLE

/* Transceiver Module Test (Ch7 activates NRF OR HOPERF) */
#define TEST_RF_MODULES RF_MODULETEST_ENABLE

#endif /* CONFIG_SWITCHES_H_ */

 /*
 * config_switches.h
 *
 *  Created on: 20.12.2012
 *      Author: X-ArigitaJ
 */
