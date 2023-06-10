#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Convex.h"

/* input data */
extern PSET data;

int main()
{
  /* make it */
  printPset(&data);
  printf("Sorting points...\n");
  sortPoints(data.point, data.num);
  printPset(&data);
  ConvexHull *convexHull = getConvexHull(&data, Clockwise, Largest);
  // Generate a convex hull that has the same points as the input data
  // ConvexHull *convexHull = allocateConvexHull(data.num, Clockwise, Smallest);
  printConvexHull(convexHull);
  return 0;
};

/* other functions */

// Calculate the angle of the vector src -> target using arctan
// Returns the angle in radians in the range [-pi, pi]
float getAngle(POINT *src, POINT *target)
{
  float angle = atan2(target->y - src->y, target->x - src->x);
  return angle;
}

// Calculate the difference between two angles as angle2 - angle1.
// Both angle1 and angle2 must be in the range [-pi, pi].
// Returns the difference in radians in the range [-pi, pi].
float getAngleDifference(float angle1, float angle2)
{
  // Calculate the difference between two angles as angle2 - angle1.
  // Both angle1 and angle2 must be in the range [-pi, pi].
  // Returns the difference in radians in the range [-pi, pi].
  float difference = angle2 - angle1;
  if (difference > M_PI)
  {
    difference -= 2 * M_PI;
  }
  else if (difference < -M_PI)
  {
    difference += 2 * M_PI;
  }
  return difference;
}

// Calculate the center point of the triangle with points p1, p2, p3.
POINT *getTriangleCenterPoint(POINT *p1, POINT *p2, POINT *p3)
{
  POINT *centerPoint = (POINT *)malloc(sizeof(POINT));
  if (centerPoint == NULL)
  {
    printf("Error: Failed to allocate memory for centerPoint\n");
    exit(1);
  }
  centerPoint->x = (p1->x + p2->x + p3->x) / 3;
  centerPoint->y = (p1->y + p2->y + p3->y) / 3;
  return centerPoint;
}

// Calculate a rotation direction of the three points, [0] -> [1] -> [2] seen from the center point of three.
// If the rotation is counter-clockwise, return CounterClockwise.
// If the rotation is clockwise, return Clockwise.
// Otherwise, return Neither.
RotateDirection getRotateDirection(POINT *p1, POINT *p2, POINT *p3)
{
  // If there is a pair of the same exact points, return Neither
  if (p1->x == p2->x && p1->y == p2->y)
  {
    return Neither;
  }
  if (p2->x == p3->x && p2->y == p3->y)
  {
    return Neither;
  }
  if (p3->x == p1->x && p3->y == p1->y)
  {
    return Neither;
  }

  POINT *src = getTriangleCenterPoint(p1, p2, p3);

  float angle1 = getAngle(src, p1);
  float angle2 = getAngle(src, p2);
  float angle3 = getAngle(src, p3);

  float angleDifference1to2 = getAngleDifference(angle1, angle2);
  float angleDifference2to3 = getAngleDifference(angle2, angle3);

  if (angleDifference1to2 > 0 && angleDifference2to3 > 0)
  {
    return CounterClockwise;
  }
  else if (angleDifference1to2 < 0 && angleDifference2to3 < 0)
  {
    return Clockwise;
  }
  else
  {
    return Neither;
  }
}

// Adjust the beggining point of the convex hull
ConvexHull *adjustConvexHullBegginingPoint(ConvexHull *convexHull, BegginingPoint begginingPoint)
{
  // Allocate memory for new convex hull
  ConvexHull *adjustedConvexHull = allocateConvexHull(convexHull->num, convexHull->rotateDirection, begginingPoint);
  if (adjustedConvexHull == NULL)
  {
    printf("Error: Failed to allocate memory for adjustedConvexHull\n");
    exit(1);
  }

  // Find the x-coord that new beggining point should have
  // If the beggining point is Smallest, find the smallest x-coord
  // If the beggining point is Largest, find the largest x-coord
  float extremumXCoord = convexHull->point[0].x;
  int extremumXCoordIndex = 0;
  for (int i = 1; i < convexHull->num; i++)
  {
    if (begginingPoint == Smallest)
    {
      if (convexHull->point[i].x < extremumXCoord)
      {
        extremumXCoord = convexHull->point[i].x;
        extremumXCoordIndex = i;
      }
    }
    if (begginingPoint == Largest)
    {
      if (convexHull->point[i].x > extremumXCoord)
      {
        extremumXCoord = convexHull->point[i].x;
        extremumXCoordIndex = i;
      }
    }
  }

  // Copy the convex hull to the reordered array
  for (int i = 0; i < convexHull->num; i++)
  {
    adjustedConvexHull->point[i] = *getCovexHullPoint(convexHull, extremumXCoordIndex + i);
  }

  return adjustedConvexHull;
}

