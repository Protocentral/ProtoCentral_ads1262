/*
 * 01-Basic-Usage for the ADS1262 32-bit ADC Library
 * 
 * SPDX-License-Identifier: MIT
 * 
 * Author: Ashwin Whitchurch, Protocentral Electronics
 * Contact: support@protocentral.com
 * Copyright (c) 2020-2025 ProtoCentral Electronics
 * 
 * This example demonstrates the basic usage of the ADS1262 library
 * showing differential voltage measurement between AIN0 and AIN1.
 * 
 * Features demonstrated:
 * - Device initialization and configuration
 * - ESP32 SPI port selection support
 * - Blocking voltage readings with detailed output
 * - Non-blocking async voltage readings
 * - Raw ADC counts display
 * - Multiple output formats (V, mV)
 * - Device information display
 * 
 * Hardware Connections:
 * |ADS1262 Pin | Function        | Arduino Pin | ESP32 (VSPI) |
 * |------------|:---------------:|:-----------:|:-------------:|
 * | DRDY       | Data Ready      | D6 (DRDY)   | D6 (DRDY)     |
 * | MISO       | SPI Data Out    | D12         | D19           |
 * | MOSI       | SPI Data In     | D11         | D23           |
 * | SCLK       | SPI Clock       | D13         | D18           |
 * | CS         | Chip Select     | D7 (CS)     | D7 (CS)       |
 * | START      | Start Convert   | D5 (START)  | D5 (START)    |
 * | PWDN       | Power Down      | D4 (PWDN)   | D4 (PWDN)     |
 * | DVDD       | Digital VDD     | +5V         | +3.3V         |
 * | DGND       | Digital Ground  | GND         | GND           |
 * | AVDD       | Analog VDD      | +5V         | +3.3V         |
 * | AGND       | Analog Ground   | GND         | GND           |
 * | AIN0       | Positive Input  | Signal +    | Signal +      |
 * | AIN1       | Negative Input  | Signal -    | Signal -      |
 * 
 * Pin assignments can be modified by changing the #define values above.
 * 
 * License: MIT License
 */

#include <ads1262.h>

// ADS1262 control pins - modify these to match your hardware connections
#define ADS1262_CS_PIN    7   // Chip Select
#define ADS1262_DRDY_PIN  6   // Data Ready
#define ADS1262_START_PIN 5   // Start Conversion
#define ADS1262_PWDN_PIN  4   // Power Down

// ESP32 SPI pins (VSPI)
#if defined(ARDUINO_ARCH_ESP32)
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23
#endif

// Create ADS1262 instance with defined pins
ADS1262 adc(ADS1262_CS_PIN, ADS1262_DRDY_PIN, ADS1262_START_PIN, ADS1262_PWDN_PIN);

void setup() {
    delay(2000); // Wait for system stabilization
    Serial.begin(115200);
    delay(2000); // Wait for USB enumeration
    Serial.println("ADS1262 01-Basic-Usage");
    Serial.println("======================");
    
#if defined(ARDUINO_ARCH_ESP32)
    // Initialize with custom SPI pins for ESP32
    if (!adc.begin(SCK_PIN, MISO_PIN, MOSI_PIN)) {
        Serial.println("Failed to initialize ADS1262!");
        Serial.println("Check connections and power supply.");
        while (1) {
            delay(1000);
        }
    }
#else
    // Initialize the ADS1262 with default configuration
    if (!adc.begin()) {
        Serial.println("Failed to initialize ADS1262!");
        Serial.println("Check connections and power supply.");
        while (1) {
            delay(1000);
        }
    }
#endif
    
    Serial.println("ADS1262 initialized successfully!");
    
    // Print device information
    Serial.print("Library Version: ");
    Serial.println(adc.getLibraryVersion());
    Serial.print("Device ID: 0x");
    Serial.println(adc.readDeviceID(), HEX);
    
    // Configure ADC settings
    adc.setDataRate(ADS1262_DR_100_SPS);        // 100 samples per second
    adc.setGain(ADS1262_GAIN_1);                // Gain = 1
    adc.setReference(ADS1262_REF_INTERNAL_2_5V); // Use internal 2.5V reference
    adc.setInputMux(ADS1262_AIN0, ADS1262_AIN1); // Differential: AIN0 - AIN1
    
    Serial.println("Configuration complete. Starting measurements...");
    Serial.println("Time(ms)\tRaw ADC\t\tVoltage(V)\tVoltage(mV)");
    Serial.println("-------\t\t-------\t\t----------\t-----------");
}

void loop() {
    static uint32_t lastReading = 0;
    uint32_t currentTime = millis();
    
    // Take a reading every second
    if (currentTime - lastReading >= 1000) {
        lastReading = currentTime;
        
        // Read raw ADC counts
        int32_t rawCounts = adc.readRaw();
        
        // Convert to voltage
        float voltage = adc.readVoltage();
        
        // Display results
        Serial.print(currentTime);
        Serial.print("\t\t");
        Serial.print(rawCounts);
        Serial.print("\t\t");
        Serial.print(voltage, 6);
        Serial.print("\t\t");
        Serial.println(voltage * 1000.0, 3);
    }
    
    // Example of non-blocking read (always available)
    float asyncVoltage;
    if (adc.readVoltageAsync(asyncVoltage)) {
        // Data was available immediately
        // You can process it here if needed for real-time applications
        // Uncomment the line below to see async readings
        // Serial.println("Async reading available: " + String(asyncVoltage, 6) + " V");
    }
}
