/*
 *  Copyright (c) 2015 Warren J. Jasper <wjasper@ncsu.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef USB_TEMP_H
#define USB_TEMP_H

#ifdef __cplusplus
extern "C" {
#endif

#define USB_TEMP_PID (0x008D)

#define USB_TEMP_DIO_DIR_IN  (0x01)
#define USB_TEMP_DIO_DIR_OUT (0x00)


// Channels
#define USB_TEMP_CH0  (0x0)  // Channel 0
#define USB_TEMP_CH1  (0x1)  // Channel 1
#define USB_TEMP_CH2  (0x2)  // Channel 2
#define USB_TEMP_CH3  (0x3)  // Channel 3
#define USB_TEMP_CH4  (0x4)  // Channel 4
#define USB_TEMP_CH5  (0x5)  // Channel 5
#define USB_TEMP_CH6  (0x6)  // Channel 6
#define USB_TEMP_CH7  (0x7)  // Channel 7
#define USB_TEMP_CJC0 (0x80) // Cold Junction Compensator 0
#define USB_TEMP_CJC1 (0x81) // Cold Junction Compensator 1

// Configuration Items
#define USB_TEMP_ADC_0 (0x0)  // Setting for ADC 0
#define USB_TEMP_ADC_1 (0x1)  // Setting for ADC 1
#define USB_TEMP_ADC_2 (0x2)  // Setting for ADC 2
#define USB_TEMP_ADC_3 (0x3)  // Setting for ADC 3

// Sub Items
#define USB_TEMP_SENSOR_TYPE     (0x00) // Sensor type  Read Only
#define USB_TEMP_CONNECTION_TYPE (0x01) // Connection type - RTD & Thermistor
#define USB_TEMP_FILTER_RATE     (0x02) // Filter update rate
#define USB_TEMP_EXCITATION      (0x03) // Currect excitation
#define USB_TEMP_VREF            (0x04) // Measured Vref value
#define USB_TEMP_I_value_0       (0x05) // Measured I value @ 10uA
#define USB_TEMP_I_value_1       (0x06) // Measured I value @ 210uA
#define USB_TEMP_I_value_2       (0x07) // Measured I value @ 10uA (3 wire connection)
#define USB_TEMP_V_value_0       (0x08) // Measured V value @ 10uA
#define USB_TEMP_V_value_1       (0x09) // Measured V value @ 210uA
#define USB_TEMP_V_value_2       (0x0a) // Measured V value @ 210uA (3 wire connection)
#define USB_TEMP_CH_0_TC         (0x10) // Thermocouple type for channel 0
#define USB_TEMP_CH_1_TC         (0x11) // Thermocouple type for channel 1
#define USB_TEMP_CH_0_GAIN       (0x12) // Channel 0 gain value
#define USB_TEMP_CH_1_GAIN       (0x13) // Channel 1 gain value
#define USB_TEMP_CH_0_COEF_0     (0x14) // Coefficient 0
#define USB_TEMP_CH_1_COEF_0     (0x15) // Coefficient 0
#define USB_TEMP_CH_0_COEF_1     (0x16) // Coefficient 1
#define USB_TEMP_CH_1_COEF_1     (0x17) // Coefficient 1
#define USB_TEMP_CH_0_COEF_2     (0x18) // Coefficient 2
#define USB_TEMP_CH_1_COEF_2     (0x19) // Coefficient 2
#define USB_TEMP_CH_0_COEF_3     (0x1a) // Coefficient 3
#define USB_TEMP_CH_1_COEF_3     (0x1b) // Coefficient 3

// Possible Values
#define USB_TEMP_RTD           (0x0)
#define USB_TEMP_THERMISTOR    (0x1)
#define USB_TEMP_THERMOCOUPLE  (0x2)
#define USB_TEMP_SEMICONDUCTOR (0x3)
#define USB_TEMP_DISABLED      (0x4)

#define USB_TEMP_FREQ_500_HZ   (0x1)
#define USB_TEMP_FREQ_250_HZ   (0x2)
#define USB_TEMP_FREQ_125_HZ   (0x3)
#define USB_TEMP_FREQ_62_5_HZ  (0x4)
#define USB_TEMP_FREQ_50_HZ    (0x5)
#define USB_TEMP_FREQ_39_2_HZ  (0x6)
#define USB_TEMP_FREQ_33_3_HZ  (0x7)
#define USB_TEMP_FREQ_19_6_HZ  (0x8)
#define USB_TEMP_FREQ_16_7_HZ  (0x9)

//#define USB_TEMP_FREQ_16_7_HZ  (0xa)
#define USB_TEMP_FREQ_12_5_HZ  (0xb)
#define USB_TEMP_FREQ_10_HZ    (0xc)
#define USB_TEMP_FREQ_8_33_HZ  (0xd)
#define USB_TEMP_FREQ_6_25_HZ  (0xe)
#define USB_TEMP_FREQ_4_17_HZ  (0xf)

#define USB_TEMP_GAIN_1X       (0x0)
#define USB_TEMP_GAIN_2X       (0x1)
#define USB_TEMP_GAIN_4X       (0x2)
#define USB_TEMP_GAIN_8X       (0x3)
#define USB_TEMP_GAIN_16X      (0x4)
#define USB_TEMP_GAIN_32X      (0x5)
#define USB_TEMP_GAIN_64X      (0x6)
#define USB_TEMP_GAIN_128X     (0x7)

/* For connection types RTD & thermistor */
#define USB_TEMP_TWO_WIRE_ONE_SENSOR (0x0)
#define USB_TEMP_TWO_WIRE_TWO_SENSOR (0x1)
#define USB_TEMP_THREE_WIRE          (0x2)
#define USB_TEMP_FOUR_WIRE           (0x3)

