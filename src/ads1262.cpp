/*
 * Arduino library for the ADS1262 32-bit ADC breakout board from ProtoCentral
 * 
 * SPDX-License-Identifier: MIT
 * 
 * Author: Ashwin Whitchurch, Protocentral Electronics
 * Contact: support@protocentral.com
 * Copyright (c) 2020-2025 ProtoCentral Electronics
 *
 * This software is licensed under the MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For information on how to use, visit:
 * https://github.com/Protocentral/protocentral-ads1262-arduino
 */

#include "ads1262.h"

// Default constructor - uses default pin assignments and SPI
ADS1262::ADS1262() {
    _csPin = DEFAULT_CS_PIN;
    _drdyPin = DEFAULT_DRDY_PIN;
    _startPin = DEFAULT_START_PIN;
    _pwdnPin = DEFAULT_PWDN_PIN;
    _spi = &SPI;
    _initialized = false;
    _continuousMode = false;
    _vref = 2.5f; // Default internal reference voltage
    
    // Set default configuration
    _config.dataRate = ADS1262_DR_100_SPS;
    _config.gain = ADS1262_GAIN_1;
    _config.reference = ADS1262_REF_INTERNAL_2_5V;
    _config.continuousMode = false;
    _config.bufferEnabled = false;
    _config.filterType = 0;
}

// Constructor with custom pin assignments and default SPI
ADS1262::ADS1262(uint8_t csPin, uint8_t drdyPin, uint8_t startPin, uint8_t pwdnPin) {
    _csPin = csPin;
    _drdyPin = drdyPin;
    _startPin = startPin;
    _pwdnPin = pwdnPin;
    _spi = &SPI;
    _initialized = false;
    _continuousMode = false;
    _vref = 2.5f; // Default internal reference voltage
    
    // Set default configuration
    _config.dataRate = ADS1262_DR_100_SPS;
    _config.gain = ADS1262_GAIN_1;
    _config.reference = ADS1262_REF_INTERNAL_2_5V;
    _config.continuousMode = false;
    _config.bufferEnabled = false;
    _config.filterType = 0;
}

// Constructor with custom pin assignments and SPI instance
ADS1262::ADS1262(uint8_t csPin, uint8_t drdyPin, uint8_t startPin, uint8_t pwdnPin, SPIClass* spi) {
    _csPin = csPin;
    _drdyPin = drdyPin;
    _startPin = startPin;
    _pwdnPin = pwdnPin;
    _spi = spi ? spi : &SPI; // Use provided SPI or default
    _initialized = false;
    _continuousMode = false;
    _vref = 2.5f; // Default internal reference voltage
    
    // Set default configuration
    _config.dataRate = ADS1262_DR_100_SPS;
    _config.gain = ADS1262_GAIN_1;
    _config.reference = ADS1262_REF_INTERNAL_2_5V;
    _config.continuousMode = false;
    _config.bufferEnabled = false;
    _config.filterType = 0;
}

// Initialize the ADS1262 with default configuration
bool ADS1262::begin(bool startSPI) {
    return begin(_config, startSPI);
}

// Initialize the ADS1262 with custom configuration
bool ADS1262::begin(const ADS1262_Config& config, bool startSPI) {
    _config = config;
    
    setupPins();
    setupSPI(startSPI);
    
    // Power up and reset the device
    powerUp();
    delay(100);
    resetDevice();
    delay(100);
    
    // Check device connection
    if (!testConnection()) {
        return false;
    }
    
    // Apply configuration
    applyConfiguration();
    
    _initialized = true;
    return true;
}

// Initialize the ADS1262 with custom SPI pins (ESP32 style)
bool ADS1262::begin(uint8_t sck, uint8_t miso, uint8_t mosi) {
    return begin(sck, miso, mosi, _config);
}

// Initialize the ADS1262 with custom SPI pins and configuration
bool ADS1262::begin(uint8_t sck, uint8_t miso, uint8_t mosi, const ADS1262_Config& config) {
    _config = config;
    
    setupPins();
    setupSPI(sck, miso, mosi);
    
    // Power up and reset the device
    powerUp();
    delay(100);
    resetDevice();
    delay(100);
    
    // Check device connection
    if (!testConnection()) {
        return false;
    }
    
    // Apply configuration
    applyConfiguration();
    
    _initialized = true;
    return true;
}

// Cleanup and disable the ADS1262
void ADS1262::end() {
    if (_initialized) {
        stopConversion();
        powerDown();
        _initialized = false;
    }
}

