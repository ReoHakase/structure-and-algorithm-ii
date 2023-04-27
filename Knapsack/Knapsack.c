# include <stdlib.h>
# include <stdio.h>
# include "Knapsack.h"

/* test data */
extern unsigned num;
extern OBJ object[];
extern float knapsack;

int main() {
	for ( unsigned i = 0; i < num; i ++ )
		object[i].performance = object[i].value / object[i].weight;
	sortObject(object, num);
	/* make it */
}

void sortObject(OBJ obj[], unsigned n) {
	/* make it! */
}
