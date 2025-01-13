/***************************************************
  Consentium IoT - TinyML Occupancy Classification Example
  ---------------------------------------------------------
  This example demonstrates the use of Consentium's TinyML 
  and EdgeNeuron AI platform for real-time occupancy classification 
  using the AMG8833 Grid-EYE thermal sensor and TensorFlow Lite Micro.

  Features:
  - Compatible with ESP32 and Raspberry Pi Pico W edge boards
  - Real-time inference with TensorFlow Lite Micro
  - Classifies occupancy into predefined categories:
      - Empty
      - One person
      - Two persons
      - Three persons

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

#include <Wire.h>
#include <EdgeNeuron.h>
#include <EdgeMath.h>
#include <SparkFun_GridEYE_Arduino_Library.h>
#include "edge_neuron_model.h"

// Sensor and computation objects
GridEYE grideye;
EdgeMath edgemath;

// Memory allocation for TensorFlow Lite model
constexpr int kTensorArenaSize = 10000;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

// Define tensor sizes
constexpr int kInputSize = 64;       // Input tensor size
constexpr int kOutputSize = 4;      // Output tensor size

// Arrays for data processing
float input_data[kInputSize];       // Input data array
float output_data[kOutputSize];     // Output data array

// Class labels
String class_array[] = {"Empty", "One_person", "Two_persons", "Three_persons"};

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("***************************************************");
  Serial.println("*    Consentium IoT - Occupancy Classification   *");
  Serial.println("*        Powered by EdgeNeuron AI Platform       *");
  Serial.println("*-------------------------------------------------*");
  Serial.println("*        Real-time inference with TinyML         *");
  Serial.println("***************************************************\n");

  // Initialize I2C communication
  Wire.begin();

  // Initialize the Grid-EYE sensor
  if (!grideye.begin()) {
    Serial.println("[Consentium ERROR] GridEYE sensor initialization failed!");
    while (true); // Halt execution
  }
  Serial.println("[Consentium INFO] GridEYE sensor initialized successfully!");

  // Initialize the EdgeNeuron model
  if (!initializeModel(edge_neuron_model, tensor_arena, kTensorArenaSize)) {
    Serial.println("[Consentium ERROR] Model initialization failed!");
    while (true); // Halt execution
  }
  Serial.println("[Consentium INFO] Model initialized successfully!");
}

void loop() {
  Serial.println("\n[Consentium INFO] Capturing temperature data from Grid-EYE...");
  
  // Capture temperature data from Grid-EYE
  for (int i = 0; i < kInputSize; i++) {
    input_data[i] = grideye.getPixelTemperature(i);
    if (isnan(input_data[i])) {
      input_data[i] = 0.0; // Replace invalid sensor data
    }
  }
  Serial.println("[Consentium INFO] Data capture complete.");

  // Preprocess the input data
  Serial.println("[Consentium INFO] Preprocessing input data...");
  edgemath.scaler_transform(input_data, scaler_mean, scaler_scale, kInputSize);
  Serial.println("[Consentium INFO] Data preprocessing complete.");

  // Set the model input tensor
  for (int i = 0; i < kInputSize; i++) {
    setModelInput(input_data[i], i);
  }

  // Perform inference
  Serial.println("[Consentium INFO] Running inference...");
  unsigned long start_time = millis();
  if (!runModelInference()) {
    Serial.println("[Consentium ERROR] Inference failed!");
    return;
  }
  unsigned long inference_time = millis() - start_time;
  Serial.print("[Consentium INFO] Inference complete. Time taken: ");
  Serial.print(inference_time);
  Serial.println(" ms");

  // Retrieve output data
  for (int i = 0; i < kOutputSize; i++) {
    output_data[i] = getModelOutput(i);
  }

  // Determine the predicted class
  int class_pos = edgemath.argmax(output_data, kOutputSize);
  Serial.println("[Consentium INFO] Predicted Class: " + class_array[class_pos]);

  // Log class probabilities
  Serial.println("[Consentium INFO] Class probabilities:");
  for (int i = 0; i < kOutputSize; i++) {
    Serial.print("  ");
    Serial.print(class_array[i]);
    Serial.print(": ");
    Serial.println(output_data[i]);
  }

  // Wait before the next inference cycle
  delay(1000);
}
