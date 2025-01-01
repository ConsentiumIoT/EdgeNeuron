#include "EdgeStream.h"

void logData(const char* sensor_name, vector<double> sensor_data){
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