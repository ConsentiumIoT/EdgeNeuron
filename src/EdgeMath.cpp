#include "EdgeMath.h"

EdgeMath::EdgeMath() {}

void EdgeMath::scaler_transform(float *data_array, const float* mean, const float* scale, int size){
  for(int i=0; i<size; i++){
    data_array[i] = ((data_array[i] - mean[i]) / scale[i]);
  }
}

int EdgeMath::argmax(float *data_array, int size) {
    float max = data_array[0];
    int max_index = 0;

    for (int i = 1; i < size; i++) {
        if (data_array[i] > max) {
            max = data_array[i];
            max_index = i;
        }
    }

    return max_index; // Return the index of the maximum value
}

float EdgeMath::quantize(float value, float min, float max, int bits = 8) {
    int q_min = 0;
    int q_max = (1 << bits) - 1;
    int scaled_value = round(((value - min) / (max - min)) * (q_max - q_min));
    return scaled_value / float(q_max - q_min) * (max - min) + min;
}


float EdgeMath::dequantize(int quantized_value, float min, float max, int bits = 8) {
    int q_min = 0;
    int q_max = (1 << bits) - 1;
    return quantized_value / float(q_max - q_min) * (max - min) + min;
}

void EdgeMath::hamming_window(float* window, int N) {
    for (int n = 0; n < N; ++n) {
        window[n] = 0.54 - 0.46 * cos(2 * PI * n / (N - 1));
    }
}

void EdgeMath::hanning_window(float* window, int N) {
    for (int n = 0; n < N; ++n) {
        window[n] = 0.5 * (1 - cos(2 * PI * n / (N - 1)));
    }
}

