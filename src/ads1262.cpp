////////////////////////////////////////////////////////////////////////////////////////////
//    Demo code for the protocentral-ads1262 sensor breakout board
//
//    Copyright (c) 2013 ProtoCentral
//
//    This example measures raw capacitance across CHANNEL0 and Gnd and
//    prints on serial terminal
//    
//    this example gives differential voltage across the AN0 And AN1 in mV
//
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
//     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//  
//     For information on how to use, visit https://github.com/Protocentral/ProtoCentral_ads1262
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <ads1262.h>
#include <SPI.h>

char* ads1262::ads1262_Read_Data()
{
  static char SPI_Dummy_Buff[6];
  digitalWrite(ADS1262_CS_PIN, LOW);
   
	for (int i = 0; i < 6; ++i)
	{
		SPI_Dummy_Buff[i] = SPI.transfer(CONFIG_SPI_MASTER_DUMMY);
	}
	
  digitalWrite(ADS1262_CS_PIN, HIGH);
	
	return SPI_Dummy_Buff;
}

void ads1262::ads1262_Init()
{
  // start the SPI library:
  SPI.begin();
  SPI.setBitOrder(MSBFIRST); 
  //CPOL = 0, CPHA = 1
  SPI.setDataMode(SPI_MODE1);
  // Selecting 1Mhz clock for SPI
  SPI.setClockDivider(SPI_CLOCK_DIV8); // DIV16

  ads1262_Reset();
  delay(100);
  
  ads1262_Hard_Stop();
  delay(350);
  
  ads1262_Reg_Write(POWER, 0x11); 		//Set sampling rate to 125 SPS
  delay(10);
  ads1262_Reg_Write(INTERFACE, 0x05);	//Lead-off comp off, test signal disabled
  delay(10);
  ads1262_Reg_Write(MODE0, 0x00);		//Lead-off defaults
  delay(10);
  ads1262_Reg_Write(MODE1, 0x80);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);
  ads1262_Reg_Write(MODE2, 0x06);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);
  ads1262_Reg_Write(INPMUX, 0x01);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(OFCAL0, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(OFCAL1, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(OFCAL2, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(FSCAL0, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(FSCAL1, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(FSCAL2, 0x40);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(IDACMUX, 0xBB);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
   ads1262_Reg_Write(IDACMAG, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);  
  ads1262_Reg_Write(REFMUX, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(TDACP, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(TDACN, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(GPIOCON, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(GPIODIR, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(GPIODAT, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(ADC2CFG, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(ADC2MUX, 0x01);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(ADC2OFC0, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(ADC2OFC1, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(ADC2FSC0, 0x00);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);    
  ads1262_Reg_Write(ADC2FSC1, 0x40);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);
 // ads1262_Start_Read_Data_Continuous();
  delay(10);
  ads1262_Enable_Start();
}

void ads1262::ads1262_Reset()
{
  digitalWrite(ADS1262_PWDN_PIN, HIGH);
  delay(100);					// Wait 100 mSec
  digitalWrite(ADS1262_PWDN_PIN, LOW);
  delay(100);
  digitalWrite(ADS1262_PWDN_PIN, HIGH);
  delay(100);
}

void ads1262::ads1262_Disable_Start()
{
  digitalWrite(ADS1262_START_PIN, LOW);
  delay(20);
}

void ads1262::ads1262_Enable_Start()
{
  digitalWrite(ADS1262_START_PIN, HIGH);
  delay(20);
}

void ads1262::ads1262_Hard_Stop (void)
{
  digitalWrite(ADS1262_START_PIN, LOW);
  delay(100);
}


void ads1262::ads1262_Start_Data_Conv_Command (void)
{
  ads1262_SPI_Command_Data(START);					// Send 0x08 to the ADS1x9x
}

void ads1262::ads1262_Soft_Stop (void)
{
  ads1262_SPI_Command_Data(STOP);                   // Send 0x0A to the ADS1x9x
}

void ads1262::ads1262_Start_Read_Data_Continuous (void)
{
  //ads1262_SPI_Command_Data(RDATAC);					// Send 0x10 to the ADS1x9x
}

void ads1262::ads1262_Stop_Read_Data_Continuous (void)
{
  //ads1262_SPI_Command_Data(SDATAC);					// Send 0x11 to the ADS1x9x
}

void ads1262::ads1262_SPI_Command_Data(unsigned char data_in)
{
  byte data[1];
  //data[0] = data_in;
  digitalWrite(ADS1262_CS_PIN, LOW);
  delay(2);
  digitalWrite(ADS1262_CS_PIN, HIGH);
  delay(2);
  digitalWrite(ADS1262_CS_PIN, LOW);
  delay(2);
  SPI.transfer(data_in);
  delay(2);
  digitalWrite(ADS1262_CS_PIN, HIGH);
}

//Sends a write command to SCP1000
void ads1262::ads1262_Reg_Write (unsigned char READ_WRITE_ADDRESS, unsigned char DATA)
{
  // now combine the register address and the command into one byte:
  byte dataToSend = READ_WRITE_ADDRESS | WREG;
  
   digitalWrite(ADS1262_CS_PIN, LOW);
   delay(2);
   digitalWrite(ADS1262_CS_PIN, HIGH);
   delay(2);
  // take the chip select low to select the device:
  digitalWrite(ADS1262_CS_PIN, LOW);
  delay(2);
  SPI.transfer(dataToSend); //Send register location
  SPI.transfer(0x00);		//number of register to wr
  SPI.transfer(DATA);		//Send value to record into register
  
  delay(2);
  // take the chip select high to de-select:
  digitalWrite(ADS1262_CS_PIN, HIGH);
}