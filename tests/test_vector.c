#include "test_vector.h"

TEST(VectorMath, Difference)
{
    // Test case 1: Standard subtraction
    {
        const vector3 vec_left = {5.0f, 6.0f, 7.0f};
        const vector3 vec_right = {1.0f, 2.0f, 3.0f};
        const vector3 expected_result = {4.0f, 4.0f, 4.0f};
        vector3 actual_result;
        
        difVektor(vec_left, vec_right, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 2: Subtraction involving negative numbers
    {
        const vector3 vec_left = {1.0f, -2.0f, 3.0f};
        const vector3 vec_right = {-1.0f, 1.0f, -1.0f};
        const vector3 expected_result = {2.0f, -3.0f, 4.0f};
        vector3 actual_result;
        
        difVektor(vec_left, vec_right, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 3: Subtracting a vector from itself (should yield zero vector)
    {
        const vector3 vec_left = {10.0f, 20.0f, 30.0f};
        const vector3 vec_right = {10.0f, 20.0f, 30.0f};
        const vector3 expected_result = {0.0f, 0.0f, 0.0f};
        vector3 actual_result;
        
        difVektor(vec_left, vec_right, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 4: Subtracting from a zero vector
    {
        const vector3 vec_left = {0.0f, 0.0f, 0.0f};
        const vector3 vec_right = {2.0f, -3.0f, 5.0f};
        const vector3 expected_result = {-2.0f, 3.0f, -5.0f};
        vector3 actual_result;
        
        difVektor(vec_left, vec_right, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }
}

TEST(VectorMath, Normalize)
{
    // Test case 1: Normalizing a simple axis-aligned vector
    {
        const vector3 input_vec = {3.0f, 0.0f, 0.0f};
        const vector3 expected_result = {1.0f, 0.0f, 0.0f};
        vector3 actual_result;
        
        normalizeVector(input_vec, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 2: Normalizing an angled vector (1, 1, 1)
    {
        const vector3 input_vec = {1.0f, 1.0f, 1.0f};
        const vector3 expected_result = {0.57735027f, 0.57735027f, 0.57735027f};
        vector3 actual_result;
        
        normalizeVector(input_vec, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 3: Normalizing a vector with negative components
    {
        const vector3 input_vec = {0.0f, -4.0f, 3.0f};
        const vector3 expected_result = {0.0f, -0.8f, 0.6f};
        vector3 actual_result;
        
        normalizeVector(input_vec, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 4: Edge case - normalizing a zero vector
    {
        const vector3 input_vec = {0.0f, 0.0f, 0.0f};
        const vector3 expected_result = {0.0f, 0.0f, 0.0f};
        vector3 actual_result;
        
        normalizeVector(input_vec, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }
}

TEST(VectorMath, CrossProduct)
{
    // Test case 1: Basic orthogonal cross product (X cross Y = Z)
    {
        const vector3 vec_a = {1.0f, 0.0f, 0.0f};
        const vector3 vec_b = {0.0f, 1.0f, 0.0f};
        const vector3 expected_result = {0.0f, 0.0f, 1.0f};
        vector3 actual_result;
        
        crossProdukt(vec_a, vec_b, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 2: Flipped inputs should yield the opposite vector (-Z)
    {
        const vector3 vec_a = {0.0f, 1.0f, 0.0f};
        const vector3 vec_b = {1.0f, 0.0f, 0.0f};
        const vector3 expected_result = {0.0f, 0.0f, -1.0f};
        vector3 actual_result;
        
        crossProdukt(vec_a, vec_b, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 3: Cross product of a vector with itself should be zero
    {
        const vector3 vec_a = {1.0f, 2.0f, 3.0f};
        const vector3 vec_b = {1.0f, 2.0f, 3.0f};
        const vector3 expected_result = {0.0f, 0.0f, 0.0f};
        vector3 actual_result;
        
        crossProdukt(vec_a, vec_b, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }

    // Test case 4: Standard complex cross product
    {
        const vector3 vec_a = {1.0f, -2.0f, 3.0f};
        const vector3 vec_b = {4.0f, 0.0f, -5.0f};
        const vector3 expected_result = {10.0f, 17.0f, 8.0f};
        vector3 actual_result;
        
        crossProdukt(vec_a, vec_b, actual_result);
        EXPECT_EQ(actual_result, expected_result, 3 * sizeof(GLfloat));
    }
}

TEST(VectorMath, DotProduct)
{
    // Test case 1: Orthogonal vectors (dot product should be 0)
    {
        const vector3 vec_a = {1.0f, 0.0f, 0.0f};
        const vector3 vec_b = {0.0f, 1.0f, 0.0f};
        const float expected_result = 0.0f;
        
        float actual_result = dotProduct(vec_a, vec_b);
        EXPECT_EQ(&actual_result, &expected_result, 1 * sizeof(GLfloat));
    }

    // Test case 2: Parallel unit vectors (dot product should be 1)
    {
        const vector3 vec_a = {1.0f, 0.0f, 0.0f};
        const vector3 vec_b = {1.0f, 0.0f, 0.0f};
        const float expected_result = 1.0f;
        
        float actual_result = dotProduct(vec_a, vec_b);
        EXPECT_EQ(&actual_result, &expected_result, 1 * sizeof(GLfloat));
    }

    // Test case 3: Standard dot product calculation
    {
        const vector3 vec_a = {1.0f, 2.0f, 3.0f};
        const vector3 vec_b = {4.0f, -5.0f, 6.0f};
        const float expected_result = 12.0f; // (1*4) + (2*-5) + (3*6) = 4 - 10 + 18 = 12
        
        float actual_result = dotProduct(vec_a, vec_b);
        EXPECT_EQ(&actual_result, &expected_result, 1 * sizeof(GLfloat));
    }

    // Test case 4: Dot product against a zero vector
    {
        const vector3 vec_a = {1.0f, 2.0f, 3.0f};
        const vector3 vec_b = {0.0f, 0.0f, 0.0f};
        const float expected_result = 0.0f;
        
        float actual_result = dotProduct(vec_a, vec_b);
        EXPECT_EQ(&actual_result, &expected_result, 1 * sizeof(GLfloat));
    }
}