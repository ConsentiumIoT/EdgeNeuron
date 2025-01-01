/***************************************************
  Consentium IoT - TinyML Data Acquisition Example
  -------------------------------------------------
  This example demonstrates the use of Consentium's TinyML library 
  for logging sensor data and preparing it for real-time inference 
  or processing.

  Features:
  - Compatible with ESP32 and Raspberry Pi Pico W edge boards
  - Data acquisition from sensor (e.g., IMU) for logging purposes
  - Logging data values to serial for monitoring and debugging
  - Modular structure for easy integration of further processing or ML models

  Tutorials and Documentation:
  Visit us at: https://docs.consentiumiot.com

  For Support:
  Email: official@consentiumiot.com

  MIT license - Redistribution must include this header.
 ****************************************************/


#include <EdgeStream.h> 

void setup() {
  // Initialize Serial communication at 9600 baud rate
  Serial.begin(9600);
  while (!Serial)
    continue;  // Wait for the Serial port to be ready
}

void loop() {
  // Define the sensor name and sample sensor values (can be replaced with actual sensor data)
  const char* sensorName = "IMU";
  vector<double> sensorValues = {1, 2, 3};

  // Log the sensor data to the Serial Monitor
  logData(sensorName, sensorValues);

  // Delay of 1 second before the next loop iteration
  delay(1000);
}