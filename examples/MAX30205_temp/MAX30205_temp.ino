//////////////////////////////////////////////////////////////////////////////////////////
//
//    Arduino example for the MAX30205 body temperature sensor breakout board
//
//    Author: Ashwin Whitchurch
//    Copyright (c) 2018 ProtoCentral
//
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//   NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//   For information on how to use, visit https://github.com/protocentral/ProtoCentral_MAX30205
/////////////////////////////////////////////////////////////////////////////////////////

/*

This program Print temperature on terminal

Hardware Connections (Breakoutboard to Arduino):
Vin  - 5V (3.3V is allowed)
GND - GND
SDA - A4 (or SDA)
SCL - A5 (or SCL)
*/

#include <Wire.h>
#include "Protocentral_MAX30205.h"
MAX30205 tempSensor;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  tempSensor.begin();   // set continuos mode, active mode
}

void loop() {

	float temp = tempSensor.getTemperature(); // read temperature for every 100ms
	Serial.print(temp ,2);
	Serial.println("'c" );
	delay(100);
}
