/**
 * @file test_deq.c
 * @brief A comprehensive test suite for a double-ended queue (Deq) implementation.
 *
 * This file contains multiple test cases covering both basic operations (put/get)
 * and more advanced scenarios (mapping, string conversion, and moderate stress testing).
 * Each test is self-contained, and a simple framework is used to count passed and failed tests.
 *
 * Usage:
 *   gcc -o test_deq test_deq.c deq.c
 *   ./test_deq
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deq.h"

/* -------------------------------------------------------------------------
   Simple testing framework:
   - tests_run:    how many tests have been attempted
   - tests_passed: how many tests actually passed
   - test(condition, test_name): increments counters, prints PASS/FAIL
   ------------------------------------------------------------------------- */
static int tests_passed = 0;
static int tests_run    = 0;

/**
 * @brief Checks a boolean condition and logs whether the test passed or failed.
 *
 * @param condition  Boolean expression indicating success/failure of the test
 * @param test_name  A descriptive string to identify the test being performed
 */
static void test(int condition, const char* test_name) {
    tests_run++;
    if (condition) {
        tests_passed++;
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

/* -------------------------------------------------------------------------
   Test 1: Creation and Deletion
   - Creates a new Deq
   - Checks the initial length is 0
   - Deletes the Deq (ensuring no crash)
   ------------------------------------------------------------------------- */
static void test_create_delete() {
    Deq q = deq_new();
    test(q != NULL, "Create new Deq (q != NULL)");
    test(deq_len(q) == 0, "New Deq length is 0");

    // Delete the queue (no function results to check other than no crash)
    deq_del(q, NULL);
    test(1, "Delete Deq (no crash)");
}

/* -------------------------------------------------------------------------
   Test 2: Basic put & get from each end
   - Demonstrates head_put and tail_put
   - Retrieves items via head_get and tail_get
   - Confirms expected sequence is maintained
   ------------------------------------------------------------------------- */
static void test_put_get() {
    Deq q = deq_new();

    // Head puts
    deq_head_put(q, "A");
    deq_head_put(q, "B");
    test(deq_len(q) == 2, "Length after 2 head_put");

    // Tail puts
    deq_tail_put(q, "C");
    deq_tail_put(q, "D");
    test(deq_len(q) == 4, "Length after 2 tail_put (total 4)");

    // Check order from the head perspective:
    // B (head), A, C, D (tail)
    test(strcmp((char*)deq_head_ith(q, 0), "B") == 0, "head_ith(0) = B");
    test(strcmp((char*)deq_head_ith(q, 1), "A") == 0, "head_ith(1) = A");
    test(strcmp((char*)deq_head_ith(q, 2), "C") == 0, "head_ith(2) = C");
    test(strcmp((char*)deq_head_ith(q, 3), "D") == 0, "head_ith(3) = D");

    // Remove via get (head & tail)
    char *rem1 = (char*)deq_head_get(q);
    test(strcmp(rem1, "B") == 0, "deq_head_get -> B");

    char *rem2 = (char*)deq_tail_get(q);
    test(strcmp(rem2, "D") == 0, "deq_tail_get -> D");

    // Now queue should have A, C
    test(deq_len(q) == 2, "Length == 2 after removing two elements");

    // Cleanup
    deq_del(q, NULL);
}

/* -------------------------------------------------------------------------
   Test 3: ith() from head and tail
   - Inserts items at the head
   - Checks that head_ith() and tail_ith() return correct values
   - Out-of-range checks are commented out because ith() calls ERROR() and exits
   ------------------------------------------------------------------------- */
static void test_ith() {
    Deq q = deq_new();

    // Let's put 3,2,1 at the head => queue is 3,2,1
    deq_head_put(q, "1");
    deq_head_put(q, "2");
    deq_head_put(q, "3");
    test(deq_len(q) == 3, "Length after 3 head_put");

    // Check from head
    test(strcmp((char*)deq_head_ith(q, 0), "3") == 0, "head_ith(0) = 3");
    test(strcmp((char*)deq_head_ith(q, 1), "2") == 0, "head_ith(1) = 2");
    test(strcmp((char*)deq_head_ith(q, 2), "1") == 0, "head_ith(2) = 1");

    // Check from tail
    test(strcmp((char*)deq_tail_ith(q, 0), "1") == 0, "tail_ith(0) = 1");
    test(strcmp((char*)deq_tail_ith(q, 1), "2") == 0, "tail_ith(1) = 2");
    test(strcmp((char*)deq_tail_ith(q, 2), "3") == 0, "tail_ith(2) = 3");

    deq_del(q, NULL);
}

/* -------------------------------------------------------------------------
   Test 4: rem() from head or tail side
   - Inserts A, B, C, D at the tail
   - Removes items by searching from either head or tail
   - Validates the resulting queue state each time
   ------------------------------------------------------------------------- */
static void test_rem() {
    Deq q = deq_new();

    // Put A, B, C, D at the tail => queue is A, B, C, D
    deq_tail_put(q, "A");
    deq_tail_put(q, "B");
    deq_tail_put(q, "C");
    deq_tail_put(q, "D");
    test(deq_len(q) == 4, "Length == 4 after tail_put A,B,C,D");

    // Remove "B" by searching from head side
    char* r1 = (char*)deq_head_rem(q, "B");
    test(r1 && strcmp(r1, "B") == 0, "deq_head_rem(\"B\") => B found");
    test(deq_len(q) == 3, "Length == 3 after removing B");

    // Now queue is A, C, D
    test(strcmp((char*)deq_head_ith(q, 0), "A") == 0, "Remaining[0] = A");
    test(strcmp((char*)deq_head_ith(q, 1), "C") == 0, "Remaining[1] = C");
    test(strcmp((char*)deq_head_ith(q, 2), "D") == 0, "Remaining[2] = D");

    // Remove "A" by searching from tail side
    char* r2 = (char*)deq_tail_rem(q, "A");
    test(r2 && strcmp(r2, "A") == 0, "deq_tail_rem(\"A\") => A found");
    test(deq_len(q) == 2, "Length == 2 after removing A");

    // Now queue is C, D
    test(strcmp((char*)deq_head_ith(q, 0), "C") == 0, "Remaining[0] = C");
    test(strcmp((char*)deq_head_ith(q, 1), "D") == 0, "Remaining[1] = D");

    // Remove an item not in queue
    char* r3 = (char*)deq_tail_rem(q, "X");
    test(r3 == NULL, "deq_tail_rem(\"X\") => not found => NULL");
    test(deq_len(q) == 2, "Length still == 2");

    // Remove "C" from head
    char* r4 = (char*)deq_head_rem(q, "C");
    test(r4 && strcmp(r4, "C") == 0, "deq_head_rem(\"C\") => C found");
    test(deq_len(q) == 1, "Length == 1 after removing C");

    // Remove "D" from tail
    char* r5 = (char*)deq_tail_rem(q, "D");
    test(r5 && strcmp(r5, "D") == 0, "deq_tail_rem(\"D\") => D found");
    test(deq_len(q) == 0, "Length == 0 => empty now");

    // Remove from empty queue
    char* r6 = (char*)deq_tail_rem(q, "Z");
    test(r6 == NULL, "deq_tail_rem(\"Z\") on empty => NULL");

    deq_del(q, NULL);
}

/* -------------------------------------------------------------------------
   HELPER FUNCTIONS FOR MAPPING & STRING CONVERSION TESTS
   map_uppercase(Data d): modifies the string in-place to uppercase.
   starify(Data d): returns a newly allocated string wrapped in asterisks.
   We must free the result from starify() once it's used.
   ------------------------------------------------------------------------- */

/**
 * @brief Map function to convert a modifiable string to uppercase (in-place).
 *
 * @param d  Data pointer expected to be a char* on the heap (writable).
 */
static void map_uppercase(Data d) {
    char *str = (char*)d;
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32; // convert to uppercase
        }
        str++;
    }
}

