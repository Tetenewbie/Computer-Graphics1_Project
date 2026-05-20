#ifndef VECTOR_H
#define VECTOR_H

typedef float vector3[3];
typedef float GLfloat;

void difVektor(const vector3 vec_left, const vector3 vec_right, vector3 actual_result);
void normalizeVector(const vector3 input_vec, vector3 actual_result);
void crossProdukt(const vector3 vec_a, const vector3 vec_b, vector3 actual_result);
float dotProduct(const vector3 vec_a, const vector3 vec_b);

#endif 
