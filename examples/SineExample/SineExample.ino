/***************************************************
  This is Consentium's TinyML library
  ----> https://consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This Consentium's TinyML library works only for ESP32/Raspberry Pi Pico W compatible Edge boards. 
  
  Written by Debjyoti Chowdhury for Consentium.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <EdgeNeuron.h>
#include "model.h"

// Tensor arena for TensorFlow Lite to store tensors
constexpr int kTensorArenaSize = 2000;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

// Define the range for x (between 0 and 2π)
float x = 0.0;
float step = 0.1; // Step size for x increments

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Continuous sine function inference example.");
  Serial.println("Initializing TensorFlow Lite Micro Interpreter...");

  // Initialize the model
  if (!initializeModel(model, tensor_arena, kTensorArenaSize)) {
    Serial.println("Model initialization failed!");
    while (true);  // Halt execution on initialization failure
  }

  Serial.println("Model initialization done.");
  Serial.println("Running continuous inference on sine function.");
}

void loop() {
  // Ensure x stays within the [0, 2π] range (reset after 2π)
  if (x > 6.28) {
    x = 0.0;
  }

  // Set input value in the model's input tensor
  setModelInput(x, 0);

  // Run the inference
  if (!runModelInference()) {
    Serial.println("Inference Failed!");
    return;
  }

  // Get the predicted output
  float y_predicted = getModelOutput(0);

  // Get the actual sine of x
  float y_actual = sin(x);

  // Print both the predicted and actual sine values
  Serial.print("Input x: ");
  Serial.print(x, 2);
  Serial.print(" | Predicted sin(x): ");
  Serial.print(y_predicted, 2);
  Serial.print(" | Actual sin(x): ");
  Serial.println(y_actual, 2);

  // Increment x by the defined step size for the next loop iteration
  x += step;

  // Add a small delay to make the output readable in the Serial Monitor
  delay(500);  // Adjust delay as needed for your use case
}