/* For connection types Semiconductor */
#define USB_TEMP_SINGLE_ENDED        (0x00)
#define USB_TEMP_DIFFERENTIAL        (0x01)  

/* Current excitation values */
#define USB_TEMP_EXCITATION_OFF       (0x0)
#define USB_TEMP_MU_A_10              (0x1)  //  10 micro Amps
#define USB_TEMP_MU_A_210             (0x2)  // 210 micro Amps

/* function prototypes for the USB-TEMP */
void usbDConfigPort_USBTEMP(hid_device *hid, uint8_t direction);
void usbDConfigBit_USBTEMP(hid_device *hid, uint8_t bit_num, uint8_t direction);
void usbDIn_USBTEMP(hid_device *hid, uint8_t* value);
void usbDInBit_USBTEMP(hid_device *hid, uint8_t bit_num, uint8_t* value);
void usbDOut_USBTEMP(hid_device *hid, uint8_t value);
void usbDOutBit_USBTEMP(hid_device *hid, uint8_t bit_num, uint8_t value);
void usbInitCounter_USBTEMP(hid_device *hid);
uint32_t usbReadCounter_USBTEMP(hid_device *hid);
void usbTin_USBTEMP(hid_device *hid, uint8_t channel, uint8_t units, float *value);
void usbTinScan_USBTEMP(hid_device *hid, uint8_t start_chan, uint8_t end_chan, uint8_t units, float value[]);

void usbReadMemory_USBTEMP(hid_device *hid, uint16_t address, uint8_t type, uint8_t count, uint8_t memory[]);
int usbWriteMemory_USBTEMP(hid_device *hid, uint16_t address, uint8_t type, uint8_t count, uint8_t data[]);
void usbBlink_USBTEMP(hid_device *hid);
int usbReset_USBTEMP(hid_device *hid);
uint8_t usbGetStatus_USBTEMP(hid_device *hid);
void usbSetItem_USBTEMP(hid_device *hid, uint8_t item, uint8_t subitem, float fValue);
int usbGetItem_USBTEMP(hid_device *hid, uint8_t item, uint8_t subitem, void* value);
void usbCalibrate_USBTEMP(hid_device *hid);
uint8_t  usbGetBurnoutStatus_USBTEMP(hid_device *hid, uint8_t mask);
void usbPrepareDownload_USBTEMP(hid_device *hid, uint8_t micro);
void usbWriteCode_USBTEMP(hid_device *hid, uint32_t address, uint8_t count, uint8_t data[]);
int usbReadCode_USBTEMP(hid_device *hid, uint32_t address, uint8_t count, uint8_t data[]);
void usbWriteSerial_USBTEMP(hid_device *hid, uint8_t serial[8]);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif //USB_TEMP_H
