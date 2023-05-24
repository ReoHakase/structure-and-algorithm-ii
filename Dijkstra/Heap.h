#include <stdlib.h>
#include <stdio.h>

typedef struct
{
  unsigned num;
  unsigned *val;
} HEAP;

HEAP *newHeap(unsigned num);
void freeHeap(HEAP *heap);

unsigned removeRoot(HEAP *heap, unsigned *dist);
void changeHeap(HEAP *heap, unsigned pos, unsigned *dist);
static void heapify(HEAP *heap, unsigned pos, unsigned *dist);
