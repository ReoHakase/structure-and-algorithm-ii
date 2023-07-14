#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// AVLノードを生成する
AVLNode *allocateAVLNode(int value)
{
  AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
  node->value = value;
  node->height = 0;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void freeAVLNode(AVLNode *node)
{
  if (node == NULL)
  {
    return;
  }

  // 子に対して再起呼び出し
  freeAVLNode(node->left);
  freeAVLNode(node->right);

  // 削除するノードの親が持つ参照を削除する
  if (node->parent != NULL)
  {
    if (node->parent->left == node)
    {
      node->parent->left = NULL;
    }
    if (node->parent->right == node)
    {
      node->parent->right = NULL;
    }
  }

  free(node);
}

// ノードの高さを更新する。
// ただし、葉ノードの高さは0とする。
// また、ノードの高さが更新された場合は、親ノードの平衡定数と高さも更新する。
// また、子ノードの高さがすでに更新されていることを前提とする。
void updateAVLNodeHeight(AVLNode *node)
{
  while (node != NULL)
  {
    int oldHeight = node->height;

    // 子ノードの高さの最大値を取得
    int leftHeight = -1;
    int rightHeight = -1;
    if (node->left != NULL)
    {
      leftHeight = node->left->height;
    }
    if (node->right != NULL)
    {
      rightHeight = node->right->height;
    }
    int maxHeight = (leftHeight > rightHeight ? leftHeight : rightHeight);
    // ノードの高さを更新
    node->height = maxHeight + 1;
    // もしノードの高さが更新されていれば、親ノードの平衡定数と高さも更新する
    if (oldHeight != node->height)
    {
      printf("高さを更新しました。\x1b[34m(%d -> %d)\x1b[0m ", oldHeight, node->height);
      printAVLTree(node, 0);
      updateAVLNodeBalance(node->parent);
      node = node->parent;
    }
    else
    {
      break;
    }
  }
}

// ノードの平衡定数を更新する。
// ただし、葉ノードの高さは0とする。
// また、子ノードの高さがすでに更新されていることを前提とする。
void updateAVLNodeBalance(AVLNode *node)
{
  if (node == NULL)
  {
    return;
  }
  // 左右の子ノードの側のみで辿って行った場合の、与えられたノードの高さを取得
  int leftSideHeight = 0;
  int rightSideHeight = 0;
  if (node->left != NULL)
  {
    leftSideHeight = node->left->height + 1;
  }
  if (node->right != NULL)
  {
    rightSideHeight = node->right->height + 1;
  }
  node->balance = leftSideHeight - rightSideHeight;
  printf("平衡定数を更新しました。\x1b[34m(L%d - R%d = %d)\x1b[0m", leftSideHeight, rightSideHeight, node->balance);
  printAVLTree(node, 0);
}

AVLNode *insertAVLNode(AVLNode *root, int value)
{
  // root が NULL である場合、つまり木にノードが存在しない場合、新規ノードを作成して root に設定する。
  if (root == NULL)
  {
    return allocateAVLNode(value);
  }

  // root が NULL でない場合、挿入位置を探索する。
  // 必ず葉として高さ0で挿入される。
  AVLNode *newNode = allocateAVLNode(value);
  AVLNode *currentNode = root;
  while (1)
  {
    NodeComparisonResult result = compareNode(currentNode, newNode);
    if (result == Equal)
    {
      // 既に同じ値が挿入されている場合、挿入を行わずに終了する。
      freeAVLNode(newNode);
      return root;
    }
    else if (result == Less)
    {
      // 挿入値が現在のノードより小さい場合、左側のノードを探索する。
      if (currentNode->left == NULL)
      {
        // 左側に挿入可能な場合、新規ノードを挿入する。
        currentNode->left = newNode;
        newNode->parent = currentNode;
        // 挿入後に高さと平衡定数を更新する。
        newNode->height = -1; // 初回の挿入時は、高さの更新の伝播を強制させるため、高さを-1に設定する。
        updateAVLNodeHeight(newNode);

        break;
      }
      else
      {
        // 左側に挿入不可能な場合、左側のノードを探索する。
        currentNode = currentNode->left;
      }
    }
    else
    {
      // 挿入値が現在のノードより大きい場合、右側のノードを探索する。
      if (currentNode->right == NULL)
      {
        // 右側に挿入可能な場合、新規ノードを挿入する。
        currentNode->right = newNode;
        newNode->parent = currentNode;
        // 挿入後に高さと平衡定数を更新する。
        newNode->height = -1; // 初回の挿入時は、高さの更新の伝播を強制させるため
        updateAVLNodeHeight(newNode);
        break;
      }
      else
      {
        // 右側に挿入不可能な場合、右側のノードを探索する。
        currentNode = currentNode->right;
      }
    }
  }

  printf("挿入しました。平衡定数が不正なノードの修正を開始します。");
  printAVLTree(root, 0);

  // 挿入後に平衡定数が不正なノードを修正する。
  root = fixAVLNodeBalance(root, newNode);

  return root;
}

AVLNode *fixAVLNodeBalance(AVLNode *root, AVLNode *node)
{
  // 与えられたノードから根方向に向かって、平衡定数が不正なノードを探査する。
  AVLNode *alphaNode = node; // β ノードの子ノード
  AVLNode *betaNode = NULL;  // 不正な平衡定数を持つノード

  while (alphaNode->parent != NULL)
  {
    if (alphaNode->parent->balance == 2 || alphaNode->parent->balance == -2)
    {
      betaNode = alphaNode->parent;
      break;
    }
    alphaNode = alphaNode->parent;
  }

  // β ノードが存在しない場合、木の高さが変化しないため、回転処理は不要である。
  if (betaNode == NULL)
  {
    return root;
  }

  printf("平衡定数が不正なノードを発見しました。α: %d, β: %d\n", alphaNode->value, betaNode->value);

  // β ノードの平衡定数が不正な場合、回転処理を行う。
  // パターン1. 最も高い部分木が、βノードの左左孫に存在する場合
  if (alphaNode->balance == 1 && betaNode->balance == 2)
  {
    printf("パターン1. 最も高い部分木が、βノードの左左孫に存在する場合\n");
    // βノードを右側に回転させる。
    int shouldUpdateRoot = betaNode->parent == NULL;
    AVLNode *newPossibleRoot = rotateNodes(betaNode, Right);
    if (shouldUpdateRoot)
    {
      root = newPossibleRoot;
    }
    return root;
  }

  // パターン2. 最も高い部分木が、βノードの右左孫に存在する場合
  if (alphaNode->balance == 1 && betaNode->balance == -2)
  {
    printf("パターン2. 最も高い部分木が、βノードの右左孫に存在する場合\n");
    // αノードを右側に回転させる。
    // 今、αノードには必ず親のβノードが存在するから、根は変化しない。
    rotateNodes(alphaNode, Right);
    // βノードを左側に回転させる。
    int shouldUpdateRoot = betaNode->parent == NULL;
    AVLNode *newPossibleRoot = rotateNodes(betaNode, Left);
    if (shouldUpdateRoot)
    {
      root = newPossibleRoot;
    }
    return root;
  }

  // パターン3. 最も高い部分木が、βノードの左右孫に存在する場合
  if (alphaNode->balance == -1 && betaNode->balance == 2)
  {
    printf("パターン3. 最も高い部分木が、βノードの左右孫に存在する場合\n");
    // αノードを左側に回転させる。
    // 今、αノードには必ず親のβノードが存在するから、根は変化しない。
    rotateNodes(alphaNode, Left);
    printf("αノードを左側に回転させるました。");
    printAVLTree(root, 0);
    // βノードを右側に回転させる。
    int shouldUpdateRoot = betaNode->parent == NULL;
    AVLNode *newPossibleRoot = rotateNodes(betaNode, Right);
    if (shouldUpdateRoot)
    {
      root = newPossibleRoot;
    }
    return root;
  }

  // パターン4. 最も高い部分木が、βノードの右右孫に存在する場合
  if (alphaNode->balance == -1 && betaNode->balance == -2)
  {
    printf("パターン4. 最も高い部分木が、βノードの右右孫に存在する場合\n");
    // βノードを左側に回転させる。
    int shouldUpdateRoot = betaNode->parent == NULL;
    AVLNode *newPossibleRoot = rotateNodes(betaNode, Left);
    if (shouldUpdateRoot)
    {
      root = newPossibleRoot;
    }
    return root;
  }

  return root;
}

// AVL木からノードを検索する
AVLNode *searchAVLNode(AVLNode *root, int value)
{
  AVLNode *currentNode = root;

  while (currentNode != NULL)
  {
    if (currentNode->value == value)
      return currentNode;

    if (value < currentNode->value)
      currentNode = currentNode->left;
    else
      currentNode = currentNode->right;
  }

  return NULL;
}

AVLNode *deleteAVLNode(AVLNode *root, int value)
{
  // 削除対象のノードを探査する
  AVLNode *targetNode = searchAVLNode(root, value);

  // もし削除対象が見つからなかったら、何もしない
  if (targetNode == NULL)
  {
    printf("削除対象が存在しません。\n");
    return root;
  }

  while (targetNode != NULL)
  {
    printf("削除対象のノードを表示します。");
    printAVLTree(targetNode, 0);
    // ケース0. 削除対象が根で唯一のノードの場合
    if (targetNode->parent == NULL && targetNode->left == NULL && targetNode->right == NULL)
    {
      printf("ケース0. 削除対象が根ノードで葉ノードな場合\n");
      freeAVLNode(targetNode);
      return NULL;
    }

    // ケース1. 削除対象が葉ノードな場合
    if (targetNode->left == NULL && targetNode->right == NULL)
    {
      printf("ケース1. 削除対象が葉ノードな場合\n");
      // 今、必ず親は存在する
      AVLNode *parentNode = targetNode->parent;
      AVLNode *siblingNode = getAVLNodeSibling(targetNode);
      // ノードを削除
      freeAVLNode(targetNode);
      updateAVLNodeBalance(parentNode);
      updateAVLNodeHeight(parentNode);
      // 平衡定数が正しくなるように修正する
      if (siblingNode)
      {
        // ケース1.1. 削除するノードに兄弟ノードが存在する場合
        // 兄弟ノードから平衡定数が正しくないノードを根方向に向かって探査して修正する
        root = fixAVLNodeBalance(root, siblingNode);
        return root;
      }
      else
      {
        // ケース1.2. 削除するノードに兄弟ノードが存在しない場合
        return root;
      }
    }

    // ケース2. 削除対象が子ノードを1つ持つ場合
    // ケース2.1 削除対象が子ノードを左側に1つ持つ場合
    else if (targetNode->right == NULL)
    {
      printf("ケース2.1. 削除対象が子ノードを左側に1つ持つ場合 ");
      // 削除対象のノードが左側の子を持たない場合、右側の子を削除対象のノードの位置に移動させる。
      if (targetNode == root)
      {
        printf("(対象が根)\n");
        // もし対象が根の場合は、新しい根を右側の子にする。
        AVLNode *oldRoot = root;
        root = oldRoot->right;
        root->parent = NULL;
        // 参照が切れた削除対象のノードを削除する。
        oldRoot->right = NULL;
        freeAVLNode(oldRoot);
        break;
      }
      else
      {
        printf("(対象が根でない)\n");
        // もし対象が根でない場合は、削除対象のノードの親の子を右側の子にする。
        AVLNode *parentNode = targetNode->parent;
        NodeDirection targetNodeVariant = getAVLNodeVariant(targetNode);
        updateRedBlackNodeChild(parentNode, targetNode->left, targetNodeVariant);
        // 親ノードの高さと平衡定数を更新する。
        updateAVLNodeBalance(parentNode);
        updateAVLNodeHeight(parentNode);
        // 参照が切れた削除対象のノードを削除する。
        targetNode->left = NULL;
        freeAVLNode(targetNode);
        break;
      }
    }

    // ケース2.2 削除対象が子ノードを右側に1つ持つ場合
    else if (targetNode->left == NULL)
    {
      printf("ケース2.2. 削除対象が子ノードを右側に1つ持つ場合\n");
      // 削除対象のノードが左側の子を持たない場合、右側の子を削除対象のノードの位置に移動させる。
      if (targetNode == root)
      {
        // もし対象が根の場合は、新しい根を右側の子にする。
        AVLNode *oldRoot = root;
        root = oldRoot->right;
        root->parent = NULL;
        // 参照が切れた削除対象のノードを削除する。
        oldRoot->right = NULL;
        freeAVLNode(oldRoot);
        break;
      }
      else
      {
        // もし対象が根でない場合は、削除対象のノードの親の子を右側の子にする。
        AVLNode *parentNode = targetNode->parent;
        NodeDirection targetNodeVariant = getAVLNodeVariant(targetNode);
        updateRedBlackNodeChild(parentNode, targetNode->right, targetNodeVariant);
        // 親ノードの高さと平衡定数を更新する。
        updateAVLNodeBalance(parentNode);
        updateAVLNodeHeight(parentNode);
        // 参照が切れた削除対象のノードを削除する。
        targetNode->right = NULL;
        freeAVLNode(targetNode);
        break;
      }
    }

    // ケース3. 削除対象が2つの子を持つ場合
    else
    {
      printf("ケース3. 削除対象が2つの子を持つ場合\n");
      NodeDirection decendantSearchDirection = Left;
      AVLNode *closestNode = getClosestDecendant(targetNode, decendantSearchDirection); // 左右どっちでもいい
      swapAVLNodeValue(targetNode, closestNode);
      // 最も近いノードを削除する。この時、最も近いノードは絶対に子を一つしか持たない。
      printf("### %d を削除します。\n", closestNode->value);
      targetNode = closestNode;
      continue;
    }
  }
  return root;
}

// AVLノードを降順で表示する
void printAVLTree(AVLNode *root, int level)
{
  if (root == NULL)
  {
    if (level == 0)
      printf("木構造が存在しません。\n");
    return;
  }
  if (level == 0)
  {
    printf("木構造を降順で表示します。\n");
  }
  printAVLTree(root->right, level + 1);
  for (int i = 0; i < level; i++)
  {
    printf("\x1b[37m|\x1b[0m\t");
  }
  if (root->parent == NULL)
  {
    printf("♢ >>>\t");
  }
  else
  {
    if (root->parent->left == root)
    {
      printf("└\t");
    }
    else
    {
      printf("┌\t");
    }
  }
  char leftDisplay[24] = "";
  char rightDisplay[24] = "";
  char parentDisplay[24] = "";
  if (root->left != NULL)
  {
    sprintf(leftDisplay, "L%d", root->left->value);
    // sprintf(leftDisplay, "L");
  }
  if (root->right != NULL)
  {
    sprintf(rightDisplay, "R%d", root->right->value);
    // sprintf(leftDisplay, "R");
  }
  if (root->parent != NULL)
  {
    sprintf(parentDisplay, "P%d", root->parent->value);
    // sprintf(leftDisplay, "P");
  }
  printf("%d \x1b[36m⚖️%d \x1b[31mh%d\x1b[0m \x1b[34m[%s %s %s]\x1b[0m\n", root->value, root->balance, root->height, leftDisplay, rightDisplay, parentDisplay);
  printAVLTree(root->left, level + 1);
}

int main(void)
{
  AVLNode *root = NULL;

  int insertedValues[] = {60, 40, 30, 10, 50, 20};
  for (int i = 0; i < 6; i++)
  {
    printf("\n# %d を挿入します。\n", insertedValues[i]);
    root = insertAVLNode(root, insertedValues[i]);
    printf("挿入しました。");
    printAVLTree(root, 0);
  }

  printf("\n# 30 を探索します。\n");
  AVLNode *node30 = searchAVLNode(root, 30);
  if (node30 != NULL)
  {
    printf("探索に成功しました。");
    printAVLTree(node30, 0);
  }
  else
  {
    printf("\x1b[31m探索に失敗しました。\x1b[0m\n");
  }

  printf("\n# 55 を探索します。\n");
  AVLNode *node55 = searchAVLNode(root, 55);
  if (node55 != NULL)
  {
    printf("探索に成功しました。");
    printAVLTree(node55, 0);
  }
  else
  {
    printf("\x1b[31m探索に失敗しました。\x1b[0m\n");
  }

  int deletedValues[] = {20, 40, 60, 30, 50, 10};
  for (int i = 0; i < 6; i++)
  {
    printf("\n# %d を削除します。\n", deletedValues[i]);
    root = deleteAVLNode(root, deletedValues[i]);
    printAVLTree(root, 0);
  }

  return 0;
}
