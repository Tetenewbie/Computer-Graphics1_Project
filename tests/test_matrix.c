#include "test_matrix.h"

TEST(MatrixMath, Identity)
{
    // Verify that the identity function generates a standard identity matrix
    matrix4x4 expected_identity = {
        1, 0, 0, 0, 
        0, 1, 0, 0, 
        0, 0, 1, 0, 
        0, 0, 0, 1
    };
    
    // Ensure the reference format is correctly configured before comparing
    cmo_to_rmo(expected_identity, expected_identity);
    
    matrix4x4 actual_identity;
    identity(actual_identity);
    EXPECT_EQ(actual_identity, expected_identity, sizeof(*expected_identity) * 16);
}

TEST(MatrixMath, Multiply)
{
    // Test case 1: Matrix multiplication by identity matrix
    {
        const matrix4x4 mat_left = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        const matrix4x4 mat_identity = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        const matrix4x4 expected_result = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        
        matrix4x4 actual_result;
        multiply(actual_result, mat_left, mat_identity);
        EXPECT_EQ(actual_result, expected_result, sizeof(*expected_result) * 16);
    }

    // Test case 2: Multiplication of two disparate matrices
    {
        const matrix4x4 mat_left = {
            1.0f, 0.0f, 5.0f, 0.0f,
            0.0f, 3.0f, 0.0f, 7.0f,
            2.0f, 0.0f, 6.0f, 0.0f,
            0.0f, 4.0f, 0.0f, 8.0f
        };
        const matrix4x4 mat_right = {
            0.5f, -1.0f, 2.0f, 0.0f,
            1.0f, 0.5f, 0.0f, 3.0f,
            0.0f, 2.0f, -1.0f, 0.0f,
            -1.0f, 0.0f, 0.5f, -2.0f
        };
        const matrix4x4 expected_result = {
            4.5f, -3.0f, 14.5f, -7.0f,
            1.0f, 13.5f, 5.0f, 27.5f,
            -2.0f, 6.0f, -6.0f, 14.0f,
            0.0f, -8.0f, -2.0f, -16.0f
        };
        
        matrix4x4 actual_result;
        multiply(actual_result, mat_left, mat_right);
        EXPECT_EQ(actual_result, expected_result, sizeof(*expected_result) * 16);
    }
}

TEST(MatrixMath, Translate)
{
    // Test case 1: Applying translation to an identity matrix
    {
        const matrix4x4 input_mat = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        const vector3 offset = {10.0f, 20.0f, 30.0f};
        const matrix4x4 expected_result = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            10.0f, 20.0f, 30.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        translate(actual_result, input_mat, offset);
        EXPECT_EQ(actual_result, expected_result, sizeof(*expected_result) * 16);
    }

    // Test case 2: Translating by a zero vector (should remain unchanged)
    {
        const matrix4x4 input_mat = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        const vector3 offset = {0.0f, 0.0f, 0.0f};
        
        matrix4x4 actual_result;
        translate(actual_result, input_mat, offset);
        EXPECT_EQ(actual_result, input_mat, sizeof(*input_mat) * 16);
    }
}

