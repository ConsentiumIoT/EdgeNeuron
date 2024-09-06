#include "EdgeNeuron.h"

tflite::AllOpsResolver tflOpsResolver;
const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Function to initialize the model
bool initializeModel(const unsigned char* model, byte* tensorArena, int tensorArenaSize) {
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema version mismatch!");
    return false;
  }

  // Create the interpreter
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize);

  // Allocate memory for input and output tensors
  TfLiteStatus allocateStatus = tflInterpreter->AllocateTensors();
  if (allocateStatus != kTfLiteOk) {
    Serial.println("Failed to allocate tensors!");
    return false;
  }

  // Get pointers to input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);

  if (!tflInputTensor || !tflOutputTensor) {
    Serial.println("Failed to get input/output tensors!");
    return false;
  }

  return true;
}

// Function to set input data
bool setModelInput(float inputValue, int index) {
  if (tflInputTensor == nullptr || index < 0 || index >= tflInputTensor->bytes / sizeof(float)) {
    Serial.println("Input tensor index out of range!");
    return false;
  }

  // Set input value
  tflInputTensor->data.f[index] = inputValue;
  return true;
}

// Function to run inference
bool runModelInference() {
  if (!tflInterpreter) {
    Serial.println("Interpreter not initialized!");
    return false;
  }

  // Invoke the interpreter
  TfLiteStatus invokeStatus = tflInterpreter->Invoke();
  if (invokeStatus != kTfLiteOk) {
    Serial.println("Failed to run inference!");
    return false;
  }

  return true;
}

// Function to get output data
float getModelOutput(int index) {
  if (tflOutputTensor == nullptr || index < 0 || index >= tflOutputTensor->bytes / sizeof(float)) {
    Serial.println("Output tensor index out of range!");
    return -1;
  }

  // Return the output value
  return tflOutputTensor->data.f[index];
}

// Optional function to free memory
void cleanupModel() {
  if (tflInterpreter) {
    delete tflInterpreter;
    tflInterpreter = nullptr;
  }
}
