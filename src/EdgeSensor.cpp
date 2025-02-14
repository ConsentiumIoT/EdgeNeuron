#include "EdgeSensor.h"

EdgeSensor::EdgeSensor(i2s_port_t i2s_port) {
    this->i2s_port = i2s_port;  
    this->sBuffer = nullptr;  // Initialize to null before allocation
}

EdgeSensor::~EdgeSensor() {
    if (this->sBuffer) {
        delete[] this->sBuffer;
    }
}

void EdgeSensor::setPins(int sck_pin, int ws_pin, int sd_pin) {
    const i2s_pin_config_t pin_config = {
        .bck_io_num = sck_pin,
        .ws_io_num = ws_pin,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = sd_pin
    };

    i2s_set_pin(this->i2s_port, &pin_config);
}

void EdgeSensor::beginMic(int buff_len) {
    this->bufferLen = buff_len;
    this->sBuffer = new int16_t[buff_len];  // Allocate buffer dynamically

    const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = this->bufferLen,
        .use_apll = true,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    i2s_driver_install(this->i2s_port, &i2s_config, 0, NULL);
}

float EdgeSensor::readMeanAudio() {
    if (!this->sBuffer) {
        return 0.0f;  // Safety check in case buffer is not initialized
    }

    size_t bytesIn = 0;
    esp_err_t result = i2s_read(this->i2s_port, this->sBuffer, this->bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);

    if (result != ESP_OK || bytesIn == 0) {
        return 0.0f;  // Return zero if no data is read
    }

    int16_t samples_read = bytesIn / sizeof(int16_t);
    float mean = 0.0f;

    for (int i = 0; i < samples_read; i++) {
        mean += this->sBuffer[i];
    }

    return mean / samples_read;
}
