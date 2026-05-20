#include "vector.h"
#include <math.h>

void difVektor(const vector3 vec_left, const vector3 vec_right, vector3 actual_result) 
{
    actual_result[0] = vec_left[0] - vec_right[0];
    actual_result[1] = vec_left[1] - vec_right[1];
    actual_result[2] = vec_left[2] - vec_right[2];
}

void normalizeVector(const vector3 input_vec, vector3 actual_result) 
{
    float len = sqrtf(input_vec[0] * input_vec[0] +
                      input_vec[1] * input_vec[1] +
                      input_vec[2] * input_vec[2]);
                      
    if (len > 0.0f) {
        actual_result[0] = input_vec[0] / len;
        actual_result[1] = input_vec[1] / len;
        actual_result[2] = input_vec[2] / len;
    } else {
        actual_result[0] = 0.0f;
        actual_result[1] = 0.0f;
        actual_result[2] = 0.0f;
    }
}

void crossProdukt(const vector3 vec_a, const vector3 vec_b, vector3 actual_result) 
{
    actual_result[0] = vec_a[1] * vec_b[2] - vec_a[2] * vec_b[1];
    actual_result[1] = vec_a[2] * vec_b[0] - vec_a[0] * vec_b[2];
    actual_result[2] = vec_a[0] * vec_b[1] - vec_a[1] * vec_b[0];
}

float dotProduct(const vector3 vec_a, const vector3 vec_b) 
{
    return vec_a[0] * vec_b[0] + vec_a[1] * vec_b[1] + vec_a[2] * vec_b[2];
}