#!/bin/bash

# Compile all ADS1262 examples for Adafruit QT Py ESP32-C3
# Usage: ./compile_examples.sh

BOARD_FQBN="esp32:esp32:adafruit_qtpy_esp32c3"
EXAMPLES_DIR="/Users/akw/Documents/Arduino/libraries/protocentral-ads1262-arduino/examples"

echo "=============================================="
echo "Compiling ADS1262 Examples for QT Py ESP32-C3"
echo "=============================================="
echo ""

cd "$EXAMPLES_DIR"

# Array of example directories
examples=(
    "01-Basic-Usage"
    "02-Simple-Differential" 
    "03-Simple-Differential-Extended"
    "04-Advanced-Usage"
)

success_count=0
total_count=${#examples[@]}

for example in "${examples[@]}"; do
    echo "----------------------------------------------"
    echo "Compiling: $example"
    echo "----------------------------------------------"
    
    if [ -d "$example" ]; then
        if arduino-cli compile --fqbn "$BOARD_FQBN" "$example"; then
            echo "✅ SUCCESS: $example compiled successfully"
            ((success_count++))
        else
            echo "❌ FAILED: $example compilation failed"
        fi
    else
        echo "⚠️  SKIP: Directory $example not found"
    fi
    
    echo ""
done

echo "=============================================="
echo "Compilation Summary"
echo "=============================================="
echo "Board: Adafruit QT Py ESP32-C3"
echo "Successful: $success_count/$total_count examples"
echo "=============================================="