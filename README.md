# EdgeNeuron
## TensorFlow Lite Micro Library in Arduino Style

**EdgeNeuron** is an Arduino-friendly wrapper around **TensorFlow Lite Micro**, making it easier to run Tiny Machine Learning (TinyML) models on Arduino boards. It simplifies the process by providing an intuitive, _Arduino-style_ API and avoiding complex C++ constructs like pointers, making it more accessible to those familiar with standard Arduino development.

## Features

- Simplified APIs for running TensorFlow Lite Micro models on Arduino.
- Avoids complex C++ constructs (like raw pointers) to ensure easy use in Arduino sketches.
- Built for TinyML experimentation on powerful Arduino boards.

## Installation

You can install **EdgeNeuron** via the Arduino IDE Library Manager or by downloading a `.zip` file from this GitHub repository. 

## Usage Examples

The examples provided with **EdgeNeuron** demonstrate how to run pre-trained machine learning models on Arduino. Each example includes the required model files.

### General TinyML Development Process:

1. **Create an Arduino Sketch to Collect Data:** Use an Arduino board to collect the data needed for training your model.
2. **Define a Neural Network Model:** In an external TensorFlow development environment (e.g., Google Colaboratory), define and create your deep neural network (DNN) model.
3. **Train the Model:** Train the model on the collected dataset in the TensorFlow environment.
4. **Convert and Save the Model:** After training, convert the model to TensorFlow Lite format. Save the model as a `.h` file containing a static byte array of the model.
5. **Prepare a New Arduino Sketch for Inference:** Write a new Arduino sketch that uses the trained model for inference.
6. **Include Necessary Headers:** In the sketch, include `EdgeNeuron.h` and your model header file (`model.h`).
7. **Define the Tensor Arena:** Declare a byte array to serve as a memory buffer (tensor arena) for the model's operations.
8. **Initialize the Model:** Use the `modelInit()` function to load the model and allocate tensors.
9. **Set Input Data:** Insert input data into the model using the `modelSetInput()` function.
10. **Run Inference:** Call `modelRunInference()` to perform inference.
11. **Read Output Data:** Retrieve the output data using `modelGetOutput()`.

## Reference

### Functions

#### `bool modelInit(const unsigned char* model, byte* tensorArena, int tensorArenaSize)`

- Initializes the TensorFlow Lite Micro environment, sets up the model, resolves operations, and allocates memory for input and output tensors.
- **Returns:** `true` on success, `false` on failure.

#### `bool modelSetInput(float inputValue, int index)`

- Sets the value at a specific index in the input tensor.
- **Returns:** `true` on success, `false` if the index is out of range or an error occurs.

#### `bool modelRunInference()`

- Executes the inference operation by invoking the TensorFlow Lite Micro interpreter.
- **Returns:** `true` on success, `false` if the inference fails.

#### `float modelGetOutput(int index)`

- Retrieves the output value from the specified index in the output tensor.
- **Returns:** The output value, or `-1` if the index is out of range or an error occurs.

---

### Future Work

- Additional support for more advanced machine learning models and examples.
- Enhanced compatibility with other TensorFlow Lite features.
