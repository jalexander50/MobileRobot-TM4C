/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

/* Board Header file */
#include "Board.h"

#include <string.h>
#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* SimpleLink Wi-Fi Host Driver Header files */
#include <simplelink.h>


/* Local Platform Specific Header file */
#include "sockets.h"

#include "registers.h"
#include <Definitions.h>
#include <type.h>
#include <stdint.h>
#include <stdbool.h>

/* Port number for listening for TCP packets */
#define TCPPORT         4050

#define TCPPACKETSIZE   256
#define TASKSTACKSIZE   8192
#define SPI_MSG_LENGTH	2


extern bool smartConfigFlag;
Task_Struct tcpTaskStruct, motorTaskStruct;
Char tcpTaskStack[TASKSTACKSIZE], motorTaskStack[TASKSTACKSIZE];

Semaphore_Struct semStruct;
Semaphore_Handle semHandle;
//////////registers for drv8711//////////////
struct CTRL_Register	G_CTRL_REG;
struct TORQUE_Register 	G_TORQUE_REG;
struct OFF_Register 	G_OFF_REG;
struct BLANK_Register	G_BLANK_REG;
struct DECAY_Register 	G_DECAY_REG;
struct STALL_Register 	G_STALL_REG;
struct DRIVE_Register 	G_DRIVE_REG;
struct STATUS_Register 	G_STATUS_REG;
#define REGWRITE    0x00
#define REGREAD     0x80

////////////////LEDDAR////////////////////////
#define SLAVE_ADDRESS	0x01
#define ARRAYSIZE(v) (sizeof(v)/sizeof(v[0]))
unsigned char dataBuffer[25];
char buffer[TCPPACKETSIZE];
int data[2];
//int intensities[100];
//int CountDet = 0;
int steps =0;

static uint8_t CRC_HI[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};

//CRC low value
static uint8_t CRC_LO[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};



//code to do the 16 bit CRC16 check
bool CRC16(uint8_t *aBuffer, uint8_t aLength, bool aCheck)
{
	uint8_t lCRCHi = 0xFF; // high uint8_t of CRC initialized
	uint8_t lCRCLo = 0xFF; // low uint8_t of CRC initialized
	int i;

	for (i = 0; i<aLength; ++i)
	{
		int lIndex = lCRCLo ^ aBuffer[i]; // calculate the CRC
		lCRCLo = lCRCHi ^ CRC_HI[lIndex];
		lCRCHi = CRC_LO[lIndex];
	}

	if (aCheck)
	{
		return ( aBuffer[aLength] == lCRCLo ) && ( aBuffer[aLength+1] == lCRCHi );
	}
	else
	{
		aBuffer[aLength] = lCRCLo;
		aBuffer[aLength+1] = lCRCHi;
		return true;
	}
};

