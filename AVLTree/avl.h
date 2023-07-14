typedef enum NodeDirection
{
  None = 0,
  Left = 1, // falsyな0と区別するために1を設定
  Right
} NodeDirection;

typedef enum NodeComparisonResult
{
  Equal,
  Less,
  Greater
} NodeComparisonResult;

typedef struct AVLNode
{
  int value;
  int height;  // ノードの高さ
  int balance; // ノードの平衡定数
  struct AVLNode *parent;
  struct AVLNode *left;
  struct AVLNode *right;
} AVLNode;

// ノード方向を反転する
// Left -> Right, Right -> Left, それ以外 -> None
NodeDirection invertNodeDirection(NodeDirection direction);

// AVLノードの兄弟ノードを取得する。
// もし存在しない場合、 NULL を返す。
AVLNode *getAVLNodeSibling(AVLNode *node);

// ノードの子の参照を更新する
// 双方向リストのように、親の参照を更新する
void updateRedBlackNodeChild(AVLNode *parent, AVLNode *newChild, NodeDirection variant);

// 与えられたノードの子孫のうち、指定された方向にある最も近いノードを返す
AVLNode *getClosestDecendant(AVLNode *node, NodeDirection variant);


// 値 `value` を持つAVLノードを作成する。
AVLNode *allocateAVLNode(int value);

// ノードを解放する。
// ただし、子孫ノードも再帰的に解放する。
// また、親が存在する場合は、親からの参照も削除する。
void freeAVLNode(AVLNode *node);

// ノードの値を交換する
void swapAVLNodeValue(AVLNode *node1, AVLNode *node2);

// target ノードの値が basis ノードのものに対して大きいか小さいかを比較する
NodeComparisonResult compareNode(AVLNode *basis, AVLNode *target);

// 回転前の親ノード `source` を指定して、ノードを `direction` 方向に回転させる。
// `direction` に `Left` を指定した場合、左回転を行う。ただし、回転後に親となる右の子ノードが存在しない場合は回転しない。
// `direction` に `Right` を指定した場合、右回転を行う。ただし、回転後に親となる左の子ノードが存在しない場合は回転しない。
// 回転後の親ノードを返す。
AVLNode *rotateNodes(AVLNode *source, NodeDirection direction);

// ノードが左の子なのか右の子なのかを判定する
// 親が存在しない場合、 `None` を返す
NodeDirection getAVLNodeVariant(AVLNode *node);

// ノードの高さを更新する。
// ただし、葉ノードの高さは0とする。
// また、子ノードの高さがすでに更新されていることを前提とする。
void updateAVLNodeHeight(AVLNode *node);

// ノードの平衡定数を更新する。
// ただし、葉ノードの平衡定数は0とする。
// また、子ノードの平衡定数がすでに更新されていることを前提とする。
void updateAVLNodeBalance(AVLNode *node);

// AVL木にノードを挿入する
AVLNode *insertAVLNode(AVLNode *root, int value);

// AVL木のノードを平衡化する
// 与えられたノードから根方向に向かって不正なノードを探査し、平衡化を行う
AVLNode *fixAVLNodeBalance(AVLNode *root, AVLNode *node);

// AVL木からノードを検索する
AVLNode *searchAVLNode(AVLNode *root, int value);

// AVL木からノードを削除する
AVLNode *deleteAVLNode(AVLNode *root, int value);

// AVLノードを降順で表示する
void printAVLTree(AVLNode *root, int level);