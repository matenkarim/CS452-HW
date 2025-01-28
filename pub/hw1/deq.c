/**
 * @file deq.c
 * @brief Implementation of a double-ended queue (deque).
 *
 * This file implements a double-ended queue (deque) using a doubly linked list. 
 * It provides efficient insertion, deletion, and traversal operations at both ends.
 *
 * Features include:
 * - Insertion and deletion at both head and tail.
 * - Accessing and removing elements by index or value.
 * - Iteration and string representation support.
 *
 * @author Jim Buffenbarger
 * @author Maten Karim
 * @date 27 Jan 2025
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];        // next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

/**
 * @brief Inserts a new node with the given data at the specified end of the deque.
 *
 * This function creates a new node with the provided data and inserts it either
 * at the head or the tail of the deque, depending on the specified end. If the
 * deque is empty, the new node becomes both the head and the tail.
 *
 * @param r Pointer to the deque representation.
 * @param e The end where the new node should be inserted (Head or Tail).
 * @param d The data to be stored in the new node.
 *
 * @note If the deque representation pointer is NULL, the function returns immediately.
 * @note If memory allocation for the new node fails, an error is reported.
 */
static void put(Rep r, End e, Data d) {
  // Sanity check
  if (!r) return; // Should be caught by rep(q) but just in case

  // Create a new node
  Node n = (Node) malloc(sizeof(*n));
  if (!n) ERROR("malloc() failed in put()");
  n->data = d;
  n->np[Head] = NULL;
  n->np[Tail] = NULL;

  // If the list is empty, set the head and tail to the new node
  if (r->len == 0) {
    r->ht[Head] = n;
    r->ht[Tail] = n;
    r->len = 1;
    return;
  }

  if (e == Head) {
    // Insert at the head
    Node oldHead = r->ht[Head];
    n->np[Tail] = oldHead;      // New node next -> old head
    oldHead->np[Head] = n;      // Old head prev -> new node
    r->ht[Head] = n;            // Update rep's head pointer
  } else {
    // Insert at the tail
    Node oldTail = r->ht[Tail];
    n->np[Head] = oldTail;      // New node prev -> old tail
    oldTail->np[Tail] = n;      // Old tail next -> new node
    r->ht[Tail] = n;            // Update rep's tail pointer
  }

  r->len++;
}

/**
 * ith - Retrieve the data at the i-th position from the specified end of the deque.
 * 
 * @param r: The representation of the deque.
 * @param e: The end from which to start (Head or Tail).
 * @param i: The index of the element to retrieve.
 * 
 * @return The data at the i-th position, or 0 if the deque is empty or the index is out of bounds.
 * 
 * This function traverses the deque starting from the specified end (Head or Tail) and moves
 * towards the other end until it reaches the i-th position. If the index is out of bounds,
 * an error is raised.
 */
static Data ith(Rep r, End e, int i)  { 
  // Ensure the deque representation is valid
  if (!r) return 0;

  // Check if the index is within bounds
  if (i < 0 || i >= r->len) ERROR("Index out of bounds!");

  // Start traversal from the specified end (Head or Tail)
  Node n = (e == Head) ? r->ht[Head] : r->ht[Tail];

  // Traverse the deque to the i-th position
  while (i > 0) {
    n = (e == Head) ? n->np[Tail] : n->np[Head];
    i--;
  }

  // Return the data at the i-th position, or 0 if the node is null
  return (n ? n->data : 0);
}

/**
 * @brief Retrieves and removes an element from the specified end of the deque.
 *
 * This function removes an element from either the head or the tail of the deque,
 * depending on the specified end, and returns the data stored in that element.
 *
 * @param r A pointer to the deque representation (Rep) structure.
 * @param e An enum value indicating which end to remove the element from (Head or Tail).
 * @return The data stored in the removed element, or 0 if the deque is empty or the representation is invalid.
 */
static Data get(Rep r, End e)         { 
  if (!r || r->len ==0) {
    // empty queue or invalid rep
    return 0;
  }

  Node toRemove = (e == Head) ? r->ht[Head] : r->ht[Tail];
  Data d = toRemove->data;

  if (r->len == 1) {
    // Only one node in the list
    r->ht[Head] = NULL;
    r->ht[Tail] = NULL;
  } else if (e == Head) {
    // Remove the head
    Node newHead = toRemove->np[Tail];
    newHead->np[Head] = NULL;
    r->ht[Head] = newHead;
  } else {
    // Remove the tail
    Node newTail = toRemove->np[Head];
    newTail->np[Tail] = NULL;
    r->ht[Tail] = newTail;
  }

  free(toRemove);
  r->len--;
  return d;

}

/**
 * @brief Removes a node with the specified data from the deque.
 *
 * This function searches for a node containing the specified data in the deque
 * and removes it. The search starts from the end specified by the parameter `e`.
 * If the node is found and removed, the function returns the data of the removed node.
 * If the node is not found, the function returns 0.
 *
 * @param r A pointer to the deque representation.
 * @param e The end from which to start the search (Head or Tail).
 * @param d The data to search for and remove.
 * @return The data of the removed node if found, otherwise 0.
 */

static Data rem(Rep r, End e, Data d) { 
  if (!r || r->len == 0) return 0;

  // Start from whichever end is specified
  Node n = (e == Head) ? r->ht[Head] : r->ht[Tail];
  while (n) {
    if (n->data == d) {
      // Found the node to remove
      if (r->len == 1) {
        // Only one node in the list
        r->ht[Head] = NULL;
        r->ht[Tail] = NULL;
      } else if (n == r->ht[Head]) {
        // Removing the head
        Node newHead = n->np[Tail];
        newHead->np[Head] = NULL;
        r->ht[Head] = newHead;
      } else if (n == r->ht[Tail]) {
        // Removing the tail
        Node newTail = n->np[Head];
        newTail->np[Tail] = NULL;
        r->ht[Tail] = newTail;
      } else {
        // Removing from the middle
        Node prev = n->np[Head];
        Node next = n->np[Tail];
        prev->np[Tail] = next;
        next->np[Head] = prev;
      }

      Data out = n->data;
      free(n);
      r->len--;
      return out;
    }

    // Go to next or prev depending on 'End'
    n = (e == Head) ? n->np[Tail] : n->np[Head];
  }

  return 0; // Not found
}

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head);   }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail);   }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}