void sendRequest(){
			char input;
		    UART_Handle uart;
		    UART_Params uartParams;
		    unsigned int detcount = 0;
		    //const char dataBuffer[25];

		    /* Create a UART with data processing off. */
		    UART_Params_init(&uartParams);
		    uartParams.writeDataMode = UART_DATA_BINARY;
		    uartParams.readDataMode = UART_DATA_BINARY;
		    uartParams.readReturnMode = UART_RETURN_FULL;
		    uartParams.readEcho = UART_ECHO_OFF;
		    uartParams.baudRate = 115200;
		    uart = UART_open(Board_UART1, &uartParams);

		    if (uart == NULL) {
		        System_abort("Error opening the UART");
		    }
		    //CountDet=0;
		    //while(CountDet < 100){

				dataBuffer[0] =  SLAVE_ADDRESS;
				dataBuffer[1] =  0x04;
				dataBuffer[2] =  0;
				dataBuffer[3] =  20;
				dataBuffer[4] =  0;
				dataBuffer[5] =  10;

				CRC16(dataBuffer, 6, false);
				int i = 0;
				//transmit data over A2 UART module
				UART_write(uart, &dataBuffer, 8);
				UART_read(uart, &dataBuffer, 25);


						//Timestamp
						TimeStamp = ((unsigned long)dataBuffer[5]) << 24;
						TimeStamp += ((unsigned long)dataBuffer[6]) << 16;
						TimeStamp += ((unsigned long)dataBuffer[3]) <<8;
						TimeStamp += dataBuffer[4];

						// Internal Temperature
						Temperature = dataBuffer[7];
						Temperature += ((float)dataBuffer[8])/256;


						NbDet = dataBuffer[10];

								if (NbDet > ARRAYSIZE(Detections))
								{
									return ERR_LEDDAR_NB_DETECTIONS;
								}


								i = 11;
								for (detcount = 0; detcount < NbDet; detcount++)
								{
									// For each detection:
									// Bytes 0 and 1 = distance in cm
									// Bytes 2-3 are amplitude*256


									Detections[detcount].Distance = ((unsigned int)dataBuffer[i])*256 + dataBuffer[i+1];
									//MAP_UART_transmitData(EUSCI_A0_BASE, Detections[detcount].Distance);
									Detections[detcount].Amplitude = ((float)dataBuffer[i+2]) + ((float)dataBuffer[i+3])/256;

									i += 4;
								}
								data[0] = Detections[0].Distance;
								data[1] = steps;
								//Motor();
			//					CountDet++;
		    //}

							UART_close(uart);
}

////////////////////////////////END LEDDAR//////////////////////////////////////////////




//////////////////////////////////MOTOR/////////////////////////////////////////////


Void MotorFxn()
{
	while(1){
		/* Get access to resource */
		Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);
		if(steps<360){
			WriteAllRegisters();
			ReadAllRegisters();
			G_CTRL_REG.RSTEP = 0x01;
			steps++;
		}
		else{
			WriteAllRegisters();
			ReadAllRegisters();
			G_CTRL_REG.RSTEP = 0x01;
			steps = 0;
		}
		Semaphore_post(semHandle);
	}

	    //Task_sleep((UInt) arg0);


}

unsigned int SPI_DRV8711_ReadWrite(unsigned char dataHi, unsigned char dataLo)
{
	unsigned int readData =0;
	unsigned char masterRxBuffer[SPI_MSG_LENGTH];
	unsigned char masterTxBuffer[SPI_MSG_LENGTH];
	masterTxBuffer[0] = dataHi;
	masterTxBuffer[1] = dataLo;

	SPI_Handle masterSpi;
	SPI_Transaction masterTransaction;
	bool transferOK;


		    /* Initialize SPI handle as default master */
		    masterSpi = SPI_open(Board_SPI1, NULL);
		    if (masterSpi == NULL) {
		        System_abort("Error initializing SPI\n");
		    }
		    else {
		        //System_printf("SPI initialized\n");
		    }

		    /* Initialize master SPI transaction structure */
		        masterTransaction.count = SPI_MSG_LENGTH;
		        masterTransaction.txBuf = (Ptr)masterTxBuffer;
		        masterTransaction.rxBuf = (Ptr)masterRxBuffer;

		    /* Initiate SPI transfer */
		        //transferOK = SPI_transfer(masterSpi, &masterTransaction);

	masterTxBuffer[0] = dataHi;
	masterTxBuffer[1] = dataLo;

	GPIO_write(CS, 1);
	transferOK = SPI_transfer(masterSpi, &masterTransaction);

	unsigned char temp = masterRxBuffer[0];
	readData |= (temp << 8);
	temp = masterRxBuffer[1];
	readData |= (temp);


	GPIO_write(CS, 0);
	SPI_close(masterSpi);
	return readData;


}



