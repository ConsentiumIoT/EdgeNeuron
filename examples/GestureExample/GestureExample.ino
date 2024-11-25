/***************************************************
  Consentium IoT - TinyML Gesture Recognition
  -------------------------------------------------
  This library demonstrates how to use the Consentium TinyML library
  with TensorFlow Lite and MPU6050 to classify gestures based on 
  accelerometer and gyroscope data.

  Features:
  - Compatible with ESP32 and Raspberry Pi Pico W edge boards
  - Gesture detection using trained machine learning models
  - Real-time inference on-device using TensorFlow Lite Micro

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

// Include necessary libraries
#include <MPU6050_light.h>  // MPU6050 library
#include <Wire.h>           // I2C library
#include <EdgeNeuron.h>     // TensorFlow Lite wrapper for Arduino
#include "model.h"          // Trained TinyML model

// Create MPU6050 instance
MPU6050 mpu(Wire);

// Define constants for gesture detection
const float accelerationThreshold = 2.5;  // Threshold (in G values) to detect a gesture start
const int numSamples = 119;               // Number of samples for a single gesture
const int inputLength = 714;              // Input tensor size (6 values * 119 samples)

// Tensor Arena for TensorFlow Lite
constexpr int tensorArenaSize = 8 * 1024; // Memory size for tensor operations
alignas(16) byte tensorArena[tensorArenaSize]; // Allocate memory aligned to 16 bytes

// Gesture labels
const char* GESTURES[] = {
  "punch",
  "flex"
};
#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  // Start Serial communication
  Serial.begin(9600);
  
  // Initialize I2C communication
  Wire.begin();

  // Initialize the MPU6050 sensor
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("[Consentium IoT] MPU6050 initialization failed!");
    while (true);  // Stop execution if sensor initialization fails
  }

  Serial.println("[Consentium IoT] MPU6050 initialized.");
  
  // Calibrate MPU6050
  mpu.calcOffsets();
  Serial.println("[Consentium IoT] MPU6050 calibrated.");

  // Initialize the TensorFlow Lite model
  Serial.println("[Consentium IoT] Initializing TensorFlow Lite model...");
  if (!initializeModel(model, tensorArena, tensorArenaSize)) {
    Serial.println("[Consentium IoT] Model initialization failed!");
    while (true);  // Stop execution if model initialization fails
  }
  Serial.println("[Consentium IoT] Model initialization complete.");
  Serial.println();
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // Wait for significant movement (exceeding the threshold)
  while (true) {
    mpu.update();

    // Read accelerometer values
    aX = mpu.getAccX();
    aY = mpu.getAccY();
    aZ = mpu.getAccZ();

    // Calculate total acceleration magnitude
    float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

    // Check if the acceleration exceeds the threshold
    if (aSum >= accelerationThreshold) {
      Serial.println("[Consentium IoT] Gesture detected. Collecting data...");
      break;
    }
  }

  int samplesRead = 0; // Reset sample counter

  // Collect data for the gesture
  while (samplesRead < numSamples) {
    mpu.update();

    // Read accelerometer and gyroscope values
    aX = mpu.getAccX();
    aY = mpu.getAccY();
    aZ = mpu.getAccZ();
    gX = mpu.getGyroX();
    gY = mpu.getGyroY();
    gZ = mpu.getGyroZ();

    // Normalize the sensor data
    aX = (aX + 4.0) / 8.0;
    aY = (aY + 4.0) / 8.0;
    aZ = (aZ + 4.0) / 8.0;
    gX = (gX + 2000.0) / 4000.0;
    gY = (gY + 2000.0) / 4000.0;
    gZ = (gZ + 2000.0) / 4000.0;

    // Place normalized values into the model's input tensor
    setModelInput(aX, samplesRead * 6 + 0);
    setModelInput(aY, samplesRead * 6 + 1);
    setModelInput(aZ, samplesRead * 6 + 2);
    setModelInput(gX, samplesRead * 6 + 3);
    setModelInput(gY, samplesRead * 6 + 4);
    setModelInput(gZ, samplesRead * 6 + 5);

    samplesRead++;

    // Once all samples are collected, run the inference
    if (samplesRead == numSamples) {
      if (!runModelInference()) {
        Serial.println("[Consentium IoT] Inference failed!");
        return;
      }

      // Print inference results
      Serial.println("[Consentium IoT] Inference Results:");
      for (int i = 0; i < NUM_GESTURES; i++) {
        Serial.print("  ");
        Serial.print(GESTURES[i]);
        Serial.print(": ");
        Serial.print(getModelOutput(i) * 100, 2);
        Serial.println("%");
      }
      Serial.println();
    }
  }
}
