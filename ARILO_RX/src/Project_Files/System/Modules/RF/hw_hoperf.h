/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hw_hoperf.h                                                */
/*                                                                              */
/* DESCRIPTION:      Control Transceiver RFM23bp                                */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/

#ifndef HW_HOPERF_H_
#define HW_HOPERF_H_

/*------------------------------------------------------------------------------*/
/* Header-Files (#include)                                                      */
/*------------------------------------------------------------------------------*/
#include "Arilo_Types.h"

/*------------------------------------------------------------------------------*/
/* Defines                                                                      */
/*------------------------------------------------------------------------------*/

/* Rates */
#define RATE_VERYSLOW 1
#define RATE_SLOW     2
#define RATE_FAST     3

// This is the bit in the SPI address that marks it as a write
#define HOPERF_SPI_WRITE_MASK 0x80

// This is the maximum message length that can be supported by this library. Limited by
// the single message length octet in the header.
// Yes, 255 is correct even though the FIFO size in the HOPERF is only
// 64 octets. We use interrupts to refill the Tx FIFO during transmission and to empty the
// Rx FIFO during reception
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
#define HOPERF_MAX_MESSAGE_LEN_TX SL_TX_PLOAD_WIDTH
#define HOPERF_MAX_MESSAGE_LEN_RX SL_RX_PLOAD_WIDTH

// Max number of octets the HOPERF Rx and Tx FIFOs can hold
#define HOPERF_FIFO_SIZE 64

// Keep track of the mode the HOPERF is in
#define HOPERF_MODE_IDLE         0
#define HOPERF_MODE_RX           1
#define HOPERF_MODE_TX           2

// These values we set for FIFO thresholds (4, 55) are actually the same as the POR values
#define HOPERF_TXFFAEM_THRESHOLD 4
#define HOPERF_RXFFAFULL_THRESHOLD 55

// This is the default node address,
#define HOPERF_DEFAULT_NODE_ADDRESS 0x12

// This address in the TO addreess signifies a broadcast
#define HOPERF_BROADCAST_ADDRESS 0xff

// Number of registers to be passed to setModemConfig()
#define HOPERF_NUM_MODEM_CONFIG_REGS 18

