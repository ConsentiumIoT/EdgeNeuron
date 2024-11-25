/***************************************************
  Consentium IoT - TinyML Sine Function Example
  -------------------------------------------------
  This example demonstrates the use of Consentium's TinyML library 
  for continuous inference of the sine function using TensorFlow Lite Micro.

  Features:
  - Compatible with ESP32 and Raspberry Pi Pico W edge boards
  - Real-time inference with TensorFlow Lite Micro
  - Compares model predictions with actual sine function values

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/

#include <EdgeNeuron.h>  // TensorFlow Lite wrapper for Arduino
#include "model.h"       // Pre-trained TinyML model

// Tensor arena for TensorFlow Lite to store tensors
constexpr int kTensorArenaSize = 2000;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

// Define the range and step size for x (0 to 2π)
float x = 0.0;     // Input value for the sine function
float step = 0.1;  // Step size for incrementing x

void setup() {
  // Start Serial communication
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial to initialize

  // Branding and initialization messages
  Serial.println("Consentium IoT - TinyML Sine Function Example");
  Serial.println("---------------------------------------------");
  Serial.println("Initializing TensorFlow Lite Micro Interpreter...");

  // Initialize TensorFlow Lite Micro model
  if (!initializeModel(model, tensor_arena, kTensorArenaSize)) {
    Serial.println("[Consentium IoT] Model initialization failed!");
    while (true);  // Halt execution on initialization failure
  }

  Serial.println("[Consentium IoT] Model initialization successful.");
  Serial.println("Starting continuous inference on sine function.");
}

void loop() {
  // Ensure x stays within the [0, 2π] range by resetting after 2π
  if (x > 6.28) {
    x = 0.0;
  }

  // Set input value in the model's input tensor
  setModelInput(x, 0);

  // Run the inference
  if (!runModelInference()) {
    Serial.println("[Consentium IoT] Inference failed!");
    return;
  }

  // Retrieve the predicted output from the model
  float y_predicted = getModelOutput(0);

  // Calculate the actual sine value of x
  float y_actual = sin(x);

  // Print the input, predicted, and actual sine values
  Serial.print("Input x: ");
  Serial.print(x, 2);
  Serial.print(" | Predicted sin(x): ");
  Serial.print(y_predicted, 2);
  Serial.print(" | Actual sin(x): ");
  Serial.println(y_actual, 2);

  // Increment x by the step size for the next loop iteration
  x += step;

  // Add a delay for better readability in the Serial Monitor
  delay(500);  // Adjust as needed
}
