#include <stdlib.h>
#include <stdio.h>
#include "MSP.h"
#define INT_MIN -2147483648

/* input data */
extern unsigned num;
extern int data[];

int main()
{
  printf("%d\n", maxSubarray(0, num));
}

int maxSubarray(unsigned from, unsigned num)
{
  printf("Fn called >> from: %d, num: %d\n", from, num);
  /* stop recursive call */
  // Stop once there is no more data to divide
  if (num == 1)
  {
    return data[from];
  }

  if (num == 2)
  {
    int max = data[from];
    if (data[from + 1] > max)
    {
      max = data[from + 1];
    }
    if (data[from] + data[from + 1] > max)
    {
      max = data[from] + data[from + 1];
    }
    return max;
  }

  /* divide */
  // Divide the data into two halves
  int leftSubArrayBeginning = (int)from;
  int leftSubArrayLength = (int)num / 2;
  int leftSubArrayEnd = leftSubArrayBeginning + (leftSubArrayLength - 1);
  int rightSubArrayBeginning = leftSubArrayBeginning + leftSubArrayLength;
  int rightSubArrayLength = (int)num - leftSubArrayLength;
  int rightSubArrayEnd = rightSubArrayBeginning + (rightSubArrayLength - 1);
  printf("Calculating leftMax\n");
  int leftOnlyMax = maxSubarray(leftSubArrayBeginning, leftSubArrayLength);
  printf("Calculating rightMax\n");
  int rightOnlyMax = maxSubarray(rightSubArrayBeginning, rightSubArrayLength);

  /* conquer */
  // Find the maximum subarray that crosses the midpoint
  // Since returning the indice is not required, we can just return the sum
  // of the left and right maximum subarrays
  printf("Calc mid >> from: %d, num: %d\n", from, num);

  // Find the maximum sum of subarray that ends at the end of the left half
  printf("Calc leftCrossingMax >> start: %d, end: %d\n", leftSubArrayEnd, leftSubArrayBeginning);
  int leftCrossingMax = INT_MIN;
  {
    int sum = 0;
    for (int i = leftSubArrayEnd; i >= leftSubArrayBeginning; i--)
    {
      printf("i: %d\n", i);
      sum += data[i];
      if (sum > leftCrossingMax)
      {
        leftCrossingMax = sum;
      }
    }
  }

  // Find the maximum sum of subarray that starts at the beginning of the right half
  printf("Calc rightCrossingMax >> start: %d, end: %d\n", rightSubArrayBeginning, rightSubArrayEnd);
  int rightCrossingMax = INT_MIN;
  {
    int sum = 0;
    for (int i = rightSubArrayBeginning; i <= rightSubArrayEnd; i++)
    {
      printf("i: %d\n", i);
      sum += data[i];
      if (sum > rightCrossingMax)
      {
        rightCrossingMax = sum;
      }
    }
  }

  // Combine the two maximum sums
  int crossingMax = leftCrossingMax + rightCrossingMax;

  /* combine */
  // Return the maximum of the three maximums
  int max = leftOnlyMax;
  if (rightOnlyMax > max)
  {
    max = rightOnlyMax;
  }
  if (crossingMax > max)
  {
    max = crossingMax;
  }
  return max;
}
