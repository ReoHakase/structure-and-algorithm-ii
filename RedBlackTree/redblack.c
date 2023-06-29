#include "redblack.h"
#include <stdio.h>
#include <stdlib.h>

RedBlackNode *allocateRedBlackNode(int value, RedBlackNodeColor color)
{
  RedBlackNode *node = (RedBlackNode *)malloc(sizeof(RedBlackNode));
  if (node == NULL)
  {
    printf("新規ノードのメモリ割り当てに失敗しました。");
    exit(1);
  }
  node->value = value;
  node->color = color;
  node->left = NULL;
  node->right = NULL;
  node->parent = NULL;
  return node;
}

// 指定されたノードとその子孫を全て解放する
// 指定された親のノードが持つ参照も削除する
void freeRedBlackNode(RedBlackNode *node)
{
  if (node == NULL)
  {
    return;
  }

  // 子に対して再起呼び出し
  freeRedBlackNode(node->left);
  freeRedBlackNode(node->right);

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

void printRedBlackTree(RedBlackNode *root, int level)
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
  printRedBlackTree(root->right, level + 1);
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
  }
  if (root->right != NULL)
  {
    sprintf(rightDisplay, "R%d", root->right->value);
  }
  if (root->parent != NULL)
  {
    sprintf(parentDisplay, "P%d", root->parent->value);
  }
  printf("%d %s \x1b[34m[%s %s %s]\x1b[0m\n", root->value, root->color == Red ? "\x1b[31m(Red)\x1b[0m" : "\x1b[37m(Black)\x1b[0m", leftDisplay, rightDisplay, parentDisplay);
  printRedBlackTree(root->left, level + 1);
}

/* target ノードが basis ノードに対して大きいか小さいか等しいかを調べる */
NodeComparisonResult compareNode(RedBlackNode *basis, RedBlackNode *target)
{
  if (target->value == basis->value)
  {
    return Equal;
  }
  else if (target->value < basis->value)
  {
    return Less;
  }
  else
  {
    return Greater;
  }
}

// ノードを方向 direction に向かって回転させる
// もし回転前の source が木構造の根だったら、回転後の根を返す
// そうでなかったら、 NULL を返す
RedBlackNode *rotateNodes(RedBlackNode *source, NodeDirection direction)
{
  if (source == NULL)
    return source;
  // printf("回転します。\n");
  if (direction == Left)
  {
    // 左回転の場合、右ノードが存在しないと回転できない
    if (source->right == NULL)
    {
      printf("右のノードが存在しないため、左回転できません。\n");
      return source;
    }
    RedBlackNode *rightNode = source->right;
    RedBlackNode *rightLeftNode = rightNode != NULL ? rightNode->left : NULL;
    RedBlackNode *parent = source->parent;
    source->right = rightLeftNode;
    rightNode->left = source;
    source->parent = rightNode;
    rightNode->parent = parent;
    if (rightLeftNode != NULL)
    {
      rightLeftNode->parent = source;
    }
    if (parent != NULL)
    {
      if (parent->left == source)
      {
        parent->left = rightNode;
      }
      else
      {
        parent->right = rightNode;
      }
    }
    return rightNode;
  }

  if (direction == Right)
  {
    // 右回転の場合、左ノードが存在しないと回転できない
    if (source->left == NULL)
    {
      printf("左のノードが存在しないため、右回転できません。\n");
      return source;
    }
    RedBlackNode *leftNode = source->left;
    RedBlackNode *leftRightNode = leftNode != NULL ? leftNode->right : NULL;
    RedBlackNode *parent = source->parent;
    source->left = leftRightNode;
    leftNode->right = source;
    source->parent = leftNode;
    leftNode->parent = parent;
    if (leftRightNode != NULL)
    {
      leftRightNode->parent = source;
    }
    if (parent != NULL)
    {
      if (parent->left == source)
      {
        parent->left = leftNode;
      }
      else
      {
        parent->right = leftNode;
      }
    }
    return leftNode;
  }
  return source;
}

