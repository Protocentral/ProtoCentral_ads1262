////////////////////////////////////////////////////////////////////////////////////////////
//    04-Advanced-Usage for the ADS1262 32-bit ADC Library
//    
//    SPDX-License-Identifier: MIT
//    
//    Author: Ashwin Whitchurch, Protocentral Electronics
//    Contact: support@protocentral.com
//    Copyright (c) 2020-2025 ProtoCentral Electronics
//
//    This example demonstrates advanced features of the ADS1262 library including:
//    - Custom configuration
//    - Multiple input channels
//    - Continuous mode
//    - Error handling
//    - Calibration
//    
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
////////////////////////////////////////////////////////////////////////////////////////////

#include <ads1262.h>

// ADS1262 control pins - modify these to match your hardware connections
#define ADS1262_CS_PIN    7   // Chip Select
#define ADS1262_DRDY_PIN  6   // Data Ready
#define ADS1262_START_PIN 5   // Start Conversion
#define ADS1262_PWDN_PIN  4   // Power Down

// Create ADS1262 instance with defined pins
ADS1262 adc(ADS1262_CS_PIN, ADS1262_DRDY_PIN, ADS1262_START_PIN, ADS1262_PWDN_PIN);

// Measurement channels
struct Channel {
    ADS1262_InputChannel positive;
    ADS1262_InputChannel negative;
    const char* name;
};

Channel channels[] = {
    {ADS1262_AIN0, ADS1262_AIN1, "CH0-CH1"},
    {ADS1262_AIN2, ADS1262_AIN3, "CH2-CH3"},
    {ADS1262_AIN4, ADS1262_AINCOM, "CH4-COM"},
    {ADS1262_TEMP_SENSOR_P, ADS1262_TEMP_SENSOR_N, "Temperature"}
};

const int numChannels = sizeof(channels) / sizeof(channels[0]);
int currentChannel = 0;

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for USB enumeration
    Serial.println("ADS1262 Example-3-Advanced-Usage");
    Serial.println("================================");
    
    // Create custom configuration
    ADS1262_Config config;
    config.dataRate = ADS1262_DR_400_SPS;         // High speed sampling
    config.gain = ADS1262_GAIN_4;                 // 4x gain
    config.reference = ADS1262_REF_INTERNAL_2_5V; // Internal reference
    config.continuousMode = false;                 // Single-shot mode
    config.bufferEnabled = true;                   // Enable input buffer
    config.filterType = 0;                         // Default filter
    
    // Initialize with custom configuration
    if (!adc.begin(config)) {
        Serial.println("Failed to initialize ADS1262!");
        handleError();
    }
    
    Serial.println("ADS1262 initialized successfully!");
    printDeviceInfo();
    
    // Perform device test
    if (!adc.testConnection()) {
        Serial.println("Device connection test failed!");
        handleError();
    }
    
    Serial.println("Device connection test passed!");
    
    // Optional: Perform calibration
    Serial.println("Performing self-calibration...");
    adc.performSelfCalibration();
    Serial.println("Calibration complete!");
    
    Serial.println("\nStarting multi-channel measurements...");
    Serial.println("Channel\t\tRaw ADC\t\tVoltage(V)\tVoltage(mV)");
    Serial.println("-------\t\t-------\t\t----------\t-----------");
}