// Return the convex hull of the given point set.
// Note that the input pset is assumed to be sorted in ascending order of x-coordinate.
// If rotateDirection is Clockwise, return the convex hull in clockwise order.
// If rotateDirection is CounterClockwise, return the convex hull in counter-clockwise order.
// If begginingPoint is Smallest, return the convex hull starting from the point with the smallest x-coordinate.
// If begginingPoint is Largest, return the convex hull starting from the point with the largest x-coordinate.
ConvexHull *getConvexHull(const PSET *pset, RotateDirection rotateDirection, BegginingPoint begginingPoint)
{
  unsigned n = pset->num;

  // Log debug information
  printf("\n[getConvexHull] n = %u, rotateDirection = %s, begginingPoint = %s\n", n, rotateDirection == CounterClockwise ? "CounterClockwise" : "Clockwise", begginingPoint == Smallest ? "Smallest" : "Largest");
  for (int i = 0; i < n; i++)
  {
    printf("(%.2f, %.2f), ", pset->point[i].x, pset->point[i].y);
  }
  printf("\n");

  if (n <= 0)
  {
    printf("Error: The number of points is less than or equal to 0.\n");
    exit(1);
  }

  // If the number of points is less or equal to 3, calculate the convex hull by brute force
  if (n == 1)
  {
    ConvexHull *convexHull = allocateConvexHull(1, rotateDirection, begginingPoint);
    convexHull->point[0] = pset->point[0];
    printConvexHull(convexHull);
    return convexHull;
  }

  if (n == 2)
  {
    ConvexHull *convexHull = allocateConvexHull(2, rotateDirection, begginingPoint);
    switch (begginingPoint)
    {
    case Smallest:
      convexHull->point[0] = pset->point[0];
      convexHull->point[1] = pset->point[1];
      break;
    case Largest:
      convexHull->point[0] = pset->point[1];
      convexHull->point[1] = pset->point[0];
      break;
    }
    printConvexHull(convexHull);
    return convexHull;
  }

  if (n == 3)
  {
    ConvexHull *clockwiseConvexHull = allocateConvexHull(3, Clockwise, begginingPoint);

    // Set the begginning point of the convex hull and initial scaffolding of the convex hull
    // Suppose both rotateDirection and rawRotateDirection are CounterClockwise
    if (begginingPoint == Smallest)
    {

      // Calculate the rotation direction of the three points
      RotateDirection rawRotateDirection = getRotateDirection(&pset->point[0], &pset->point[1], &pset->point[2]);

      if (rawRotateDirection == Clockwise)
      {
        // If 0 -> 1 -> 2 is clockwise
        clockwiseConvexHull->point[0] = pset->point[0];
        clockwiseConvexHull->point[1] = pset->point[1];
        clockwiseConvexHull->point[2] = pset->point[2];
      }
      else
      {
        // If 0 -> 1 -> 2 is counter-clockwise
        clockwiseConvexHull->point[0] = pset->point[0];
        clockwiseConvexHull->point[1] = pset->point[2];
        clockwiseConvexHull->point[2] = pset->point[1];
      }
    }
    else if (begginingPoint == Largest)
    {
      // Calculate the rotation direction of the three points
      RotateDirection rawRotateDirection = getRotateDirection(&pset->point[2], &pset->point[1], &pset->point[0]);

      if (rawRotateDirection == Clockwise)
      {
        // If 2 -> 1 -> 0 is clockwise
        clockwiseConvexHull->point[0] = pset->point[2];
        clockwiseConvexHull->point[1] = pset->point[1];
        clockwiseConvexHull->point[2] = pset->point[0];
      }
      else
      {
        // If 2 -> 1 -> 0 is counter-clockwise
        clockwiseConvexHull->point[0] = pset->point[2];
        clockwiseConvexHull->point[1] = pset->point[0];
        clockwiseConvexHull->point[2] = pset->point[1];
      }
    }

    // Now, clockwiseConvexHull is in clockwise order, beggining from the certain extremum point.
    // Reorder the convex hull in the given rotation direction
    ConvexHull *convexHull = changeConvexHullRotateDirection(clockwiseConvexHull, rotateDirection);
    freeConvexHull(clockwiseConvexHull);

    printConvexHull(convexHull);

    return convexHull;
  }

  // Now, the number of points is greater than 3
  // Divide the point set into two subsets, left and right
  unsigned leftNum = n / 2;
  unsigned rightNum = n - leftNum;

  PSET *leftSubPset = (PSET *)malloc(sizeof(PSET));
  if (leftSubPset == NULL)
  {
    printf("Error: Failed to allocate memory for leftSubPset\n");
    exit(1);
  }
  leftSubPset->num = leftNum;
  leftSubPset->point = &pset->point[0];

  PSET *rightSubPset = (PSET *)malloc(sizeof(PSET));
  if (rightSubPset == NULL)
  {
    printf("Error: Failed to allocate memory for rightSubPset\n");
    exit(1);
  }
  rightSubPset->num = rightNum;
  rightSubPset->point = &pset->point[leftNum];

  // Recursively calculate the convex hull of the left subset and the right subset
  ConvexHull *leftConvexHull = getConvexHull(leftSubPset, CounterClockwise, Largest);
  ConvexHull *rightConvexHull = getConvexHull(rightSubPset, Clockwise, Smallest);

  // Merge the convex hulls of the left subset and the right subset
  ConvexHull *mergedConvexHull = mergeConvexHulls(leftConvexHull, rightConvexHull);
  freeConvexHull(leftConvexHull);
  freeConvexHull(rightConvexHull);

  // Adjust the beggining point of the convex hull
  ConvexHull *adjustedMergedConvexHull = adjustConvexHullBegginingPoint(mergedConvexHull, begginingPoint);
  freeConvexHull(mergedConvexHull);

  // Reorder the convex hull in the given rotation direction
  ConvexHull *resultConvexHull = changeConvexHullRotateDirection(adjustedMergedConvexHull, rotateDirection);
  freeConvexHull(adjustedMergedConvexHull);

  printf("Merged convex hull with %d points:", resultConvexHull->num);
  printConvexHull(resultConvexHull);
  return resultConvexHull;
}

