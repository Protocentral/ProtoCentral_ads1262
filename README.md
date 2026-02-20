ADS1262 Breakout Board
=======================

ADS1262 32-bit, 11-Channel Multifunction Low-Noise, High-Resolution ADC Breakout Board (PC-4143)

[Don't have it yet? Buy one here: ProtoCentral ADS1262 32-bit precision ADC breakout board (PC-4143)](https://protocentral.com/product/protocentral-ads1262-32-bit-precision-adc-breakout-board/)
![ADC breakout](extras/ADS1262.jpg)

The 24-bit ADS1220 ADC still not enough for you? Need more? Look no further than the ADS1262 from Texas Instruments — a 32-bit Delta-Sigma ADC with a built-in PGA, voltage reference, current sources and everything else but the kitchen sink.

What does 32-bit precision mean to you? Compared to a commonly used 12-bit ADC which can provide 4,096 levels of the input analog voltage, a 32-bit ADC can, in theory, provide 4,294,967,296 levels!

Now plug-in your high-precision strain gauge or load cell directly to this ADC without any external analog signal conditioning. Whether it is for ultra-high precision weighing scales, strain gauges, RTDs or chemical sensors, you can be assured that you will get the highest available precision from this breakout board.

Features
---------
* Ultra-high precision 32-bit Delta-Sigma ADC
* 11 programmable analog inputs (AIN0-AIN9 + AINCOM)
* Differential Input Programmable Gain Amplifier (PGA): 1x to 32x
* Programmable Data Rates: 2.5 SPS to 38.4 kSPS
* 2 built-in current sources for sensor excitation
* Built-in programmable digital filter
* Internal Voltage Reference (2.5 V)
* Internal temperature sensor
* SPI interface
* 3.3V or 5V operation

Specifications
---------------

### Input Ranges (with 2.5V internal reference)

| Gain | Input Range  | Optimal Use Case              |
|:----:|:------------:|-------------------------------|
| 1x   | +/-2.5V      | High voltage signals          |
| 2x   | +/-1.25V     | Medium voltage signals        |
| 4x   | +/-625mV     | Low voltage signals           |
| 8x   | +/-312.5mV   | Very low voltage signals      |
| 16x  | +/-156.25mV  | Strain gauges                 |
| 32x  | +/-78.125mV  | Ultra-low signals, load cells |

Applications
-------------
* High precision analog voltage measurement
* Strain gauges and load cells
* RTD (Resistance Temperature Detector) interfacing
* Thermocouple measurement
* High precision chemical sensors (pH, conductivity)
* Multi-channel data acquisition systems
* Calibration and reference equipment

Wiring to your Arduino
-----------------------
| ADS1262 Pin | Pin Function         | Arduino Connection |
|-------------|:--------------------:|-------------------:|
| DRDY        | Data Ready Output    | D6                 |
| MISO        | Slave Out (SPI)      | D12                |
| MOSI        | Slave In (SPI)       | D11                |
| SCLK        | Serial Clock (SPI)   | D13                |
| CS          | Chip Select          | D7                 |
| START       | Start Conversion     | D5                 |
| PWDN        | Power Down / Reset   | D4                 |
| DVDD        | Digital VDD          | +5V                |
| DGND        | Digital Gnd          | GND                |
| AN0-AN9     | Analog Inputs        | Analog Input       |
| AINCOM      | Analog Input Common  |                    |
| AVDD        | Analog VDD           | +5V                |
| AGND        | Analog Gnd           | GND                |

Getting Started
----------------

Install the library through the Arduino Library Manager (search for "ProtoCentral ADS1262") or clone this repository into your Arduino libraries folder.

```cpp
#include <ads1262.h>

ADS1262 adc;

void setup() {
    Serial.begin(115200);

    if (!adc.begin()) {
        Serial.println("Failed to initialize ADS1262!");
        while(1);
    }

    // Configure for your application
    adc.setDataRate(ADS1262_DR_100_SPS);
    adc.setGain(ADS1262_GAIN_1);
    adc.setInputMux(ADS1262_AIN0, ADS1262_AIN1);
}

void loop() {
    float voltage = adc.readVoltage();
    Serial.print("Voltage: ");
    Serial.print(voltage, 6);
    Serial.println(" V");
    delay(1000);
}
```

The example sketches are configured for internal Vref = 2.50V and differential voltage measurement across AIN0 and AIN1.

Repository Contents
-------------------
* **/src** - Arduino library source files
* **/examples** - Example Arduino sketches
* **/Hardware** - All Eagle design files (.brd, .sch)
* **/extras** - Includes the datasheet


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
