# include <stdlib.h>
# include <stdio.h>
# include "Matrix.h"
# include "Strassen.h"

extern MAT data1, data2;

int main() {
	MAT *matA = &data1;
	MAT *matB = &data2;
	MAT *matC = newMat(matA->x, matB->y);
	printMat(matA);
	puts("multiplied by");
	printMat(matB);
	puts("equals");
	Strassen(matA, matB, matC);
	printMat(matC);
}
