/*
 *
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#include "pmd.h"
#include "usb-1608G.h"

#define MAX_COUNT     (0xffff)
#define FALSE 0
#define TRUE 1

/* Test Program */
int toContinue()
{
  int answer;
  answer = 0; //answer = getchar();
  printf("Continue [yY]? ");
  while((answer = getchar()) == '\0' ||
    answer == '\n');
  return ( answer == 'y' || answer == 'Y');
}

int main (int argc, char **argv)
{
  libusb_device_handle *udev = NULL;
  struct tm calDate;
  
  double voltage;
  double frequency, duty_cycle;
  float temperature;
  usbDevice1608G usb1608G;

  int ch;
  int i, j, m, k, nchan, repeats;
  int nread;
  int nScans = 0;
  uint8_t input;
  int temp, ret;
  uint8_t options;
  char serial[9];
  float period;
  uint16_t version;
  uint16_t status;
  int usb1608GX_2AO = FALSE;
  int flag;
  uint16_t value, data;
  uint16_t *sdataIn;         // holds 16 bit unsigned analog input data
  uint16_t dataOut[512];     // holds 16 bit unsigned analog output data

  uint8_t mode, gain, channel;

  udev = NULL;

  ret = libusb_init(NULL);
  if (ret < 0) {
    perror("libusb_init: Failed to initialize libusb");
    exit(1);
  }

  // There are 2 firmware versions of the board.  See MCC for details.
  if ((udev = usb_device_find_USB_MCC(USB1608G_V2_PID, NULL))) {
    printf("Success, found a USB 1608G!\n");
    usbInit_1608G(udev, 2);
  } else if ((udev = usb_device_find_USB_MCC(USB1608GX_V2_PID, NULL))) {
    printf("Success, found a USB 1608GX!\n");
    usbInit_1608G(udev, 2);
  } else if ((udev = usb_device_find_USB_MCC(USB1608GX_2AO_V2_PID, NULL))) {
    printf("Success, found a USB 1608GX_2AO!\n");
    usb1608GX_2AO = TRUE;
    usbInit_1608G(udev, 2);
  } else if ((udev = usb_device_find_USB_MCC(USB1608G_PID, NULL))) {
    printf("Success, found a USB 1608G!\n");
    usbInit_1608G(udev, 1);
  } else if ((udev = usb_device_find_USB_MCC(USB1608GX_PID, NULL))) {
    printf("Success, found a USB 1608GX!\n");
    usbInit_1608G(udev, 1);
  } else if ((udev = usb_device_find_USB_MCC(USB1608GX_2AO_PID, NULL))) {
    printf("Success, found a USB 1608GX_2AO!\n");
    usb1608GX_2AO = TRUE;
    usbInit_1608G(udev, 1);
  } else {
    printf("Failure, did not find a USB 1608G series device!\n");
    return 0;
  }

  //print out the wMaxPacketSize.  Should be 512
  printf("wMaxPacketSize = %d\n", usb_get_max_packet_size(udev,0));

  // clear the AOut FIFO
  //  usbAOutScanClearFIFO_USB1608GX_2AO(udev);

  usbBuildGainTable_USB1608G(udev, usb1608G.table_AIn);
  for (i = 0; i < NGAINS_1608G; i++) {
    printf("Gain: %d   Slope = %f   Offset = %f\n", i, usb1608G.table_AIn[i][0], usb1608G.table_AIn[i][1]);
  }

  if (usb1608GX_2AO) {
    usbBuildGainTable_USB1608GX_2AO(udev, usb1608G.table_AOut);
    printf("\n");
    for (i = 0; i < NCHAN_AO_1608GX; i++) {
      printf("VDAC%d:    Slope = %f    Offset = %f\n", i, usb1608G.table_AOut[i][0], usb1608G.table_AOut[i][1]);
    }
  }

  usbCalDate_USB1608G(udev, &calDate);
  printf("\n");
  printf("MFG Calibration date = %s\n", asctime(&calDate));

  while(1) {
    printf("\nUSB 1608G/1608GX/1608GX_2AO Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink\n");
    printf("Hit 'c' to test counter\n");
    printf("Hit 'C' to test continuous sampling at 1000 Hz.\n");
    printf("Hit 'd' to test digital IO\n");
    printf("Hit 'i' to test Analog Input\n");
    printf("Hit 'I' to test Analog Input Scan\n");
    printf("Hit 'o' to test Analog Output\n");
    printf("Hit 'O' to test Analog Output Scan\n");
    printf("Hit 'p' to test Pulse Width Modulation\n");
    printf("Hit 'r' to reset the device\n");
    printf("Hit 's' to get serial number\n");
    printf("Hit 'S' to get Status\n");
    printf("Hit 't' to test the timers\n");
    printf("Hit 'T' to get temperature\n");
    printf("Hit 'v' to get version numbers\n");
    printf("Hit 'e' to exit\n");

    while((ch = getchar()) == '\0' || ch == '\n');
    switch(ch) {
      case 'b': /* test to see if LED blinks */
        printf("Enter number or times to blink: ");
        scanf("%hhd", &options);
        usbBlink_USB1608G(udev, options);
	break;
      case 'c':
        usbCounterInit_USB1608G(udev, COUNTER0);
        printf("Connect DIO0 to CTR0\n");
	usbDTristateW_USB1608G(udev, 0xf0);
        toContinue();
        for (i = 0; i < 100; i++) {
	  usbDLatchW_USB1608G(udev, 0x0);
	  usbDLatchW_USB1608G(udev, 0x1);
        }
        printf("Count = %d.  Should read 100.\n", usbCounter_USB1608G(udev, COUNTER0));
        break;      
      case 'd':
        printf("\nTesting Digital I/O...\n");
	printf("connect pins DIO[0-3] <--> DIO[4-7]\n");
	usbDTristateW_USB1608G(udev,0xf0);
	printf("Digital Port Tristate Register = %#x\n", usbDTristateR_USB1608G(udev));
	do {
          printf("Enter a byte number [0-0xf] : " );
          scanf("%x", &temp);
	  temp &= 0xf;
          usbDLatchW_USB1608G(udev, (uint16_t)temp);
	  temp = usbDLatchR_USB1608G(udev);
          input = usbDPort_USB1608G(udev) >> 4;
          printf("The number you entered = %#x   Latched value = %#x\n\n",input, temp);
	  for (i = 0; i < 4; i++) {
	    printf("Bit %d = %d\n", i, (temp>>i)&0x1);
	  }
        } while (toContinue());
        break;
      case 'p':
        printf("Enter desired frequency: ");
        scanf("%lf", &frequency);
        printf("Enter duty cycle: ");
        scanf("%lf", &duty_cycle);
	flag = fcntl(fileno(stdin), F_GETFL);
	fcntl(0, F_SETFL, flag | O_NONBLOCK);
        do {
	  usbDLatchW_USB1608G(udev, (uint16_t) 0x1);
          usleep((useconds_t) 1000000./frequency*duty_cycle);
	  usbDLatchW_USB1608G(udev, (uint16_t) 0x0);
          usleep((useconds_t) 1000000./frequency*(1. - duty_cycle));
	} while (!isalpha(getchar()));
	fcntl(fileno(stdin), F_SETFL, flag);
        usbDLatchW_USB1608G(udev, (uint16_t) 0x0);
        break;
      case 'e':
	usbDLatchW_USB1608G(udev, 0x0);                  // zero out the DIO
	if (usb1608GX_2AO) {
	  usbAOutScanStop_USB1608GX_2AO(udev);
	  usbAOut_USB1608GX_2AO(udev, 0, 0x0, usb1608G.table_AOut);
	  usbAOut_USB1608GX_2AO(udev, 1, 0x0, usb1608G.table_AOut);
	}
        cleanup_USB1608G(udev);
        return 0;
      case 'i':
	printf("Enter 0 for Single Ended, 1 for Differential: ");
	scanf("%hhd", &mode);
	if (mode == USB1608G_SINGLE_ENDED) {
  	  mode = USB1608G_SINGLE_ENDED;
	  printf("Input channel [0-15]: ");
	  scanf("%hhd", &channel);
	} else {
	  mode = USB1608G_DIFFERENTIAL;
	  printf("Input channel [0-7]: ");
	  scanf("%hhd", &channel);
	}	  
	printf("Gain Range for channel %d: 1 = +/-10V  2 = +/- 5V  3 = +/- 2V  4 = +/- 1V: ",channel);
	while((ch = getchar()) == '\0' || ch == '\n');
	switch(ch) {
	  case '1': gain = USB1608G_BP_10V; break;
  	  case '2': gain = USB1608G_BP_5V; break;
	  case '3': gain = USB1608G_BP_2V; break;
	  case  '4': gain = USB1608G_BP_1V; break;
	  default:  gain = USB1608G_BP_10V; break;
	}
	mode |= USB1608G_LAST_CHANNEL ; 
	usb1608G.list[0].range = gain;
        usb1608G.list[0].mode = mode;
	usb1608G.list[0].channel = channel;
	usbAInConfig_USB1608G(udev,&usb1608G);
	for (i = 0; i < 20; i++) {
	  value = usbAIn_USB1608G(udev, channel);
	  if (value >=  0xfffd) {
	    printf("DAC is saturated at +FS\n");
	  } else if  (value <= 0x60) {
	    printf("DAC is saturated at -FS\n");
	  } else {
	    value = rint(value*usb1608G.table_AIn[gain][0] + usb1608G.table_AIn[gain][1]);
	  }
	  printf("Channel %d  Mode = %#x  Gain = %d Sample[%d] = %#x Volts = %lf\n",
		 usb1608G.list[0].channel, usb1608G.list[0].mode, usb1608G.list[0].range, i, value, volts_USB1608G(gain, value));
	  usleep(50000);	  
	}
        break;
      case 'I':
	printf("Testing USB-1608G Multi-Channel Analog Input Scan.\n");
	usbAInScanStop_USB1608G(udev);
	usbAInScanClearFIFO_USB1608G(udev);
	printf("Enter 0 for Single Ended, 1 for Differential: ");
	scanf("%hhd", &mode);
	if (mode == USB1608G_SINGLE_ENDED) {
  	  mode = USB1608G_SINGLE_ENDED;
	  printf("enter number of channels (1-16) :");
	  scanf("%d", &nchan);
	  if (nchan > 16) break;
	} else {
	  mode = USB1608G_DIFFERENTIAL;
	  printf("enter number of channels (1-8) :");
	  scanf("%d", &nchan);
	  if (nchan > 8) break;
	}	  
        printf("Enter number of scans: ");
        scanf("%d", &nScans);
	usb1608G.count = nScans;
	usb1608G.retrig_count = 0;
        printf("Enter number of repeats: ");
        scanf("%d", &repeats);
        printf("Enter sampling frequency [Hz]: ");
	scanf("%lf", &frequency);
	usb1608G.frequency = frequency;
        for (channel = 0; channel < nchan; channel++) {
	  printf("Gain Range for channel %d: 1 = +/-10V  2 = +/- 5V  3 = +/- 2V  4 = +/- 1V: ",channel);
	  while((ch = getchar()) == '\0' || ch == '\n');
	  switch(ch) {
	    case '1': gain = USB1608G_BP_10V; break;
	    case '2': gain = USB1608G_BP_5V; break;
	    case '3': gain = USB1608G_BP_2V; break;
	    case '4': gain = USB1608G_BP_1V; break;
	    default:  gain = USB1608G_BP_10V; break;
	  }
	  usb1608G.list[channel].range = gain;  
	  usb1608G.list[channel].mode = mode;
	  usb1608G.list[channel].channel = channel;
	}
        usb1608G.list[nchan-1].mode |= USB1608G_LAST_CHANNEL;
	usbAInConfig_USB1608G(udev, &usb1608G);
	usb1608G.mode = 0x0;
        if ((sdataIn = malloc(2*nchan*nScans)) == NULL) {
	  perror("Can not allocate memory for sdataIn");
	  break;
	}
        for (m = 0; m < repeats; m++) {
	  printf("\n\n---------------------------------------");
	  printf("\nrepeat: %d\n", m);
	  usbAInScanStart_USB1608G(udev, &usb1608G);
	  ret = usbAInScanRead_USB1608G(udev, &usb1608G, sdataIn);
	  printf("Number bytes read = %d  (should be %d)\n", ret, 2*nchan*nScans);
	  for (i = 0; i < nScans; i++) {
	    printf("%6d", i);
	    for (j = 0; j < nchan; j++) {
              gain = usb1608G.list[j].range;
	      k = i*nchan + j;
	      if (sdataIn[k] >= 0xfffd) {
		printf("DAC is saturated at +FS\n");
	      } else if (sdataIn[k] <= 0x60) {
		printf("DAC is saturated at -FS\n");
	      } else {
		data = rint(sdataIn[k]*usb1608G.table_AIn[gain][0] + usb1608G.table_AIn[gain][1]);
	      printf(", %8.4lf", volts_USB1608G(gain, data));
	      }
	    }
	    printf("\n");
	  }
	}
	printf("\n\n---------------------------------------");
	free(sdataIn);
        break;
      case 'C':
      	printf("Testing USB-1608G Analog Input Scan in continuous mode 16 channels\n");
        printf("Hit any key to exit\n");
	printf("Enter desired sampling frequency (greater than 1000): ");
	scanf("%lf", &frequency);
	usbAInScanStop_USB1608G(udev);
        nScans = 0;         // for continuous mode
        nchan = 16;         // 16 channels
	gain = USB1608G_BP_10V;
	// mode = USB1608G_DIFFERENTIAL;
	mode = USB1608G_SINGLE_ENDED;

        for (channel = 0; channel < nchan; channel++) {
	  usb1608G.list[channel].range = gain;
	  usb1608G.list[channel].mode = mode;
	  usb1608G.list[channel].channel = channel;
	}
	usb1608G.list[nchan-1].mode |= USB1608G_LAST_CHANNEL;
	usbAInConfig_USB1608G(udev, &usb1608G);

	nread = 256;
	if ((sdataIn = malloc(2*nchan*nread)) == NULL) {
	  perror("Can not allocate memory for sdataIn");
	  break;
	}
	sleep(1);
	usb1608G.count = 0;
	usb1608G.retrig_count = 0;
	usb1608G.frequency = frequency;
	usb1608G.options = 0x1;
        i = 0;
        usbAInScanStart_USB1608G(udev, &usb1608G);
	flag = fcntl(fileno(stdin), F_GETFL);
	fcntl(0, F_SETFL, flag | O_NONBLOCK);
        do {
	  usbAInScanRead_USB1608G(udev, &usb1608G, sdataIn);
          if (i%100 == 0) {
            printf("Scan = %d\n", i);
	  }
          i++;
	} while (!isalpha(getchar()));
	fcntl(fileno(stdin), F_SETFL, flag);
        usbAInScanStop_USB1608G(udev);
	usbAInScanClearFIFO_USB1608G(udev);
	free(sdataIn);
        sleep(2); // let things settle down.
        break;
      case 'o':
        if (!(usb1608GX_2AO)) {
	  printf("Analog output only on the USB-1608GX_2AO model.\n");
  	  break;
        }
        printf("Enter voltage: ");
	scanf("%lf", &voltage);
	usbAOut_USB1608GX_2AO(udev, 0, voltage, usb1608G.table_AOut);
	usbAOutR_USB1608GX_2AO(udev, 0, &voltage, usb1608G.table_AOut);
	printf("Analog Output Voltage = %f V\n", voltage);
        break;
      case 'O':
	if (!(usb1608GX_2AO)) {
	  printf("Analog output only on the USB-1608GX_2AO model.\n");
	  break;
	}
	channel = 0;
	printf("Test of Analog Output Scan.\n");
	printf("Hook scope up to VDAC 0\n");
	printf("Enter desired frequency of sine wave [Hz]: ");
	scanf("%lf", &frequency);
        frequency *= 512.;
	for (i = 0; i < 512; i++) {
	  voltage = 10.*sin(2.*M_PI*i/512.);
          voltage = (voltage/10.*32768. + 32768.);
	  dataOut[i] = voltage*usb1608G.table_AOut[channel][0] + usb1608G.table_AOut[channel][1];
	}
        usbAOutScanStop_USB1608GX_2AO(udev);
	usbAOutWrite_USB1608GX_2AO(udev, dataOut, sizeof(dataOut));
	usbAOutScanStart_USB1608GX_2AO(udev, 0, 0, frequency,  USB1608G_AO_CHAN0);
	flag = fcntl(fileno(stdin), F_GETFL);
	fcntl(0, F_SETFL, flag | O_NONBLOCK);
	do {
	  usbAOutWrite_USB1608GX_2AO(udev, dataOut, sizeof(dataOut));
	  // printf("ret = %d  status = %#x\n", ret, usbStatus_USB1608G(udev));
	} while (!isalpha(getchar()));
	fcntl(fileno(stdin), F_SETFL, flag);
	usbAOutScanStop_USB1608GX_2AO(udev);
	break;
      case 'r':
	usbReset_USB1608G(udev);
	return 0;
	break;
      case 's':
        usbGetSerialNumber_USB1608G(udev, serial);
        printf("Serial number = %s\n", serial);
        break;
      case 'S':
	status = usbStatus_USB1608G(udev);
        printf("Status = %#x\n", status);
	break;
      case 't':
        printf("Enter frequency of timer: ");
        scanf("%lf", &frequency);
	period = 1000./frequency;
	usbTimerPeriodW_USB1608G(udev, period);
	usbTimerPulseWidthW_USB1608G(udev, period / 2);
	usbTimerCountW_USB1608G(udev, 0);
	usbTimerDelayW_USB1608G(udev, 0);
	usbTimerControlW_USB1608G(udev, 0x1);
	toContinue();
	usbTimerControlW_USB1608G(udev, 0x0);
        break;
      case 'T':
        usbTemperature_USB1608G(udev, &temperature);
	printf("Temperature = %.2f deg C  or  %.2f deg F \n", temperature, 9.0/5.0*temperature + 32.);
	break;
      case 'v':
	version = 0xbeef;
        usbFPGAVersion_USB1608G(udev, &version);
	printf("FPGA version %02x.%02x\n", version >> 0x8, version & 0xff);
	break;
      default:
        break;
    }
  }
}

