	/*
  ADS1262.h - Library for ADS1262 Shield Arduino Firmwar.
  Created by Protocentral, December 27, 2013.
  Released into the public domain.
*/

#ifndef ads1262_h
#define ads1262_h

#include "Arduino.h"

#define CONFIG_SPI_MASTER_DUMMY   0xFF

// Register Read Commands
#define  RREG    0x20;		//Read n nnnn registers starting at address r rrrr
                                //first byte 001r rrrr (2xh)(2) - second byte 000n nnnn(2)
#define WREG    0x40;		//Write n nnnn registers starting at address r rrrr
                                //first byte 010r rrrr (2xh)(2) - second byte 000n nnnn(2)

#define START		0x08		//Start/restart (synchronize) conversions
#define STOP		0x0A		//Stop conversion
#define RDATAC      0x10		//Enable Read Data Continuous mode. 

//This mode is the default mode at power-up.
#define SDATAC		0x11		//Stop Read Data Continuously mode
#define RDATA		0x12		//Read data by command; supports multiple read back.

//Pin declartion the other you need are controlled by the SPI library
const int ADS1262_DRDY_PIN = 6;
const int ADS1262_CS_PIN = 7;
const int ADS1262_START_PIN = 5;
const int ADS1262_PWDN_PIN = 4;

//register address
#define	POWER		0x01
#define	INTERFACE	0x02
#define	MODE0		0x03
#define	MODE1		0x04
#define	MODE2		0x05
#define	INPMUX		0x06
#define	OFCAL0		0x07
#define	OFCAL1		0x08
#define	OFCAL2		0x09
#define	FSCAL0		0x0A
#define	FSCAL1		0x0B
#define	FSCAL2		0x0C
#define	IDACMUX		0x0D
#define	IDACMAG		0x0E
#define	REFMUX		0x0F
#define	TDACP		0x10
#define	TDACN		0x11
#define	GPIOCON		0x12
#define	GPIODIR		0x13
#define	GPIODAT		0x14
#define	ADC2CFG		0x15
#define	ADC2MUX		0x16
#define	ADC2OFC0	0x17
#define	ADC2OFC1	0x18
#define	ADC2FSC0	0x19
#define	ADC2FSC1	0x1A



class ads1262
{
  public:
    static void ads1262_Init(void);
    static void ads1262_Reset(void);
	static void ads1262_Reg_Write (unsigned char READ_WRITE_ADDRESS, unsigned char DATA);
	static void ads1262_Reg_Read (unsigned char READ_WRITE_ADDRESS);
    static void ads1262_SPI_Command_Data(unsigned char data_in);
    static void ads1262_Disable_Start(void);
    static void ads1262_Enable_Start(void);
    static void ads1262_Hard_Stop (void);
    static void ads1262_Start_Data_Conv_Command (void);
    static void ads1262_Soft_Stop (void);
    static void ads1262_Start_Read_Data_Continuous (void);
    static void ads1262_Stop_Read_Data_Continuous (void);
    static char* ads1262_Read_Data(void);
};

#endif

