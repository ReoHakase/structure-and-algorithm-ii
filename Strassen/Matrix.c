# include <stdlib.h>
# include <stdio.h>
# include "Matrix.h"

static void errorMat(char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

MAT *newMat(unsigned sizeX, unsigned sizeY) {
	MAT *new;
	if ( (new = (MAT *) malloc(sizeof(MAT))) == NULL )
		errorMat("newMat: no more memory");
	new->x = sizeX;
	new->y = sizeY;
	if ( (new->v = (int *) calloc(sizeX*sizeY, sizeof(int))) == NULL  )
		errorMat("newMat: too large");
	return new;
}

void freeMat(MAT *mat) {
	free(mat->v);
	free(mat);
}

int getMat(MAT *mat, unsigned x, unsigned y) {
	if ( x >= mat->x ) errorMat("getMat: x is out of range");
	if ( y >= mat->y ) errorMat("getMat: y is out of range");
	return ( mat->v[x * mat->y + y]);
}

void setMat(MAT *mat, unsigned x, unsigned y, int val) {
	if ( x >= mat->x ) errorMat("setMat: x is out of range");
	if ( y >= mat->y ) errorMat("setMat: y is out of range");
	mat->v[x * mat->y + y] = val;

}

void printMat(MAT *mat) {
	int x, y;
	for ( x = 0; x < mat->x; x ++ ) {
		for ( y = 0; y < mat->y; y ++ ) printf(" %d", getMat(mat, x, y));
		putchar('\n');
	}
}
