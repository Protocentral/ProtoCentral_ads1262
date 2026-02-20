/*
 * 02-Simple-Differential for the ADS1262 32-bit ADC Library
 * 
 * SPDX-License-Identifier: MIT
 * 
 * Author: Ashwin Whitchurch, Protocentral Electronics
 * Contact: support@protocentral.com
 * Copyright (c) 2020-2025 ProtoCentral Electronics
 * 
 * This example demonstrates simple differential voltage measurement
 * between any two analog input pins with minimal configuration.
 * 
 * Features demonstrated:
 * - Simple differential measurement setup
 * - Multiple input channel selection
 * - Basic voltage display
 * - Minimal code for quick prototyping
 * 
 * Hardware Connections:
 * |ADS1262 Pin | Function        | Arduino Pin | ESP32         |
 * |------------|:---------------:|:-----------:|:-------------:|
 * | DRDY       | Data Ready      | D6          | D6            |
 * | MISO       | SPI Data Out    | D12         | D19           |
 * | MOSI       | SPI Data In     | D11         | D23           |
 * | SCLK       | SPI Clock       | D13         | D18           |
 * | CS         | Chip Select     | D7          | D7            |
 * | START      | Start Convert   | D5          | D5            |
 * | PWDN       | Power Down      | D4          | D4            |
 * | AIN0       | Positive Input  | Signal +    | Signal +      |
 * | AIN2       | Negative Input  | Signal -    | Signal -      |
 * 
 * License: MIT License
 */

#include <ads1262.h>

// ADS1262 control pins - modify these to match your hardware connections
#define ADS1262_CS_PIN    7   // Chip Select
#define ADS1262_DRDY_PIN  6   // Data Ready
#define ADS1262_START_PIN 5   // Start Conversion
#define ADS1262_PWDN_PIN  4   // Power Down

// Create ADS1262 instance with defined pins
ADS1262 adc(ADS1262_CS_PIN, ADS1262_DRDY_PIN, ADS1262_START_PIN, ADS1262_PWDN_PIN);

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for USB enumeration
    Serial.println("ADS1262 02-Simple-Differential");
    Serial.println("==============================");
    
    // Initialize ADS1262
    if (!adc.begin()) {
        Serial.println("Failed to initialize ADS1262!");
        while (1) delay(1000);
    }
    
    // Simple configuration
    adc.setDataRate(ADS1262_DR_20_SPS);     // 20 samples per second
    adc.setGain(ADS1262_GAIN_1);            // Gain = 1
    adc.setInputMux(ADS1262_AIN0, ADS1262_AIN2); // AIN0 - AIN2
    
    Serial.println("Measuring AIN0 - AIN2...");
    Serial.println("Voltage(V)");
    Serial.println("----------");
}

void loop() {
    float voltage = adc.readVoltage();
    Serial.println(voltage, 6);
    delay(1000);
}
