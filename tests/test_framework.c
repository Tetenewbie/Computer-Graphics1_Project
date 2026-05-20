#include "test_framework.h"

// Global state to track test outcomes across all files
TestStatistics global_test_stats;
bool current_test_failed = false; // Required to track the current test status

void initialize_test_framework(void)
{
    // Start with a clean slate for both passing and failing assertions
    global_test_stats.passed = 0;
    global_test_stats.failed = 0;
}

void check_memory_match(const void * actual,
                        const void * expected,
                        const size_t len,
                        const char file[],
                        const size_t line,
                        const char actual_expr[],
                        const char expected_expr[])
{
    // Check if the memory blocks match exactly byte-for-byte
    if (memcmp(actual, expected, len) == 0)
    {
        global_test_stats.passed++;
    }
    else
    {
        global_test_stats.failed++;
        current_test_failed = true;
    } 
}

void check_float_array_match(const float * actual,
                             const float * expected,
                             const size_t len,
                             const char file[],
                             const size_t line,
                             const char actual_expr[],
                             const char expected_expr[])
{
    bool arrays_match = true;
    
    // Iterate through the arrays, checking each element with a tolerance of 10 bits precision
    for (size_t i = 0; i < len; i++)
    {
        if (!is_float_nearly_equal(actual[i], expected[i], 10.0f))
        {
            // A single mismatched element means the entire array comparison fails
            arrays_match = false;
            current_test_failed = true;
            break;
        } 
    }

    if (arrays_match)
    {
        global_test_stats.passed++;
    }
    else
    {
        global_test_stats.failed++;
        // <-- Removed the leftover fprintf that would print actual values
    }
}

int conclude_test_run(void)
{
    //  OK nicht ok
    printf("\n=== Zusammenfassung ===\nBestanden: %zu\nFehlgeschlagen: %zu\n",
            global_test_stats.passed,
            global_test_stats.failed);

    // Return 0 if everything passed (success exit code), otherwise 1 (failure exit code)
    return (global_test_stats.failed == 0) ? 0 : 1;
}