// Register names
#define HOPERF_REG_00_DEVICE_TYPE                         0x00
#define HOPERF_REG_01_VERSION_CODE                        0x01
#define HOPERF_REG_02_DEVICE_STATUS                       0x02
#define HOPERF_REG_03_INTERRUPT_STATUS1                   0x03
#define HOPERF_REG_04_INTERRUPT_STATUS2                   0x04
#define HOPERF_REG_05_INTERRUPT_ENABLE1                   0x05
#define HOPERF_REG_06_INTERRUPT_ENABLE2                   0x06
#define HOPERF_REG_07_OPERATING_MODE1                     0x07
#define HOPERF_REG_08_OPERATING_MODE2                     0x08
#define HOPERF_REG_09_OSCILLATOR_LOAD_CAPACITANCE         0x09
#define HOPERF_REG_0A_UC_OUTPUT_CLOCK                     0x0a
#define HOPERF_REG_0B_GPIO_CONFIGURATION0                 0x0b
#define HOPERF_REG_0C_GPIO_CONFIGURATION1                 0x0c
#define HOPERF_REG_0D_GPIO_CONFIGURATION2                 0x0d
#define HOPERF_REG_0E_IO_PORT_CONFIGURATION               0x0e
#define HOPERF_REG_0F_ADC_CONFIGURATION                   0x0f
#define HOPERF_REG_10_ADC_SENSOR_AMP_OFFSET               0x10
#define HOPERF_REG_11_ADC_VALUE                           0x11
#define HOPERF_REG_12_TEMPERATURE_SENSOR_CALIBRATION      0x12
#define HOPERF_REG_13_TEMPERATURE_VALUE_OFFSET            0x13
#define HOPERF_REG_14_WAKEUP_TIMER_PERIOD1                0x14
#define HOPERF_REG_15_WAKEUP_TIMER_PERIOD2                0x15
#define HOPERF_REG_16_WAKEUP_TIMER_PERIOD3                0x16
#define HOPERF_REG_17_WAKEUP_TIMER_VALUE1                 0x17
#define HOPERF_REG_18_WAKEUP_TIMER_VALUE2                 0x18
#define HOPERF_REG_19_LDC_MODE_DURATION                   0x19
#define HOPERF_REG_1A_LOW_BATTERY_DETECTOR_THRESHOLD      0x1a
#define HOPERF_REG_1B_BATTERY_VOLTAGE_LEVEL               0x1b
#define HOPERF_REG_1C_IF_FILTER_BANDWIDTH                 0x1c
#define HOPERF_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE         0x1d
#define HOPERF_REG_1E_AFC_TIMING_CONTROL                  0x1e
#define HOPERF_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE   0x1f
#define HOPERF_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE    0x20
#define HOPERF_REG_21_CLOCK_RECOVERY_OFFSET2              0x21
#define HOPERF_REG_22_CLOCK_RECOVERY_OFFSET1              0x22
#define HOPERF_REG_23_CLOCK_RECOVERY_OFFSET0              0x23
#define HOPERF_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1    0x24
#define HOPERF_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0    0x25
#define HOPERF_REG_26_RSSI                                0x26
#define HOPERF_REG_27_RSSI_THRESHOLD                      0x27
#define HOPERF_REG_28_ANTENNA_DIVERSITY1                  0x28
#define HOPERF_REG_29_ANTENNA_DIVERSITY2                  0x29
#define HOPERF_REG_2A_AFC_LIMITER                         0x2a
#define HOPERF_REG_2B_AFC_CORRECTION_READ                 0x2b
#define HOPERF_REG_2C_OOK_COUNTER_VALUE_1                 0x2c
#define HOPERF_REG_2D_OOK_COUNTER_VALUE_2                 0x2d
#define HOPERF_REG_2E_SLICER_PEAK_HOLD                    0x2e
#define HOPERF_REG_30_DATA_ACCESS_CONTROL                 0x30
#define HOPERF_REG_31_EZMAC_STATUS                        0x31
#define HOPERF_REG_32_HEADER_CONTROL1                     0x32
#define HOPERF_REG_33_HEADER_CONTROL2                     0x33
#define HOPERF_REG_34_PREAMBLE_LENGTH                     0x34
#define HOPERF_REG_35_PREAMBLE_DETECTION_CONTROL1         0x35
#define HOPERF_REG_36_SYNC_WORD3                          0x36
#define HOPERF_REG_37_SYNC_WORD2                          0x37
#define HOPERF_REG_38_SYNC_WORD1                          0x38
#define HOPERF_REG_39_SYNC_WORD0                          0x39
#define HOPERF_REG_3A_TRANSMIT_HEADER3                    0x3a
#define HOPERF_REG_3B_TRANSMIT_HEADER2                    0x3b
#define HOPERF_REG_3C_TRANSMIT_HEADER1                    0x3c
#define HOPERF_REG_3D_TRANSMIT_HEADER0                    0x3d
#define HOPERF_REG_3E_PACKET_LENGTH                       0x3e
#define HOPERF_REG_3F_CHECK_HEADER3                       0x3f
#define HOPERF_REG_40_CHECK_HEADER2                       0x40
#define HOPERF_REG_41_CHECK_HEADER1                       0x41
#define HOPERF_REG_42_CHECK_HEADER0                       0x42
#define HOPERF_REG_43_HEADER_ENABLE3                      0x43
#define HOPERF_REG_44_HEADER_ENABLE2                      0x44
#define HOPERF_REG_45_HEADER_ENABLE1                      0x45
#define HOPERF_REG_46_HEADER_ENABLE0                      0x46
#define HOPERF_REG_47_RECEIVED_HEADER3                    0x47
#define HOPERF_REG_48_RECEIVED_HEADER2                    0x48
#define HOPERF_REG_49_RECEIVED_HEADER1                    0x49
#define HOPERF_REG_4A_RECEIVED_HEADER0                    0x4a
#define HOPERF_REG_4B_RECEIVED_PACKET_LENGTH              0x4b
#define HOPERF_REG_50_ANALOG_TEST_BUS_SELECT              0x50
#define HOPERF_REG_51_DIGITAL_TEST_BUS_SELECT             0x51
#define HOPERF_REG_52_TX_RAMP_CONTROL                     0x52
#define HOPERF_REG_53_PLL_TUNE_TIME                       0x53
#define HOPERF_REG_55_CALIBRATION_CONTROL                 0x55
#define HOPERF_REG_56_MODEM_TEST                          0x56
#define HOPERF_REG_57_CHARGE_PUMP_TEST                    0x57
#define HOPERF_REG_58_CHARGE_PUMP_CURRENT_TRIMMING        0x58
#define HOPERF_REG_59_DIVIDER_CURRENT_TRIMMING            0x59
#define HOPERF_REG_5A_VCO_CURRENT_TRIMMING                0x5a
#define HOPERF_REG_5B_VCO_CALIBRATION                     0x5b
#define HOPERF_REG_5C_SYNTHESIZER_TEST                    0x5c
#define HOPERF_REG_5D_BLOCK_ENABLE_OVERRIDE1              0x5d
#define HOPERF_REG_5E_BLOCK_ENABLE_OVERRIDE2              0x5e
#define HOPERF_REG_5F_BLOCK_ENABLE_OVERRIDE3              0x5f
#define HOPERF_REG_60_CHANNEL_FILTER_COEFFICIENT_ADDRESS  0x60
#define HOPERF_REG_61_CHANNEL_FILTER_COEFFICIENT_VALUE    0x61
#define HOPERF_REG_62_CRYSTAL_OSCILLATOR_POR_CONTROL      0x62
#define HOPERF_REG_63_RC_OSCILLATOR_COARSE_CALIBRATION    0x63
#define HOPERF_REG_64_RC_OSCILLATOR_FINE_CALIBRATION      0x64
#define HOPERF_REG_65_LDO_CONTROL_OVERRIDE                0x65
#define HOPERF_REG_66_LDO_LEVEL_SETTINGS                  0x66
#define HOPERF_REG_67_DELTA_SIGMA_ADC_TUNING1             0x67
#define HOPERF_REG_68_DELTA_SIGMA_ADC_TUNING2             0x68
#define HOPERF_REG_69_AGC_OVERRIDE1                       0x69
#define HOPERF_REG_6A_AGC_OVERRIDE2                       0x6a
#define HOPERF_REG_6B_GFSK_FIR_FILTER_COEFFICIENT_ADDRESS 0x6b
#define HOPERF_REG_6C_GFSK_FIR_FILTER_COEFFICIENT_VALUE   0x6c
#define HOPERF_REG_6D_TX_POWER                            0x6d
#define HOPERF_REG_6E_TX_DATA_RATE1                       0x6e
#define HOPERF_REG_6F_TX_DATA_RATE0                       0x6f
#define HOPERF_REG_70_MODULATION_CONTROL1                 0x70
#define HOPERF_REG_71_MODULATION_CONTROL2                 0x71
#define HOPERF_REG_72_FREQUENCY_DEVIATION                 0x72
#define HOPERF_REG_73_FREQUENCY_OFFSET1                   0x73
#define HOPERF_REG_74_FREQUENCY_OFFSET2                   0x74
#define HOPERF_REG_75_FREQUENCY_BAND_SELECT               0x75
#define HOPERF_REG_76_NOMINAL_CARRIER_FREQUENCY1          0x76
#define HOPERF_REG_77_NOMINAL_CARRIER_FREQUENCY0          0x77
#define HOPERF_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT    0x79
#define HOPERF_REG_7A_FREQUENCY_HOPPING_STEP_SIZE         0x7a
#define HOPERF_REG_7C_TX_FIFO_CONTROL1                    0x7c
#define HOPERF_REG_7D_TX_FIFO_CONTROL2                    0x7d
#define HOPERF_REG_7E_RX_FIFO_CONTROL                     0x7e
#define HOPERF_REG_7F_FIFO_ACCESS                         0x7f