void WriteAllRegisters()
{
    unsigned char dataHi = 0x00;
    unsigned char dataLo = 0x00;

    // Write CTRL Register
    dataHi = REGWRITE | (G_CTRL_REG.Address << 4) | (G_CTRL_REG.DTIME << 2) | (G_CTRL_REG.ISGAIN);
    dataLo = (G_CTRL_REG.EXSTALL << 7) | (G_CTRL_REG.MODE << 3) | (G_CTRL_REG.RSTEP << 2) | (G_CTRL_REG.RDIR << 1) | (G_CTRL_REG.ENBL);
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write TORQUE Register
    dataHi = REGWRITE | (G_TORQUE_REG.Address << 4) | (G_TORQUE_REG.SIMPLTH);
    dataLo = G_TORQUE_REG.TORQUE;
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write OFF Register
    dataHi = REGWRITE | (G_OFF_REG.Address << 4) | (G_OFF_REG.PWMMODE);
    dataLo = G_OFF_REG.TOFF;
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write BLANK Register
    dataHi = REGWRITE | (G_BLANK_REG.Address << 4) | (G_BLANK_REG.ABT);
    dataLo = G_BLANK_REG.TBLANK;
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write DECAY Register
    dataHi = REGWRITE | (G_DECAY_REG.Address << 4) | (G_DECAY_REG.DECMOD);
    dataLo = G_DECAY_REG.TDECAY;
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write STALL Register
    dataHi = REGWRITE | (G_STALL_REG.Address << 4) | (G_STALL_REG.VDIV << 2) | (G_STALL_REG.SDCNT);
    dataLo = G_STALL_REG.SDTHR;
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write DRIVE Register
    dataHi = REGWRITE | (G_DRIVE_REG.Address << 4) | (G_DRIVE_REG.IDRIVEP << 2) | (G_DRIVE_REG.IDRIVEN);
    dataLo = (G_DRIVE_REG.TDRIVEP << 6) | (G_DRIVE_REG.TDRIVEN << 4) | (G_DRIVE_REG.OCPDEG << 2) | (G_DRIVE_REG.OCPTH);
    SPI_DRV8711_ReadWrite(dataHi, dataLo);

    // Write STATUS Register
    dataHi = REGWRITE | (G_STATUS_REG.Address << 4);
    dataLo = (G_STATUS_REG.STDLAT << 7) | (G_STATUS_REG.STD << 6) | (G_STATUS_REG.UVLO << 5) | (G_STATUS_REG.BPDF << 4) | (G_STATUS_REG.APDF << 3) | (G_STATUS_REG.BOCP << 2) | (G_STATUS_REG.AOCP << 1) | (G_STATUS_REG.OTS);
    SPI_DRV8711_ReadWrite(dataHi, dataLo);
}