// Allocate memory for a convex hull
ConvexHull *allocateConvexHull(unsigned n, RotateDirection rotateDirection, BegginingPoint begginingPoint)
{
  ConvexHull *convexHull = (ConvexHull *)malloc(sizeof(ConvexHull));
  if (convexHull == NULL)
  {
    printf("Error: Failed to allocate memory for ConvexHull\n");
    exit(1);
  }
  POINT *points = (POINT *)malloc(sizeof(POINT) * n);
  if (points == NULL)
  {
    printf("Error: Failed to allocate memory for POINT array\n");
    exit(1);
  }
  convexHull->num = n;
  convexHull->point = points;
  convexHull->rotateDirection = rotateDirection;
  convexHull->begginingPoint = begginingPoint;
  return convexHull;
}

// Free memory for a convex hull
void freeConvexHull(ConvexHull *convexHull)
{
  free(convexHull->point);
  free(convexHull);
}

// Print a convex hull
void printConvexHull(ConvexHull *convexHull)
{
  printf("\nPrint ConvexHull with %u points, in %s order, beggining with a point that have %s x-coord.:\n", convexHull->num, convexHull->rotateDirection == CounterClockwise ? "CounterClockwise" : "Clockwise", convexHull->begginingPoint == Smallest ? "smallest" : "largest");
  for (int i = 0; i < convexHull->num; i++)
  {
    printf("[%u] (%.2f, %.2f)\n", i, convexHull->point[i].x, convexHull->point[i].y);
  }
  printf("\n");
}

// Change the angle direction of a convex hull
// The begginning point of the convex hull stays the same
ConvexHull *changeConvexHullRotateDirection(const ConvexHull *convexHull, RotateDirection targetRotateDirection)
{
  ConvexHull *newConvexHull = allocateConvexHull(convexHull->num, targetRotateDirection, convexHull->begginingPoint);
  newConvexHull->point[0] = convexHull->point[0];
  for (int i = 1; i < convexHull->num; i++)
  {
    newConvexHull->point[i] = convexHull->point[convexHull->num - i];
  }
  return newConvexHull;
}

