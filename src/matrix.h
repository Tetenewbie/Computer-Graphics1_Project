#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

// 1D arrays representing column-major matrices
typedef float matrix4x4[16];
typedef float matrix3x3[9];

float degrees_to_radians(float d);
void identity(matrix4x4 out);
void multiply(matrix4x4 out, const matrix4x4 left, const matrix4x4 right);
void translate(matrix4x4 out, const matrix4x4 in, const vector3 offset);
void scale(matrix4x4 out, const matrix4x4 in, const vector3 scale_vec);
void rotatex(matrix4x4 out, const matrix4x4 in, float angle_rads);
void rotatey(matrix4x4 out, const matrix4x4 in, float angle_rads);
void rotatez(matrix4x4 out, const matrix4x4 in, float angle_rads);
void cmo_to_rmo(const matrix4x4 cmo, matrix4x4 rmo);

// 3x3 operations
int invert3x3(matrix3x3 out, const matrix3x3 in);
void extract3x3_from4x4(matrix3x3 out, const matrix4x4 in);
void compute_normal_matrix(matrix3x3 out, const matrix4x4 in);

#endif

