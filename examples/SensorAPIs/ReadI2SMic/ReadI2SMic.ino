/***************************************************
  Consentium IoT - TinyML Audio Processing Example
  -------------------------------------------------
  This example demonstrates real-time audio analysis 
  using an I2S microphone (INMP441) on ESP32. It 
  computes the mean audio signal level for further 
  processing in TinyML applications via EdgeNeuron AI.

  Features:
  - Uses I2S protocol for high-fidelity audio sampling
  - Computes mean signal level for ML inference
  - Ready for integration with TinyML and Edge AI models

  Tutorials and Documentation:
  Visit: https://docs.consentiumiot.com

  MIT License - Redistribution must include this header.
****************************************************/

#include <EdgeSensor.h>

// I2S microphone pin connections
int ws_pin = 7;  // L/R clock (word select)
int sd_pin = 4;  // Data pin
int sck_pin = 6; // Bit clock
#define I2S_PORT I2S_NUM_0

// Initialize I2S microphone
EdgeSensor i2s_mic(I2S_PORT);

void setup() {
    Serial.begin(115200);

    // Initialize I2S microphone
    i2s_mic.beginMic();
    i2s_mic.setPins(sck_pin, ws_pin, sd_pin);

    Serial.println("Consentium IoT - Audio Processing Initialized");
}

void loop() {
    int rangelimit = 3000;
    Serial.print(-rangelimit);
    Serial.print(" ");
    Serial.print(rangelimit);
    Serial.print(" ");

    // Read mean audio level from I2S microphone
    float meanAudio = i2s_mic.readMeanAudio();
    Serial.println(meanAudio);
}