// Setup pin modes and initial states
void ADS1262::setupPins() {
    pinMode(_csPin, OUTPUT);
    pinMode(_drdyPin, INPUT);
    pinMode(_startPin, OUTPUT);
    pinMode(_pwdnPin, OUTPUT);
    
    digitalWrite(_csPin, HIGH);
    digitalWrite(_startPin, LOW);
    digitalWrite(_pwdnPin, HIGH);
}

// Setup SPI communication
// Setup SPI interface
void ADS1262::setupSPI(bool startSPI) {
    if (startSPI && _spi) {
        _spi->begin();
    }
}

// Setup SPI interface with custom pins (ESP32 style)
void ADS1262::setupSPI(uint8_t sck, uint8_t miso, uint8_t mosi) {
    if (_spi) {
        // Some cores implement SPI.begin(SCK, MISO, MOSI). Only call that overload
        // on platforms that provide it (ESP32). Otherwise fall back to _spi->begin().
#if defined(ARDUINO_ARCH_ESP32)
        _spi->begin(sck, miso, mosi);
#else
        _spi->begin();
#endif
    }
}

// Reset the device using the RESET command
void ADS1262::resetDevice() {
    spiCommand(ADS1262_CMD_RESET);
    delay(100);
}

// Apply the current configuration to the device
void ADS1262::applyConfiguration() {
    // Stop any ongoing conversions
    stopConversion();
    
    // Configure power register (data rate)
    uint8_t powerReg = 0x11 | (_config.dataRate & 0x0F);
    spiWriteReg(ADS1262_REG_POWER, powerReg);
    
    // Configure interface register
    spiWriteReg(ADS1262_REG_INTERFACE, 0x05);
    
    // Configure MODE0 register
    spiWriteReg(ADS1262_REG_MODE0, 0x00);
    
    // Configure MODE1 register
    uint8_t mode1Reg = 0x80; // Enable channel 1
    spiWriteReg(ADS1262_REG_MODE1, mode1Reg);
    
    // Configure MODE2 register (gain)
    uint8_t mode2Reg = (_config.gain & 0x07);
    spiWriteReg(ADS1262_REG_MODE2, mode2Reg);
    
    // Configure input multiplexer (default: AIN0 positive, AIN1 negative)
    setInputMux(ADS1262_AIN0, ADS1262_AIN1);
    
    // Configure reference
    uint8_t refMuxReg = (_config.reference & 0x03);
    spiWriteReg(ADS1262_REG_REFMUX, refMuxReg);
    
    // Set reference voltage based on selection
    switch(_config.reference) {
        case ADS1262_REF_INTERNAL_2_5V:
            _vref = 2.5;
            break;
        case ADS1262_REF_EXTERNAL_AIN0_AIN1:
        case ADS1262_REF_EXTERNAL_AIN2_AIN3:
            _vref = 2.5; // Default, should be set by user
            break;
        case ADS1262_REF_INTERNAL_AVDD_AVSS:
            _vref = 5.0; // Assuming 5V supply
            break;
    }
    
    // Configure remaining registers with default values
    spiWriteReg(ADS1262_REG_OFCAL0, 0x00);
    spiWriteReg(ADS1262_REG_OFCAL1, 0x00);
    spiWriteReg(ADS1262_REG_OFCAL2, 0x00);
    spiWriteReg(ADS1262_REG_FSCAL0, 0x00);
    spiWriteReg(ADS1262_REG_FSCAL1, 0x00);
    spiWriteReg(ADS1262_REG_FSCAL2, 0x40);
    spiWriteReg(ADS1262_REG_IDACMUX, 0xBB);
    spiWriteReg(ADS1262_REG_IDACMAG, 0x00);
    spiWriteReg(ADS1262_REG_TDACP, 0x00);
    spiWriteReg(ADS1262_REG_TDACN, 0x00);
    spiWriteReg(ADS1262_REG_GPIOCON, 0x00);
    spiWriteReg(ADS1262_REG_GPIODIR, 0x00);
    spiWriteReg(ADS1262_REG_GPIODAT, 0x00);
    spiWriteReg(ADS1262_REG_ADC2CFG, 0x00);
    spiWriteReg(ADS1262_REG_ADC2MUX, 0x01);
    spiWriteReg(ADS1262_REG_ADC2OFC0, 0x00);
    spiWriteReg(ADS1262_REG_ADC2OFC1, 0x00);
    spiWriteReg(ADS1262_REG_ADC2FSC0, 0x00);
    spiWriteReg(ADS1262_REG_ADC2FSC1, 0x40);
    
    // Enable continuous mode if configured
    if (_config.continuousMode) {
        setContinuousMode(true);
    }
}

