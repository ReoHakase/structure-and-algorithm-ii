#include <stdlib.h>
#include <stdio.h>
#include "Knapsack.h"

/* test data */
extern unsigned num;
extern OBJ object[];
extern float knapsack;

int main()
{
  for (unsigned i = 0; i < num; i++)
    object[i].performance = object[i].value / object[i].weight;

  printf("\n# Input objects:\n");
  printObject(object, num);
  printf("\n");

  // Sort the objects by their performance using quick sort in descending order
  sortObject(object, num);

  printf("\n# Sorted objects:\n");
  printObject(object, num);
  printf("\n");

  // Solve the knapsack problem
  printf("\n# Solving the knapsack problem:\n");
  float remainingCapacity = knapsack; // W
  float totalValue = 0;               // V

  // For all objects
  for (unsigned i = 0; i < num; i++)
  {
    printf("Remaining capacity: %.2f\n", remainingCapacity);

    // If the knapsack is full, stop
    if (remainingCapacity <= 0)
    {
      break;
    }
    // If the object can be added entirely to the knapsack
    if (remainingCapacity >= object[i].weight)
    {
      // Print the object
      printf("Adding an object:\n");
      printObject(&object[i], 1);

      // Add the object to the knapsack
      remainingCapacity -= object[i].weight;
      totalValue += object[i].value;
    }
    // If the object cannot be added to the knapsack
    else
    {
      // Print the object
      printf("Adding an object partially; %.4f%%:\n", remainingCapacity / object[i].weight * 100);
      printObject(&object[i], 1);

      // Add a fraction of the object to the knapsack
      totalValue += object[i].performance * remainingCapacity;
      remainingCapacity = 0;
    }
  }

  // Print the optimal solution
  printf("\n# Optimal solution:\n");
  printf("Total value: %.2f\n", totalValue);
}

//* Sort the objects by their performance time using quick sort in descending order
//* The pivot is the median of the first, middle, and last element
//* O(nlogn) ~ O(n^2) depending on the pivot
void sortObject(OBJ obj[], unsigned n)
{
  // Sort the objivities by their performance time using quick sort
  // The pivot is the median of the first, middle, and last element
  // to avoid the worst case of O(n^2)
  // The pivot is swapped with the last element
  printf("Sorting objects: n = %u\n", n);

  // If there is none or only one element, return
  if (n <= 1)
  {
    return;
  }

  // If there are only two elements, swap them if the first element has a later performance time than the second element
  if (n == 2)
  {
    if (obj[0].performance < obj[1].performance)
    {
      swapObject(obj, 0, 1);
    }
    return;
  }

  // Find an index of the element that has the median of the first, middle, and last element
  unsigned pivotIndex = findIdealPivotIndex(obj, n);
  printf("Pivot index: %u\n", pivotIndex);

  // Swap the pivot with the first element
  swapObject(obj, pivotIndex, 0);

  // Partition the array
  // The earlier the performance time is, the closer to the beginning of the array
  int i = 1;
  int j = (int)n - 1;
  while (1)
  {
    // Find the first element that has a performance time later than the pivot
    while (i < n && obj[i].performance >= obj[0].performance)
    {
      i++;
    }

    // Find the last element that has a performance time earlier than the pivot
    while (j > 0 && obj[j].performance <= obj[0].performance)
    {
      j--;
    }

    // If the first element that has a performance time later than the pivot is after the last element that has a performance time earlier than the pivot, break
    if (i >= j)
    {
      break;
    }

    // Swap the two elements
    swapObject(obj, i, j);
  }
  unsigned earlierPartitionLastIndex = (unsigned)i - 1;
  unsigned laterPartitionFirstIndex = (unsigned)i;
  // Swap the pivot with the last element of the earlier partition
  swapObject(obj, 0, earlierPartitionLastIndex);
  earlierPartitionLastIndex--;
  unsigned earlierPartitionLength = earlierPartitionLastIndex + 1;
  unsigned laterPartitionLength = n - laterPartitionFirstIndex;

  // Sort the earlier partition
  sortObject(obj, earlierPartitionLength);

  // Sort the later partition
  sortObject(obj + laterPartitionFirstIndex, laterPartitionLength);

  return;
}

unsigned findIdealPivotIndex(OBJ obj[], unsigned n)
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
  if (obj[first].performance <= obj[middle].performance && obj[middle].performance <= obj[last].performance)
  {
    // If the middle element is the median, return the index of the middle element
    return middle;
  }

  // If the middle element is not the median, check if the first element is the median
  if (obj[middle].performance <= obj[first].performance && obj[first].performance <= obj[last].performance)
  {
    // If the first element is the median, return the index of the first element
    return first;
  }

  // If the first and middle element are not the median, the last element must be the median
  // Return the index of the last element
  return last;
}

//* Swap the contents of two objects
void swapObject(OBJ obj[], unsigned i, unsigned j)
{
  if (i == j)
  {
    return;
  }

  // Allocate memory for a temporary object
  OBJ *temp = malloc(sizeof(OBJ));

  if (temp == NULL)
  {
    printf("Error allocating memory for temp object\n");
    exit(1);
  }

  // Copy the contents of a into temp
  temp->name = obj[i].name;
  temp->weight = obj[i].weight;
  temp->value = obj[i].value;
  temp->performance = obj[i].performance;

  // Copy the contents of b into a
  obj[i].name = obj[j].name;
  obj[i].weight = obj[j].weight;
  obj[i].value = obj[j].value;
  obj[i].performance = obj[j].performance;

  // Copy the contents of temp into b
  obj[j].name = temp->name;
  obj[j].weight = temp->weight;
  obj[j].value = temp->value;
  obj[j].performance = temp->performance;

  return;
}

//* Print the contents of an array of objects
void printObject(OBJ obj[], unsigned n)
{
  printf("index\tObject\tWeight\tValue\tPerformance\n");
  for (unsigned i = 0; i < n; i++)
  {
    printf("%u\t%s\t%.2f\t%.2f\t%.2f\n", i, obj[i].name, obj[i].weight, obj[i].value, obj[i].performance);
  }
  return;
}