/**
 * @brief String converter that adds '*' around a string (heap-allocates new string).
 *
 * For example, "Hello" -> "*Hello*". The caller (or deq_str internals) should free
 * the returned string when done.
 *
 * @param d  Data pointer expected to be a const char*
 * @return A newly allocated string with asterisks around the original content.
 */
static char* starify(Data d) {
    const char* input = (char*)d;
    size_t inlen = strlen(input);
    // We'll do "*...*" => we need 2 extra chars + null terminator
    char* out = (char*)malloc(inlen + 3);
    sprintf(out, "*%s*", input);
    return out;
}

/* -------------------------------------------------------------------------
   Test 5: deq_map and deq_str
   - We add two heap-allocated strings, "alpha" and "beta"
   - Use map_uppercase to modify them in-place
   - Check the results in the deque
   - Use deq_str with starify() and with NULL to generate string representations
   ------------------------------------------------------------------------- */
static void test_map_and_str() {
    Deq q = deq_new();

    // Put some modifiable strings on the heap
    // Because if we use string literals, we can't legally modify them
    char* s1 = strdup("alpha"); 
    char* s2 = strdup("beta");
    deq_tail_put(q, s1);
    deq_tail_put(q, s2);

    // Use deq_map to uppercase them
    deq_map(q, map_uppercase);

    // Now s1 should be "ALPHA" and s2 should be "BETA"
    test(strcmp((char*)deq_head_ith(q, 0), "ALPHA") == 0, "After map_uppercase, first item is ALPHA");
    test(strcmp((char*)deq_head_ith(q, 1), "BETA") == 0, "After map_uppercase, second item is BETA");

    // Check deq_str with a custom function
    char *str_with_stars = deq_str(q, starify);
    // Should produce "*ALPHA* *BETA*"
    test(strcmp(str_with_stars, "*ALPHA* *BETA*") == 0, "deq_str() with starify => '*ALPHA* *BETA*'");
    free(str_with_stars);

    // Check deq_str with NULL function => "ALPHA BETA"
    char *normal_str = deq_str(q, NULL);
    test(strcmp(normal_str, "ALPHA BETA") == 0, "deq_str() with NULL => 'ALPHA BETA'");
    free(normal_str);

    // Cleanup the queue. Note that deq_del won't free s1, s2 unless we pass a function,
    // so we'll manually remove and free them:
    char* got1 = (char*)deq_head_get(q); // ALPHA
    char* got2 = (char*)deq_head_get(q); // BETA
    free(got1);
    free(got2);

    deq_del(q, NULL);
}

