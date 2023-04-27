typedef struct  {
	unsigned x;
	unsigned y;
	int *v;
} MAT;

MAT *newMat(unsigned sizeX, unsigned sizeY);
void freeMat(MAT *mat);

int getMat(MAT *mat, unsigned x,  unsigned y);
void setMat(MAT *mat, unsigned x, unsigned y, int val);

void printMat(MAT *mat);