RedBlackNode *insertRedBlackNode(RedBlackNode *root, int value)
{
  // root が NULL である場合、つまり木にノードが存在しない場合、新規ノードを作成して root に設定する。
  if (root == NULL)
  {
    return allocateRedBlackNode(value, Black);
  }

  // root が NULL でない場合、挿入位置を探索する。
  // 必ず葉として赤色で挿入される。
  RedBlackNode *newNode = allocateRedBlackNode(value, Red);
  RedBlackNode *currentNode = root;
  while (1)
  {
    NodeComparisonResult result = compareNode(currentNode, newNode);
    if (result == Equal)
    {
      // 既に同じ値が挿入されている場合、挿入を行わずに終了する。
      freeRedBlackNode(newNode);
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
        break;
      }
      else
      {
        // 右側に挿入不可能な場合、右側のノードを探索する。
        currentNode = currentNode->right;
      }
    }
  }

  // 修正は親に伝播する。
  while (newNode->parent != NULL)
  {
    // デバッグ用
    // printf("現在の木構造: ");
    // printRedBlackTree(root, 0);
    // printf("色修正の対象になるノード: ");
    // printRedBlackTree(newNode, 0);

    // 挿入後の木が色の制約を満たしているかを調べる。
    // 調べる対象は新規ノードとその親ノードである。
    // もし親ノードが黒だったら、木の色の制約を満たしているので終了する。
    if (newNode->parent->color == Black)
      break;

    // 親ノードが赤だったら、木の色の制約を満たすように修正する。
    // 親が根の場合、親を黒にする。
    if (newNode->parent == root)
    {
      newNode->parent->color = Black;
      break;
    }

    // 親が根でない場合、祖先との大小関係に合わせて回転を行う。
    // この時、実行前の木は色制約を満たしているから、それぞれ親は赤、祖父は黒である。

    NodeComparisonResult newToParent = compareNode(newNode->parent, newNode);
    NodeComparisonResult parentToGrandParent = compareNode(newNode->parent->parent, newNode->parent);

    // 1. 新規ノード < 親 < 祖父 の場合、右回転を行い、新規ノードを黒色にする。
    if (newToParent == Less && parentToGrandParent == Less)
    {
      newNode->color = Black;
      RedBlackNode *source = newNode->parent->parent;
      RedBlackNode *newSource = rotateNodes(source, Right);
      if (source == root)
      {
        // 根を回転させた場合、新しい根を設定する。
        root = newSource;
        // 新しい根を黒に設定する
        root->color = Black;
      }
      // 親に対しても修正を行う
      newNode = newNode->parent;
      continue;
    }

    // 2. 新規ノード < 親 かつ 親 > 祖父 の場合、親で右回転したのち、祖父で左回転を行い、元々の親ノードを黒色にする。
    if (newToParent == Less && parentToGrandParent == Greater)
    {
      newNode->parent->color = Black;
      rotateNodes(newNode->parent, Right);
      rotateNodes(newNode->parent, Left);
      // 親に対しても修正を行う
      newNode = newNode;
      continue;
    }

    // 3. 新規ノード > 親 かつ 親 < 祖父 の場合、親で左回転したのち、祖父で右回転を行い、元々の親ノードを黒色にする。
    if (newToParent == Greater && parentToGrandParent == Less)
    {
      newNode->parent->color = Black;
      rotateNodes(newNode->parent, Left);
      rotateNodes(newNode->parent, Right);
      // 親に対しても修正を行う
      newNode = newNode;
      continue;
    }

    // 4. 新規ノード > 親 > 祖父 の場合、左x回転を行い、新規ノードを黒色にする。
    if (newToParent == Greater && parentToGrandParent == Greater)
    {
      newNode->color = Black;
      RedBlackNode *source = newNode->parent->parent;
      RedBlackNode *newSource = rotateNodes(source, Left);
      if (source == root)
      {
        // 根を回転させた場合、新しい根を設定する。
        root = newSource;
        // 新しい根を黒に設定する
        root->color = Black;
      }
      // 親に対しても修正を行う
      newNode = newNode->parent;
      continue;
    }
  }

  return root;
}

