ADS1262 Breakout Board
=======================

[![Ads1262](https://www.protocentral.com/3827-tm_thickbox_default/protocentral-ads1262-32-bit-precision-adc-breakout-board.jpg)  *ADS1262 32-bit, 11-Multifunction Low Noise,high resolution ADC Breakout Board (PC-4143)*](https://www.protocentral.com/breakout-boards/1005-protocentral-ads1262-32-bit-precision-adc-breakout-board.html)

Why would you need a 32-bit ADC? When 24-bit is just not enough. 
If your design demands high precision analog measurements, this new breakout board for the ADS1262 32-bit Sigma-delta ADC can get the job done.
Most of the previously available 32-bit ADCs have been difficult to use since they require a separate voltage reference and a lot of external components among other things. The ADS1262 gets rid of almost all external components with an internal voltage reference and internal clock. This is one of the easiest to use 32-bit ADCs that we have come across. 
We also provide the Arduino libraries for interfacing this chip with your Arduino. To install the library, download the zip file from our github site, rename it to ProtoCentral_ADS1262 and unzip this file in your Arduino libraries folder.

Features
---------
* Ultra-high precision 32-bit ΔΣ ADC
* 11 programmable analog inputs
* Differential Input Programmable Gain Amplifier (PGA)
* Programmable Data Rates: Up to 38 kSPS
* 2 Built-in current sources for sensor excitation
* Built-in programmable digital filter
* Internal Voltage Reference (2.5 V)

Applications
-------------
* High Precision analog voltage measurement
* RTD interfacing
* High precision chemical sensors

Wiring to your Arduino
----------------------
|ads1220 pin label| Pin Function         |Arduino Connection|
|-----------------|:--------------------:|-----------------:|
| DRDY            | Data ready Output pin|  D6              |             
| MISO            | Slave Out            |  D12             |
| MOSI            | Slave In             |  D11             |
| SCLK            | Serial Clock         |  D13             |
| CS              | Chip Select          |  D7              |
| START           | Start Conversion     |  D5              | 
| PWDN            | Power Down/Reset     |  D4              |
| DVDD            | Digital VDD          |  +5V             |
| DGND            | Digital Gnd          |  Gnd             |
| AN0-AN9         | Analog Input         |  Analog Input    |
| AINCOM          | Analog input common  |                  |
| AVDD            | Analog VDD           |  -               |
| AGND            | Analog Gnd           |  -               |
  
  ** The example sketch is configured for internal Vref=2.048, provide the differential voltage across AN0 and AN1.
  

Repository Contents
-------------------
* **/Libraries** - Arduino library and example sketches.
* **/Hardware** - All Eagle design files (.brd, .sch)
* **/extras** - includes the datasheet
 

License Information
-------------------
This product is open source!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to Protocentral and release anything derivative under the same license.
