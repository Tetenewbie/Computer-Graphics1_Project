#include "matrix.h"
#include <math.h>
#include <string.h>

void identity(matrix4x4 out) 
{
    memset(out, 0, 16 * sizeof(float));
    out[0]  = 1.0f;
    out[5]  = 1.0f;
    out[10] = 1.0f;
    out[15] = 1.0f;
}

void multiply(matrix4x4 out, const matrix4x4 left, const matrix4x4 right) 
{
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            out[c * 4 + r] = left[0 * 4 + r] * right[c * 4 + 0] +
                             left[1 * 4 + r] * right[c * 4 + 1] +
                             left[2 * 4 + r] * right[c * 4 + 2] +
                             left[3 * 4 + r] * right[c * 4 + 3];
        }
    }
}

void translate(matrix4x4 out, const matrix4x4 in, const vector3 offset) 
{
    // Copy the first 3 columns
    for (int i = 0; i < 12; i++) {
        out[i] = in[i];
    }
    // Calculate the translated 4th column
    for (int i = 0; i < 4; i++) {
        out[12 + i] = in[i] * offset[0] +
                      in[4 + i] * offset[1] +
                      in[8 + i] * offset[2] +
                      in[12 + i];
    }
}

void scale(matrix4x4 out, const matrix4x4 in, const vector3 scale_vec) 
{
    for (int i = 0; i < 4; i++) {
        out[i]      = in[i] * scale_vec[0];
        out[4 + i]  = in[4 + i] * scale_vec[1];
        out[8 + i]  = in[8 + i] * scale_vec[2];
        out[12 + i] = in[12 + i]; // W column is unscaled
    }
}

void rotatex(matrix4x4 out, const matrix4x4 in, float angle_rads) 
{
    float c = cosf(angle_rads);
    float s = sinf(angle_rads);
    matrix4x4 r = {0};
    
    r[0] = 1.0f;
    r[5] = c;  r[6] = s;
    r[9] = -s; r[10]= c;
    r[15]= 1.0f;
    
    multiply(out, in, r);
}

void rotatey(matrix4x4 out, const matrix4x4 in, float angle_rads) 
{
    float c = cosf(angle_rads);
    float s = sinf(angle_rads);
    matrix4x4 r = {0};
    
    r[0] = c;  r[2] = -s;
    r[5] = 1.0f;
    r[8] = s;  r[10]= c;
    r[15]= 1.0f;
    
    multiply(out, in, r);
}

void rotatez(matrix4x4 out, const matrix4x4 in, float angle_rads) 
{
    float c = cosf(angle_rads);
    float s = sinf(angle_rads);
    matrix4x4 r = {0};
    
    r[0] = c;  r[1] = s;
    r[4] = -s; r[5] = c;
    r[10]= 1.0f;
    r[15]= 1.0f;
    
    multiply(out, in, r);
}

void cmo_to_rmo(const matrix4x4 cmo, matrix4x4 rmo) 
{
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            rmo[r * 4 + c] = cmo[c * 4 + r];
        }
    }
}

int invert3x3(matrix3x3 out, const matrix3x3 in) 
{
    float det = in[0] * (in[4] * in[8] - in[5] * in[7]) -
                in[3] * (in[1] * in[8] - in[2] * in[7]) +
                in[6] * (in[1] * in[5] - in[2] * in[4]);

    if (fabsf(det) < 1e-6f) return 0; // Failure (matrix is singular)

    float invdet = 1.0f / det;
    out[0] = (in[4] * in[8] - in[5] * in[7]) * invdet;
    out[1] = (in[7] * in[2] - in[1] * in[8]) * invdet;
    out[2] = (in[1] * in[5] - in[2] * in[4]) * invdet;
    out[3] = (in[6] * in[5] - in[3] * in[8]) * invdet;
    out[4] = (in[0] * in[8] - in[6] * in[2]) * invdet;
    out[5] = (in[3] * in[2] - in[0] * in[5]) * invdet;
    out[6] = (in[3] * in[7] - in[6] * in[4]) * invdet;
    out[7] = (in[6] * in[1] - in[0] * in[7]) * invdet;
    out[8] = (in[0] * in[4] - in[3] * in[1]) * invdet;
    return 1;
}

void extract3x3_from4x4(matrix3x3 out, const matrix4x4 in) 
{
    out[0] = in[0]; out[1] = in[1]; out[2] = in[2];
    out[3] = in[4]; out[4] = in[5]; out[5] = in[6];
    out[6] = in[8]; out[7] = in[9]; out[8] = in[10];
}

void compute_normal_matrix(matrix3x3 out, const matrix4x4 in) 
{
    matrix3x3 ext, inv;
    extract3x3_from4x4(ext, in);
    invert3x3(inv, ext);
    
    // Transpose directly into 'out'
    out[0] = inv[0]; out[1] = inv[3]; out[2] = inv[6];
    out[3] = inv[1]; out[4] = inv[4]; out[5] = inv[7];
    out[6] = inv[2]; out[7] = inv[5]; out[8] = inv[8];
}