// These register masks etc are named wherever possible
// corresponding to the bit and field names in the RF-22 Manual
// HOPERF_REG_00_DEVICE_TYPE                      0x00
#define HOPERF_DEVICE_TYPE_RX_TRX                 0x08
#define HOPERF_DEVICE_TYPE_TX                     0x07

// HOPERF_REG_02_DEVICE_STATUS                    0x02
#define HOPERF_FFOVL                              0x80
#define HOPERF_FFUNFL                             0x40
#define HOPERF_RXFFEM                             0x20
#define HOPERF_HEADERR                            0x10
#define HOPERF_FREQERR                            0x08
#define HOPERF_LOCKDET                            0x04
#define HOPERF_CPS                                0x03
#define HOPERF_CPS_IDLE                           0x00
#define HOPERF_CPS_RX                             0x01
#define HOPERF_CPS_TX                             0x10

// HOPERF_REG_03_INTERRUPT_STATUS1                0x03
#define HOPERF_IFFERROR                           0x80
#define HOPERF_ITXFFAFULL                         0x40
#define HOPERF_ITXFFAEM                           0x20
#define HOPERF_IRXFFAFULL                         0x10
#define HOPERF_IEXT                               0x08
#define HOPERF_IPKSENT                            0x04
#define HOPERF_IPKVALID                           0x02
#define HOPERF_ICRCERROR                          0x01

