typedef struct
{
  float x;
  float y;
} POINT;

typedef struct
{
  unsigned num;
  POINT *point;
} PSET;

typedef enum
{
  Neither,
  Clockwise,
  CounterClockwise,
} RotateDirection;

typedef enum
{
  Smallest,
  Largest
} BegginingPoint;

typedef struct
{
  unsigned num;
  POINT *point;
  RotateDirection rotateDirection;
  BegginingPoint begginingPoint;
} ConvexHull;

// PSET *allocatePset(unsigned n);
// void freePset(PSET *pset);

ConvexHull *allocateConvexHull(unsigned n, RotateDirection rotateDirection, BegginingPoint begginingPoint);
void freeConvexHull(ConvexHull *convexHull);

// Calculate the angle of the vector src -> target using arctan
// Returns the angle in radians in the range [-pi, pi]
float getAngle(POINT *src, POINT *target);

// Calculate the difference between two angles as angle2 - angle1.
// Both angle1 and angle2 must be in the range [-pi, pi].
// Returns the difference in radians in the range [-pi, pi].
float getAngleDifference(float angle1, float angle2);

// Calculate the center point of the triangle with points p1, p2, p3.
POINT *getTriangleCenterPoint(POINT *p1, POINT *p2, POINT *p3);

// Calculate a rotation direction of the three points, [0] -> [1] -> [2] seen from the center point of three.
// If the rotation is counter-clockwise, return CounterClockwise.
// If the rotation is clockwise, return Clockwise.
// Otherwise, return Neither.
RotateDirection getRotateDirection(POINT *p1, POINT *p2, POINT *p3);

// Return the convex hull of the given point set.
// Note that the input pset is assumed to be sorted in ascending order of x-coordinate.
// If rotateDirection is Clockwise, return the convex hull in clockwise order.
// If rotateDirection is CounterClockwise, return the convex hull in counter-clockwise order.
// If begginingPoint is Smallest, return the convex hull starting from the point with the smallest x-coordinate.
// If begginingPoint is Largest, return the convex hull starting from the point with the largest x-coordinate.
ConvexHull *getConvexHull(const PSET *pset, RotateDirection rotateDirection, BegginingPoint begginingPoint);

// Get the point of a convex hull
POINT *getCovexHullPoint(const ConvexHull *convexHull, int index);

// Adjust the beggining point of the convex hull
ConvexHull *adjustConvexHullBegginingPoint(ConvexHull *convexHull, BegginingPoint begginingPoint);

// Change the angle direction of a convex hull
// The begginning point of the convex hull stays the same
ConvexHull *changeConvexHullRotateDirection(const ConvexHull *convexHull, RotateDirection targetRotateDirection);

// Print the convex hull
void printConvexHull(ConvexHull *convexHull);

// Merge two convex hulls
// Left convex hull is assumed to be in counter-clockwise order, beggining from the point with the largest x-coordinate
// Right convex hull is assumed to be in clockwise order, beggining from the point with the smallest x-coordinate
ConvexHull *mergeConvexHulls(ConvexHull *leftConvexHull, ConvexHull *rightConvexHull);

// Sort points in pset in ascending order of x-coordinate using quick sort
void sortPoints(POINT points[], unsigned n);
unsigned findIdealPivotIndex(POINT points[], unsigned n);
void swapPoints(POINT *p1, POINT *p2);

void printPset(PSET *pset);

int max(int a, int b);