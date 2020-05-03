/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_nRF24L01.h                                              */
/*                                                                              */
/* DESCRIPTION:      Control Transceiver NRF24L01+                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_NRF24L01_H_
#define HW_NRF24L01_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Shockburst Mode */
#define ENHS_DISABLED 0
#define ENHS_ENABLED 1

/* Flush Active/Inactive */
#define FLUSH_DISALED 0
#define FLUSH_ENABLED 1

/* Change switching method */
#define CHSWITCH_STANDARD 0
#define CHSWITCH_ONTHEFLY 1

//****************************************************************//
/* SPI(nRF24L01) commands */
#define NRF_READ_REG        0x00  // Define read command to register
#define NRF_WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define W_TX_PAYLOAD_NO_ACK 0xB0 // No ACK of TX Payload

//***************************************************//
/* SPI(nRF24L01) registers(addresses) */
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define FEATURE         0x1D  // Feature Register
#define MAX_RT  		0x10  	// Max #of TX retrans interrupt
#define TX_DS   		0x20  	// TX data sent interrupt
#define RX_DR   		0x40  	// RX data received
//-------------------------------------------

/*------------------------------------------------------------------------------*/
/* Global Function Prototypes                                                   */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Init_NRF                                                   */
/*                                                                              */
/* DESCRIPTION:      Initializes the required register of the NRF24L01+         */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Init_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             DeInit_NRF                                                 */
/*                                                                              */
/* DESCRIPTION:      De-Initializes the NRF24L01+                               */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void DeInit_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             CheckTRX_NRF                                               */
/*                                                                              */
/* DESCRIPTION:      Checks the status of the NRF24L01+                         */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t CheckTRX_NRF(uint8_t mode);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             TxDataWireless_NRF                                         */
/*                                                                              */
/* DESCRIPTION:      Transmission of the predefined Structure Type Info data    */
/*                                                                              */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void TxDataWireless_NRF(uint8_t *buff);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             TX_Mode_NRF                                                */
/*                                                                              */
/* DESCRIPTION:      This function initializes one nRF24L01 device to           */
/*                   TX mode, set TX address, set RX address for auto.ack,      */
/*                   fill TX payload, select RF channel, datarate & TX pwr.     */
/*                   PWR_UP is set, CRC(2 bytes) is enabled, & PRIM:TX.         */
/*                                                                              */
/*                   ToDo: One high pulse(>10us) on CE will now send this       */
/*                   packet and expect an acknowledgment from the RX device.    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Tx_Mode_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Channel_NRF                                            */
/*                                                                              */
/* DESCRIPTION:      Sets desired channel                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Set_Channel_NRF(uint8_t channel);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Address_NRF                                            */
/*                                                                              */
/* DESCRIPTION:      Sets desired address                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Set_Address_NRF(uint8_t address);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             RxDataWireless_NRF                                         */
/*                                                                              */
/* DESCRIPTION:      Reception of the info in buffer                            */
/*                                                                              */
/* PARAMETERS:       In: Buffer                                                 */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void RxDataWireless_NRF(uint8_t *buff);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Rx_Mode_NRF                                                */
/*                                                                              */
/* DESCRIPTION:      Activate RX Mode                                           */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Rx_Mode_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Comm_Ready_NRF                                             */
/*                                                                              */
/* DESCRIPTION:      Checks if NRF24L01+ has received information               */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_READY, 1: READY                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t Comm_Ready_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Get_Ch_Quality_NRF                                         */
/*                                                                              */
/* DESCRIPTION:      Gets the quality of the current channel                    */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t Get_Ch_Quality_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Sent_Done_NRF                                              */
/*                                                                              */
/* DESCRIPTION:      Checks if NRF24L01+ has sent the information requested     */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_READY, 1: READY                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t Sent_Done_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Max_Ret_NRF                                                */
/*                                                                              */
/* DESCRIPTION:      Checks if NRF24L01+ has reached the Maximum Retransmit     */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_YET, 1: MAXIMUM                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern uint8_t Max_Ret_NRF(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_RW                                                     */
/*                                                                              */
/* DESCRIPTION:      Writes one byte to nRF24L01, and return the byte read      */
/*                   from nRF24L01 during write, according to SPI protocol      */
/*                                                                              */
/* PARAMETERS:       In: byte                                                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_RW(uint8_t byte);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_RW_Reg                                                 */
/*                                                                              */
/* DESCRIPTION:      Writes value 'value' to nRF24L01 register 'reg'            */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        value: Value                                          */
/*                   Out: status                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_Read                                                   */
/*                                                                              */
/* DESCRIPTION:      Reads one byte from nRF24L01 register 'reg'                */
/*                                                                              */
/* PARAMETERS:       In: reg: Register Number                                   */
/*                   Out: value read                                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_Read(uint8_t reg);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_Read_Buf                                               */
/*                                                                              */
/* DESCRIPTION:      Reads 'bytes' #of bytes from register 'reg'                */
/*                   Typically used to read RX payload, Rx/Tx address           */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        bytes: Number of Bytes to be read                     */
/*                   Out: pBuf: Buffer filled                                   */
/*                        status: status                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);


/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI_Write_Buf                                              */
/*                                                                              */
/* DESCRIPTION:      Writes contents of buffer '*pBuf' to nRF24L01              */
/*                   Typically used to write TX payload, Rx/Tx address          */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        pBuf: Buffer to be written                            */
/*                        bytes: Number of Bytes to be read                     */
/*                   Out: status: status                                        */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             nRF24L01_Interrupt                                         */
/*                                                                              */
/* DESCRIPTION:      Code to be executed if the Interrupt in INTERRUPT pin      */
/*                   is activated                                               */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
/* extern void nRF24L01_Interrupt(void); */

#endif /* HW_NRF24L01_H_ */
