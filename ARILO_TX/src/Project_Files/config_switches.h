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

/* AriloTX HW revisions                                              */
/* STM32F103RET6 QFP64 512KByte                                      */
/* (change define in stm32f10x.h to STM32F10X_HD, stm32_flash.Ld and */
/*  startup_stm32f10x_xx.S)                                          */
/* RFM22 Radio                                                       */
/* 8MHz Oscillator, change HSE_VALUE in precompiler settings!        */
#define ARILOTX_HW1 0
/* STM32F103CBT6 QFP48 128KByte                                      */
/* (change define in stm32f10x.h to STM32F10X_MD, stm32_flash.Ld and */
/*  startup_stm32f10x_xx.S)                                          */
/* RFM23BP Radio                                                     */
/* 12MHz Oscillator, change HSE_VALUE in precompiler settings!       */
#define ARILOTX_HW2 1
/* STM32F103CBT6 QFP48 128KByte                                      */
/* (change define in stm32f10x.h to STM32F10X_MD, stm32_flash.Ld and */
/*  startup_stm32f10x_xx.S)                                          */
/* RFM95W Radio                                                      */
/* 12MHz Oscillator, change HSE_VALUE in precompiler settings!       */
#define ARILOTX_HW3 2

/* Select AriloTX HW revision */
#define ARILOTX_HWREV ARILOTX_HW1

/* Buzzer Control */
#define BUZZ_ACT_INIT_SONG INIT_SONG_OFF

/* RF Handler */
#define RF_HANDLER_TXPERIOD TWO_MS_PERIOD
#define RF_HANDLER_RXPERIOD ONE_MS_PERIOD
#define RF_HANDLER_FHSPEED  FH_SLOW
#define RF_HANDLER_BANNEDCH BANNED_DISABLED

/* SafeLink Handler */
#define SL_HANDLER_TXPERIOD ONE_CYCLE_PERIOD
#define SL_HANDLER_RXPERIOD ONE_CYCLE_PERIOD
#define SL_HANDLER_FH		FH_ENABLE
#define SL_HANDLER_CHQ      Q_PKTSUCC

/* Transceiver NRF24L01+ */
#define RFM_ENHANCED_SHOCKBURST ENHS_DISABLED
#define RFM_FLUSH_TX FLUSH_ENABLED
#define RFM_FLUSH_RX FLUSH_ENABLED
#define RFM_CHSWITCH CHSWITCH_ONTHEFLY

/* Transceiver HopeRF */
#define HOPERF_AIRRATE RATE_VERYSLOW

/* Communications */
#define TEST_SAFELINK SL_TEST_DISABLE

/* Telemetry */
#define TEL_USE_TXTEMP USE_TX_TEMP

/* Control PPM */
#define CTR_SERVO_TEST SERVO_TEST_OFF

#endif /* CONFIG_SWITCHES_H_ */

 /*
 * config_switches.h
 *
 *  Created on: 20.12.2012
 *      Author: X-ArigitaJ
 */