TEST(MatrixMath, Scale)
{
    // Test case 1: Applying scale to an identity matrix
    {
        const matrix4x4 input_mat = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        const vector3 scale_vec = {2.0f, -0.5f, 3.0f};
        const matrix4x4 expected_result = {
            2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 3.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        scale(actual_result, input_mat, scale_vec);
        EXPECT_EQ(actual_result, expected_result,  16);
    }

    // Test case 2: Scaling by 1.0 (should remain unchanged)
    {
        const matrix4x4 input_mat = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        const vector3 scale_vec = {1.0f, 1.0f, 1.0f};
        
        matrix4x4 actual_result;
        scale(actual_result, input_mat, scale_vec);
        EXPECT_EQ(actual_result, input_mat, 16);
    }
}

TEST(MatrixMath, RotateX)
{
    // Test case 1: 90 degree (PI/2) rotation around X-axis
    {
        const matrix4x4 input_mat = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        const float angle_rads = M_PI_2;
        const matrix4x4 expected_result = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        rotatex(actual_result, input_mat, angle_rads);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 2: 0 degree rotation (should remain unchanged)
    {
        const matrix4x4 input_mat = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        const float angle_rads = 0.0f;
        
        matrix4x4 actual_result;
        rotatex(actual_result, input_mat, angle_rads);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, input_mat, 16);
    }
}

TEST(MatrixMath, RotateY)
{
    // Test case 1: 90 degree (PI/2) rotation around Y-axis
    {
        const matrix4x4 input_mat = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        const float angle_rads = M_PI_2;
        const matrix4x4 expected_result = {
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        rotatey(actual_result, input_mat, angle_rads);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 2: 0 degree rotation (should remain unchanged)
    {
        const matrix4x4 input_mat = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        const float angle_rads = 0.0f;
        
        matrix4x4 actual_result;
        rotatey(actual_result, input_mat, angle_rads);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, input_mat, 16);
    }
}

TEST(MatrixMath, RotateZ)
{
    // Test case 1: 90 degree (PI/2) rotation around Z-axis
    {
        const matrix4x4 input_mat = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        const float angle_rads = M_PI_2;
        const matrix4x4 expected_result = {
            0.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix4x4 actual_result;
        rotatez(actual_result, input_mat, angle_rads);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 16);
    }

    // Test case 2: 0 degree rotation (should remain unchanged)
    {
        const matrix4x4 input_mat = {
            1.0f, 5.0f, 9.0f, 13.0f,
            2.0f, 6.0f, 10.0f, 14.0f,
            3.0f, 7.0f, 11.0f, 15.0f,
            4.0f, 8.0f, 12.0f, 16.0f
        };
        const float angle_rads = 0.0f;
        
        matrix4x4 actual_result;
        rotatez(actual_result, input_mat, angle_rads);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, input_mat, 16);
    }
}

TEST(MatrixMath, ConvertCmoToRmo)
{
    // Converts a Column-Major Order matrix to Row-Major Order (transposition equivalent in 1D memory)
    const matrix4x4 mat_cmo = {
        1, 2, 3, 4, 
        5, 6, 7, 8, 
        9, 10, 11, 12, 
        13, 14, 15, 16
    };
    const matrix4x4 expected_rmo = {
        1, 5, 9, 13, 
        2, 6, 10, 14, 
        3, 7, 11, 15, 
        4, 8, 12, 16
    };
    
    matrix4x4 actual_rmo;
    cmo_to_rmo(mat_cmo, actual_rmo);
    EXPECT_EQ(actual_rmo, expected_rmo, sizeof(*expected_rmo) * 16);
}

TEST(MatrixMath, Invert3x3)
{
    // Test case 1: Inverting a valid matrix
    {
        const matrix3x3 input_mat = {
            1, 2, 3, 
            0, 1, 4, 
            5, 6, 0
        };
        const matrix3x3 expected_result = {
            -24, 18, 5, 
            20, -15, -4, 
            -5, 4, 1
        };
        matrix3x3 actual_result;
        
        const int actual_status = invert3x3(actual_result, input_mat);
        const int expected_status = 1; // Indicates success
        
        EXPECT_EQ(&actual_status, &expected_status, sizeof(expected_status));
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 9);
    }

    // Test case 2: Inverting a diagonal matrix
    {
        const matrix3x3 input_mat = {
            2, 0, 0, 
            0, 5, 0, 
            0, 0, 10
        };
        const matrix3x3 expected_result = {
            0.5f, 0, 0, 
            0, 0.2f, 0, 
            0, 0, 0.1f
        };
        matrix3x3 actual_result;
        
        const int actual_status = invert3x3(actual_result, input_mat);
        const int expected_status = 1; 
        
        EXPECT_EQ(&actual_status, &expected_status, sizeof(expected_status));
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 9);
    }

    // Test case 3: Attempting to invert a non-invertible (singular) matrix
    {
        matrix3x3 input_mat = {
            1, 2, 3, 
            4, 5, 6, 
            5, 7, 9
        };
        
        const int actual_status = invert3x3(input_mat, input_mat);
        const int expected_status = 0; // Indicates failure (determinant is 0)
        
        EXPECT_EQ(&actual_status, &expected_status, sizeof(expected_status));
    }
}

TEST(MatrixMath, Extract3x3From4x4)
{
    // Test case 1: Extracting upper-left 3x3 subset from a populated 4x4 matrix
    {
        const matrix4x4 input_mat = {
            2, 3, 4, 5, 
            6, 7, 8, 9, 
            10, 11, 12, 13, 
            14, 15, 16, 17
        };
        const matrix3x3 expected_result = {
            2, 3, 4, 
            6, 7, 8, 
            10, 11, 12
        };
        
        matrix3x3 actual_result;
        extract3x3_from4x4(actual_result, input_mat);
        EXPECT_EQ(actual_result, expected_result, sizeof(*expected_result) * 9);
    }

    // Test case 2: Extracting from a zeroed matrix
    {
        const matrix4x4 input_mat = {0}; 
        const matrix3x3 expected_result = {0}; 
        
        matrix3x3 actual_result;
        extract3x3_from4x4(actual_result, input_mat);
        EXPECT_EQ(actual_result, expected_result, sizeof(*expected_result) * 9);
    }
}

TEST(MatrixMath, ComputeNormalMatrix)
{
    // Computes the inverse transpose of the upper-left 3x3 section for lighting normals
    //    
    // Test case 1
    {
        const matrix4x4 input_mat = {
            0, 0, 1, 0, 
            0, 1, 0, 0, 
            -1, 0, 0, 0, 
            0, 0, 0, 1
        };
        const matrix3x3 expected_result = {
            0, 0, 1, 
            0, 1, 0, 
            -1, 0, 0
        };
        
        matrix3x3 actual_result;
        compute_normal_matrix(actual_result, input_mat);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 9);
    }

    // Test case 2: Matrix with scaling components
    {
        const matrix4x4 input_mat = {
            2, 0, 0, 0, 
            0, 0.5f, 0, 0, 
            0, 0, 1, 0, 
            0, 0, 0, 1
        };
        const matrix3x3 expected_result = {
            0.5f, 0, 0, 
            0, 2, 0, 
            0, 0, 1
        };
        
        matrix3x3 actual_result;
        compute_normal_matrix(actual_result, input_mat);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 9);
    }

    // Test case 3: Sheared matrix
    {
        const matrix4x4 input_mat = {
            0, -2, 0, 10, 
            1, 0, 0, 5, 
            0, 0, 1, 0, 
            0, 0, 0, 1
        };
        const matrix3x3 expected_result = {
            0, -0.5f, 0, 
            1, 0, 0, 
            0, 0, 1
        };
        
        matrix3x3 actual_result;
        compute_normal_matrix(actual_result, input_mat);
        EXPECT_EQ_FLOAT_ARRAY(actual_result, expected_result, 9);
    }
}
