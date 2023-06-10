typedef enum RedBlackNodeColor
{
  Red,
  Black
} RedBlackNodeColor;

typedef struct RedBlackNode
{
  int value;
  enum RedBlackNodeColor color;
  struct RedBlackNode *left;
  struct RedBlackNode *right;
  struct RedBlackNode *parent;
} RedBlackNode;

typedef enum NodeRotationDirection
{
  ToLeft,
  ToRight
} NodeRotationDirection;

RedBlackNode *allocateRedBlackNode(int value, RedBlackNodeColor color);
void freeRedBlackNode(RedBlackNode *node);
RedBlackNode *insertRedBlackNode(RedBlackNode *root, int value);
void deleteRedBlackNode(RedBlackNode *root, int value);
RedBlackNode *rotateNodes(RedBlackNode *source, NodeRotationDirection direction);
RedBlackNode *searchRedBlackNode(RedBlackNode *root, int value);
void printRedBlackTree(RedBlackNode *root, int level);

typedef enum NodeComparisonResult
{
  Equal,
  Less,
  Greater
} NodeComparisonResult;

NodeComparisonResult compareNode(RedBlackNode *basis, RedBlackNode *target);
