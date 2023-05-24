typedef struct
{
  unsigned x; // 行数 rows
  unsigned y; // 列数 columns
  int *v;
} MAT;

MAT *newMat(unsigned sizeX, unsigned sizeY);
void freeMat(MAT *mat);

int getMat(MAT *mat, unsigned x, unsigned y);
void setMat(MAT *mat, unsigned x, unsigned y, int val);

void printMat(MAT *mat);

void negateMatrix(MAT *src, MAT *dst);
void addMatrix(MAT *matA, MAT *matB, MAT *matC);
void Strassen(MAT *matA, MAT *matB, MAT *matC);
