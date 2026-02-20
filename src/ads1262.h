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

#ifndef ADS1262_H
#define ADS1262_H

#include <Arduino.h>
#include <SPI.h>

// Library version
#define ADS1262_LIBRARY_VERSION "2.0.1"

// SPI Configuration
#define ADS1262_SPI_DUMMY_BYTE    0xFF
#define ADS1262_SPI_CLOCK_SPEED   1000000  // 1MHz

// ADS1262 Commands
#define ADS1262_CMD_NOP           0x00    // No operation
#define ADS1262_CMD_RESET         0x06    // Reset device
#define ADS1262_CMD_START         0x08    // Start/restart conversions
#define ADS1262_CMD_STOP          0x0A    // Stop conversion
#define ADS1262_CMD_RDATA         0x12    // Read data by command
#define ADS1262_CMD_RDATAC        0x10    // Enable read data continuous mode
#define ADS1262_CMD_SDATAC        0x11    // Stop read data continuous mode
#define ADS1262_CMD_RREG          0x20    // Read register (ORed with register address)
#define ADS1262_CMD_WREG          0x40    // Write register (ORed with register address)

// ADS1262 Register Addresses
#define ADS1262_REG_ID            0x00    // Device ID
#define ADS1262_REG_POWER         0x01    // Power control
#define ADS1262_REG_INTERFACE     0x02    // Interface control
#define ADS1262_REG_MODE0         0x03    // Mode control 0
#define ADS1262_REG_MODE1         0x04    // Mode control 1
#define ADS1262_REG_MODE2         0x05    // Mode control 2
#define ADS1262_REG_INPMUX        0x06    // Input multiplexer
#define ADS1262_REG_OFCAL0        0x07    // Offset calibration 0
#define ADS1262_REG_OFCAL1        0x08    // Offset calibration 1
#define ADS1262_REG_OFCAL2        0x09    // Offset calibration 2
#define ADS1262_REG_FSCAL0        0x0A    // Full-scale calibration 0
#define ADS1262_REG_FSCAL1        0x0B    // Full-scale calibration 1
#define ADS1262_REG_FSCAL2        0x0C    // Full-scale calibration 2
#define ADS1262_REG_IDACMUX       0x0D    // IDAC multiplexer
#define ADS1262_REG_IDACMAG       0x0E    // IDAC magnitude
#define ADS1262_REG_REFMUX        0x0F    // Reference multiplexer
#define ADS1262_REG_TDACP         0x10    // Test DAC positive
#define ADS1262_REG_TDACN         0x11    // Test DAC negative
#define ADS1262_REG_GPIOCON       0x12    // GPIO connection
#define ADS1262_REG_GPIODIR       0x13    // GPIO direction
#define ADS1262_REG_GPIODAT       0x14    // GPIO data
#define ADS1262_REG_ADC2CFG       0x15    // ADC2 configuration
#define ADS1262_REG_ADC2MUX       0x16    // ADC2 multiplexer
#define ADS1262_REG_ADC2OFC0      0x17    // ADC2 offset calibration 0
#define ADS1262_REG_ADC2OFC1      0x18    // ADC2 offset calibration 1
#define ADS1262_REG_ADC2FSC0      0x19    // ADC2 full-scale calibration 0
#define ADS1262_REG_ADC2FSC1      0x1A    // ADC2 full-scale calibration 1

// Default pin assignments (can be overridden in constructor)
static constexpr uint8_t DEFAULT_CS_PIN = 7;     ///< Default chip select pin
static constexpr uint8_t DEFAULT_DRDY_PIN = 6;   ///< Default data ready pin  
static constexpr uint8_t DEFAULT_START_PIN = 5;  ///< Default start pin
static constexpr uint8_t DEFAULT_PWDN_PIN = 4;   ///< Default power down pin