// HOPERF_REG_04_INTERRUPT_STATUS2                0x04
#define HOPERF_ISWDET                             0x80
#define HOPERF_IPREAVAL                           0x40
#define HOPERF_IPREAINVAL                         0x20
#define HOPERF_IRSSI                              0x10
#define HOPERF_IWUT                               0x08
#define HOPERF_ILBD                               0x04
#define HOPERF_ICHIPRDY                           0x02
#define HOPERF_IPOR                               0x01

// HOPERF_REG_05_INTERRUPT_ENABLE1                0x05
#define HOPERF_ENFFERR                            0x80
#define HOPERF_ENTXFFAFULL                        0x40
#define HOPERF_ENTXFFAEM                          0x20
#define HOPERF_ENRXFFAFULL                        0x10
#define HOPERF_ENEXT                              0x08
#define HOPERF_ENPKSENT                           0x04
#define HOPERF_ENPKVALID                          0x02
#define HOPERF_ENCRCERROR                         0x01

// HOPERF_REG_06_INTERRUPT_ENABLE2                0x06
#define HOPERF_ENSWDET                            0x80
#define HOPERF_ENPREAVAL                          0x40
#define HOPERF_ENPREAINVAL                        0x20
#define HOPERF_ENRSSI                             0x10
#define HOPERF_ENWUT                              0x08
#define HOPERF_ENLBDI                             0x04
#define HOPERF_ENCHIPRDY                          0x02
#define HOPERF_ENPOR                              0x01

// HOPERF_REG_07_OPERATING_MODE                   0x07
#define HOPERF_SWRES                              0x80
#define HOPERF_ENLBD                              0x40
#define HOPERF_ENWT                               0x20
#define HOPERF_X32KSEL                            0x10
#define HOPERF_TXON                               0x08
#define HOPERF_RXON                               0x04
#define HOPERF_PLLON                              0x02
#define HOPERF_XTON                               0x01

// HOPERF_REG_08_OPERATING_MODE2                  0x08
#define HOPERF_ANTDIV                             0xc0
#define HOPERF_RXMPK                              0x10
#define HOPERF_AUTOTX                             0x08
#define HOPERF_ENLDM                              0x04
#define HOPERF_FFCLRRX                            0x02
#define HOPERF_FFCLRTX                            0x01

