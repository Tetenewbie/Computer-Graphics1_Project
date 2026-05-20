#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/**
 * @def TEST(suite, name)
 * @brief Defines a new test case. The block of code following this macro forms the test body.
 * @param suite The logical grouping or module of the test (e.g., VectorMath).
 * @param name The specific behavior being tested.
 */
#define TEST(suite, name) \
    void test_##suite##_##name(void)

/**
 * @def TEST_CALL(suite, name)
 * @brief Executes a defined test case and logs its execution to the console.
 */

 extern bool current_test_failed;
// recommended 
 #define TEST_CALL(suite, name) \
    { \
        current_test_failed = false; \
        test_##suite##_##name(); \
        if (current_test_failed) { \
            printf(#suite "_" #name ": Nicht OK\n"); \
        } else { \
            printf(#suite "_" #name ": OK\n"); \
        } \
    }

/**
 * @def EXPECT_EQ(actual, expected, len)
 * @brief Verifies if two memory blocks are strictly identical.
 */
#define EXPECT_EQ(actual, expected, len) \
    check_memory_match((const void *) actual, (const void *) expected, len, __FILE__, __LINE__, #actual, #expected);

/**
 * @def EXPECT_EQ_FLOAT_ARRAY(actual, expected, len)
 * @brief Verifies if two float arrays are equal, accounting for floating-point imprecision.
 */
#define EXPECT_EQ_FLOAT_ARRAY(actual, expected, len) \
    check_float_array_match((const float *) actual, (const float *) expected, len, __FILE__, __LINE__, #actual, #expected);

// Tracks the total number of passed and failed test assertions.
typedef struct
{
    size_t passed;
    size_t failed;
} TestStatistics;

extern TestStatistics global_test_stats;

/**
 * @brief Checks if two floats are close enough to be considered equal.
 * * @param val_a First float.
 * @param val_b Second float to compare against.
 * @param precision_bits Determines the tolerance limit (tolerance = 1 / 2^precision_bits).
 * @return true if the values are within the tolerance margin, false otherwise.
 */
static inline bool is_float_nearly_equal(const float val_a, const float val_b, const float precision_bits)
{
    const float tolerance = 1.0f / powf(2.0f, precision_bits);
    return fabsf(val_a - val_b) < tolerance;
}

// Resets the global test tracking statistics.
void initialize_test_framework(void);

// Prints a final summary of all run tests and returns an appropriate exit code.
int conclude_test_run(void);

// Internal utility to compare raw memory. Triggered by EXPECT_EQ.
void check_memory_match(const void * actual, const void * expected, const size_t len, const char file[], const size_t line, const char actual_expr[], const char expected_expr[]);

// Internal utility to compare float arrays. Triggered by EXPECT_EQ_FLOAT_ARRAY.
void check_float_array_match(const float * actual, const float * expected, const size_t len, const char file[], const size_t line, const char actual_expr[], const char expected_expr[]);

#endif // TEST_FRAMEWORK_H