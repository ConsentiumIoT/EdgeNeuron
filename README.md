# **EdgeNeuron**
### **TensorFlow Lite Micro Library in Arduino Style**

**EdgeNeuron** is an Arduino-friendly wrapper around **TensorFlow Lite Micro**, simplifying the process of running Tiny Machine Learning (TinyML) models on Arduino boards. It provides a beginner-friendly, _Arduino-style_ API, eliminating the need for complex C++ constructs like raw pointers, making it accessible to developers familiar with standard Arduino programming.

---

## **Features**
- **Intuitive API:** Simplified functions for deploying TensorFlow Lite Micro models on Arduino-compatible boards.
- **No Raw Pointers:** Designed to avoid complex C++ constructs, enabling easy integration into Arduino sketches.
- **Optimized for TinyML:** Tailored for experimentation and development on boards with sufficient computational power.

---

## **Installation**

### **Via Arduino IDE:**
1. Open the Arduino IDE.
2. Navigate to **Sketch > Include Library > Manage Libraries**.
3. Search for `EdgeNeuron` and click **Install**.

### **Manual Installation:**
1. Download the latest `.zip` file from this GitHub repository.
2. In the Arduino IDE, navigate to **Sketch > Include Library > Add .ZIP Library**.
3. Select the downloaded `.zip` file to install the library.

---

## **Usage Examples**

The provided examples demonstrate how to:
- Deploy pre-trained models for inference.
- Read data from sensors, preprocess it, and input it into the model.
- Retrieve and process model outputs.

---

## **TinyML Development Workflow**

1. **Data Collection:**  
   Use an Arduino sketch to collect sensor data required for training your model.

2. **Model Creation:**  
   Define and develop a deep neural network (DNN) in a TensorFlow environment (e.g., Google Colaboratory).

3. **Model Training:**  
   Train the model using the collected dataset in TensorFlow.

4. **Model Conversion:**  
   Convert the trained model to TensorFlow Lite format and export it as a `.h` file containing a static byte array.

5. **Inference Sketch Preparation:**  
   Write an Arduino sketch to deploy the trained model for real-time inference.

6. **Include Required Files:**  
   Add `EdgeNeuron.h` and your model's header file (`model.h`) to your sketch.

7. **Tensor Arena Definition:**  
   Allocate memory for model operations by declaring a byte array (tensor arena).

8. **Model Initialization:**  
   Use the `modelInit()` function to load the model and prepare input/output tensors.

9. **Input Data Assignment:**  
   Populate the input tensor with data using `modelSetInput()`.

10. **Run Inference:**  
    Perform inference using `modelRunInference()`.

11. **Retrieve Outputs:**  
    Extract the results using `modelGetOutput()`.

---

## **API Reference**

### **Model Initialization**
#### `bool modelInit(const unsigned char* model, byte* tensorArena, int tensorArenaSize)`
Initializes the TensorFlow Lite Micro environment, sets up the model, resolves operations, and allocates memory for tensors.  
**Returns:**  
- `true` if successful.  
- `false` if an error occurs.

---

### **Input Data Assignment**
#### `bool modelSetInput(float inputValue, int index)`
Sets the value at a specific index in the input tensor.  
**Parameters:**  
- `inputValue`: The input value to be set.  
- `index`: The index within the input tensor.  
**Returns:**  
- `true` if successful.  
- `false` if the index is invalid or an error occurs.

---

### **Model Inference**
#### `bool modelRunInference()`
Executes the inference operation using TensorFlow Lite Micro interpreter.  
**Returns:**  
- `true` if successful.  
- `false` if inference fails.

---

### **Output Retrieval**
#### `float modelGetOutput(int index)`
Retrieves the output value at a specified index in the output tensor.  
**Parameters:**  
- `index`: The index within the output tensor.  
**Returns:**  
- The output value if successful.  
- `-1` if the index is invalid or an error occurs.

---

## **Future Enhancements**
- Support for advanced machine learning models (e.g., object detection).
- Extended compatibility with additional TensorFlow Lite features.
- Improved optimization for memory-constrained devices.

---

## **Contributing**

We welcome contributions to enhance EdgeNeuron!  
- Submit bug reports or feature requests via the [GitHub Issues page](https://github.com/consentiumiot/edgeneuron/issues).  
- Fork the repository, make changes, and submit a pull request.

---

## **License**

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for more details.

---

## **Support and Documentation**

Visit [Consentium IoT Documentation](https://docs.consentiumiot.com) for tutorials, examples, and additional resources.  
For further assistance, email us at [official@consentiumiot.com](mailto:official@consentiumiot.com).

---

### **Badges**
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
