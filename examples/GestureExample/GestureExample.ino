/***************************************************
  This is Consentium's TinyML library
  ----> https://docs.consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This Consentium's TinyML library works only for ESP32/Raspberry Pi Pico W compatible Edge boards. 
  
  Written by Debjyoti Chowdhury for Consentium.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// Uncomment library definition, according to your board version

#include <MPU6050_light.h>  // MPU6050 library
#include <Wire.h>            // I2C library for ESP32
#include <EdgeNeuron.h>      // TensorFlow Lite wrapper for Arduino
#include "model.h"           // Trained model

MPU6050 mpu(Wire);           // Create MPU6050 instance

const float accelerationThreshold = 2.5;  // Threshold (in G values) to detect a "gesture" start
const int numSamples = 119;                // Number of samples for a single gesture
int samplesRead;                           // Sample counter
const int inputLength = 714;               // Input tensor size (6 values * 119 samples)

// Tensor Arena memory area for TensorFlow Lite to store tensors
constexpr int tensorArenaSize = 8 * 1024;
alignas(16) byte tensorArena[tensorArenaSize];

// Gesture labels table
const char* GESTURES[] = {
  "punch",
  "flex"
};
#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Start I2C communication

  // Initialize MPU6050 sensor
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("MPU6050 initialization failed!");
    while (true);  // Stop execution on failure
  }

  Serial.println("MPU6050 initialized.");
  
  // Set accelerometer and gyroscope sampling rates
  mpu.calcOffsets();  // Calibrate MPU6050
  
  Serial.println();
  Serial.println("Initializing TensorFlow Lite model...");
  if (!initializeModel(model, tensorArena, tensorArenaSize)) {
    Serial.println("Model initialization failed!");
    while (true);  // Stop execution on failure
  }
  Serial.println("Model initialization done.");
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // Wait for significant movement (exceeding the threshold)
  while (true) {
    mpu.update();  // Update sensor readings
    
    // Read accelerometer values
    aX = mpu.getAccX();
    aY = mpu.getAccY();
    aZ = mpu.getAccZ();

    // Compute the total acceleration magnitude
    float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

    // Check if the acceleration exceeds the threshold (gesture detected)
    if (aSum >= accelerationThreshold) {
      samplesRead = 0;  // Reset sample counter
      break;            // Exit waiting loop
    }
  }

  // Collect data for gesture
  while (samplesRead < numSamples) {
    mpu.update();  // Update sensor readings

    // Read accelerometer and gyroscope values
    aX = mpu.getAccX();
    aY = mpu.getAccY();
    aZ = mpu.getAccZ();
    gX = mpu.getGyroX();
    gY = mpu.getGyroY();
    gZ = mpu.getGyroZ();

    // Normalize sensor data (since the model was trained on normalized values)
    aX = (aX + 4.0) / 8.0;
    aY = (aY + 4.0) / 8.0;
    aZ = (aZ + 4.0) / 8.0;
    gX = (gX + 2000.0) / 4000.0;
    gY = (gY + 2000.0) / 4000.0;
    gZ = (gZ + 2000.0) / 4000.0;

    // Place the 6 values (acceleration and gyroscope) into the model's input tensor
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
        Serial.println("Inference failed!");
        return;
      }

      // Retrieve output values and print them
      for (int i = 0; i < NUM_GESTURES; i++) {
        Serial.print(GESTURES[i]);
        Serial.print(": ");
        Serial.print(getModelOutput(i) * 100, 2);
        Serial.println("%");
      }
      Serial.println();
    }
  }
}

