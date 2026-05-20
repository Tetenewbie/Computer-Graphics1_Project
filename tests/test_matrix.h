#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

#include <math.h>
#include "test_framework.h"
#include "../src/matrix.h"

TEST(MatrixMath, Identity);
TEST(MatrixMath, Multiply);
TEST(MatrixMath, Translate);
TEST(MatrixMath, Scale);
TEST(MatrixMath, RotateX);
TEST(MatrixMath, RotateY);
TEST(MatrixMath, RotateZ);
TEST(MatrixMath, ConvertCmoToRmo);
TEST(MatrixMath, Invert3x3);
TEST(MatrixMath, Extract3x3From4x4);
TEST(MatrixMath, ComputeNormalMatrix);

#endif // TEST_MATRIX_H