// 値 value を持つノードを探査する
// 見つかった場合、その値を含むノードを返す
// 見つからなかった場合、 NULL を返す
RedBlackNode *searchRedBlackNode(RedBlackNode *root, int value)
{
  if (root == NULL)
  {
    return NULL;
  }
  RedBlackNode *queryNode = allocateRedBlackNode(value, Black);
  RedBlackNode *currentNode = root;
  while (1)
  {
    NodeComparisonResult queryToCurrent = compareNode(currentNode, queryNode);
    if (queryToCurrent == Equal)
    {
      // 探査対象のノードを発見した場合、そのノードを返す。
      freeRedBlackNode(queryNode);
      return currentNode;
    }
    else if (queryToCurrent == Less)
    {
      // 探査対象のノードが現在のノードより小さい場合、左側のノードを探索する。
      if (currentNode->left == NULL)
      {
        // 左側に探査対象のノードが存在しない場合、探査失敗として NULL を返す。
        freeRedBlackNode(queryNode);
        return NULL;
      }
      else
      {
        // 左側に探査対象のノードが存在する場合、左側のノードを探索する。
        currentNode = currentNode->left;
      }
    }
    else
    {
      // 探査対象のノードが現在のノードより大きい場合、右側のノードを探索する。
      if (currentNode->right == NULL)
      {
        // 右側に探査対象のノードが存在しない場合、探査失敗として NULL を返す。
        freeRedBlackNode(queryNode);
        return NULL;
      }
      else
      {
        // 右側に探査対象のノードが存在する場合、右側のノードを探索する。
        currentNode = currentNode->right;
      }
    }
  }
  return NULL;
}

// 二つのノードの値だけを入れ替える
// もしどちらかが存在しなかったら、何もしない
void swapRedBlackNodeValue(RedBlackNode *a, RedBlackNode *b)
{
  if (a == NULL || b == NULL)
    return;

  RedBlackNode *temp = allocateRedBlackNode(a->value, a->color);
  a->value = b->value;
  // a->color = b->color;
  b->value = temp->value;
  // b->color = temp->color;

  return;
}

// ノードの子の参照を更新する
// 双方向リストのように、親の参照を更新する
void updateRedBlackNodeChild(RedBlackNode *parent, RedBlackNode *newChild, NodeDirection variant)
{
  if (parent == NULL || newChild == NULL)
    return;

  // 親の左側の子を更新する場合
  if (variant == Left)
  {
    parent->left->parent = NULL;
    parent->left = newChild;
    newChild->parent = parent;
    return;
  }

  // 親の右側の子を更新する場合
  if (variant == Right)
  {
    parent->right->parent = NULL;
    parent->right = newChild;
    newChild->parent = parent;
    return;
  }
}

// 与えられたノードが、その親ノードにとって左側の子か右側の子かを返す
// 親ノードが存在しない場合、 0を返す
NodeDirection getRedBlackNodeVariant(RedBlackNode *child)
{
  if (child == NULL || child->parent == NULL)
  {
    return 0;
  }
  if (child->parent->left == child)
  {
    return Left;
  }
  if (child->parent->right == child)
  {
    return Right;
  }
  return 0;
}

// 与えられたノードの子孫のうち、指定された方向にある最も近いノードを返す
RedBlackNode *getClosestDecendant(RedBlackNode *node, NodeDirection variant)
{
  if (node == NULL)
    return NULL;

  if (variant == Left)
  {
    if (node->left == NULL)
      return node;
    RedBlackNode *closestNode = node->left;
    while (closestNode->right != NULL)
    {
      closestNode = closestNode->right;
    }
    return closestNode;
  }

  if (variant == Right)
  {
    if (node->right == NULL)
      return node;
    RedBlackNode *closestNode = node->right;
    while (closestNode->left != NULL)
    {
      closestNode = closestNode->left;
    }
    return closestNode;
  }

  return NULL;
}

