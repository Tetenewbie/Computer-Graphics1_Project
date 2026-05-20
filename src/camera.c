#include "camera.h"
#include "vector.h"
#include <math.h>

void lookAt(matrix4x4 out, const vector3 eye, const vector3 target, const vector3 up) 
{
    vector3 zaxis, xaxis, yaxis, diff;
    
    difVektor(eye, target, diff);
    normalizeVector(diff, zaxis);
    
    crossProdukt(up, zaxis, xaxis);
    normalizeVector(xaxis, xaxis);
    
    crossProdukt(zaxis, xaxis, yaxis);
    
    // Column 0
    out[0] = xaxis[0];  out[1] = yaxis[0];  out[2] = zaxis[0];  out[3] = 0.0f;
    // Column 1
    out[4] = xaxis[1];  out[5] = yaxis[1];  out[6] = zaxis[1];  out[7] = 0.0f;
    // Column 2
    out[8] = xaxis[2];  out[9] = yaxis[2];  out[10]= zaxis[2];  out[11]= 0.0f;
    // Column 3 (Translation)
    out[12] = -dotProduct(xaxis, eye);
    out[13] = -dotProduct(yaxis, eye);
    out[14] = -dotProduct(zaxis, eye);
    out[15] = 1.0f;
}

void perspective(matrix4x4 out, float fovy, float aspect, float zNear, float zFar) 
{
    float f = 1.0f / tanf(fovy / 2.0f);
    
    for (int i = 0; i < 16; i++) {
        out[i] = 0.0f;
    }
    
    // Scaled by 1.0/zNear
    out[0]  = (f / aspect) / zNear;
    out[5]  = f / zNear;
    out[10] = -(zFar + zNear) / (zFar - zNear) / zNear;
    out[11] = -1.0f / zNear;
    out[14] = -(2.0f * zFar * zNear) / (zFar - zNear) / zNear;
}