void loop() {
    static uint32_t lastReading = 0;
    uint32_t currentTime = millis();
    
    // Switch channels every 2 seconds
    if (currentTime - lastReading >= 2000) {
        lastReading = currentTime;
        
        // Set input multiplexer for current channel
        adc.setInputMux(channels[currentChannel].positive, 
                       channels[currentChannel].negative);
        
        // Wait for settling time
        delay(100);
        
        // Take measurement
        int32_t rawCounts = adc.readRaw();
        float voltage = adc.countsToVoltage(rawCounts);
        
        // Display results
        Serial.print(channels[currentChannel].name);
        Serial.print("\t\t");
        Serial.print(rawCounts);
        Serial.print("\t\t");
        Serial.print(voltage, 6);
        Serial.print("\t\t");
        Serial.println(voltage * 1000.0, 3);
        
        // Move to next channel
        currentChannel = (currentChannel + 1) % numChannels;
        
        // Special handling for temperature sensor
        if (currentChannel == 3) {  // Temperature channel
            float tempC = convertTemperature(rawCounts);
            Serial.print("\t\t\t\t\t\t\tTemperature: ");
            Serial.print(tempC, 2);
            Serial.println(" °C");
        }
    }
    
    // Demonstrate continuous mode for fast sampling
    static bool continuousModeDemo = false;
    static uint32_t continuousStart = 0;
    
    if (currentTime > 20000 && !continuousModeDemo) {
        Serial.println("\n--- Demonstrating Continuous Mode ---");
        continuousModeDemo = true;
        continuousStart = currentTime;
        
        // Set to single channel for continuous mode
        adc.setInputMux(ADS1262_AIN0, ADS1262_AIN1);
        adc.setContinuousMode(true);
        adc.startConversion();
        
        Serial.println("Continuous mode active - sampling at high speed...");
    }
    
    if (continuousModeDemo && (currentTime - continuousStart < 5000)) {
        // Read data in continuous mode
        int32_t result;
        if (adc.readRawAsync(result)) {
            float voltage = adc.countsToVoltage(result);
            Serial.print("Fast: ");
            Serial.print(voltage, 6);
            Serial.println(" V");
        }
    } else if (continuousModeDemo && (currentTime - continuousStart >= 5000)) {
        // Stop continuous mode
        adc.setContinuousMode(false);
        adc.stopConversion();
        continuousModeDemo = false;
        Serial.println("Continuous mode demonstration complete.");
        Serial.println("Returning to channel scanning...\n");
    }
}

void printDeviceInfo() {
    Serial.print("Library Version: ");
    Serial.println(adc.getLibraryVersion());
    
    Serial.print("Device ID: 0x");
    Serial.println(adc.readDeviceID(), HEX);
    
    // Read some configuration registers
    Serial.print("Power Register: 0x");
    Serial.println(adc.readRegister(ADS1262_REG_POWER), HEX);
    
    Serial.print("Mode2 Register: 0x");
    Serial.println(adc.readRegister(ADS1262_REG_MODE2), HEX);
    
    Serial.print("Reference Mux: 0x");
    Serial.println(adc.readRegister(ADS1262_REG_REFMUX), HEX);
    
    Serial.println();
}

void handleError() {
    Serial.println("System halted due to error.");
    Serial.println("Please check:");
    Serial.println("1. Wiring connections");
    Serial.println("2. Power supply (3.3V or 5V)");
    Serial.println("3. SPI configuration");
    
    while (1) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }
}

float convertTemperature(int32_t rawCounts) {
    // ADS1262 temperature sensor conversion
    // This is a simplified conversion - refer to datasheet for exact formula
    float voltage = adc.countsToVoltage(rawCounts);
    
    // Temperature coefficient is approximately 405 µV/°C
    // With 0°C intercept at approximately 122.4 mV
    float tempC = (voltage - 0.1224) / 0.000405;
    
    return tempC;
}

// Utility function to change gain on the fly
void demonstrateGainControl() {
    Serial.println("\n--- Gain Control Demonstration ---");
    
    ADS1262_Gain gains[] = {ADS1262_GAIN_1, ADS1262_GAIN_2, ADS1262_GAIN_4, 
                           ADS1262_GAIN_8, ADS1262_GAIN_16, ADS1262_GAIN_32};
    const char* gainNames[] = {"1x", "2x", "4x", "8x", "16x", "32x"};
    
    adc.setInputMux(ADS1262_AIN0, ADS1262_AIN1);
    
    for (int i = 0; i < 6; i++) {
        adc.setGain(gains[i]);
        delay(100); // Allow settling time
        
        int32_t raw = adc.readRaw();
        float voltage = adc.readVoltage();
        
        Serial.print("Gain ");
        Serial.print(gainNames[i]);
        Serial.print(": Raw=");
        Serial.print(raw);
        Serial.print(", Voltage=");
        Serial.print(voltage, 6);
        Serial.println(" V");
        
        delay(1000);
    }
    
    // Reset to original gain
    adc.setGain(ADS1262_GAIN_4);
    Serial.println("Gain demonstration complete.\n");
}
