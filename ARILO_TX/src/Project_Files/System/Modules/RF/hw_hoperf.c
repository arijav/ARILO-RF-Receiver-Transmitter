/*                                                                              */
/* NAME:             hw_hoperf.c                                                */
/*                                                                              */
/* DESCRIPTION:      Control Transceiver RFM23bp                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "hw_commonrf.h"
#include <string.h>
#include <math.h>
#include "../MCU/hw_timers.h"
#include "../MCU/hw_common.h"
#include "../MCU/hw_spi.h"
#include "../../Low_Level_State_Machines/Error_Handler/Error_Handler_LLSM.h"
#include "../../../config_switches.h"

/*------------------------------------------------------------------------------*/
/* Local Variables                                                              */
/*------------------------------------------------------------------------------*/
// These are indexed by the values of ModemConfigChoice
// Canned modem configurations generated with
// http://www.hoperf.com/upload/rf/HOPERFB%2023B%2031B%2042B%2043B%20Register%20Settings_RevB1-v5.xls
// Stored in flash (program) memory to save SRAM
ModemConfig MODEM_CONFIG_TABLE[] =
{
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x00, 0x08 }, // Unmodulated carrier
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x33, 0x08 }, // FSK, PN9 random modulation, 2, 5

    // All the following enable FIFO with reg 71
    //  1c,   1f,   20,   21,   22,   23,   24,   25,   2c,   2d,   2e,   58,   69,   6e,   6f,   70,   71,   72
    // FSK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x22, 0x08 }, // 2, 5
    { 0x1b, 0x03, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x22, 0x3a }, // 2.4, 36
    { 0x1d, 0x03, 0xa1, 0x20, 0x4e, 0xa5, 0x00, 0x13, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x22, 0x48 }, // 4.8, 45
    { 0x1e, 0x03, 0xd0, 0x00, 0x9d, 0x49, 0x00, 0x45, 0x40, 0x0a, 0x20, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x22, 0x48 }, // 9.6, 45
    { 0x2b, 0x03, 0x34, 0x02, 0x75, 0x25, 0x07, 0xff, 0x40, 0x0a, 0x1b, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x22, 0x0f }, // 19.2, 9.6
    { 0x02, 0x03, 0x68, 0x01, 0x3a, 0x93, 0x04, 0xd5, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x22, 0x1f }, // 38.4, 19.6
    { 0x06, 0x03, 0x45, 0x01, 0xd7, 0xdc, 0x07, 0x6e, 0x40, 0x0a, 0x2d, 0x80, 0x60, 0x0e, 0xbf, 0x0c, 0x22, 0x2e }, // 57.6. 28.8
    { 0x8a, 0x03, 0x60, 0x01, 0x55, 0x55, 0x02, 0xad, 0x40, 0x0a, 0x50, 0x80, 0x60, 0x20, 0x00, 0x0c, 0x22, 0xc8 }, // 125, 125

    // GFSK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
    // These differ from FSK only in register 71, for the modulation type
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x23, 0x08 }, // 2, 5
    { 0x1b, 0x03, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x23, 0x3a }, // 2.4, 36
    { 0x1d, 0x03, 0xa1, 0x20, 0x4e, 0xa5, 0x00, 0x13, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x23, 0x48 }, // 4.8, 45
    { 0x1e, 0x03, 0xd0, 0x00, 0x9d, 0x49, 0x00, 0x45, 0x40, 0x0a, 0x20, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x23, 0x48 }, // 9.6, 45
    { 0x2b, 0x03, 0x34, 0x02, 0x75, 0x25, 0x07, 0xff, 0x40, 0x0a, 0x1b, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x23, 0x0f }, // 19.2, 9.6
    { 0x02, 0x03, 0x68, 0x01, 0x3a, 0x93, 0x04, 0xd5, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x23, 0x1f }, // 38.4, 19.6
    { 0x06, 0x03, 0x45, 0x01, 0xd7, 0xdc, 0x07, 0x6e, 0x40, 0x0a, 0x2d, 0x80, 0x60, 0x0e, 0xbf, 0x0c, 0x23, 0x2e }, // 57.6. 28.8
    { 0x8a, 0x03, 0x60, 0x01, 0x55, 0x55, 0x02, 0xad, 0x40, 0x0a, 0x50, 0x80, 0x60, 0x20, 0x00, 0x0c, 0x23, 0xc8 }, // 125, 125

    // OOK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
    { 0x51, 0x03, 0x68, 0x00, 0x3a, 0x93, 0x01, 0x3d, 0x2c, 0x11, 0x28, 0x80, 0x60, 0x09, 0xd5, 0x2c, 0x21, 0x08 }, // 1.2, 75
    { 0xc8, 0x03, 0x39, 0x20, 0x68, 0xdc, 0x00, 0x6b, 0x2a, 0x08, 0x2a, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x21, 0x08 }, // 2.4, 335
    { 0xc8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x29, 0x04, 0x29, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x21, 0x08 }, // 4.8, 335
    { 0xb8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x82, 0x29, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x21, 0x08 }, // 9.6, 335
    { 0xa8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x41, 0x29, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x21, 0x08 }, // 19.2, 335
    { 0x98, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x20, 0x29, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x21, 0x08 }, // 38.4, 335
    { 0x98, 0x03, 0x96, 0x00, 0xda, 0x74, 0x00, 0xdc, 0x28, 0x1f, 0x29, 0x80, 0x60, 0x0a, 0x3d, 0x0c, 0x21, 0x08 }, // 40, 335

};

