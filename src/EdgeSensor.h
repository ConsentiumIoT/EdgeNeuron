#ifndef EDGESENSOR_H
#define EDGESENSOR_H

#include <Arduino.h>
#include <driver/i2s.h>

class EdgeSensor {
    public:
        EdgeSensor(i2s_port_t i2s_port);  // Constructor
        ~EdgeSensor();  // Destructor for memory cleanup

        void setPins(int sck_pin, int ws_pin, int sd_pin);
        void beginMic(int buff_len=64, int sampling_freq=44100);
        float readMeanAudio();  // No need to pass buffer explicitly

    private:
        i2s_port_t i2s_port;
        int bufferLen;
        int16_t* sBuffer;  // Dynamic buffer
};

#endif  // EDGESENSOR_H