// 値を含むノードを削除した後の木構造の新しい根を返す
RedBlackNode *deleteRedBlackNode(RedBlackNode *root, int value)
{
  RedBlackNode *targetNode = searchRedBlackNode(root, value);
  RedBlackNode *affectedSubtreeRoot = NULL;
  RedBlackNode *nodeToBeDeletedAfterward = NULL;

  while (targetNode != NULL)
  {
    // もし削除対象が存在しなかったら、何もしない
    if (targetNode == NULL)
      return root;

    RedBlackNodeColor targetColor = targetNode->color;

    // 1. 通常の二分探査木の削除と同様に、削除対象のノードを削除する。

    // 1.1 削除対象のノードが葉な場合
    if (targetNode->left == NULL && targetNode->right == NULL)
    {
      // 削除対象のノードが子を持たない場合、そのノードを削除するだけで終了する。
      // もし対象が根の場合は、新しい根を NULL にする。
      if (targetNode == root)
      {
        root = NULL;
      }
      if (targetColor == Black)
      {
        if (targetNode->parent != NULL && targetNode->parent->color == Red)
        {
          // printf("末端の黒ノードを削除します");
          printRedBlackTree(targetNode, 0);
          RedBlackNode *newSource = rotateNodes(targetNode->parent, getRedBlackNodeVariant(targetNode));
          if (targetNode->parent == root)
          {
            root = newSource;
          }
        }
      }
      freeRedBlackNode(targetNode);
      break;
    }

    // 1.2. 削除対象のノードが子を一つ右側だけに持つ場合
    else if (targetNode->left == NULL)
    {
      // 削除対象のノードが左側の子を持たない場合、右側の子を削除対象のノードの位置に移動させる。
      if (targetNode == root)
      {
        // もし対象が根の場合は、新しい根を右側の子にする。
        RedBlackNode *oldRoot = root;
        root = oldRoot->right;
        root->color = Black;
        root->parent = NULL;
        // 参照が切れた削除対象のノードを削除する。
        oldRoot->right = NULL;
        freeRedBlackNode(oldRoot);
        break;
      }
      else
      {
        // もし対象が根でない場合は、削除対象のノードの親の子を右側の子にする。
        NodeDirection targetNodeVariant = getRedBlackNodeVariant(targetNode);
        updateRedBlackNodeChild(targetNode->parent, targetNode->right, targetNodeVariant);
        // 黒ノードが足りなくなった部分木の根を記録する。
        if (targetColor == Black)
        {
          affectedSubtreeRoot = targetNode->right;
        }
        // 参照が切れた削除対象のノードを削除する。
        targetNode->right = NULL;
        freeRedBlackNode(targetNode);
        break;
      }
    }

    // 1.3. 削除対象のノードが子を一つ左側だけに持つ場合
    else if (targetNode->right == NULL)
    {
      // 削除対象のノードが右側の子を持たない場合、左側の子を削除対象のノードの位置に移動させる。
      if (targetNode == root)
      {
        // もし対象が根の場合は、新しい根を左側の子にする。
        RedBlackNode *oldRoot = root;
        root = oldRoot->left;
        root->color = Black;
        root->parent = NULL;
        // 参照が切れた削除対象のノードを削除する。
        oldRoot->left = NULL;
        freeRedBlackNode(oldRoot);
        break;
      }
      else
      {
        // もし対象が根でない場合は、削除対象のノードの親の子を左側の子にする。
        NodeDirection targetNodeVariant = getRedBlackNodeVariant(targetNode);
        updateRedBlackNodeChild(targetNode->parent, targetNode->left, targetNodeVariant);
        // 黒ノードが足りなくなった部分木の根を記録する。
        if (targetColor == Black)
        {
          affectedSubtreeRoot = targetNode->left;
        }
        // 参照が切れた削除対象のノードを削除する。
        targetNode->left = NULL;
        freeRedBlackNode(targetNode);
        break;
      }
    }

    // 1.4. 削除対象のノードが2つの子を持つ場合
    else
    {
      // printf("削除対象のノードが2つの子を持つ場合\n");
      // 削除対象のノードの左側の子孫のうち、最も近いノードの値を削除対象のノードの値と交換する。
      // 最も値の近い子孫を探すときの、最初の方向
      NodeDirection decendantSearchDirection = Left;
      RedBlackNode *closestNode = getClosestDecendant(targetNode, decendantSearchDirection); // 左右どっちでもいい
      swapRedBlackNodeValue(targetNode, closestNode);
      // 最も近いノードを削除する。この時、最も近いノードは絶対に子を一つしか持たない。
      targetNode = closestNode;
      continue;
    }
  }

  // 2. 色の修正が必要な場合、修正を行う。
  for (int iterationCount = 0; affectedSubtreeRoot != NULL && iterationCount < 5; iterationCount++)
  {
    // 黒ノードが足りない部分木の根の方向
    NodeDirection rootVariant = getRedBlackNodeVariant(affectedSubtreeRoot);
    NodeDirection innerDirection = rootVariant;
    NodeDirection outerDirection = invertNodeDirection(innerDirection);
    // 黒ノードが足りない部分木の根の親 α
    RedBlackNode *parent = affectedSubtreeRoot->parent;
    // 黒ノードが足りない部分木の根の兄弟 β
    RedBlackNode *sibling = getRedBlackNodeSibling(affectedSubtreeRoot);
    // 黒ノードが足りない部分木の根の兄弟の子のうち、値が近い方 γ
    RedBlackNode *innerNephew = getRedBlackNodeChild(sibling, innerDirection);
    // RedBlackNode *innerNephew = NULL;
    // 黒ノードが足りない部分木の根の兄弟の子のうち、値が遠い方 δ
    RedBlackNode *outerNephew = getRedBlackNodeChild(sibling, outerDirection);
    // RedBlackNode *outerNephew = NULL;

    // // デバッグ用
    // printf(">> 現在の木構造: ");
    // printRedBlackTree(root, 0);
    // printf("黒ノードが不足している部分木: ");
    // printRedBlackTree(affectedSubtreeRoot, 0);
    // printf("rootVariant: %s\n", rootVariant == 0 ? "None (0)" : rootVariant == Left ? "Left"
    //                                                                                 : "Right");
    // printf("innnerDirection: %s\n", innerDirection == 0 ? "None (0)" : innerDirection == Left ? "Left"
    //                                                                                           : "Right");
    // printf("outerDirection: %s\n", outerDirection == 0 ? "None (0)" : outerDirection == Left ? "Left"
    //                                                                                          : "Right");
    // printf("parent: ");
    // printRedBlackTree(parent, 0);
    // printf("sibling: ");
    // printRedBlackTree(sibling, 0);
    // printf("innerNephew: ");
    // printRedBlackTree(innerNephew, 0);
    // printf("outerNephew: ");
    // printRedBlackTree(outerNephew, 0);

    // もし黒ノードが足りない部分木が全体の根だったら、黒ノードを足すだけで終了する。
    if (parent == NULL)
    {
      // printf("もし黒ノードが足りない部分木が全体の根だったら、黒ノードを足すだけで終了する。\n");
      affectedSubtreeRoot->color = Black; // これ必要？
      break;
    }

    // 1. 兄弟ノード β が黒の場合
    if (sibling != NULL && sibling->color == Black)
    {
      // printf("1. 兄弟ノード β が黒の場合");
      // 1.1. 兄弟の値が近い方の子 γ が赤の場合
      if (innerNephew != NULL && innerNephew->color == Red)
      {
        // printf(" 1.1. 兄弟の値が近い方の子 γ が赤の場合\n");
        RedBlackNodeColor originalParentColor = parent->color;
        // 兄弟ノード β を外側へ回転する
        RedBlackNode *newSourceBeta = rotateNodes(sibling, outerDirection);
        if (sibling == root)
        {
          root = newSourceBeta;
        }
        // 親ノード α を内側へ回転する
        RedBlackNode *newSourceAlpha = rotateNodes(parent, innerDirection);
        if (parent == root)
        {
          root = newSourceAlpha;
        }
        // 色を更新する
        innerNephew->color = originalParentColor; // 今、値が近い方 γ は、元々の親ノード α の位置に移動している
        parent->color = Black;
        // 黒ノードが足りなくなった部分木の根を記録する。
        affectedSubtreeRoot = NULL;
        continue;
      }

      // 1.2. 兄弟の値が遠い方の子 δ が赤の場合
      if (outerNephew != NULL && outerNephew->color == Red)
      {
        // printf(" 1.2. 兄弟の値が遠い方の子 δ が赤の場合\n");
        RedBlackNodeColor originalParentColor = parent->color;
        // 親ノード α を内側へ回転する
        RedBlackNode *newSource = rotateNodes(parent, innerDirection);
        if (parent == root)
        {
          root = newSource;
        }
        // 色を更新する
        sibling->color = originalParentColor;
        parent->color = Black;
        // 黒ノードが足りなくなった部分木の根を記録する。
        affectedSubtreeRoot = NULL;
        continue;
      }
    }

    // 2. 兄弟ノード β が黒で、尚且つその2つの子 γ, δ が全て赤でない場合
    if ((sibling != NULL && sibling->color == Black) && (innerNephew == NULL || innerNephew->color == Black) && (outerNephew == NULL || outerNephew->color == Black))
    {
      // printf("2. 兄弟ノード β とその2つの子 γ, δ が全て黒の場合\n");
      // 2.1. 親ノード α が赤の場合
      if (parent->color == Red)
      {
        // printf(" 2.1. 親ノード α が赤の場合\n");
        // 色を更新する
        sibling->color = Red;
        parent->color = Black;
        // 黒ノードが足りなくなった部分木の根を記録する。
        affectedSubtreeRoot = NULL;
        continue;
      }

      // 2.2. 親ノード α が黒の場合
      if (parent->color == Black)
      {
        // printf(" 2.2. 親ノード α が黒の場合\n");
        // 色を更新する
        sibling->color = Red;
        // 黒ノードが足りなくなった部分木の根を記録する。
        affectedSubtreeRoot = parent;
        continue;
      }
    }

    // 3. 親ノード α が黒で、兄弟ノード β が赤の場合
    if (parent->color == Black && (sibling != NULL && sibling->color == Red))
    {
      // printf(" 3. 親ノード α が黒で、兄弟ノード β が赤の場合\n");
      // 親ノード α を内側へ回転する
      RedBlackNode *newSource = rotateNodes(parent, innerDirection);
      if (parent == root)
      {
        root = newSource;
      }
      // 色を更新する
      parent->color = Red;
      sibling->color = Black;
      // 同じ部分木に対してもう一度修正を行う
      continue;
    }
  }

  if (nodeToBeDeletedAfterward != NULL)
  {
    printf("削除予約されているノード: ");
    printRedBlackTree(nodeToBeDeletedAfterward, 0);
    freeRedBlackNode(nodeToBeDeletedAfterward);
  }

  return root;
}