/* -------------------------------------------------------------------------
   Test 6: Larger-scale usage
   - Inserts 100 items at the tail
   - Removes 50 from the head, then 50 from the tail
   - Checks that the length is updated properly
   - Frees allocated strings
   ------------------------------------------------------------------------- */
static void test_large() {
    Deq q = deq_new();
    // Insert 100 items at the tail
    for (int i = 0; i < 100; i++) {
        char buffer[32];
        sprintf(buffer, "Item%d", i);
        // Must allocate on heap if we want to store them safely & possibly modify them
        deq_tail_put(q, strdup(buffer));
    }
    test(deq_len(q) == 100, "Length == 100 after inserting 100 items");

    // Remove 50 items from the head
    for (int i = 0; i < 50; i++) {
        char *front = (char*)deq_head_get(q);
        // We won't test the exact string, just free it
        free(front);
    }
    test(deq_len(q) == 50, "Length == 50 after removing 50 from head");

    // Remove remaining 50 items from the tail
    for (int i = 0; i < 50; i++) {
        char *end = (char*)deq_tail_get(q);
        free(end);
    }
    test(deq_len(q) == 0, "Length == 0 after removing all items");

    deq_del(q, NULL);
}

/**
 * @brief Main function, runs all tests in sequence and prints a summary.
 */
int main(void) {
    printf("=== Running Full-Coverage Test Suite for Deq ===\n\n");

    test_create_delete();
    test_put_get();
    test_ith();
    test_rem();
    test_map_and_str();
    test_large();

    printf("\n==========================\n");
    printf("Tests run   : %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    if (tests_passed == tests_run) {
        printf("All tests PASSED!\n");
    } else {
        printf("Some tests FAILED.\n");
    }
    return (tests_passed == tests_run) ? 0 : 1;
}
