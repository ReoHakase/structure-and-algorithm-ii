#include <stdlib.h>
#include <stdio.h>
#include "Activity.h"

/* input data */
extern unsigned num;
extern ACT activity[];

int main()
{
  printf("\n# Input activities:\n");
  printActivity(activity, num);
  printf("\n");

  // O(nlogn) ~ O(n^2) depending on the pivot
  sortActivity(activity, num);

  printf("\n# Sorted activities:\n");
  printActivity(activity, num);
  printf("\n");

  printf("# Optimal solution:\n");
  unsigned time = 0;

  // O(n)
  for (unsigned cursor = 0; cursor < num; cursor++)
  {
    /* find a local optimal solution */
    // If the start time of the current activity is later than the current time,
    // it can be added to the optimal solution
    if (activity[cursor].start >= time)
    {
      /* print the local optimal solution */
      printActivity(&activity[cursor], 1);
      time = activity[cursor].finish;
    }
  }
}

void sortActivity(ACT act[], unsigned n)
{
  // Sort the activities by their finish time using quick sort
  // The pivot is the median of the first, middle, and last element
  // to avoid the worst case of O(n^2)
  // The pivot is swapped with the last element
  printf("Sorting activities: n = %u\n", n);

  // If there is none or only one element, return
  if (n <= 1)
  {
    return;
  }

  // If there are only two elements, swap them if the first element has a later finish time than the second element
  if (n == 2)
  {
    if (act[0].finish > act[1].finish)
    {
      swapActivity(act, 0, 1);
    }
    return;
  }

  // Find an index of the element that has the median of the first, middle, and last element
  unsigned pivotIndex = findIdealPivotIndex(act, n);
  printf("Pivot index: %u\n", pivotIndex);

  // Swap the pivot with the first element
  swapActivity(act, pivotIndex, 0);

  // Partition the array
  // The earlier the finish time is, the closer to the beginning of the array
  int i = 1;
  int j = (int)n - 1;
  while (1)
  {
    // Find the first element that has a finish time later than the pivot
    while (i < n && act[i].finish <= act[0].finish)
    {
      i++;
    }

    // Find the last element that has a finish time earlier than the pivot
    while (j > 0 && act[j].finish >= act[0].finish)
    {
      j--;
    }

    // If the first element that has a finish time later than the pivot is after the last element that has a finish time earlier than the pivot, break
    if (i >= j)
    {
      break;
    }

    // Swap the two elements
    swapActivity(act, i, j);
  }
  unsigned earlierPartitionLastIndex = (unsigned)i - 1;
  unsigned laterPartitionFirstIndex = (unsigned)i;
  // Swap the pivot with the last element of the earlier partition
  swapActivity(act, 0, earlierPartitionLastIndex);
  earlierPartitionLastIndex--;
  unsigned earlierPartitionLength = earlierPartitionLastIndex + 1;
  unsigned laterPartitionLength = n - laterPartitionFirstIndex;

  // Sort the earlier partition
  sortActivity(act, earlierPartitionLength);

  // Sort the later partition
  sortActivity(act + laterPartitionFirstIndex, laterPartitionLength);

  return;
}

unsigned findIdealPivotIndex(ACT act[], unsigned n)
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
  if (act[first].finish <= act[middle].finish && act[middle].finish <= act[last].finish)
  {
    // If the middle element is the median, return the index of the middle element
    return middle;
  }

  // If the middle element is not the median, check if the first element is the median
  if (act[middle].finish <= act[first].finish && act[first].finish <= act[last].finish)
  {
    // If the first element is the median, return the index of the first element
    return first;
  }

  // If the first and middle element are not the median, the last element must be the median
  // Return the index of the last element
  return last;
}

void swapActivity(ACT act[], unsigned i, unsigned j)
{
  // Swap the two activities

  // If the two indices are the same, return
  if (i == j)
  {
    printf("Swapped activities: [%u] and [%u] (skipped)\n", i, j);
    return;
  }

  // Allocate memory
  ACT *temp = (ACT *)malloc(sizeof(ACT));
  // If memory allocation fails, exit the program
  if (temp == NULL)
  {
    printf("Memory allocation failed at swapActivity()\n");
    exit(1);
  }

  // Copy the first activity to the temp variable
  temp->name = act[i].name;
  temp->start = act[i].start;
  temp->finish = act[i].finish;

  // Copy the second activity to the first activity
  act[i].name = act[j].name;
  act[i].start = act[j].start;
  act[i].finish = act[j].finish;

  // Copy the temp variable to the second activity
  act[j].name = temp->name;
  act[j].start = temp->start;
  act[j].finish = temp->finish;

  // Free the memory
  free(temp);

  printf("Swapped activities: [%u] and [%u]\n", i, j);
}

void printActivity(ACT act[], unsigned n)
{
  // Print the activities
  for (unsigned i = 0; i < n; i++)
  {
    printf("[%u] An activity named \"%s\" starts at %u and ends at %u.\n", i, act[i].name, act[i].start, act[i].finish);
  }
}