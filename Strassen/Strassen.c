#include <stdlib.h>
#include <stdio.h>
#include "Matrix.h"

// This function copies a submatrix from one matrix into another matrix. It copies the submatrix of src starting at row srcTop and column srcLeft into the submatrix of dst starting at row dstTop and column dstLeft. It copies rows rows and columns columns.
// If the source matrix is too small, then just pad the destination matrix with zeros
// If the destionation matrix is too small, then truncate the source matrix
void copyMatrix(MAT *src, unsigned srcTop, unsigned srcLeft, MAT *dst, unsigned dstTop, unsigned dstLeft, unsigned rows, unsigned columns)
{
  // Copy elements from src to dst
  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < columns; j++)
    {
      // If the source element is out of range, then just pad the destination element with zero
      if (srcTop + i >= src->x || srcLeft + j >= src->y)
      {
        setMat(dst, dstTop + i, dstLeft + j, 0);
        continue;
      }

      // If the destionation matrix is too small, then truncate the source matrix
      if (dstTop + i >= dst->x || dstLeft + j >= dst->y)
      {
        continue;
      }

      setMat(dst, dstTop + i, dstLeft + j, getMat(src, srcTop + i, srcLeft + j));
    }
  }
}

//* Negate all elements of a matrix
void negateMatrix(MAT *src, MAT *dst)
{
  for (unsigned i = 0; i < src->x; i++)
  {
    for (unsigned j = 0; j < src->y; j++)
    {
      setMat(dst, i, j, -getMat(src, i, j));
    }
  }
}

//* Add two matrices (C = A + B)
void addMatrix(MAT *matA, MAT *matB, MAT *matC)
{
  for (unsigned i = 0; i < matA->x; i++)
  {
    for (unsigned j = 0; j < matA->y; j++)
    {
      setMat(matC, i, j, getMat(matA, i, j) + getMat(matB, i, j));
    }
  }
}