// 与えられた方向と逆の方向を返す
NodeDirection invertNodeDirection(NodeDirection variant)
{
  if (variant == Left)
  {
    return Right;
  }
  if (variant == Right)
  {
    return Left;
  }
  return 0;
};

// 与えられたのノードの子を返す
RedBlackNode *getRedBlackNodeChild(RedBlackNode *node, NodeDirection variant)
{
  if (node == NULL)
    return NULL;
  if (variant == Left)
  {
    return node->left;
  }
  if (variant == Right)
  {
    return node->right;
  }
  return NULL;
}

// 与えられたノードの兄弟を返す
RedBlackNode *getRedBlackNodeSibling(RedBlackNode *node)
{
  if (node == NULL)
    return NULL;
  if (node->parent == NULL)
    return NULL;
  return getRedBlackNodeChild(node->parent, invertNodeDirection(getRedBlackNodeVariant(node)));
}

int main(void)
{
  // 60, 40, 30, 10, 50, 20 の順で挿入
  RedBlackNode *root = NULL;

  int insertedValues[] = {60, 40, 30, 10, 50, 20};
  for (int i = 0; i < 6; i++)
  {
    printf("\n# %d を挿入します。\n", insertedValues[i]);
    root = insertRedBlackNode(root, insertedValues[i]);
    printf("挿入しました。");
    printRedBlackTree(root, 0);
  }

  // 30, 55 を探索する。
  printf("\n# 30 を探索します。\n");
  RedBlackNode *node30 = searchRedBlackNode(root, 30);
  if (node30 != NULL)
  {
    printf("探索に成功しました。");
    printRedBlackTree(node30, 0);
  }
  else
  {
    printf("\x1b[31m探索に失敗しました。\x1b[0m\n");
  }

  printf("\n# 55 を探索します。\n");
  RedBlackNode *node55 = searchRedBlackNode(root, 55);
  if (node55 != NULL)
  {
    printf("探索に成功しました。");
    printRedBlackTree(node55, 0);
  }
  else
  {
    printf("\x1b[31m探索に失敗しました。\x1b[0m\n");
  }

  // 20, 40, 60, 30, 50, 10の順で削除
  int deletedValues[] = {20, 40, 60, 30, 50, 10};
  for (int i = 0; i < 6; i++)
  {
    printf("\n# %d を削除します。\n", deletedValues[i]);
    root = deleteRedBlackNode(root, deletedValues[i]);
    printRedBlackTree(root, 0);
  }
}