// Merge two convex hulls
// Left convex hull is assumed to be in counter-clockwise order, beggining from the point with the largest x-coordinate
// Right convex hull is assumed to be in clockwise order, beggining from the point with the smallest x-coordinate
ConvexHull *mergeConvexHulls(ConvexHull *leftConvexHull, ConvexHull *rightConvexHull)
{
  // 1. Find the rightmost point of the left hull and the leftmost point of the right hull.
  POINT *rightmostLeft = getCovexHullPoint(leftConvexHull, 0);
  POINT *leftmostRight = getCovexHullPoint(rightConvexHull, 0);

  // 2. Find the upper tangent.
  int leftUpper = 0;
  int rightUpper = 0;
  while (1)
  {
    // Check if leftUpper -> rightUpper -> rightUpper + 1 is counter-clockwise.
    // If so, increment rightUpper, which means that the upper tangent is not found yet.
    if (getRotateDirection(getCovexHullPoint(leftConvexHull, leftUpper), getCovexHullPoint(rightConvexHull, rightUpper), getCovexHullPoint(rightConvexHull, rightUpper + 1)) == CounterClockwise)
    {
      rightUpper++;
    }
    else
    {
      // Check if rightUpper -> leftUpper -> leftUpper + 1 is clockwise.
      // If so, increment leftUpper, which means that the upper tangent is not found yet.
      if (getRotateDirection(getCovexHullPoint(rightConvexHull, rightUpper), getCovexHullPoint(leftConvexHull, leftUpper), getCovexHullPoint(leftConvexHull, leftUpper + 1)) == Clockwise)
      {
        leftUpper++;
      }
      else
      {
        // Otherwise, the upper tangent is found.
        break;
      }
    }
  }

  // 3. Find the lower tangent.
  int leftLower = 0;
  int rightLower = 0;
  while (1)
  {
    // Check if leftLower -> rightLower -> rightLower - 1 is clockwise.
    // If so, decrement rightLower, which means that the lower tangent is not found yet.
    if (getRotateDirection(getCovexHullPoint(leftConvexHull, leftLower), getCovexHullPoint(rightConvexHull, rightLower), getCovexHullPoint(rightConvexHull, rightLower - 1)) == Clockwise)
    {
      rightLower--;
    }
    else
    {
      // Check if rightLower -> leftLower -> leftLower - 1 is counter-clockwise.
      // If so, decrement leftLower, which means that the lower tangent is not found yet.
      if (getRotateDirection(getCovexHullPoint(rightConvexHull, rightLower), getCovexHullPoint(leftConvexHull, leftLower), getCovexHullPoint(leftConvexHull, leftLower - 1)) == CounterClockwise)
      {
        leftLower--;
      }
      else
      {
        // Otherwise, the lower tangent is found.
        break;
      }
    }
  }

  leftLower = -leftLower;
  rightLower = -rightLower;

  // 4. Create the merged hull.
  int mergedConvexHullNum = (leftConvexHull->num - max(leftUpper + leftLower - 1, 0)) + (rightConvexHull->num - max(rightUpper + rightLower - 1, 0));
  printf("[Merging] leftUpper=%d, leftLower=%d, rightUpper=%d, rightLower=%d, mergedConvexHullNum=%d\n", leftUpper, leftLower, rightUpper, rightLower, mergedConvexHullNum);
  ConvexHull *mergedConvexHull = allocateConvexHull(mergedConvexHullNum, Clockwise, Largest);

  // Starting from the top tangent to the bottom tangent, copy the convex hull of the right subset in clockwise order
  int cursor = 0;
  for (int i = rightUpper; i < rightConvexHull->num - rightLower; i++)
  {
    printf("[Merging] cursor=%d, Copying a point with index %d from right convex: (%.2f, %.2f)\n", cursor, i, rightConvexHull->point[i].x, rightConvexHull->point[i].y);
    mergedConvexHull->point[cursor] = rightConvexHull->point[i];
    cursor++;
  }

  // Starting from the bottom tangent to the top tangent, copy the convex hull of the left subset in clockwise order
  for (int i = leftConvexHull->num - leftLower - 1; i >= leftUpper; i--)
  {
    printf("[Merging] cursor=%d, Copying a point with index %d from left convex: (%.2f, %.2f)\n", cursor, i, leftConvexHull->point[i].x, leftConvexHull->point[i].y);
    mergedConvexHull->point[cursor] = leftConvexHull->point[i];
    cursor++;
  }

  return mergedConvexHull;
}

// Get the point of a convex hull
// It automatically considers that the convex hull is cyclic
// If the index is negative, add the number of points to the index until it becomes positive
// If the index is larger than the number of points, subtract the number of points from the index until it becomes smaller than the number of points
POINT *getCovexHullPoint(const ConvexHull *convexHull, int index)
{
  int num = (int)convexHull->num;
  int resolvedIndex = index;

  // Consider that convex hull is cyclic
  // If the index is negative, add the number of points to the index until it becomes positive
  while (resolvedIndex < 0)
  {
    resolvedIndex += num;
  }

  // If the index is larger than the number of points, subtract the number of points from the index until it becomes smaller than the number of points
  while (resolvedIndex >= num)
  {
    resolvedIndex -= num;
  }

  // Assert that the index is in the range [0, num)
  printf("[getCovexHullPoint] num=%d, Given: %d, Resolved: %d\n", convexHull->num, index, resolvedIndex);

  return &convexHull->point[resolvedIndex];
}

