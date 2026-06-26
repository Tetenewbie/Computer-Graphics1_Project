#include "test_camera.h"

TEST(CameraMath, LookAt)
{
    // Test case 1: Looking down the negative Z-axis from the origin
    {
        const vector3 cam_eye = {0.0f, 0.0f, 0.0f};
        const vector3 cam_target = {0.0f, 0.0f, -1.0f};
        const vector3 cam_up = {0.0f, 1.0f, 0.0f};
        const matrix4x4 expected_result = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        lookAt(actual_result, cam_eye, cam_target, cam_up);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 2: Looking straight down from the Y-axis (Up vector needs to adjust to Z)
    {
        const vector3 cam_eye = {0.0f, 5.0f, 0.0f};
        const vector3 cam_target = {0.0f, 0.0f, 0.0f};
        const vector3 cam_up = {0.0f, 0.0f, 1.0f};
        const matrix4x4 expected_result = {
            -1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -5.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        lookAt(actual_result, cam_eye, cam_target, cam_up);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 3: Looking at origin from the X-axis
    {
        const vector3 cam_eye = {5.0f, 0.0f, 0.0f};
        const vector3 cam_target = {0.0f, 0.0f, 0.0f};
        const vector3 cam_up = {0.0f, 1.0f, 0.0f};
        const matrix4x4 expected_result = {
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -5.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        lookAt(actual_result, cam_eye, cam_target, cam_up);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 4: Complex, arbitrary camera placement
    {
        const vector3 cam_eye = {10.5f, 3.4f, 0.0f};
        const vector3 cam_target = {2.3f, 4.5f, 6.7f};
        const vector3 cam_up = {8.9f, 10.11f, 12.13f};
        const matrix4x4 expected_result = {
            -0.283310f, 0.571381f, 0.770233f, 0.0f,
            0.828647f, 0.550153f, -0.103324f, 0.0f,
            -0.482784f, 0.608979f, -0.629337f, 0.0f,
            0.157351f, -7.870024f, -7.736147f, 1.0f
        };
        
        matrix4x4 actual_result;
        lookAt(actual_result, cam_eye, cam_target, cam_up);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }
}

TEST(CameraMath, Perspective)
{
    // Test case 1: Standard perspective projection (60 deg FOV)
    {
        const matrix4x4 expected_result = {
            0.0017321f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.1732051f, 0.0f, 0.0f,
            0.0f, 0.0f, -0.1020202f, -0.1f, // Updated float literal here
            0.0f, 0.0f, -2.0202021f, 0.0f
        };
        
        matrix4x4 actual_result;
        // fovy, aspect, zNear, zFar
        perspective(actual_result, 60.0f / 180.0f * M_PI, 100.0f, 10.0f, 1000.0f);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 2: Extreme wide-angle perspective 
    {
        const matrix4x4 expected_result = {
            411.484222f, 0.0f, 0.0f, 0.0f,
            0.0f, 174.551605f, 0.0f, 0.0f,
            0.0f, 0.0f, -10000.046875f, -10000.0f,
            0.0f, 0.0f, -2.000005f, 0.0f
        };
        
        matrix4x4 actual_result;
        perspective(actual_result, 178.0f / 180.0f * M_PI, 0.4242f, 0.0001f, 42.33f);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }
}
