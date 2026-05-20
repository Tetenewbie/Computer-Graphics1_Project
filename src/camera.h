#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"

void lookAt(matrix4x4 out, const vector3 eye, const vector3 target, const vector3 up);
void perspective(matrix4x4 out, float fovy, float aspect, float zNear, float zFar);

#endif 