void printPset(PSET *pset)
{
  printf("\nPrint PSET with %u points:\n", pset->num);
  for (int i = 0; i < pset->num; i++)
  {
    printf("[%u] (%.2f, %.2f)\n", i, pset->point[i].x, pset->point[i].y);
  }
  printf("\n");
}

void sortPoints(POINT points[], unsigned n)
{
  // Sort the points by their x-coordinate in ascending order using quick sort
  // The pivot is the median of the first, middle, and last element
  // to avoid the worst case of O(n^2)
  // The pivot is swapped with the last element
  printf("Sorting points: n = %u\n", n);

  // If there is none or only one element, return
  if (n <= 1)
  {
    return;
  }

  // If there are only two elements, swap them if the first element has a later finish time than the second element
  if (n == 2)
  {
    if (points[0].x > points[1].x)
    {
      swapPoints(&points[0], &points[1]);
    }
    return;
  }

  // Find an index of the element that has the median of the first, middle, and last element
  unsigned pivotIndex = findIdealPivotIndex(points, n);
  printf("Pivot index: %u\n", pivotIndex);

  // Swap the pivot with the first element
  swapPoints(&points[pivotIndex], &points[0]);

  // Partition the array
  // The earlier the finish time is, the closer to the beginning of the array
  int i = 1;
  int j = (int)n - 1;
  while (1)
  {
    // Find the first element that has a x-coordinate smaller than the pivot
    while (i < n && points[i].x <= points[0].x)
    {
      i++;
    }

    // Find the last element that has a x-coordinate larger than the pivot
    while (j > 0 && points[j].x >= points[0].x)
    {
      j--;
    }

    // If the first element that has a finish time later than the pivot is after the last element that has a finish time earlier than the pivot, break
    if (i >= j)
    {
      break;
    }

    // Swap the two elements
    swapPoints(&points[i], &points[j]);
  }
  unsigned earlierPartitionLastIndex = (unsigned)i - 1;
  unsigned laterPartitionFirstIndex = (unsigned)i;
  // Swap the pivot with the last element of the earlier partition
  swapPoints(&points[0], &points[earlierPartitionLastIndex]);
  earlierPartitionLastIndex--;
  unsigned earlierPartitionLength = earlierPartitionLastIndex + 1;
  unsigned laterPartitionLength = n - laterPartitionFirstIndex;

  // Sort the earlier partition
  sortPoints(points, earlierPartitionLength);

  // Sort the later partition
  sortPoints(points + laterPartitionFirstIndex, laterPartitionLength);

  return;
}

unsigned findIdealPivotIndex(POINT points[], unsigned n)
{
  // Find and return an index of the element that has the median of the first, middle, and last element
  // to avoid the worst case of O(n^2) in quick sort

  // If there is only one element, return 0
  if (n == 1)
  {
    return 0;
  }

  // Find the median of the first, middle, and last element
  unsigned first = 0;
  unsigned middle = n / 2; // Floor
  unsigned last = n - 1;

  // Since the middle element is most likely to be the median, check if it is the median first
  if (points[first].x <= points[middle].x && points[middle].x <= points[last].x)
  {
    // If the middle element is the median, return the index of the middle element
    return middle;
  }

  // If the middle element is not the median, check if the first element is the median
  if (points[middle].x <= points[first].x && points[first].x <= points[last].x)
  {
    // If the first element is the median, return the index of the first element
    return first;
  }

  // If the first and middle element are not the median, the last element must be the median
  // Return the index of the last element
  return last;
}

void swapPoints(POINT *p1, POINT *p2)
{
  // Swap the two points

  // If the two indices are the same, return
  if (p1 == p2)
  {
    return;
  }

  // Allocate memory
  POINT *temp = (POINT *)malloc(sizeof(POINT));

  // If memory allocation fails, exit the program
  if (temp == NULL)
  {
    printf("Memory allocation failed at swapPoints()\n");
    exit(1);
  }

  // Copy the first activity to the temp variable
  temp->x = p1->x;
  temp->y = p1->y;

  // Copy the second activity to the first activity
  p1->x = p2->x;
  p1->y = p2->y;

  // Copy the temp variable to the second activity
  p2->x = temp->x;
  p2->y = temp->y;

  // Free the memory
  free(temp);
}

int max(int a, int b)
{
  if (a > b)
  {
    return a;
  }
  return b;
}