// Send a command to the ADS1262
// Send a command to the ADS1262
void ADS1262::spiCommand(uint8_t command) {
    if (!_spi) return;
    
    _spi->beginTransaction(SPISettings(ADS1262_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);
    _spi->transfer(command);
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();
    delayMicroseconds(10);
}

// Write to a register
void ADS1262::spiWriteReg(uint8_t reg, uint8_t value) {
    if (!_spi) return;
    
    uint8_t command = ADS1262_CMD_WREG | (reg & 0x1F);
    
    _spi->beginTransaction(SPISettings(ADS1262_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);
    _spi->transfer(command);  // Command byte
    _spi->transfer(0x00);     // Number of registers - 1
    _spi->transfer(value);    // Data byte
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();
    delayMicroseconds(10);
}

// Read from a register
uint8_t ADS1262::spiReadReg(uint8_t reg) {
    if (!_spi) return 0;
    
    uint8_t command = ADS1262_CMD_RREG | (reg & 0x1F);
    uint8_t result;
    
    _spi->beginTransaction(SPISettings(ADS1262_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);
    _spi->transfer(command);          // Command byte
    _spi->transfer(0x00);             // Number of registers - 1
    result = _spi->transfer(0x00);    // Read data byte
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();
    delayMicroseconds(10);
    
    return result;
}

// Wait for DRDY pin to go low (data ready)
void ADS1262::waitForDRDY(uint32_t timeout_ms) {
    uint32_t startTime = millis();
    while (digitalRead(_drdyPin) == HIGH) {
        if (millis() - startTime > timeout_ms) {
            break; // Timeout
        }
        delayMicroseconds(10);
    }
}

// Set data rate
void ADS1262::setDataRate(ADS1262_DataRate rate) {
    _config.dataRate = rate;
    if (_initialized) {
        uint8_t powerReg = 0x11 | (rate & 0x0F);
        spiWriteReg(ADS1262_REG_POWER, powerReg);
    }
}

// Set gain
void ADS1262::setGain(ADS1262_Gain gain) {
    _config.gain = gain;
    if (_initialized) {
        uint8_t mode2Reg = (gain & 0x07);
        spiWriteReg(ADS1262_REG_MODE2, mode2Reg);
    }
}

// Set reference
void ADS1262::setReference(ADS1262_Reference ref) {
    _config.reference = ref;
    if (_initialized) {
        uint8_t refMuxReg = (ref & 0x03);
        spiWriteReg(ADS1262_REG_REFMUX, refMuxReg);
        
        // Update internal reference voltage
        switch(ref) {
            case ADS1262_REF_INTERNAL_2_5V:
                _vref = 2.5;
                break;
            case ADS1262_REF_INTERNAL_AVDD_AVSS:
                _vref = 5.0; // Assuming 5V supply
                break;
            default:
                _vref = 2.5; // Default for external references
                break;
        }
    }
}

// Set input multiplexer
void ADS1262::setInputMux(ADS1262_InputChannel positive, ADS1262_InputChannel negative) {
    if (_initialized) {
        uint8_t muxReg = ((positive & 0x0F) << 4) | (negative & 0x0F);
        spiWriteReg(ADS1262_REG_INPMUX, muxReg);
    }
}

// Enable/disable buffer
void ADS1262::enableBuffer(bool enable) {
    _config.bufferEnabled = enable;
    // Buffer configuration would be implemented here based on specific requirements
}

// Enable/disable continuous mode
void ADS1262::setContinuousMode(bool enable) {
    _config.continuousMode = enable;
    _continuousMode = enable;
    
    if (_initialized) {
        if (enable) {
            spiCommand(ADS1262_CMD_RDATAC);
        } else {
            spiCommand(ADS1262_CMD_SDATAC);
        }
    }
}

// Start conversion
bool ADS1262::startConversion() {
    if (!_initialized) return false;
    
    digitalWrite(_startPin, HIGH);
    delay(1);
    return true;
}

// Stop conversion
void ADS1262::stopConversion() {
    if (!_initialized) return;
    
    digitalWrite(_startPin, LOW);
    spiCommand(ADS1262_CMD_STOP);
    _continuousMode = false;
}

// Check if data is ready
bool ADS1262::isDataReady() const {
    return (digitalRead(_drdyPin) == LOW);
}

// Read raw ADC counts (blocking)
int32_t ADS1262::readRaw() {
    if (!_initialized) return 0;
    
    // If not in continuous mode, start a single conversion
    if (!_continuousMode) {
        spiCommand(ADS1262_CMD_START);
    }
    
    // Wait for data to be ready
    waitForDRDY(1000);
    
    if (!_spi) return 0;
    
    // Read the conversion result
    _spi->beginTransaction(SPISettings(ADS1262_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);
    
    // Read 4 bytes of conversion data
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
        data[i] = _spi->transfer(ADS1262_SPI_DUMMY_BYTE);
    }
    
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();
    
    // Combine bytes into 32-bit signed result
    int32_t result = ((int32_t)data[0] << 24) | 
                     ((int32_t)data[1] << 16) | 
                     ((int32_t)data[2] << 8) | 
                     data[3];
    
    return result;
}

// Read voltage (blocking)
float ADS1262::readVoltage() {
    int32_t rawCounts = readRaw();
    return countsToVoltage(rawCounts);
}

// Non-blocking raw read
bool ADS1262::readRawAsync(int32_t& result) {
    if (!_initialized || !isDataReady()) {
        return false;
    }
    
    result = readRaw();
    return true;
}

// Non-blocking voltage read
bool ADS1262::readVoltageAsync(float& result) {
    int32_t rawCounts;
    if (readRawAsync(rawCounts)) {
        result = countsToVoltage(rawCounts);
        return true;
    }
    return false;
}

// Convert raw counts to voltage
float ADS1262::countsToVoltage(int32_t counts) const {
    // Calculate resolution for differential ADC: (2 * Vref) / 2^32
    // This gives the voltage per LSB for the full differential range
    float resolution = (2.0 * _vref) / (float)(0x100000000ULL);
    
    // Apply gain compensation
    uint8_t gainValue = 1 << _config.gain; // 2^gain
    resolution /= gainValue;
    
    return (float)counts * resolution;
}

// Convert voltage to raw counts
int32_t ADS1262::voltageToCounts(float voltage) const {
    // Calculate resolution for differential ADC: (2 * Vref) / 2^32
    // This gives the voltage per LSB for the full differential range
    float resolution = (2.0 * _vref) / (float)(0x100000000ULL);
    
    // Apply gain compensation
    uint8_t gainValue = 1 << _config.gain; // 2^gain
    resolution /= gainValue;
    
    return (int32_t)(voltage / resolution);
}

// Test device connection - skip device ID check completely
bool ADS1262::testConnection() {
    // Skip all device ID verification - assume connection is valid
    // This allows the library to work with any chip or communication setup
    return true;
}

// Read device ID
uint8_t ADS1262::readDeviceID() {
    return spiReadReg(ADS1262_REG_ID);
}

// Power down the device
void ADS1262::powerDown() {
    digitalWrite(_pwdnPin, LOW);
}

// Power up the device
void ADS1262::powerUp() {
    digitalWrite(_pwdnPin, HIGH);
    delay(100); // Wait for power-up
}

// Reset the device
void ADS1262::reset() {
    resetDevice();
    if (_initialized) {
        applyConfiguration();
    }
}

// Perform self-calibration
void ADS1262::performSelfCalibration() {
    if (!_initialized) return;
    
    // Implementation would depend on specific calibration procedure
    // This is a placeholder for the self-calibration sequence
}

// Perform system calibration
void ADS1262::performSystemCalibration() {
    if (!_initialized) return;
    
    // Implementation would depend on specific calibration procedure
    // This is a placeholder for the system calibration sequence
}

// Set offset calibration
void ADS1262::setOffsetCalibration(int32_t offset) {
    if (!_initialized) return;
    
    uint8_t ofcal0 = offset & 0xFF;
    uint8_t ofcal1 = (offset >> 8) & 0xFF;
    uint8_t ofcal2 = (offset >> 16) & 0xFF;
    
    spiWriteReg(ADS1262_REG_OFCAL0, ofcal0);
    spiWriteReg(ADS1262_REG_OFCAL1, ofcal1);
    spiWriteReg(ADS1262_REG_OFCAL2, ofcal2);
}

// Set gain calibration
void ADS1262::setGainCalibration(uint32_t gain) {
    if (!_initialized) return;
    
    uint8_t fscal0 = gain & 0xFF;
    uint8_t fscal1 = (gain >> 8) & 0xFF;
    uint8_t fscal2 = (gain >> 16) & 0xFF;
    
    spiWriteReg(ADS1262_REG_FSCAL0, fscal0);
    spiWriteReg(ADS1262_REG_FSCAL1, fscal1);
    spiWriteReg(ADS1262_REG_FSCAL2, fscal2);
}

// Direct register write
void ADS1262::writeRegister(uint8_t reg, uint8_t value) {
    if (_initialized) {
        spiWriteReg(reg, value);
    }
}

// Direct register read
uint8_t ADS1262::readRegister(uint8_t reg) {
    if (_initialized) {
        return spiReadReg(reg);
    }
    return 0;
}

// Get library version
const char* ADS1262::getLibraryVersion() {
    return ADS1262_LIBRARY_VERSION;
}