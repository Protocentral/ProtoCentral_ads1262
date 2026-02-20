/*
 * 03-Simple-Differential-Extended for the ADS1262 32-bit ADC Library
 * 
 * SPDX-License-Identifier: MIT
 * 
 * Author: Ashwin Whitchurch, Protocentral Electronics
 * Contact: support@protocentral.com
 * Copyright (c) 2020-2025 ProtoCentral Electronics
 * 
 * This example demonstrates extended differential measurements with
 * multiple channel cycling and statistical analysis.
 * 
 * Features demonstrated:
 * - Multiple differential channel measurements
 * - Channel cycling between different input pairs
 * - Statistical analysis (min, max, average)
 * - Data logging format
 * - Real-time switching between channels
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
 * | AIN0       | Channel 1 +     | Signal 1+   | Signal 1+     |
 * | AIN1       | Channel 1 -     | Signal 1-   | Signal 1-     |
 * | AIN2       | Channel 2 +     | Signal 2+   | Signal 2+     |
 * | AIN3       | Channel 2 -     | Signal 2-   | Signal 2-     |
 * | AIN4       | Channel 3 +     | Signal 3+   | Signal 3+     |
 * | AINCOM     | Common Ref      | Common      | Common        |
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

// Channel configuration structure
struct Channel {
    ADS1262_InputChannel pos;
    ADS1262_InputChannel neg;
    const char* name;
    float sum;
    float min_val;
    float max_val;
    uint16_t count;
};

// Define measurement channels
Channel channels[] = {
    {ADS1262_AIN0, ADS1262_AIN1, "CH1(0-1)", 0.0, 999.0, -999.0, 0},
    {ADS1262_AIN2, ADS1262_AIN3, "CH2(2-3)", 0.0, 999.0, -999.0, 0},
    {ADS1262_AIN4, ADS1262_AINCOM, "CH3(4-COM)", 0.0, 999.0, -999.0, 0}
};

const uint8_t NUM_CHANNELS = sizeof(channels) / sizeof(channels[0]);
uint8_t currentChannel = 0;

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for USB enumeration
    Serial.println("ADS1262 03-Simple-Differential-Extended");
    Serial.println("=======================================");
    
    // Initialize ADS1262
    if (!adc.begin()) {
        Serial.println("Failed to initialize ADS1262!");
        while (1) delay(1000);
    }
    
    // Configure for extended measurements
    adc.setDataRate(ADS1262_DR_50_SPS);     // 50 samples per second
    adc.setGain(ADS1262_GAIN_1);            // Gain = 1
    adc.setReference(ADS1262_REF_INTERNAL_2_5V);
    
    Serial.println("Cycling through multiple differential channels...");
    Serial.println("Channel\t\tVoltage(V)\tMin(V)\t\tMax(V)\t\tAvg(V)");
    Serial.println("-------\t\t----------\t------\t\t------\t\t------");
}

void loop() {
    // Switch to current channel
    adc.setInputMux(channels[currentChannel].pos, channels[currentChannel].neg);
    
    // Allow settling time
    delay(100);
    
    // Read voltage
    float voltage = adc.readVoltage();
    
    // Update statistics
    Channel& ch = channels[currentChannel];
    ch.sum += voltage;
    ch.count++;
    if (voltage < ch.min_val) ch.min_val = voltage;
    if (voltage > ch.max_val) ch.max_val = voltage;
    float average = ch.sum / ch.count;
    
    // Display results
    Serial.print(ch.name);
    Serial.print("\t\t");
    Serial.print(voltage, 6);
    Serial.print("\t");
    Serial.print(ch.min_val, 6);
    Serial.print("\t");
    Serial.print(ch.max_val, 6);
    Serial.print("\t");
    Serial.println(average, 6);
    
    // Move to next channel
    currentChannel = (currentChannel + 1) % NUM_CHANNELS;
    
    // Add separator line every cycle
    if (currentChannel == 0) {
        Serial.println("-------\t\t----------\t------\t\t------\t\t------");
        delay(500); // Pause between cycles
    }
    
    delay(200);
}
