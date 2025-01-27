#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "deq.h"



// On Windows (_WIN32 or _WIN64), there's no default asprintf.
// This fallback function will emulate asprintf.
#if defined(_WIN32) || defined(_WIN64)
int asprintf(char **strp, const char *fmt, ...) {
    va_list args;
    va_list copy;
    va_start(args, fmt);
    va_copy(copy, args);

    // Figure out how many bytes are needed (excluding null terminator)
    int needed = _vscprintf(fmt, copy);
    va_end(copy);

    if (needed < 0) {
        va_end(args);
        return -1; // Encoding error
    }

    // Allocate memory for the formatted string (+1 for '\0')
    *strp = (char *)malloc(needed + 1);
    if (!*strp) {
        va_end(args);
        return -1; // Allocation failed
    }

    // Print to the newly-allocated string
    int written = vsnprintf(*strp, needed + 1, fmt, args);
    va_end(args);

    if (written < 0) {
        // Something went wrong; free and return error
        free(*strp);
        *strp = NULL;
        return -1;
    }
    return written;
}
#endif

#include "deq.h"

// A small helper to print the queue and its length
static void print_deq(Deq q, const char *msg) {
    char *s = deq_str(q, NULL);  // Null means treat data as (char*)
    printf("%s\n", msg);
    printf("  Deq content: \"%s\"\n", s);
    printf("  Deq length : %d\n", deq_len(q));
    free(s);
}

int main() {
    printf("Creating new Deq...\n");
    Deq q = deq_new();
    print_deq(q, "Initially empty queue:");

    printf("\n-- Testing put() --\n");
    deq_head_put(q, "A");  // put 'A' at head
    deq_head_put(q, "B");  // put 'B' at head
    deq_tail_put(q, "C");  // put 'C' at tail
    deq_tail_put(q, "D");  // put 'D' at tail
    print_deq(q, "After putting B,A at head, and C,D at tail:");

    printf("\n-- Testing ith() --\n");
    // from head side: i=0 -> B, i=1 -> A, i=2 -> C, i=3 -> D
    // from tail side: i=0 -> D, i=1 -> C, i=2 -> A, i=3 -> B
    printf("  head_ith(0) = %s\n", (char*) deq_head_ith(q, 0));
    printf("  head_ith(2) = %s\n", (char*) deq_head_ith(q, 2));
    printf("  tail_ith(0) = %s\n", (char*) deq_tail_ith(q, 0));
    printf("  tail_ith(2) = %s\n", (char*) deq_tail_ith(q, 2));
    // out of range
    printf("  head_ith(99) = %p (expected 0)\n", deq_head_ith(q, 99));

    printf("\n-- Testing get() --\n");
    void *got = deq_head_get(q);
    printf("  deq_head_get -> %s\n", (char*) got);
    got = deq_tail_get(q);
    printf("  deq_tail_get -> %s\n", (char*) got);
    print_deq(q, "Queue after removing one from head & tail:");

    printf("\n-- Testing rem() --\n");
    // The queue now should contain A, C (in that order).
    // Remove by searching from head side
    char *toRemove = "A";
    char *remed = (char*) deq_head_rem(q, toRemove);
    printf("  deq_head_rem(\"A\") -> %s\n", remed ? remed : "(not found)");
    print_deq(q, "After removing \"A\":");

    // Put back a couple more for demonstration
    deq_tail_put(q, "Z");
    deq_head_put(q, "Y");
    // Now queue is: Y, C, Z
    print_deq(q, "After adding Y at head, Z at tail:");

    // Remove by searching from the tail side
    remed = (char*) deq_tail_rem(q, "Y");
    printf("  deq_tail_rem(\"Y\") -> %s\n", remed ? remed : "(not found)");
    remed = (char*) deq_tail_rem(q, "XYZ"); // Not in queue
    printf("  deq_tail_rem(\"XYZ\") -> %s\n", remed ? remed : "(not found)");
    print_deq(q, "After removing \"Y\" (from tail side):");

    printf("\n-- Testing get() on small queue --\n");
    // Now the queue might have just C, Z
    got = deq_head_get(q);
    printf("  deq_head_get -> %s\n", got ? (char*) got : "0");
    got = deq_head_get(q);
    printf("  deq_head_get -> %s\n", got ? (char*) got : "0");
    got = deq_head_get(q); // queue empty now
    printf("  deq_head_get on empty -> %p (expected 0)\n", got);

    print_deq(q, "Finally (should be empty):");

    // Always clean up
    printf("\n-- Calling deq_del(q, NULL) --\n");
    deq_del(q, NULL);
    // q is freed here; using q after this is invalid.

    printf("All tests complete.\n");
    return 0;
}
