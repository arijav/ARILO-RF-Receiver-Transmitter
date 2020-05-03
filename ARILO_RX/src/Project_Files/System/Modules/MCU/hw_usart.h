/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_usart.c                                                 */
/*                                                                              */
/* DESCRIPTION:      USART support functions for STM32                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_USART_H_
#define HW_USART_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines & Typedef                                                            */
/*------------------------------------------------------------------------------*/
#define USART_RXBUFFERSIZE   5
#define USARTRX_AVAILABLE 1
#define USARTRX_NOTAVAILABLE 0

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             usart_init                                                   */
/*                                                                              */
/* DESCRIPTION:      Inits. SPI as Master                                       */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void usart_init(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             usart_available                                            */
/*                                                                              */
/* DESCRIPTION:      Returns USART availability                                 */
/*                                                                              */
/* PARAMETERS:       Out: Availability Flag                                     */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t usart_available(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             usart_getbuffer                                            */
/*                                                                              */
/* DESCRIPTION:      Returns Pointer to RX Buffer                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t usart_getbuffer(uint8_t *rx_buffer);

#endif /* HW_USART_H_ */

/*
 * hw_usart.h
 *
 *  Created on: 06.09.2013
 *      Author: ArigitaJ
 */
