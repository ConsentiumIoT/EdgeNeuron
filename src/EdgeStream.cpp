#include "EdgeStream.h"

EdgeStream::EdgeStream() {} // EdgeStream default constructor

void EdgeStream::begin(int baud_rate){
  // Initialize Serial communication at 9600 baud rate
  Serial.begin(baud_rate);
  while (!Serial)
    continue;  // Wait for the Serial port to be ready
}

void EdgeStream::logData(const char* sensor_name, vector<double> sensor_data){
  int sensor_num = sensor_data.size();
  JsonDocument doc;

  // Add values in the document
  doc["sensorName"] = sensor_name;

  // Add an array
  JsonArray data = doc["sensorValues"].to<JsonArray>();

  for(int i = 0; i<sensor_num; i++){
    data.add(sensor_data[i]);
  }
  
  // Generate the minified JSON and send it to the Serial port
  serializeJson(doc, Serial);

  Serial.println();
}