// HOPERF_REG_0F_ADC_CONFIGURATION                0x0f
#define HOPERF_ADCSTART                           0x80
#define HOPERF_ADCDONE                            0x80
#define HOPERF_ADCSEL                             0x70
#define HOPERF_ADCSEL_INTERNAL_TEMPERATURE_SENSOR 0x00
#define HOPERF_ADCSEL_GPIO0_SINGLE_ENDED          0x10
#define HOPERF_ADCSEL_GPIO1_SINGLE_ENDED          0x20
#define HOPERF_ADCSEL_GPIO2_SINGLE_ENDED          0x30
#define HOPERF_ADCSEL_GPIO0_GPIO1_DIFFERENTIAL    0x40
#define HOPERF_ADCSEL_GPIO1_GPIO2_DIFFERENTIAL    0x50
#define HOPERF_ADCSEL_GPIO0_GPIO2_DIFFERENTIAL    0x60
#define HOPERF_ADCSEL_GND                         0x70
#define HOPERF_ADCREF                             0x0c
#define HOPERF_ADCREF_BANDGAP_VOLTAGE             0x00
#define HOPERF_ADCREF_VDD_ON_3                    0x08
#define HOPERF_ADCREF_VDD_ON_2                    0x0c
#define HOPERF_ADCGAIN                            0x03

// HOPERF_REG_10_ADC_SENSOR_AMP_OFFSET            0x10
#define HOPERF_ADCOFFS                            0x0f

// HOPERF_REG_12_TEMPERATURE_SENSOR_CALIBRATION   0x12
#define HOPERF_TSRANGE                            0xc0
#define HOPERF_TSRANGE_M64_64C                    0x00
#define HOPERF_TSRANGE_M64_192C                   0x40
#define HOPERF_TSRANGE_0_128C                     0x80
#define HOPERF_TSRANGE_M40_216F                   0xc0
#define HOPERF_ENTSOFFS                           0x20
#define HOPERF_ENTSTRIM                           0x10
#define HOPERF_TSTRIM                             0x0f

// HOPERF_REG_14_WAKEUP_TIMER_PERIOD1             0x14
#define HOPERF_WTR                                0x3c
#define HOPERF_WTD                                0x03

// HOPERF_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE      0x1d
#define HOPERF_AFBCD                              0x80
#define HOPERF_ENAFC                              0x40
#define HOPERF_AFCGEARH                           0x38
#define HOPERF_AFCGEARL                           0x07

// HOPERF_REG_1E_AFC_TIMING_CONTROL               0x1e
#define HOPERF_SWAIT_TIMER                        0xc0
#define HOPERF_SHWAIT                             0x38
#define HOPERF_ANWAIT                             0x07

// HOPERF_REG_30_DATA_ACCESS_CONTROL              0x30
#define HOPERF_ENPACRX                            0x80
#define HOPERF_MSBFRST                            0x00
#define HOPERF_LSBFRST                            0x40
#define HOPERF_CRCHDRS                            0x00
#define HOPERF_CRCDONLY                           0x20
#define HOPERF_ENPACTX                            0x08
#define HOPERF_ENCRC                              0x04
#define HOPERF_CRC                                0x03
#define HOPERF_CRC_CCITT                          0x00
#define HOPERF_CRC_CRC_16_IBM                     0x01
#define HOPERF_CRC_IEC_16                         0x02
#define HOPERF_CRC_BIACHEVA                       0x03

// HOPERF_REG_32_HEADER_CONTROL1                  0x32
#define HOPERF_BCEN                               0xf0
#define HOPERF_BCEN_NONE                          0x00
#define HOPERF_BCEN_HEADER0                       0x10
#define HOPERF_BCEN_HEADER1                       0x20
#define HOPERF_BCEN_HEADER2                       0x40
#define HOPERF_BCEN_HEADER3                       0x80
#define HOPERF_HDCH                               0x0f
#define HOPERF_HDCH_NONE                          0x00
#define HOPERF_HDCH_HEADER0                       0x01
#define HOPERF_HDCH_HEADER1                       0x02
#define HOPERF_HDCH_HEADER2                       0x04
#define HOPERF_HDCH_HEADER3                       0x08

