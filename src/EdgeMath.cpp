#include "EdgeMath.h"

EdgeMath::EdgeMath() {}

// Statistical functions
float EdgeMath::mean(float *data_array, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data_array[i];
    }
    return sum / size;
}

float EdgeMath::std(float *data_array, int size) {
    if (size < 2) return 0;  // Avoid division by zero
    float mean_value = mean(data_array, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(data_array[i] - mean_value, 2);
    }
    return sqrt(sum / (size - 1));  // Bessel's correction
}


float EdgeMath::max(float *data_array, int size) {
    float max = data_array[0];
    for (int i = 1; i < size; i++) {
        if (data_array[i] > max) {
            max = data_array[i];
        }
    }
    return max;
}

float EdgeMath::min(float *data_array, int size) {
    float min = data_array[0];
    for (int i = 1; i < size; i++) {
        if (data_array[i] < min) {
            min = data_array[i];
        }
    }
    return min;
}

float EdgeMath::sum(float *data_array, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data_array[i];
    }
    return sum;
}

float EdgeMath::skew(float *data_array, int size) {
    if (size < 3) return 0;  // Avoid division by zero
    float mean_value = mean(data_array, size);
    float std_value = std(data_array, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(data_array[i] - mean_value, 3);
    }
    return (size * sum) / ((size - 1) * (size - 2) * pow(std_value, 3));
}


float EdgeMath::kurtosis(float *data_array, int size) {
    if (size < 4) return 0;  // Avoid division by zero
    float mean_value = mean(data_array, size);
    float std_value = std(data_array, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(data_array[i] - mean_value, 4);
    }
    float raw_kurt = (size * (size + 1) * sum) / ((size - 1) * (size - 2) * (size - 3) * pow(std_value, 4));
    float correction = (3 * pow(size - 1, 2)) / ((size - 2) * (size - 3));
    return raw_kurt - correction;  // Excess kurtosis
}


// Data transformation functions
void EdgeMath::scaler_transform(float *data_array, const float* mean, const float* scale, int size){
  for(int i=0; i<size; i++){
    data_array[i] = ((data_array[i] - mean[i]) / scale[i]);
  }
}

// Numpy equivalents
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

int EdgeMath::argmin(float *data_array, int size) {
    float min = data_array[0];
    int min_index = 0;

    for (int i = 1; i < size; i++) {
        if (data_array[i] < min) {
            min = data_array[i];
            min_index = i;
        }
    }

    return min_index; // Return the index of the minimum value
}


// Quantization functions
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


// DSP functions
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

// Sensor processing functions
float EdgeMath::accleration_vector(float x, float y, float z){
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

