/**
 * @file main.c
 * @brief A simple demonstration of the Deq (Double-Ended Queue) API.
 *
 * This file shows how to create a deque, insert items at both ends,
 * retrieve and remove items, and finally clean up. It uses the Deq
 * interface provided by deq.h.
 */

#include <stdio.h>
#include <stdlib.h>


#include "deq.h"

/**
 * @brief Helper function to print the contents and length of a Deq.
 *
 * @param q   The Deq to print.
 * @param msg A brief message describing the current state or action.
 */
static void print_deq(Deq q, const char *msg) {
    // Convert the entire Deq to a string (treating data as plain char*)
    char *s = deq_str(q, NULL);

    printf("%s\n", msg);
    printf("  Deq content: \"%s\"\n", s);
    printf("  Deq length : %d\n", deq_len(q));

    // deq_str allocates a new string; we must free it.
    free(s);
}

/**
 * @brief Main entry point. Demonstrates basic Deq usage.
 *
 * @return 0 on success
 */
int main() {
    printf("=== Simple Deq Demonstration ===\n\n");

    // 1. Create a new Deq
    Deq q = deq_new();
    print_deq(q, "Start with an empty queue:");

    // 2. Add a few items
    deq_head_put(q, "Head 1");   // put 'Head 1' at the head
    deq_head_put(q, "Head 2");   // then 'Head 2' at the head
    deq_tail_put(q, "Tail 1");   // put 'Tail 1' at the tail
    deq_tail_put(q, "Tail 2");   // put 'Tail 2' at the tail

    print_deq(q, 
        "After adding Head 2, Head 1 at head and Tail 1, Tail 2 at tail:");

    // 3. Get/remove items from each end
    char *got = (char*)deq_head_get(q);
    printf("deq_head_get -> %s\n", got);
    print_deq(q, "After removing one from the head:");

    got = (char*)deq_tail_get(q);
    printf("deq_tail_get -> %s\n", got);
    print_deq(q, "After removing one from the tail:");

    // 4. Insert more items
    deq_head_put(q, "NewHead");
    deq_tail_put(q, "NewTail");
    print_deq(q, "After adding NewHead at head and NewTail at tail:");

    // 5. Demonstrate ith() from the head side
    printf("head_ith(0) -> %s\n", (char*)deq_head_ith(q, 0));
    printf("head_ith(1) -> %s\n", (char*)deq_head_ith(q, 1));

    // 6. Remove an item by searching from the tail side
    char *r = (char*)deq_tail_rem(q, "NewHead");
    printf("deq_tail_rem(\"NewHead\") -> %s\n", r ? r : "(not found)");
    print_deq(q, "After removing \"NewHead\" from tail side:");

    // 7. Cleanup
    printf("\nDeleting the queue.\n");
    deq_del(q, NULL);
    printf("Done.\n");

    return 0;
}