//* Calculate the product of two matrices (C = A * B) using Strassen's algorithm
void Strassen(MAT *matA, MAT *matB, MAT *matC)
{
  // Let m to be the number of rows of matrix A
  unsigned m = matA->x;
  // Let p to be the number of columns of matrix A and the number of rows of matrix B
  unsigned p = matA->y;
  if (p != matB->x)
  {
    printf("Error: The number of columns of matrix A must be equal to the number of rows of matrix B.\n");
    exit(1);
  }
  // Let n to be the number of columns of matrix B
  unsigned n = matB->y;

  /* stop recursive call */
  // Simply calculate the product of A and B if the sizes of A and B are small enough

  // If both matrix A and B are both 1x1 matrices, then the product of A and B is a 1x1 matrix
  if (m == 1 && p == 1 && n == 1)
  {
    printf("[Returning early] m = %d, p = %d, n = %d\n", m, p, n);
    setMat(matC, 0, 0, getMat(matA, 0, 0) * getMat(matB, 0, 0));
    return;
  }

  // If both matrix A and B are both 2x2 matrices, then the product of A and B is a 2x2 matrix
  if (m == 2 && p == 2 && n == 2)
  {
    printf("[Returning early] m = %d, p = %d, n = %d\n", m, p, n);
    setMat(matC, 0, 0, getMat(matA, 0, 0) * getMat(matB, 0, 0) + getMat(matA, 0, 1) * getMat(matB, 1, 0));
    setMat(matC, 0, 1, getMat(matA, 0, 0) * getMat(matB, 0, 1) + getMat(matA, 0, 1) * getMat(matB, 1, 1));
    setMat(matC, 1, 0, getMat(matA, 1, 0) * getMat(matB, 0, 0) + getMat(matA, 1, 1) * getMat(matB, 1, 0));
    setMat(matC, 1, 1, getMat(matA, 1, 0) * getMat(matB, 0, 1) + getMat(matA, 1, 1) * getMat(matB, 1, 1));
    return;
  }

  // If m = 1, then the product of A and B is a 1xp matrix
  if (m == 1)
  {
    printf("[Returning early] m = %d, p = %d, n = %d\n", m, p, n);
    for (unsigned j = 0; j < n; j++)
    {
      // C_{1, j} = \sum_{k = 1}^{p} A_{1, k} * B_{k, j}
      int sum = 0;
      for (unsigned k = 0; k < p; k++)
      {
        sum += getMat(matA, 0, k) * getMat(matB, k, j);
      }
      setMat(matC, 0, j, sum);
    }
    return;
  }

  // If n = 1, then the product of A and B is a mx1 matrix
  if (n == 1)
  {
    printf("[Returning early] m = %d, p = %d, n = %d\n", m, p, n);
    for (unsigned i = 0; i < m; i++)
    {
      // C_{i, 1} = \sum_{k = 1}^{p} A_{i, k} * B_{k, 1}
      int sum = 0;
      for (unsigned k = 0; k < p; k++)
      {
        sum += getMat(matA, i, k) * getMat(matB, k, 0);
      }
      setMat(matC, i, 0, sum);
    }
    return;
  }

  // If p = 1, then the product of A and B is a mxn matrix
  if (p == 1)
  {
    printf("[Returning early] m = %d, p = %d, n = %d\n", m, p, n);
    for (unsigned i = 0; i < m; i++)
    {
      for (unsigned j = 0; j < n; j++)
      {
        // C_{i, j} = A_{i, 1} * B_{1, j}
        setMat(matC, i, j, getMat(matA, i, 0) * getMat(matB, 0, j));
      }
    }
    return;
  }

  /* divide */
  /* allocate memory for temporal matrix */
  // Calculate the maximum dimension of the three matrices
  unsigned max = 2;
  if (m > max)
  {
    max = m;
  }
  if (p > max)
  {
    max = p;
  }
  if (n > max)
  {
    max = n;
  }

  // Calculate the dimension of the padded matrix
  // the dimension must be minimum one that satisfies the conditions of being power of 2 and being greater or equal to the max
  unsigned dim = 2;
  while (dim < max)
  {
    dim *= 2;
  }

  printf("[Dimension sizes] m = %d, p = %d, n = %d, dim = %d\n", m, p, n, dim);

  unsigned halfDim = dim / 2;

  // Calculate the size of the submatrices
  MAT *matA11 = newMat(halfDim, halfDim);
  MAT *matA12 = newMat(halfDim, halfDim);
  MAT *matA21 = newMat(halfDim, halfDim);
  MAT *matA22 = newMat(halfDim, halfDim);

  MAT *matB11 = newMat(halfDim, halfDim);
  MAT *matB12 = newMat(halfDim, halfDim);
  MAT *matB21 = newMat(halfDim, halfDim);
  MAT *matB22 = newMat(halfDim, halfDim);

  MAT *matP1 = newMat(halfDim, halfDim);
  MAT *matP2 = newMat(halfDim, halfDim);
  MAT *matP3 = newMat(halfDim, halfDim);
  MAT *matP4 = newMat(halfDim, halfDim);
  MAT *matP5 = newMat(halfDim, halfDim);
  MAT *matP6 = newMat(halfDim, halfDim);
  MAT *matP7 = newMat(halfDim, halfDim);

  MAT *matC11 = newMat(halfDim, halfDim);
  MAT *matC12 = newMat(halfDim, halfDim);
  MAT *matC21 = newMat(halfDim, halfDim);
  MAT *matC22 = newMat(halfDim, halfDim);

  // Populate the submatrices
  copyMatrix(matA, 0, 0, matA11, 0, 0, halfDim, halfDim);
  copyMatrix(matA, 0, halfDim, matA12, 0, 0, halfDim, halfDim);
  copyMatrix(matA, halfDim, 0, matA21, 0, 0, halfDim, halfDim);
  copyMatrix(matA, halfDim, halfDim, matA22, 0, 0, halfDim, halfDim);

  copyMatrix(matB, 0, 0, matB11, 0, 0, halfDim, halfDim);
  copyMatrix(matB, 0, halfDim, matB12, 0, 0, halfDim, halfDim);
  copyMatrix(matB, halfDim, 0, matB21, 0, 0, halfDim, halfDim);
  copyMatrix(matB, halfDim, halfDim, matB22, 0, 0, halfDim, halfDim);

  /* conquer */
  // Calculate the submatrices

  // Calculate P1 = (A11 + A22) * (B11 + B22)
  MAT *matP1Intm1 = newMat(halfDim, halfDim);
  MAT *matP1Intm2 = newMat(halfDim, halfDim);
  addMatrix(matA11, matA22, matP1Intm1);
  addMatrix(matB11, matB22, matP1Intm2);
  Strassen(matP1Intm1, matP1Intm2, matP1);
  free(matP1Intm1);
  free(matP1Intm2);

  // Calculate P2 = (A21 + A22) * B11
  MAT *matP2Intm1 = newMat(halfDim, halfDim);
  addMatrix(matA21, matA22, matP2Intm1);
  Strassen(matP2Intm1, matB11, matP2);
  free(matP2Intm1);

  // Calculate P3 = A11 * (B12 - B22)
  MAT *matP3Intm1 = newMat(halfDim, halfDim);
  MAT *matP3Intm2 = newMat(halfDim, halfDim);
  negateMatrix(matB22, matP3Intm1);
  addMatrix(matB12, matP3Intm1, matP3Intm2);
  Strassen(matA11, matP3Intm2, matP3);
  free(matP3Intm1);
  free(matP3Intm2);

  // Calculate P4 = A22 * (B21 - B11)
  MAT *matP4Intm1 = newMat(halfDim, halfDim);
  MAT *matP4Intm2 = newMat(halfDim, halfDim);
  negateMatrix(matB11, matP4Intm1);
  addMatrix(matB21, matP4Intm1, matP4Intm2);
  Strassen(matA22, matP4Intm2, matP4);
  free(matP4Intm1);
  free(matP4Intm2);

  // Calculate P5 = (A11 + A12) * B22
  MAT *matP5Intm1 = newMat(halfDim, halfDim);
  addMatrix(matA11, matA12, matP5Intm1);
  Strassen(matP5Intm1, matB22, matP5);
  free(matP5Intm1);

  // Calculate P6 = (A21 - A11) * (B11 + B12)
  MAT *matP6Intm1 = newMat(halfDim, halfDim);
  MAT *matP6Intm2 = newMat(halfDim, halfDim);
  MAT *matP6Intm3 = newMat(halfDim, halfDim);
  negateMatrix(matA11, matP6Intm1);
  addMatrix(matA21, matP6Intm1, matP6Intm2);
  addMatrix(matB11, matB12, matP6Intm3);
  Strassen(matP6Intm2, matP6Intm3, matP6);
  free(matP6Intm1);
  free(matP6Intm2);
  free(matP6Intm3);

  // Calculate P7 = (A12 - A22) * (B21 + B22)
  MAT *matP7Intm1 = newMat(halfDim, halfDim);
  MAT *matP7Intm2 = newMat(halfDim, halfDim);
  MAT *matP7Intm3 = newMat(halfDim, halfDim);
  negateMatrix(matA22, matP7Intm1);
  addMatrix(matA12, matP7Intm1, matP7Intm2);
  addMatrix(matB21, matB22, matP7Intm3);
  Strassen(matP7Intm2, matP7Intm3, matP7);

  /* combine */
  // Calculate C11 = P1 + P4 - P5 + P7
  MAT *matC11IntmNegatedP5 = newMat(halfDim, halfDim);
  negateMatrix(matP5, matC11IntmNegatedP5);
  addMatrix(matP1, matP4, matC11);
  addMatrix(matC11, matC11IntmNegatedP5, matC11);
  addMatrix(matC11, matP7, matC11);
  free(matC11IntmNegatedP5);

  // Calculate C12 = P3 + P5
  addMatrix(matP3, matP5, matC12);

  // Calculate C21 = P2 + P4
  addMatrix(matP2, matP4, matC21);

  // Calculate C22 = P1 - P2 + P3 + P6
  MAT *matC22IntmNegatedP2 = newMat(halfDim, halfDim);
  negateMatrix(matP2, matC22IntmNegatedP2);
  addMatrix(matP1, matC22IntmNegatedP2, matC22);
  addMatrix(matC22, matP3, matC22);
  addMatrix(matC22, matP6, matC22);
  free(matC22IntmNegatedP2);

  // Copy the submatrices back to the original matrix
  copyMatrix(matC11, 0, 0, matC, 0, 0, halfDim, halfDim);
  copyMatrix(matC12, 0, 0, matC, 0, halfDim, halfDim, halfDim);
  copyMatrix(matC21, 0, 0, matC, halfDim, 0, halfDim, halfDim);
  copyMatrix(matC22, 0, 0, matC, halfDim, halfDim, halfDim, halfDim);

  /* release memory for temporal matrix */
  free(matA11);
  free(matA12);
  free(matA21);
  free(matA22);

  free(matB11);
  free(matB12);
  free(matB21);
  free(matB22);

  free(matP1);
  free(matP2);
  free(matP3);
  free(matP4);
  free(matP5);
  free(matP6);
  free(matP7);

  free(matC11);
  free(matC12);
  free(matC21);
  free(matC22);
}
