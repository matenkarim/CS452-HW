#include <stdio.h>
#include <stdlib.h>

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
    printf("=== Simple Deq Demonstration ===\n\n");

    // 1. Create a new Deq
    Deq q = deq_new();
    print_deq(q, "Start with an empty queue:");

    // 2. Add a few items
    deq_head_put(q, "Head 1");  // put 'A' at head
    deq_head_put(q, "Head 2");  // put 'B' at head
    deq_tail_put(q, "Tail 1");  // put 'C' at tail
    deq_tail_put(q, "Tail 2");  // put 'D' at tail

    print_deq(q, "After adding Head 2, Head 1 at head and Tail 1, Tail 2 at tail:");

    // 3. Get/remove items from each end
    char *got = (char*)deq_head_get(q);
    printf("deq_head_get -> %s\n", got);
    print_deq(q, "Afer removing one from the head:");

    got = (char*)deq_tail_get(q);
    printf("deq_tail_get -> %s\n", got);
    print_deq(q, "After removing one from tail:");

    // 4. Insert more items
    deq_head_put(q, "NewHead");
    deq_tail_put(q, "NewTail");
    print_deq(q, "After adding NewHead at head and NewTail at tail:");

    // 5. Demonstrate ith from head side
    printf("head_ith(0) -> %s\n", (char*)deq_head_ith(q, 0));
    printf("head_ith(1) -> %s\n", (char*)deq_head_ith(q, 1));

    // 6. Remove an item by searching from the tail side
    char *r = (char*)deq_tail_rem(q, "NewHead");
    printf("deq_tail_rem(\"NewHead\") -> %s\n", r ? r : "(not ofund)");
    print_deq(q, "After removing \"NewHead\" from tail side:");

    // 7. Cleanup
    printf("\nDeleting the queue.\n");
    deq_del(q, NULL);
    printf("Done.\n");

    return 0;

}