void ReadAllRegisters()
{
    unsigned char dataHi = 0x00;
    const unsigned char dataLo = 0x00;
    unsigned int readData = 0x00;

    // Read CTRL Register
    dataHi = REGREAD | (G_CTRL_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_CTRL_REG.DTIME        = ((readData >> 10) & 0x0003);
    G_CTRL_REG.ISGAIN       = ((readData >> 8) & 0x0003);
    G_CTRL_REG.EXSTALL      = ((readData >> 7) & 0x0001);
    G_CTRL_REG.MODE         = ((readData >> 3) & 0x000F);
    G_CTRL_REG.RSTEP        = ((readData >> 2) & 0x0001);
    G_CTRL_REG.RDIR         = ((readData >> 1) & 0x0001);
    G_CTRL_REG.ENBL         = ((readData >> 0) & 0x0001);

    // Read TORQUE Register
    dataHi = REGREAD | (G_TORQUE_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_TORQUE_REG.SIMPLTH    = ((readData >> 8) & 0x0007);
    G_TORQUE_REG.TORQUE     = ((readData >> 0) & 0x00FF);

    // Read OFF Register
    dataHi = REGREAD | (G_OFF_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_OFF_REG.PWMMODE       = ((readData >> 8) & 0x0001);
    G_OFF_REG.TOFF          = ((readData >> 0) & 0x00FF);

    // Read BLANK Register
    dataHi = REGREAD | (G_BLANK_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_BLANK_REG.ABT         = ((readData >> 8) & 0x0001);
    G_BLANK_REG.TBLANK      = ((readData >> 0) & 0x00FF);

    // Read DECAY Register
    dataHi = REGREAD | (G_DECAY_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_DECAY_REG.DECMOD      = ((readData >> 8) & 0x0007);
    G_DECAY_REG.TDECAY      = ((readData >> 0) & 0x00FF);

    // Read STALL Register
    dataHi = REGREAD | (G_STALL_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_STALL_REG.VDIV        = ((readData >> 10) & 0x0003);
    G_STALL_REG.SDCNT       = ((readData >> 8) & 0x0003);
    G_STALL_REG.SDTHR       = ((readData >> 0) & 0x00FF);

    // Read DRIVE Register
    dataHi = REGREAD | (G_DRIVE_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_DRIVE_REG.IDRIVEP     = ((readData >> 10) & 0x0003);
    G_DRIVE_REG.IDRIVEN     = ((readData >> 8) & 0x0003);
    G_DRIVE_REG.TDRIVEP     = ((readData >> 6) & 0x0003);
    G_DRIVE_REG.TDRIVEN     = ((readData >> 4) & 0x0003);
    G_DRIVE_REG.OCPDEG      = ((readData >> 2) & 0x0003);
    G_DRIVE_REG.OCPTH       = ((readData >> 0) & 0x0003);

    // Read STATUS Register
    dataHi = REGREAD | (G_STATUS_REG.Address << 4);
    readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
    G_STATUS_REG.STDLAT     = ((readData >> 7) & 0x0001);
    G_STATUS_REG.STD        = ((readData >> 6) & 0x0001);
    G_STATUS_REG.UVLO       = ((readData >> 5) & 0x0001);
    G_STATUS_REG.BPDF       = ((readData >> 4) & 0x0001);
    G_STATUS_REG.APDF       = ((readData >> 3) & 0x0001);
    G_STATUS_REG.BOCP       = ((readData >> 2) & 0x0001);
    G_STATUS_REG.AOCP       = ((readData >> 1) & 0x0001);
    G_STATUS_REG.OTS        = ((readData >> 0) & 0x0001);
}


Void tcpTask(UArg arg0, UArg arg1)
{
	void *netIF;

	    /* Open WiFi and await a connection */
	netIF = socketsStartUp();
	//server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//    while(1){
//    sendRequest();
//    //MotorFxn();
//    sendTCP(TCPPORT);
//    //Task_sleep(100);
//    //System_flush();
//	}


		int         bytesRcvd;
		int         bytesSent;
		int         status;
		int         clientfd;
		int         server;
		sockaddr_in localAddr;
		sockaddr_in clientAddr;
		socklen_t   addrlen = sizeof(clientAddr);


		server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		    if (server == -1) {
		        System_printf("Error: socket not created.\n");
		        goto shutdown;
		    }

		    memset(&localAddr, 0, sizeof(localAddr));
		        localAddr.sin_family = AF_INET;
		        localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		        localAddr.sin_port = htons(4050);

		        status = bind(server, (const sockaddr *)&localAddr, sizeof(localAddr));
		        if (status == -1) {
		            System_printf("Error: bind failed.\n");
		            goto shutdown;
		        }

		        status = listen(server, 0);
		        if (status == -1){
		            System_printf("Error: listen failed.\n");
		            goto shutdown;
		        }

		        while((clientfd = accept(server, (sockaddr *)&clientAddr, &addrlen)) > 0){

		        	Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);
		        	sendRequest();
		        	bytesSent = send(clientfd, data, sizeof(data), 0);
		        	//Motor();
		        	//Task_sleep(1000);
		        /* addrlen is a value-result param, must reset for next accept call */
		                    addrlen = sizeof(clientAddr);
		                    close(clientfd);
		                    Semaphore_post(semHandle);
		        }
		        		shutdown:
		        	   	   if (server >= 0) {
		        		   close(server);
		}
    /* Close the network - don't do this if other tasks are using it */
    socketsShutDown(netIF);
}

/*
 *  ======== main ========
 */
int main(void)
{



						///INITIALIZE THE REG VALUES//
						G_CTRL_REG.Address 	= 0x00;
				    	G_CTRL_REG.DTIME 	= 0x01;
				    	G_CTRL_REG.ISGAIN 	= 0x03;
				    	G_CTRL_REG.EXSTALL 	= 0x00;
				    	G_CTRL_REG.MODE 	= 0x00;
				    	G_CTRL_REG.RSTEP 	= 0x01;
				    	G_CTRL_REG.RDIR 	= 0x01;
				    	G_CTRL_REG.ENBL 	= 0x01;

				    	// TORQUE Register
				    	G_TORQUE_REG.Address = 0x01;
				    	G_TORQUE_REG.SIMPLTH = 0x00;
				    	G_TORQUE_REG.TORQUE  = 0x1F;

				    	// OFF Register
				    	G_OFF_REG.Address 	= 0x02;
				    	G_OFF_REG.PWMMODE 	= 0x00;
				    	G_OFF_REG.TOFF 		= 0x30;

				    	// BLANK Register
				    	G_BLANK_REG.Address = 0x03;
				    	G_BLANK_REG.ABT 	= 0x01;
				    	G_BLANK_REG.TBLANK 	= 0x08;

				    	// DECAY Register.
				    	G_DECAY_REG.Address = 0x04;
				    	G_DECAY_REG.DECMOD  = 0x11;
				    	G_DECAY_REG.TDECAY 	= 0x10;

				    	// STALL Register
				    	G_STALL_REG.Address = 0x05;
				    	G_STALL_REG.VDIV 	= 0x03;
				    	G_STALL_REG.SDCNT 	= 0x03;
				    	G_STALL_REG.SDTHR 	= 0x40;

				    	// DRIVE Register
				    	G_DRIVE_REG.Address = 0x06;
				    	G_DRIVE_REG.IDRIVEP = 0x10;
				    	G_DRIVE_REG.IDRIVEN = 0x10;
				    	G_DRIVE_REG.TDRIVEP = 0x01;
				    	G_DRIVE_REG.TDRIVEN = 0x01;
				    	G_DRIVE_REG.OCPDEG 	= 0x01;
				    	G_DRIVE_REG.OCPTH 	= 0x01;

				    	// STATUS Register
				    	G_STATUS_REG.Address = 0x07;
				    	G_STATUS_REG.STDLAT  = 0x00;
				    	G_STATUS_REG.STD     = 0x00;
				    	G_STATUS_REG.UVLO    = 0x00;
				    	G_STATUS_REG.BPDF    = 0x00;
				    	G_STATUS_REG.APDF    = 0x00;
				    	G_STATUS_REG.BOCP    = 0x00;
				    	G_STATUS_REG.AOCP    = 0x00;
				    	G_STATUS_REG.OTS     = 0x00;



    Task_Params taskParams;
    Semaphore_Params semParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    // Board_initI2C();
    // Board_initSDSPI();
    Board_initSPI();
    Board_initUART();
    // Board_initWatchdog();
    Board_initWiFi();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &tcpTaskStack;
    Task_construct(&tcpTaskStruct, (Task_FuncPtr)tcpTask, &taskParams, NULL);

    taskParams.stack = &motorTaskStack;
    taskParams.priority = 2;
    Task_construct(&motorTaskStruct, (Task_FuncPtr)MotorFxn, &taskParams, NULL);

    /* Construct a Semaphore object to be use as a resource lock, inital count 1 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semStruct, 1, &semParams);

    /* Obtain instance handle */
    semHandle = Semaphore_handle(&semStruct);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
