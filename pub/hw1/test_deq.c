#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deq.h"

// Simple testing framework counters
static int tests_passed = 0;
static int tests_run    = 0;

// A tiny helper macro or function to log test results
static void test(int condition, const char* test_name) {
    tests_run++;
    if (condition) {
        tests_passed++;
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

// Test 1: Creation and Deletion
static void test_create_delete() {
    Deq q = deq_new();
    test(q != NULL, "Create new Deq (q != NULL)");
    test(deq_len(q) == 0, "New Deq length is 0");

    // Delete the queue (no function results to check other than no crash)
    deq_del(q, NULL);
    test(1, "Delete Deq (no crash)");
}

// Test 2: Basic put & get from each end
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

    // Check order from head side:
    // B (head), A, C, D (tail)
    test(strcmp((char*)deq_head_ith(q, 0), "B") == 0, "head_ith(0) = B");
    test(strcmp((char*)deq_head_ith(q, 1), "A") == 0, "head_ith(1) = A");
    test(strcmp((char*)deq_head_ith(q, 2), "C") == 0, "head_ith(2) = C");
    test(strcmp((char*)deq_head_ith(q, 3), "D") == 0, "head_ith(3) = D");

    // Get from head
    char *rem1 = (char*)deq_head_get(q);
    test(strcmp(rem1, "B") == 0, "deq_head_get -> B");
    // Get from tail
    char *rem2 = (char*)deq_tail_get(q);
    test(strcmp(rem2, "D") == 0, "deq_tail_get -> D");

    // Now queue should have A, C
    test(deq_len(q) == 2, "Length == 2 after removing two elements");

    // Cleanup
    deq_del(q, NULL);
}

// Test 3: ith() from head and tail
static void test_ith() {
    Deq q = deq_new();

    // Let's put 3,2,1 at the head in that order => queue is 3,2,1
    deq_head_put(q, "1");
    deq_head_put(q, "2");
    deq_head_put(q, "3");
    test(deq_len(q) == 3, "Length after 3 head_put");

    // Check from head
    test(strcmp((char*)deq_head_ith(q, 0), "3") == 0, "head_ith(0) = 3");
    test(strcmp((char*)deq_head_ith(q, 1), "2") == 0, "head_ith(1) = 2");
    test(strcmp((char*)deq_head_ith(q, 2), "1") == 0, "head_ith(2) = 1");
    // test(deq_head_ith(q, 3) == NULL, "head_ith(3) out of range => NULL");

    // Check from tail
    test(strcmp((char*)deq_tail_ith(q, 0), "1") == 0, "tail_ith(0) = 1");
    test(strcmp((char*)deq_tail_ith(q, 1), "2") == 0, "tail_ith(1) = 2");
    test(strcmp((char*)deq_tail_ith(q, 2), "3") == 0, "tail_ith(2) = 3");
    // test(deq_tail_ith(q, 3) == NULL, "tail_ith(3) out of range => NULL");

    // Cleanup
    deq_del(q, NULL);
}

// Test 4: rem() from head or tail side
static void test_rem() {
    Deq q = deq_new();

    // Put A, B, C, D at the tail in that order => queue is A, B, C, D
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

// The main test runner
int main(void) {
    printf("=== Running Full-Coverage Test Suite for Deq ===\n\n");

    test_create_delete();
    test_put_get();
    test_ith();
    test_rem();

    printf("\n==========================\n");
    printf("Tests run   : %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    if(tests_passed == tests_run) {
        printf("All tests PASSED!\n");
    } else {
        printf("Some tests FAILED.\n");
    }
    return (tests_passed == tests_run) ? 0 : 1;
}
