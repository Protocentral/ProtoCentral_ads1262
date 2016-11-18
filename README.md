ADS1262 Breakout Board
=======================

[![Ads1262](https://www.protocentral.com/3826-thickbox_default/protocentral-ads1262-32-bit-precision-adc-breakout-board.jpg)*ADS1262 32-bit, 11-Multifunction Low Noise,high resolution ADC Breakout Board (PC-4143)*](https://www.protocentral.com/analog-adc-boards/1005-protocentral-ads1262-32-bit-precision-adc-breakout-board.html)

The 24-bit ADS1220 ADC still not enough for you. Need more? Look no further than the new ADS1262 IC from Texas Instruments with a 32-bit ΔΣ ADC with built-in PGAs, voltage reference, current sources and everything else but the kitchen sink.
What does 32-bit precision mean to you? Compared to a commonly used 12-bit ADC which can provide 4096 levels of the input analog voltage, a 32-bit ADC, in theory, can provide 4294967296 levels !!
Now plug-in your high precision strain gauge/load cell directly to this ADC without any analog signal conditioning required in between. Whether it is for ultra-high precision weighing scales, strain gauges or temperature sensors, you can be assured that you can get the highest available precision.
The Arduino libraries provided contain all the required functions to use all of these features with simple function calls.

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
|ads1262 pin label| Pin Function         |Arduino Connection|
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
  
  ** The example sketch is configured for internal Vref= 2.50V, provide the differential voltage across AN0 and AN1.
  

Repository Contents
-------------------
* **/Libraries** - Arduino library and example sketches.
* **/Hardware** - All Eagle design files (.brd, .sch)
* **/extras** - includes the datasheet
 
 
License Information
===================
This product is open source! Both, our hardware and software are open source and licensed under the following licenses:

Hardware
---------
All hardware is released under [Creative Commons Share-alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/).**
![CC-BY-SA-4.0](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

Software
--------
All software is released under the MIT License(http://opensource.org/licenses/MIT).**

Please check [*LICENSE.md*](LICENSE.md) for detailed license descriptions.
