# include <stdlib.h>
# include <stdio.h>
# include "Heap.h"

void changeHeap(HEAP *heap, unsigned pos) {
	/* make it */
}

static void heapify(HEAP *heap, unsigned pos) {
	/* make it */
}

unsigned removeRoot(HEAP *heap) {
	unsigned root = heap->val[0];
	heap->val[0] = heap->val[-- heap->num];
	heapify(heap, 0);
	return root;
}

HEAP *newHeap(unsigned num) {
	HEAP *new;
	if ( (new = (HEAP *)malloc(sizeof(HEAP))) == NULL ) {
		perror("no more memory");
		exit(EXIT_FAILURE);
	}
	new->num = num;
	if ( (new->val = (unsigned *)calloc(num, sizeof(unsigned))) == NULL ) {
		perror("too large");
	}
	return new;
}

void freeHeap(HEAP *heap) {
	free(heap->val);
	free(heap);
}

