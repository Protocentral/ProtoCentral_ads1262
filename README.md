ADS1262 Breakout Board
=======================

ADS1262 32-bit, 11-Multifunction Low Noise,high resolution ADC Breakout Board (PC-4143)

[Don't have it yet? Buy one here: ProtoCentral ADS1262 32-bit precision ADC breakout board (PC-4143)](https://protocentral.com/product/protocentral-ads1262-32-bit-precision-adc-breakout-board/)
![ADC breakout](https://github.com/Protocentral/ProtoCentral_ads1262/blob/master/extras/ADS1262.jpg?raw=true)

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
| AVDD            | Analog VDD           |  +5V             |
| AGND            | Analog Gnd           |  GND             |
  
  ** The example sketch is configured for internal Vref= 2.50V, provide the differential voltage across AN0 and AN1.
  

Repository Contents
-------------------
* **/Libraries** - Arduino library and example sketches.
* **/Hardware** - All Eagle design files (.brd, .sch)
* **/extras** - includes the datasheet
 
 
License Information
===================

![License](license_mark.svg)

This product is open source! Both, our hardware and software are open source and licensed under the following licenses:

Hardware
---------

**All hardware is released under the [CERN-OHL-P v2](https://ohwr.org/cern_ohl_p_v2.txt)** license.

Copyright CERN 2020.

This source describes Open Hardware and is licensed under the CERN-OHL-P v2.

You may redistribute and modify this documentation and make products
using it under the terms of the CERN-OHL-P v2 (https:/cern.ch/cern-ohl).
This documentation is distributed WITHOUT ANY EXPRESS OR IMPLIED
WARRANTY, INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY
AND FITNESS FOR A PARTICULAR PURPOSE. Please see the CERN-OHL-P v2
for applicable conditions

Software
--------

**All software is released under the MIT License(http://opensource.org/licenses/MIT).**

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Documentation
-------------
**All documentation is released under [Creative Commons Share-alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/).**
![CC-BY-SA-4.0](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

You are free to:

* Share — copy and redistribute the material in any medium or format
* Adapt — remix, transform, and build upon the material for any purpose, even commercially.
The licensor cannot revoke these freedoms as long as you follow the license terms.

Under the following terms:

* Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
* ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

Please check [*LICENSE.md*](LICENSE.md) for detailed license descriptions.