// HOPERF_REG_33_HEADER_CONTROL2                  0x33
#define HOPERF_HDLEN                              0x70
#define HOPERF_HDLEN_0                            0x00
#define HOPERF_HDLEN_1                            0x10
#define HOPERF_HDLEN_2                            0x20
#define HOPERF_HDLEN_3                            0x30
#define HOPERF_HDLEN_4                            0x40
#define HOPERF_VARPKLEN                           0x00
#define HOPERF_FIXPKLEN                           0x08
#define HOPERF_SYNCLEN                            0x06
#define HOPERF_SYNCLEN_1                          0x00
#define HOPERF_SYNCLEN_2                          0x02
#define HOPERF_SYNCLEN_3                          0x04
#define HOPERF_SYNCLEN_4                          0x06
#define HOPERF_PREALEN8                           0x01

// HOPERF_REG_6D_TX_POWER                         0x6d
#define HOPERF_TXPOW                              0x07
#define HOPERF_TXPOW_4X31                         0x08 // Not used in RFM22B
#define HOPERF_TXPOW_1DBM                         0x00
#define HOPERF_TXPOW_2DBM                         0x01
#define HOPERF_TXPOW_5DBM                         0x02
#define HOPERF_TXPOW_8DBM                         0x03
#define HOPERF_TXPOW_11DBM                        0x04
#define HOPERF_TXPOW_14DBM                        0x05
#define HOPERF_TXPOW_17DBM                        0x06
#define HOPERF_TXPOW_20DBM                        0x07
// IN RFM23B
#define HOPERF_TXPOW_LNA_SW                       0x08

// HOPERF_REG_71_MODULATION_CONTROL2              0x71
#define HOPERF_TRCLK                              0xc0
#define HOPERF_TRCLK_NONE                         0x00
#define HOPERF_TRCLK_GPIO                         0x40
#define HOPERF_TRCLK_SDO                          0x80
#define HOPERF_TRCLK_NIRQ                         0xc0
#define HOPERF_DTMOD                              0x30
#define HOPERF_DTMOD_DIRECT_GPIO                  0x00
#define HOPERF_DTMOD_DIRECT_SDI                   0x10
#define HOPERF_DTMOD_FIFO                         0x20
#define HOPERF_DTMOD_PN9                          0x30
#define HOPERF_ENINV                              0x08
#define HOPERF_FD8                                0x04
#define HOPERF_MODTYP                             0x30
#define HOPERF_MODTYP_UNMODULATED                 0x00
#define HOPERF_MODTYP_OOK                         0x01
#define HOPERF_MODTYP_FSK                         0x02
#define HOPERF_MODTYP_GFSK                        0x03

// HOPERF_REG_75_FREQUENCY_BAND_SELECT            0x75
#define HOPERF_SBSEL                              0x40
#define HOPERF_HBSEL                              0x20
#define HOPERF_FB                                 0x1f

// Define this to include Serial printing in diagnostic routines
#define HOPERF_HAVE_SERIAL

/// \brief Defines register values for a set of modem configuration registers
///
/// Defines register values for a set of modem configuration registers
/// that can be passed to setModemConfig()
/// if none of the choices in ModemConfigChoice suit your need
/// setModemConfig() writes the register values to the appropriate RF22 registers
/// to set the desired modulation type, data rate and deviation/bandwidth.
/// Suitable values for these registers can be computed using the register calculator at
/// http://www.hoperf.com/upload/rf/RF22B%2023B%2031B%2042B%2043B%20Register%20Settings_RevB1-v5.xls
typedef struct
{
	uint8_t    reg_1c;   ///< Value for register RF22_REG_1C_IF_FILTER_BANDWIDTH
	uint8_t    reg_1f;   ///< Value for register RF22_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE
	uint8_t    reg_20;   ///< Value for register RF22_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE
	uint8_t    reg_21;   ///< Value for register RF22_REG_21_CLOCK_RECOVERY_OFFSET2
	uint8_t    reg_22;   ///< Value for register RF22_REG_22_CLOCK_RECOVERY_OFFSET1
	uint8_t    reg_23;   ///< Value for register RF22_REG_23_CLOCK_RECOVERY_OFFSET0
	uint8_t    reg_24;   ///< Value for register RF22_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1
	uint8_t    reg_25;   ///< Value for register RF22_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0
	uint8_t    reg_2c;   ///< Value for register RF22_REG_2C_OOK_COUNTER_VALUE_1
	uint8_t    reg_2d;   ///< Value for register RF22_REG_2D_OOK_COUNTER_VALUE_2
	uint8_t    reg_2e;   ///< Value for register RF22_REG_2E_SLICER_PEAK_HOLD
	uint8_t    reg_58;   ///< Value for register RF22_REG_58_CHARGE_PUMP_CURRENT_TRIMMING
	uint8_t    reg_69;   ///< Value for register RF22_REG_69_AGC_OVERRIDE1
	uint8_t    reg_6e;   ///< Value for register RF22_REG_6E_TX_DATA_RATE1
	uint8_t    reg_6f;   ///< Value for register RF22_REG_6F_TX_DATA_RATE0
	uint8_t    reg_70;   ///< Value for register RF22_REG_70_MODULATION_CONTROL1
	uint8_t    reg_71;   ///< Value for register RF22_REG_71_MODULATION_CONTROL2
	uint8_t    reg_72;   ///< Value for register RF22_REG_72_FREQUENCY_DEVIATION
} ModemConfig;