float32_t hoperf_channel_list[MAX_SL_CHANNELS] =
{
	848.0,
	860.0,
	888.0,
};

uint8_t hoperf_Rssi;

uint8_t hoperf_temperature;

uint8_t current_address;

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_write                                               */
/*                                                                              */
/* DESCRIPTION:      write value in address                                     */
/*                                                                              */
/* PARAMETERS:       In: Address, value                                         */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
unsigned int hoperf_write(unsigned int addr, unsigned int val)
{
	uint8_t status;

	HOPERF_CSNL;

	SPI2_RW(addr | HOPERF_SPI_WRITE_MASK);      // select register
  	status = SPI2_RW(val);             // ..and write value to it..

	HOPERF_CSNH;

	return status;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_read                                                */
/*                                                                              */
/* DESCRIPTION:      read value from address                                    */
/*                                                                              */
/* PARAMETERS:       In: Address                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
unsigned int hoperf_read(unsigned int addr)
{
	uint8_t status;

	HOPERF_CSNL;

	SPI2_RW(addr & ~HOPERF_SPI_WRITE_MASK);     // Select register to read from..
  	status = SPI2_RW(0);    // ..then read register value

	HOPERF_CSNH;

	return status;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_temp_get                                            */
/*                                                                              */
/* DESCRIPTION:      read temp value from address                               */
/*                                                                              */
/* PARAMETERS:       In: Address                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_temp_get(void)
{
	return hoperf_temperature;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_reset                                               */
/*                                                                              */
/* DESCRIPTION:      reset hoperf txceiver                                      */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_reset()
{
	hoperf_write(HOPERF_REG_07_OPERATING_MODE1, HOPERF_SWRES);
    // Wait for it to settle
	DELAY_US(150); // SWReset time is nominally 100usec};
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Init_Hoperf                                                */
/*                                                                              */
/* DESCRIPTION:      init hoperf txceiver                                       */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Init_Hoperf(void)
{
	uint8_t deviceType;
    // Wait for HOPERF POR (up to 16msec)
	DELAY_US(16000);

	// Software reset the device
	hoperf_reset();

	/* Clear Fifos */
	hoperf_resetFifos();

	// Get the device type and check it
	// This also tests whether we are really connected to a device
	deviceType = hoperf_read(HOPERF_REG_00_DEVICE_TYPE);
	if (   deviceType != HOPERF_DEVICE_TYPE_RX_TRX
		&& deviceType != HOPERF_DEVICE_TYPE_TX)
	return 0;

	// Most of these are the POR default
	hoperf_write(HOPERF_REG_7D_TX_FIFO_CONTROL2, HOPERF_TXFFAEM_THRESHOLD);
	hoperf_write(HOPERF_REG_7E_RX_FIFO_CONTROL,  HOPERF_RXFFAFULL_THRESHOLD);
	hoperf_write(HOPERF_REG_30_DATA_ACCESS_CONTROL, HOPERF_ENPACRX | HOPERF_ENPACTX | HOPERF_ENCRC | HOPERF_CRC_CRC_16_IBM);
	// Configure the message headers
	// Here we set up the standard packet format for use by the HOPERF library
	// 8 nibbles preamble
	// 2 SYNC words 2d //, d4
	// Header length 1 (to)
	// 1 octet of data length (0 to 255)
	// 0 to 255 octets data
	// 2 CRC octets as CRC16(IBM), computed on the header, length and data
	// On reception the to address is check for validity against HOPERF_REG_3F_CHECK_HEADER3
	// or the broadcast address of 0xff
	// If no changes are made after this, the transmitted
	// to address will be 0xff, the from address will be 0xff
	// and all such messages will be accepted. This permits the out-of the box
	// HOPERF config to act as an unaddresed, unreliable datagram service
	hoperf_write(HOPERF_REG_32_HEADER_CONTROL1, HOPERF_BCEN_HEADER3 | HOPERF_HDCH_HEADER3);
	hoperf_write(HOPERF_REG_33_HEADER_CONTROL2, HOPERF_HDLEN_1 | HOPERF_SYNCLEN_2);
	hoperf_setPreambleLength(8);
	uint8_t syncwords[] = { 0x2d, 0xd4 };
	hoperf_setSyncWords(syncwords, sizeof(syncwords));
	hoperf_setPromiscuous(0);
	// Check the TO header against HOPERF_DEFAULT_NODE_ADDRESS
	hoperf_write(HOPERF_REG_3F_CHECK_HEADER3, HOPERF_DEFAULT_NODE_ADDRESS);
	// Set the default transmit header values
	hoperf_setHeaderTo(HOPERF_DEFAULT_NODE_ADDRESS);
	hoperf_setHeaderFrom(HOPERF_DEFAULT_NODE_ADDRESS);
	hoperf_setHeaderId(0);
	hoperf_setHeaderFlags(0);

	// Ensure the antenna can be switched automatically according to transmit and receive
	// This assumes GPIO0(out) is connected to TX_ANT(in) to enable tx antenna during transmit
	// This assumes GPIO1(out) is connected to RX_ANT(in) to enable rx antenna during receive
#if (ARILOTX_HWREV != ARILOTX_HW2)
	hoperf_write(HOPERF_REG_0B_GPIO_CONFIGURATION0, 0x12) ; // TX state
	hoperf_write(HOPERF_REG_0C_GPIO_CONFIGURATION1, 0x15) ; // RX state
#else
	// Reversed for HAB-RFM22B-BOA HAB-RFM22B-BO RFM23BP
	hoperf_write(HOPERF_REG_0B_GPIO_CONFIGURATION0, 0x15) ; // RX state
	hoperf_write(HOPERF_REG_0C_GPIO_CONFIGURATION1, 0x12) ; // TX state
#endif

	// Enable interrupts
	hoperf_write(HOPERF_REG_05_INTERRUPT_ENABLE1, HOPERF_ENTXFFAEM | HOPERF_ENRXFFAFULL | HOPERF_ENPKSENT | HOPERF_ENPKVALID | HOPERF_ENCRCERROR);
	hoperf_write(HOPERF_REG_06_INTERRUPT_ENABLE2, HOPERF_ENPREAVAL | HOPERF_ENRSSI);

	// Set some defaults. An innocuous ISM frequency, and reasonable pull-in
	//hoperf_setFrequency(434.0, 0.05);
	hoperf_setFrequency(hoperf_channel_list[0], 0.05);
	// Some slow, reliable default speed and modulation
	#if(HOPERF_AIRRATE == RATE_VERYSLOW)
	hoperf_setModemConfig(GFSK_Rb2_4Fd36);
	#elif(HOPERF_AIRRATE == RATE_SLOW)
	hoperf_setModemConfig(GFSK_Rb4_8Fd45);
	#elif(HOPERF_AIRRATE == RATE_FAST)
	hoperf_setModemConfig(GFSK_Rb125Fd125);
	#endif

	// Minimum power
	hoperf_setTxPower(HOPERF_TXPOW_20DBM);

	// Init temperature measurements
	hoperf_write(HOPERF_REG_12_TEMPERATURE_SENSOR_CALIBRATION, HOPERF_TSRANGE_M64_192C | HOPERF_ENTSOFFS);
	hoperf_write(HOPERF_REG_0F_ADC_CONFIGURATION, HOPERF_ADCSTART);

	return 1;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             DeInit_Hoperf                                              */
/*                                                                              */
/* DESCRIPTION:      De-init hoperf txceiver                                    */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void DeInit_Hoperf(void)
{

}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_statushandler                                       */
/*                                                                              */
/* DESCRIPTION:      HOPERF Interrupt Handler                                   */
/*                                                                              */
/* PARAMETERS:       Out: status flags                                          */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_statushandler(void)
{
    uint8_t _lastInterruptFlags[2];
    // Read the interrupt flags which clears the interrupt
    hoperf_spiBurstRead(HOPERF_REG_03_INTERRUPT_STATUS1, _lastInterruptFlags, 2);

#if 0
    // Caution: Serial printing in this interrupt routine can cause mysterious crashes
    Serial.print("interrupt ");
    Serial.print(_lastInterruptFlags[0], HEX);
    Serial.print(" ");
    Serial.println(_lastInterruptFlags[1], HEX);
    if (_lastInterruptFlags[0] == 0 && _lastInterruptFlags[1] == 0) Serial.println("FUNNY: no interrupt!");
#endif

#if 0
    // TESTING: fake an RF22_IFFERROR
    static int counter = 0;
    if (_lastInterruptFlags[0] & RF22_IPKSENT && counter++ == 10)
    {
		_lastInterruptFlags[0] = RF22_IFFERROR;
		counter = 0;
    }
#endif

    /* FIFO Underflow/Overflow Error */
    if (_lastInterruptFlags[0] & HOPERF_IFFERROR)
    {
    	hoperf_resetFifos(); // Clears the interrupt
    	ERRORHANDLER_LLSM_IFACE(HPERF_ERROR_FIFO_OF_UF, ERROR_ON);
    }
    /* FIFO almost empty */
    // Caution, any delay here may cause a FF underflow or overflow
    if (_lastInterruptFlags[0] & HOPERF_ITXFFAEM)
    {
        ERRORHANDLER_LLSM_IFACE(HPERF_ERROR_FIFO_AL_EMPTY, ERROR_ON);
		// See if more data has to be loaded into the Tx FIFO
    	// Serial.println("ITXFFAEM");
    }
    /* FIFO almost full */
    if (_lastInterruptFlags[0] & HOPERF_IRXFFAFULL)
    {
        ERRORHANDLER_LLSM_IFACE(HPERF_ERROR_FIFO_AL_FULL, ERROR_ON);
		// Caution, any delay here may cause a FF overflow
		// Read some data from the Rx FIFO
	//	Serial.println("IRXFFAFULL");
    }
	
    /* External interrupt */
    if (_lastInterruptFlags[0] & HOPERF_IEXT)
    {
        // This is not enabled by the base code, but users may want to enable it
        //hoperf_handleExternalInterrupt();
        //	Serial.println("IEXT");
    }
	
    if (_lastInterruptFlags[1] & HOPERF_IWUT)
    {
		// This is not enabled by the base code, but users may want to enable it
		//hoperf_handleWakeupTimerInterrupt();
	//	Serial.println("IWUT");
    }
	
    if (_lastInterruptFlags[0] & HOPERF_IPKSENT)
    {

    }
	
    if (_lastInterruptFlags[0] & HOPERF_IPKVALID)
    {
    	/* TODO: Read is extern */
    	//	uint8_t len = hoperf_read(HOPERF_REG_4B_RECEIVED_PACKET_LENGTH);

    //	Serial.println("IPKVALID");
	//	Serial.println(len);
	//	Serial.println(_bufLen);

		// May have already read one or more fragments
		// Get any remaining unread octets, based on the expected length
		// First make sure we dont overflow the buffer in the case of a stupid length
		// or partial bad receives
		//if (   len >  HOPERF_MAX_MESSAGE_LEN
		//	|| len < hoperf_device.bufLen)
		//{
		//	hoperf_device.rxBad++;
		//	hoperf_device.mode = HOPERF_MODE_IDLE;
		//	hoperf_clearRxBuf();
		//	return(0); // Hmmm receiver buffer overflow.
		//}

		/* TODO: Read is extern */
		//hoperf_spiBurstRead(HOPERF_REG_7F_FIFO_ACCESS, hoperf_device.buf + hoperf_device.bufLen, len - hoperf_device.bufLen);
		//hoperf_device.rxGood++;
		//hoperf_device.bufLen = len;
		//hoperf_device.mode = HOPERF_MODE_IDLE;
		//hoperf_device.rxBufValid = 1;
    }
	
    if (_lastInterruptFlags[0] & HOPERF_ICRCERROR)
    {
        ERRORHANDLER_LLSM_IFACE(HPERF_ERROR_CRC, ERROR_ON);
		Rx_Mode_Hoperf(); // Keep trying
    }
	
    if (_lastInterruptFlags[1] & HOPERF_IPREAVAL)
    {
	//	Serial.println("IPREAVAL");
		//hoperf_Rssi = hoperf_read(HOPERF_REG_26_RSSI);
		//hoperf_resetRxFifo();
    }
	
    if (_lastInterruptFlags[1] & HOPERF_IRSSI)
    {
		hoperf_Rssi = hoperf_read(HOPERF_REG_26_RSSI);
    }

    /* Power on reset (RFM suffered a Smart-Reset) */
    if (_lastInterruptFlags[1] & HOPERF_IPOR)
    {
        ERRORHANDLER_LLSM_IFACE(HPERF_ERROR_POWER_ON_RESET, ERROR_ON);
    	/* Reinit Hoperf */
    	Init_Hoperf();
    	/* Set current address */
    	Set_Address_Hoperf(current_address);
    	/* Set TX Mode */
    	Tx_Mode_Hoperf();
    }
    /* Low Battery detect */
    if (_lastInterruptFlags[1] & HOPERF_ILBD)
    {
        ERRORHANDLER_LLSM_IFACE(HPERF_ERROR_LOW_BAT, ERROR_ON);
    }

	/* Read ADC Temperature */
	hoperf_temperature = (uint8_t)(((float32_t)hoperf_read(HOPERF_REG_11_ADC_VALUE)) - 64);
	/* Init temperature measurements */
	hoperf_write(HOPERF_REG_0F_ADC_CONFIGURATION, HOPERF_ADCSTART);

    /* Return value specially for checking RX or TX done */
    return(_lastInterruptFlags[0]);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Comm_Ready_Hoperf                                          */
/*                                                                              */
/* DESCRIPTION:      Checks if Hoperf has received information (INT pin LOW)    */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_READY, 1: READY                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Comm_Ready_Hoperf(void)
{
	uint8_t status;
	status = NOT_READY;

    /* Read register STATUS's value */
    if(hoperf_statushandler() & HOPERF_IPKVALID)
    {
		/* Data received ok */
		status = READY;
	}
	
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Sent_Done_Hoperf                                           */
/*                                                                              */
/* DESCRIPTION:      Checks if Hoperf has sent the information requested        */
/*                                                                              */
/* PARAMETERS:       Out: 0: NOT_READY, 1: READY                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Sent_Done_Hoperf(void)
{
	uint8_t status;
	status = NOT_READY;

	/* read register STATUS's value */
	if(hoperf_statushandler() & HOPERF_IPKSENT)
	{
		/* Data sent ok */
		status = READY;
	}
	
	return(status);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setTxPower                                          */
/*                                                                              */
/* DESCRIPTION:      set hoperf tx power                                        */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setTxPower(uint8_t power)
{
	#if (ARILOTX_HWREV != ARILOTX_HW2)
    hoperf_write(HOPERF_REG_6D_TX_POWER, power);
	#else
    hoperf_write(HOPERF_REG_6D_TX_POWER, power | HOPERF_TXPOW_LNA_SW);
	#endif
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setFrequency                                        */
/*                                                                              */
/* DESCRIPTION:      set hoperf frequency                                       */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_setFrequency(float centre, float afcPullInRange)
{
    uint8_t fbsel = HOPERF_SBSEL;
    uint8_t afclimiter;
    if (centre < 240.0 || centre > 960.0) // 930.0 for early silicon
	return 0;
    if (centre >= 480.0)
    {
	if (afcPullInRange < 0.0 || afcPullInRange > 0.318750)
	    return 0;
	centre /= 2;
	fbsel |= HOPERF_HBSEL;
	afclimiter = afcPullInRange * 1000000.0 / 1250.0;
    }
    else
    {
	if (afcPullInRange < 0.0 || afcPullInRange > 0.159375)
	    return 0;
	afclimiter = afcPullInRange * 1000000.0 / 625.0;
    }
    centre /= 10.0;
    float integerPart = floor(centre);
    float fractionalPart = centre - integerPart;

    uint8_t fb = (uint8_t)integerPart - 24; // Range 0 to 23
    fbsel |= fb;
    uint16_t fc = fractionalPart * 64000;
    hoperf_write(HOPERF_REG_73_FREQUENCY_OFFSET1, 0);  // REVISIT
    hoperf_write(HOPERF_REG_74_FREQUENCY_OFFSET2, 0);
    hoperf_write(HOPERF_REG_75_FREQUENCY_BAND_SELECT, fbsel);
    hoperf_write(HOPERF_REG_76_NOMINAL_CARRIER_FREQUENCY1, fc >> 8);
    hoperf_write(HOPERF_REG_77_NOMINAL_CARRIER_FREQUENCY0, fc & 0xff);
    hoperf_write(HOPERF_REG_2A_AFC_LIMITER, afclimiter);
    return !(hoperf_statusRead() & HOPERF_FREQERR);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_statusRead                                          */
/*                                                                              */
/* DESCRIPTION:      status read                                                */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_statusRead()
{
    return hoperf_read(HOPERF_REG_02_DEVICE_STATUS);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setModemConfig                                      */
/*                                                                              */
/* DESCRIPTION:      set modem config                                           */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_setModemConfig(ModemConfigChoice index)
{
    if (index > (sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return 0;

    ModemConfig cfg;
    memcpy(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(ModemConfig));
    hoperf_setModemRegisters(&cfg);

    return 1;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setModemRegisters                                   */
/*                                                                              */
/* DESCRIPTION:      set modem registers                                        */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setModemRegisters(const ModemConfig* config)
{
	hoperf_write(HOPERF_REG_1C_IF_FILTER_BANDWIDTH,config->reg_1c);
	hoperf_write(HOPERF_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE,config->reg_1f);
	hoperf_spiBurstWrite(HOPERF_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE,&config->reg_20, 6);
    hoperf_spiBurstWrite(HOPERF_REG_2C_OOK_COUNTER_VALUE_1,&config->reg_2c, 3);
    hoperf_write(HOPERF_REG_58_CHARGE_PUMP_CURRENT_TRIMMING,config->reg_58);
    hoperf_write(HOPERF_REG_69_AGC_OVERRIDE1,config->reg_69);
    hoperf_spiBurstWrite(HOPERF_REG_6E_TX_DATA_RATE1,&config->reg_6e, 5);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setSyncWords                                        */
/*                                                                              */
/* DESCRIPTION:      Caution doesnt set sync word len in Header Control 2 0x33  */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setSyncWords(const uint8_t* syncWords, uint8_t len)
{
    hoperf_spiBurstWrite(HOPERF_REG_36_SYNC_WORD3, syncWords, len);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setHeaderTo                                         */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setHeaderTo(uint8_t to)
{
	hoperf_write(HOPERF_REG_3A_TRANSMIT_HEADER3, to);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setHeaderFrom                                       */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setHeaderFrom(uint8_t from)
{
	hoperf_write(HOPERF_REG_3B_TRANSMIT_HEADER2, from);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setHeaderId                                         */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setHeaderId(uint8_t id)
{
    hoperf_write(HOPERF_REG_3C_TRANSMIT_HEADER1, id);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setHeaderFlags                                      */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setHeaderFlags(uint8_t flags)
{
	hoperf_write(HOPERF_REG_3D_TRANSMIT_HEADER0, flags);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_headerTo                                            */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_headerTo(void)
{
    return hoperf_read(HOPERF_REG_47_RECEIVED_HEADER3);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_headerFrom                                          */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_headerFrom(void)
{
    return hoperf_read(HOPERF_REG_48_RECEIVED_HEADER2);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_headerId                                            */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_headerId(void)
{
    return hoperf_read(HOPERF_REG_49_RECEIVED_HEADER1);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_headerFlags                                         */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t hoperf_headerFlags(void)
{
    return hoperf_read(HOPERF_REG_4A_RECEIVED_HEADER0);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Get_Ch_Quality_Hoperf                                      */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t Get_Ch_Quality_Hoperf(void)
{
    return hoperf_Rssi;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setPromiscuous                                      */
/*                                                                              */
/* DESCRIPTION:      Caution doesnt set sync word len in Header Control 2 0x33  */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setPromiscuous(uint8_t promiscuous)
{
	hoperf_write(HOPERF_REG_43_HEADER_ENABLE3, promiscuous ? 0x00 : 0xff);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_spiBurstRead                                        */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len)
{
	HOPERF_CSNL;
	SPI2_RW(reg & ~HOPERF_SPI_WRITE_MASK); // Send the start address with the write mask off
	while (len--)
	    *dest++ = SPI2_RW(0);
	HOPERF_CSNH;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_spiBurstWrite                                       */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len)
{
	HOPERF_CSNL;
	SPI2_RW(reg | HOPERF_SPI_WRITE_MASK); // Send the start address with the write mask on
	while (len--)
		SPI2_RW(*src++);
	HOPERF_CSNH;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setPreambleLength                                   */
/*                                                                              */
/* DESCRIPTION:      top bit is in Header Control 2 0x33                        */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setPreambleLength(uint8_t nibbles)
{
	hoperf_write(HOPERF_REG_34_PREAMBLE_LENGTH, nibbles);
}

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
void RxDataWireless_Hoperf(uint8_t *buff)
{
	uint8_t len = hoperf_read(HOPERF_REG_4B_RECEIVED_PACKET_LENGTH);

	// May have already read one or more fragments
	// Get any remaining unread octets, based on the expected length
	// First make sure we dont overflow the buffer in the case of a stupid length
	// or partial bad receives
	if (len >  HOPERF_MAX_MESSAGE_LEN_RX)
	{
		return; // Hmmm receiver buffer overflow.
	}
	else
	{
		hoperf_spiBurstRead(HOPERF_REG_7F_FIFO_ACCESS, buff, len);
	}
	hoperf_resetRxFifo();
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             TxDataWireless_Hoperf                                      */
/*                                                                              */
/* DESCRIPTION:                                                                 */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void TxDataWireless_Hoperf(const uint8_t* data)
{
	hoperf_spiBurstWrite(HOPERF_REG_7F_FIFO_ACCESS, data, HOPERF_MAX_MESSAGE_LEN_TX);
	hoperf_write(HOPERF_REG_3E_PACKET_LENGTH, HOPERF_MAX_MESSAGE_LEN_TX);
	Tx_Mode_Hoperf();
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_setMode                                             */
/*                                                                              */
/* DESCRIPTION:      Sets mode                                                  */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_setMode(uint8_t mode)
{
	#if(HOPERF_AIRRATE == RATE_VERYSLOW)
	hoperf_setModemConfig(GFSK_Rb2_4Fd36);
	#elif(HOPERF_AIRRATE == RATE_SLOW)
	hoperf_setModemConfig(GFSK_Rb4_8Fd45);
	#elif(HOPERF_AIRRATE == RATE_FAST)
	hoperf_setModemConfig(GFSK_Rb125Fd125);
	#endif
	///* For 50KHz f deviation Fd = 625Hz * fd[7:0] */
	//hoperf_write(HOPERF_REG_72_FREQUENCY_DEVIATION, 0x48);
    hoperf_write(HOPERF_REG_07_OPERATING_MODE1, (mode | HOPERF_XTON));
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Rx_Mode_Hoperf                                             */
/*                                                                              */
/* DESCRIPTION:      Set Rx Mode                                                */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Rx_Mode_Hoperf(void)
{
	hoperf_setMode(HOPERF_RXON);
	//hoperf_resetRxFifo();
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Tx_Mode_Hoperf                                             */
/*                                                                              */
/* DESCRIPTION:      Set Mode                                                   */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Tx_Mode_Hoperf(void)
{
	hoperf_setMode(HOPERF_TXON);
	// Hmmm, if you dont clear the RX FIFO here, then it appears that going
	// to transmit mode in the middle of a receive can corrupt the
	// RX FIFO
	hoperf_resetRxFifo();
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Channel_Hoperf                                         */
/*                                                                              */
/* DESCRIPTION:      Sets desired channel                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Set_Channel_Hoperf(uint8_t channel)
{
	hoperf_setFrequency(hoperf_channel_list[channel], 0.05);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Address_Hoperf                                         */
/*                                                                              */
/* DESCRIPTION:      Sets desired address                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void Set_Address_Hoperf(uint8_t address)
{
	// Check the TO header against HOPERF_DEFAULT_NODE_ADDRESS
	hoperf_write(HOPERF_REG_3F_CHECK_HEADER3, address);
	// Set the default transmit header values
	hoperf_setHeaderTo(address);
	hoperf_setHeaderFrom(address);
	hoperf_setHeaderId(0);
	hoperf_setHeaderFlags(0);
	current_address = address;
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_resetFifos                                          */
/*                                                                              */
/* DESCRIPTION:      Set Tx Mode                                                */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_resetFifos(void)
{
    hoperf_write(HOPERF_REG_08_OPERATING_MODE2, HOPERF_FFCLRRX | HOPERF_FFCLRTX);
    hoperf_write(HOPERF_REG_08_OPERATING_MODE2, 0);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_resetRxFifo                                         */
/*                                                                              */
/* DESCRIPTION:      Reset Rx Fifo                                              */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_resetRxFifo(void)
{
    hoperf_write(HOPERF_REG_08_OPERATING_MODE2, HOPERF_FFCLRRX);
    hoperf_write(HOPERF_REG_08_OPERATING_MODE2, 0);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_resetTxFifo                                         */
/*                                                                              */
/* DESCRIPTION:      Reset Tx Fifo                                              */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_resetTxFifo(void)
{
    hoperf_write(HOPERF_REG_08_OPERATING_MODE2, HOPERF_FFCLRTX);
    hoperf_write(HOPERF_REG_08_OPERATING_MODE2, 0);
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI2_RW                                                    */
/*                                                                              */
/* DESCRIPTION:      Writes one byte to hoperf, and return the byte read        */
/*                   from hoperf during write, according to SPI protocol        */
/*                                                                              */
/* PARAMETERS:       In: byte                                                   */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI2_RW(uint8_t byte)
{
	/* Hardware SPI */
	byte = ReadByteSPI2(byte);
    return(byte);           		  		// return read byte
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI2_RW_Reg                                                */
/*                                                                              */
/* DESCRIPTION:      Writes value 'value' to hoperf register 'reg'              */
/*                                                                              */
/* PARAMETERS:       In:  reg: Register Number                                  */
/*                        value: Value                                          */
/*                   Out: status                                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI2_RW_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	HOPERF_CSNL;
	status = SPI2_RW(reg & ~HOPERF_SPI_WRITE_MASK);      // select register
  	SPI2_RW(value);             // ..and write value to it..
  	HOPERF_CSNH;
  	return(status);            // return hoperf status byte
}

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             SPI2_Read                                                  */
/*                                                                              */
/* DESCRIPTION:      Reads one byte from hoperf register 'reg'                  */
/*                                                                              */
/* PARAMETERS:       In: reg: Register Number                                   */
/*                   Out: value read                                            */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
uint8_t SPI2_Read(uint8_t reg)
{
	uint8_t reg_val;
	HOPERF_CSNL;
  	SPI2_RW(reg & ~HOPERF_SPI_WRITE_MASK);            // Select register to read from..
  	reg_val = SPI2_RW(0);    // ..then read register value
  	HOPERF_CSNH;
  	return(reg_val);        // return register value
}

/*
 * hw_hoperf.c
 *
 *  Created on: 09/06/2013
 *      Author: arigita
 */