// Data rates (POWER register values)
enum ADS1262_DataRate {
    ADS1262_DR_2_5_SPS    = 0x00,
    ADS1262_DR_5_SPS      = 0x01,
    ADS1262_DR_10_SPS     = 0x02,
    ADS1262_DR_16_6_SPS   = 0x03,
    ADS1262_DR_20_SPS     = 0x04,
    ADS1262_DR_50_SPS     = 0x05,
    ADS1262_DR_60_SPS     = 0x06,
    ADS1262_DR_100_SPS    = 0x07,
    ADS1262_DR_400_SPS    = 0x08,
    ADS1262_DR_1200_SPS   = 0x09,
    ADS1262_DR_2400_SPS   = 0x0A,
    ADS1262_DR_4800_SPS   = 0x0B,
    ADS1262_DR_7200_SPS   = 0x0C,
    ADS1262_DR_14400_SPS  = 0x0D,
    ADS1262_DR_19200_SPS  = 0x0E,
    ADS1262_DR_38400_SPS  = 0x0F
};

// Gain settings (MODE2 register values)
enum ADS1262_Gain {
    ADS1262_GAIN_1   = 0x00,
    ADS1262_GAIN_2   = 0x01,
    ADS1262_GAIN_4   = 0x02,
    ADS1262_GAIN_8   = 0x03,
    ADS1262_GAIN_16  = 0x04,
    ADS1262_GAIN_32  = 0x05
};

// Input channel selections
enum ADS1262_InputChannel {
    ADS1262_AIN0  = 0x00,
    ADS1262_AIN1  = 0x01,
    ADS1262_AIN2  = 0x02,
    ADS1262_AIN3  = 0x03,
    ADS1262_AIN4  = 0x04,
    ADS1262_AIN5  = 0x05,
    ADS1262_AIN6  = 0x06,
    ADS1262_AIN7  = 0x07,
    ADS1262_AIN8  = 0x08,
    ADS1262_AIN9  = 0x09,
    ADS1262_AINCOM = 0x0A,
    ADS1262_TEMP_SENSOR_P = 0x0B,
    ADS1262_TEMP_SENSOR_N = 0x0C,
    ADS1262_AVDD_MONITOR = 0x0D,
    ADS1262_DVDD_MONITOR = 0x0E,
    ADS1262_TDAC_TEST = 0x0F
};

// Reference selections
enum ADS1262_Reference {
    ADS1262_REF_INTERNAL_2_5V = 0x00,
    ADS1262_REF_EXTERNAL_AIN0_AIN1 = 0x01,
    ADS1262_REF_EXTERNAL_AIN2_AIN3 = 0x02,
    ADS1262_REF_INTERNAL_AVDD_AVSS = 0x03
};

/**
 * @brief Configuration structure for ADS1262 settings
 */
struct ADS1262_Config {
    ADS1262_DataRate dataRate;     ///< ADC sampling rate
    ADS1262_Gain gain;             ///< Programmable gain amplifier setting
    ADS1262_Reference reference;   ///< Reference voltage source
    bool continuousMode;           ///< Enable continuous conversion mode
    bool bufferEnabled;            ///< Enable input buffer
    uint8_t filterType;            ///< Digital filter type
};

/**
 * @brief Arduino library for the ADS1262 32-bit delta-sigma ADC
 * 
 * This class provides a comprehensive interface to the Texas Instruments ADS1262
 * 32-bit precision ADC. Features include:
 * - Easy configuration and initialization
 * - Both blocking and non-blocking data acquisition
 * - Automatic voltage conversion with programmable gain
 * - Support for multiple reference sources
 * - Calibration functions
 * - Error handling and device verification
 * 
 * @note This library requires the SPI library
 * 
 * @author ProtoCentral Electronics
 * @version 2.0.1
 */

class ADS1262 {
private:
    // Pin assignments
    uint8_t _csPin;
    uint8_t _drdyPin;
    uint8_t _startPin;
    uint8_t _pwdnPin;
    
    // SPI interface
    SPIClass* _spi;
    
    // Configuration
    ADS1262_Config _config;
    bool _initialized;
    bool _continuousMode;
    