/// Choices for setModemConfig() for a selected subset of common modulation types,
/// and data rates. If you need another configuration, use the register calculator.
/// and call setModemRegisters() with your desired settings
/// These are indexes into _modemConfig
typedef enum
{
	UnmodulatedCarrier = 0, ///< Unmodulated carrier for testing
	FSK_PN9_Rb2Fd5,      ///< FSK, No Manchester, Rb = 2kbs, Fd = 5kHz, PN9 random modulation for testing

	FSK_Rb2Fd5,	     ///< FSK, No Manchester, Rb = 2kbs,    Fd = 5kHz
	FSK_Rb2_4Fd36,       ///< FSK, No Manchester, Rb = 2.4kbs,  Fd = 36kHz
	FSK_Rb4_8Fd45,       ///< FSK, No Manchester, Rb = 4.8kbs,  Fd = 45kHz
	FSK_Rb9_6Fd45,       ///< FSK, No Manchester, Rb = 9.6kbs,  Fd = 45kHz
	FSK_Rb19_2Fd9_6,     ///< FSK, No Manchester, Rb = 19.2kbs, Fd = 9.6kHz
	FSK_Rb38_4Fd19_6,    ///< FSK, No Manchester, Rb = 38.4kbs, Fd = 19.6kHz
	FSK_Rb57_6Fd28_8,    ///< FSK, No Manchester, Rb = 57.6kbs, Fd = 28.8kHz
	FSK_Rb125Fd125,      ///< FSK, No Manchester, Rb = 125kbs,  Fd = 125kHz

	GFSK_Rb2Fd5,         ///< GFSK, No Manchester, Rb = 2kbs,    Fd = 5kHz
	GFSK_Rb2_4Fd36,      ///< GFSK, No Manchester, Rb = 2.4kbs,  Fd = 36kHz
	GFSK_Rb4_8Fd45,      ///< GFSK, No Manchester, Rb = 4.8kbs,  Fd = 45kHz
	GFSK_Rb9_6Fd45,      ///< GFSK, No Manchester, Rb = 9.6kbs,  Fd = 45kHz
	GFSK_Rb19_2Fd9_6,    ///< GFSK, No Manchester, Rb = 19.2kbs, Fd = 9.6kHz
	GFSK_Rb38_4Fd19_6,   ///< GFSK, No Manchester, Rb = 38.4kbs, Fd = 19.6kHz
	GFSK_Rb57_6Fd28_8,   ///< GFSK, No Manchester, Rb = 57.6kbs, Fd = 28.8kHz
	GFSK_Rb125Fd125,     ///< GFSK, No Manchester, Rb = 125kbs,  Fd = 125kHz

	OOK_Rb1_2Bw75,       ///< OOK, No Manchester, Rb = 1.2kbs,  Rx Bandwidth = 75kHz
	OOK_Rb2_4Bw335,      ///< OOK, No Manchester, Rb = 2.4kbs,  Rx Bandwidth = 335kHz
	OOK_Rb4_8Bw335,      ///< OOK, No Manchester, Rb = 4.8kbs,  Rx Bandwidth = 335kHz
	OOK_Rb9_6Bw335,      ///< OOK, No Manchester, Rb = 9.6kbs,  Rx Bandwidth = 335kHz
	OOK_Rb19_2Bw335,     ///< OOK, No Manchester, Rb = 19.2kbs, Rx Bandwidth = 335kHz
	OOK_Rb38_4Bw335,     ///< OOK, No Manchester, Rb = 38.4kbs, Rx Bandwidth = 335kHz
	OOK_Rb40Bw335        ///< OOK, No Manchester, Rb = 40kbs,   Rx Bandwidth = 335kHz
} ModemConfigChoice;

