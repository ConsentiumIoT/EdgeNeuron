#ifndef EDGEMATH_H
#define EDGEMATH_H

#include <Arduino.h>
#include <math.h>
#include <vector>

using namespace std;

#define PI 3.14159265358979

class EdgeMath{
    public:
        EdgeMath();
        void scaler_transform(float *data_array, const float* mean, const float* scale, int size);
        int argmax(float *data_array, int size);
        int argmin(float *data_array, int size);
        float mean(float *data_array, int size);
        float std(float *data_array, int size);
        float min(float *data_array, int size);
        float max(float *data_array, int size);
        float sum(float *data_array, int size);
        float skew(float *data_array, int size);
        float kurtosis(float *data_array, int size);
        float quantize(float value, float min, float max, int bits);
        float dequantize(int quantized_value, float min, float max, int bits);
        void hamming_window(float* window, int N);
        void hanning_window(float* window, int N);
        float accleration_vector(float x, float y, float z);
};


#endif  // EDGEMATH_H