    // Internal reference voltage (for calculations)
    float _vref;
    
    // Private methods
    void spiCommand(uint8_t command);
    void spiWriteReg(uint8_t reg, uint8_t value);
    uint8_t spiReadReg(uint8_t reg);
    void waitForDRDY(uint32_t timeout_ms = 1000);
    void setupSPI(bool startSPI = true);
    void setupSPI(uint8_t sck, uint8_t miso, uint8_t mosi);
    void setupPins();
    void resetDevice();
    void applyConfiguration();

public:
    // Constructors
    
    /**
     * @brief Default constructor using standard pin assignments and default SPI
     * 
     * Uses the following default pins:
     * - CS: Pin 7
     * - DRDY: Pin 6  
     * - START: Pin 5
     * - PWDN: Pin 4
     * - SPI: Default SPI instance (&SPI)
     */
    ADS1262();
    
    /**
     * @brief Constructor with custom pin assignments and default SPI
     * 
     * @param csPin Chip select pin
     * @param drdyPin Data ready pin
     * @param startPin Start conversion pin
     * @param pwdnPin Power down pin
     */
    ADS1262(uint8_t csPin, uint8_t drdyPin, uint8_t startPin, uint8_t pwdnPin);
    
    /**
     * @brief Constructor with custom pin assignments and SPI instance
     * 
     * @param csPin Chip select pin
     * @param drdyPin Data ready pin
     * @param startPin Start conversion pin
     * @param pwdnPin Power down pin
     * @param spi Pointer to SPI instance (e.g., &SPI, &SPI1, &SPI2)
     */
    ADS1262(uint8_t csPin, uint8_t drdyPin, uint8_t startPin, uint8_t pwdnPin, SPIClass* spi);
    
    // Initialization and configuration
    
    /**
     * @brief Initialize the ADS1262 with default configuration
     * 
     * @param startSPI Whether to call SPI.begin() (default: true)
     * @return true if initialization successful, false otherwise
     */
    bool begin(bool startSPI = true);
    
    /**
     * @brief Initialize the ADS1262 with custom configuration
     * 
     * @param config Configuration structure with desired settings
     * @param startSPI Whether to call SPI.begin() (default: true)
     * @return true if initialization successful, false otherwise
     */
    bool begin(const ADS1262_Config& config, bool startSPI = true);
    
    /**
     * @brief Initialize the ADS1262 with custom SPI pins (ESP32 style)
     * 
     * This method is convenient for ESP32 where different SPI ports use different pins.
     * 
     * @param sck SPI clock pin
     * @param miso SPI MISO pin
     * @param mosi SPI MOSI pin
     * @return true if initialization successful, false otherwise
     */
    bool begin(uint8_t sck, uint8_t miso, uint8_t mosi);
    
    /**
     * @brief Initialize the ADS1262 with custom SPI pins and configuration
     * 
     * @param sck SPI clock pin
     * @param miso SPI MISO pin
     * @param mosi SPI MOSI pin
     * @param config Configuration structure with desired settings
     * @return true if initialization successful, false otherwise
     */
    bool begin(uint8_t sck, uint8_t miso, uint8_t mosi, const ADS1262_Config& config);
    
    /**
     * @brief Cleanup and disable the ADS1262
     */
    void end();
    
    // Configuration methods
    
    /**
     * @brief Set the ADC data rate (sampling frequency)
     * 
     * @param rate Data rate from ADS1262_DataRate enumeration
     */
    void setDataRate(ADS1262_DataRate rate);
    
    /**
     * @brief Set the programmable gain amplifier (PGA) gain
     * 
     * @param gain Gain setting from ADS1262_Gain enumeration (1x to 32x)
     */
    void setGain(ADS1262_Gain gain);
    
    /**
     * @brief Set the voltage reference source
     * 
     * @param ref Reference source from ADS1262_Reference enumeration
     */
    void setReference(ADS1262_Reference ref);
    
