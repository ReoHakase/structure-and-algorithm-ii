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

typedef enum NodeDirection
{
  Left = 1, // 0と区別するために1を設定
  Right
} NodeDirection;

RedBlackNode *allocateRedBlackNode(int value, RedBlackNodeColor color);
void freeRedBlackNode(RedBlackNode *node);
RedBlackNode *insertRedBlackNode(RedBlackNode *root, int value);
RedBlackNode *deleteRedBlackNode(RedBlackNode *root, int value);
RedBlackNode *rotateNodes(RedBlackNode *source, NodeDirection direction);
RedBlackNode *searchRedBlackNode(RedBlackNode *root, int value);
RedBlackNode *getRedBlackNodeSibling(RedBlackNode *node);
RedBlackNode *getRedBlackNodeChild(RedBlackNode *node, NodeDirection variant);
NodeDirection invertNodeDirection(NodeDirection variant);
void updateRedBlackNodeChild(RedBlackNode *parent, RedBlackNode *newChild, NodeDirection variant);
NodeDirection getRedBlackNodeVariant(RedBlackNode *child);
RedBlackNode *getClosestDecendant(RedBlackNode *node, NodeDirection variant);
void swapRedBlackNodeValue(RedBlackNode *a, RedBlackNode *b);
void printRedBlackTree(RedBlackNode *root, int level);

typedef enum NodeComparisonResult
{
  Equal,
  Less,
  Greater
} NodeComparisonResult;

NodeComparisonResult compareNode(RedBlackNode *basis, RedBlackNode *target);
