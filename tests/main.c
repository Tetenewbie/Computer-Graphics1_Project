#include "test_framework.h"
#include "test_camera.h"
#include "test_matrix.h"
#include "test_vector.h"

int main(void)
{
    // Setup global counters
    initialize_test_framework();

    // matrix operations
    TEST_CALL(MatrixMath, Identity);
    TEST_CALL(MatrixMath, Multiply);
    TEST_CALL(MatrixMath, Translate);
    TEST_CALL(MatrixMath, Scale);
    TEST_CALL(MatrixMath, RotateX);
    TEST_CALL(MatrixMath, RotateY);
    TEST_CALL(MatrixMath, RotateZ);
    TEST_CALL(MatrixMath, ConvertCmoToRmo);
    TEST_CALL(MatrixMath, Invert3x3);
    TEST_CALL(MatrixMath, Extract3x3From4x4);
    TEST_CALL(MatrixMath, ComputeNormalMatrix);

    // amera 
    TEST_CALL(CameraMath, LookAt);
    TEST_CALL(CameraMath, Perspective);

    // 
    // Vector operations
    TEST_CALL(VectorMath, Difference);
    TEST_CALL(VectorMath, Normalize);
    TEST_CALL(VectorMath, CrossProduct);
    TEST_CALL(VectorMath, DotProduct);

    // Output results and exit securely
    return conclude_test_run();
}