    /**
     * @brief Configure the input multiplexer for differential measurement
     * 
     * @param positive Positive input channel
     * @param negative Negative input channel
     */
    void setInputMux(ADS1262_InputChannel positive, ADS1262_InputChannel negative);
    
    /**
     * @brief Enable or disable input buffer
     * 
     * @param enable true to enable buffer, false to disable
     */
    void enableBuffer(bool enable);
    
    /**
     * @brief Enable or disable continuous conversion mode
     * 
     * @param enable true for continuous mode, false for single-shot
     */
    void setContinuousMode(bool enable);
    
    // Data acquisition
    
    /**
     * @brief Start ADC conversion
     * 
     * @return true if conversion started successfully
     */
    bool startConversion();
    
    /**
     * @brief Stop ADC conversion
     */
    void stopConversion();
    
    /**
     * @brief Check if new conversion data is available
     * 
     * @return true if data is ready to read
     */
    bool isDataReady() const;
    
    /**
     * @brief Read raw ADC counts (blocking)
     * 
     * This function waits for data to be ready before returning.
     * 
     * @return Raw ADC counts as signed 32-bit integer
     */
    int32_t readRaw();
    
    /**
     * @brief Read voltage with automatic conversion (blocking)
     * 
     * Converts raw counts to voltage based on current reference and gain settings.
     * 
     * @return Voltage in volts
     */
    float readVoltage();
    
    /**
     * @brief Non-blocking raw ADC read
     * 
     * @param result Reference to store the raw counts if data is available
     * @return true if new data was available and read, false otherwise
     */
    bool readRawAsync(int32_t& result);
    
    /**
     * @brief Non-blocking voltage read
     * 
     * @param result Reference to store the voltage if data is available
     * @return true if new data was available and read, false otherwise
     */
    bool readVoltageAsync(float& result);
    
    // Advanced features
    
    /**
     * @brief Perform internal self-calibration
     * 
     * Calibrates offset and gain using internal references.
     */
    void performSelfCalibration();
    
    /**
     * @brief Perform system calibration with external references
     * 
     * Requires external precision references connected to inputs.
     */
    void performSystemCalibration();
    
    /**
     * @brief Set manual offset calibration value
     * 
     * @param offset 24-bit signed offset calibration value
     */
    void setOffsetCalibration(int32_t offset);
    
    /**
     * @brief Set manual gain calibration value
     * 
     * @param gain 24-bit unsigned full-scale calibration value
     */
    void setGainCalibration(uint32_t gain);
    
    // Utility methods
    
    /**
     * @brief Read the device ID register
     * 
     * @return Device ID (varies by revision - 0x01, 0x02, 0x03, etc.)
     */
    uint8_t readDeviceID();
    
    /**
     * @brief Test communication with the device
     * 
     * @return true (always - device ID check is bypassed)
     */
    bool testConnection();
    
    /**
     * @brief Put device into power-down mode
     */
    void powerDown();
    
    /**
     * @brief Wake device from power-down mode
     */
    void powerUp();
    
    /**
     * @brief Reset the device to default state
     */
    void reset();
    
    // Direct register access (for advanced users)
    
    /**
     * @brief Write directly to a device register
     * 
     * @param reg Register address
     * @param value Value to write
     */
    void writeRegister(uint8_t reg, uint8_t value);
    
    /**
     * @brief Read directly from a device register
     * 
     * @param reg Register address
     * @return Register value
     */
    uint8_t readRegister(uint8_t reg);
    
    // Helper methods
    
    /**
     * @brief Convert raw counts to voltage
     * 
     * @param counts Raw ADC counts
     * @return Equivalent voltage
     */
    float countsToVoltage(int32_t counts) const;
    
    /**
     * @brief Convert voltage to raw counts
     * 
     * @param voltage Voltage value  
     * @return Equivalent raw counts
     */
    int32_t voltageToCounts(float voltage) const;
    
    /**
     * @brief Get library version string
     * 
     * @return Version string
     */
    static const char* getLibraryVersion();
};

#endif // ADS1262_H

