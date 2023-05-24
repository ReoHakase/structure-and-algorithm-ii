#include <stdlib.h>
#include <stdio.h>
#include "Heap.h"

// Revise the heap after the value of the node is changed.
// Call itself recursively until the parent node is smaller than the child.
// O(logn) at worst
void changeHeap(HEAP *heap, unsigned pos, unsigned *dist)
{
  /* make it */
  // If the given node is root, do nothing
  if (pos == 0)
  {
    return;
  }

  // Calculate the position of the parent node
  unsigned posParent = (pos - 1) / 2;

  // If the parent node does exist, then check if the parent is smaller than the child.
  if (posParent < heap->num && posParent != pos)
  {
    // If current values do not satisfy the condition, then swap the parent and the child.
    if (dist[heap->val[posParent]] > dist[heap->val[pos]])
    {
      // Swap the parent and the child
      unsigned temp = heap->val[posParent];
      heap->val[posParent] = heap->val[pos];
      heap->val[pos] = temp;

      // Call changeHeap recursively against the parent node
      changeHeap(heap, posParent, dist);
    }
  }
}

// Update the specified node and its children to satisfy the heap condition.
// The value of the parent node should be smaller than the children.
// O(logn) at worst
static void heapify(HEAP *heap, unsigned pos, unsigned *dist)
{
  /* make it */
  // Calculate the positions of left child and right child
  unsigned posLeftChild = pos * 2 + 1;
  unsigned posRightChild = pos * 2 + 2;

  // A value of parent node should be smaller or equal to children

  // If the left child does exist, then check if the parent is smaller than the left child.
  if (posLeftChild < heap->num)
  {
    // If current values do not satisfy the condition, then swap the parent and the child.
    if (dist[heap->val[pos]] > dist[heap->val[posLeftChild]])
    {
      // Swap the left child and the parent
      unsigned temp = heap->val[pos];
      heap->val[pos] = heap->val[posLeftChild];
      heap->val[posLeftChild] = temp;

      // Call heapify recursively against the left child
      heapify(heap, posLeftChild, dist);
    }
  }

  // If the right child does exist, then check if the parent is smaller than the right child.
  if (posRightChild < heap->num)
  {
    // If current values do not satisfy the condition, then swap the parent and the child.
    if (dist[heap->val[pos]] > dist[heap->val[posRightChild]])
    {
      // Swap the right child and the parent
      unsigned temp = heap->val[pos];
      heap->val[pos] = heap->val[posRightChild];
      heap->val[posRightChild] = temp;

      // Call heapify recursively against the right child
      heapify(heap, posRightChild, dist);
    }
  }
}

// Remove the root node and return its value.
unsigned removeRoot(HEAP *heap, unsigned *dist)
{
  unsigned root = heap->val[0];
  heap->val[0] = heap->val[--heap->num];
  heapify(heap, 0, dist);
  return root;
}

HEAP *newHeap(unsigned num)
{
  HEAP *new;
  if ((new = (HEAP *)malloc(sizeof(HEAP))) == NULL)
  {
    perror("no more memory");
    exit(EXIT_FAILURE);
  }
  new->num = num;
  if ((new->val = (unsigned *)calloc(num, sizeof(unsigned))) == NULL)
  {
    perror("too large");
  }
  return new;
}

void freeHeap(HEAP *heap)
{
  free(heap->val);
  free(heap);
}
