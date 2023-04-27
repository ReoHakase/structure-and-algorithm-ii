#include <stdlib.h>
#include <stdio.h>
#include "MSP.h"

/* input data */
extern unsigned num;
extern int data[];

int main()
{
  printf("%d\n", maxSubarray(0, num));
}

int maxSubarray(unsigned from, unsigned num)
{
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
  unsigned leftNum = num / 2;
  unsigned rightNum = num - leftNum;
  int leftMax = maxSubarray(from, leftNum);
  int rightMax = maxSubarray(from + leftNum, rightNum);

  /* conquer */
  // Find the maximum subarray that crosses the midpoint
  // Since returning the indice is not required, we can just return the sum
  // of the left and right maximum subarrays

  // Find the maximum sum of subarray that ends at the end of the left half
  int leftMaxSum = 0;
  {
    int leftSum = 0;
    for (unsigned i = from + leftNum - 1; i >= from; i--)
    {
      leftSum += data[i];
      if (leftSum > leftMaxSum)
      {
        leftMaxSum = leftSum;
      }
    }
  }

  // Find the maximum sum of subarray that starts at the beginning of the right half
  int rightMaxSum = 0;
  {
    int rightSum = 0;
    for (unsigned i = from + leftNum; i < from + num; i++)
    {
      rightSum += data[i];
      if (rightSum > rightMaxSum)
      {
        rightMaxSum = rightSum;
      }
    }
  }

  // Combine the two maximum sums
  int midMax = leftMaxSum + rightMaxSum;

  /* combine */
  // Return the maximum of the three maximums
  int max = leftMax;
  if (rightMax > max)
  {
    max = rightMax;
  }
  if (midMax > max)
  {
    max = midMax;
  }
  return max;
}