extern uint8_t hoperf_Rssi;

extern uint8_t hoperf_temperature;

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
unsigned int hoperf_write(unsigned int addr, unsigned int val);

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
unsigned int hoperf_read(unsigned int addr);

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
extern uint8_t hoperf_temp_get(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             hoperf_reset                                               */
/*                                                                              */
/* DESCRIPTION:      Reset function hoperf                                      */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
void hoperf_reset(void);

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
extern uint8_t Init_Hoperf(void);

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
extern void DeInit_Hoperf(void);

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
uint8_t hoperf_statushandler(void);

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
extern uint8_t Comm_Ready_Hoperf(void);

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
extern uint8_t Sent_Done_Hoperf(void);

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
void hoperf_setTxPower(uint8_t power);

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
uint8_t hoperf_setFrequency(float centre, float afcPullInRange);

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
uint8_t hoperf_statusRead();

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
uint8_t hoperf_setModemConfig(ModemConfigChoice index);

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
void hoperf_setModemRegisters(const ModemConfig* config);

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
void hoperf_setSyncWords(const uint8_t* syncWords, uint8_t len);

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
void hoperf_setHeaderTo(uint8_t to);

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
void hoperf_setHeaderFrom(uint8_t from);

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
void hoperf_setHeaderId(uint8_t id);

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
void hoperf_setHeaderFlags(uint8_t flags);

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
uint8_t hoperf_headerTo(void);

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
uint8_t hoperf_headerFrom(void);

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
uint8_t hoperf_headerId(void);

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
uint8_t hoperf_headerFlags(void);

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
uint8_t Get_Ch_Quality_Hoperf(void);

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
void hoperf_setPromiscuous(uint8_t promiscuous);

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
void hoperf_spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len);

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
void hoperf_spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len);

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
void hoperf_setPreambleLength(uint8_t nibbles);

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
extern void RxDataWireless_Hoperf(uint8_t *buff);

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
extern void TxDataWireless_Hoperf(const uint8_t* data);

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
void hoperf_setMode(uint8_t mode);

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
extern void Rx_Mode_Hoperf(void);

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
extern void Tx_Mode_Hoperf(void);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Channel_Hoperf                                         */
/*                                                                              */
/* DESCRIPTION:      Sets desired channel                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Set_Channel_Hoperf(uint8_t channel);

/*------------------------------------------------------------------------------*/
/*                                                                              */
/* NAME:             Set_Address_Hoperf                                         */
/*                                                                              */
/* DESCRIPTION:      Sets desired address                                       */
/*                                                                              */
/* VERSION:          1.0                                                        */
/*                                                                              */
/*------------------------------------------------------------------------------*/
extern void Set_Address_Hoperf(uint8_t address);

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
void hoperf_resetFifos(void);

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
void hoperf_resetRxFifo(void);

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
void hoperf_resetTxFifo(void);

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
uint8_t SPI2_RW(uint8_t byte);

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
uint8_t SPI2_RW_Reg(uint8_t reg, uint8_t value);

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
uint8_t SPI2_Read(uint8_t reg);

#endif /* HW_HOPERF_H_ */

/*
 * hw_hoperf.h
 *
 *  Created on: 09/06/2013
 *      Author: arigita
 */

