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
    return;
  }
  if (level == 0)
  {
    printf("木構造を降順で表示します。\n");
  }
  printRedBlackTree(root->right, level + 1);
  for (int i = 0; i < level; i++)
    printf("|\t");
  printf("%d %s \x1b[34m[%s%s%s]\x1b[0m\n", root->value, root->color == Red ? "\x1b[31m(Red)\x1b[0m" : "\x1b[37m(Black)\x1b[0m", root->left == NULL ? "" : "L", root->right == NULL ? "" : "R", root->parent == NULL ? "" : "P");
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

// ノードを回転させる
RedBlackNode *rotateNodes(RedBlackNode *source, NodeRotationDirection direction)
{
  if (source == NULL)
    return source;
  printf("回転します。\n");
  if (direction == ToLeft)
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

  if (direction == ToRight)
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
      RedBlackNode *newSource = rotateNodes(source, ToRight);
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
      rotateNodes(newNode->parent, ToRight);
      rotateNodes(newNode->parent, ToLeft);
      // 親に対しても修正を行う
      newNode = newNode->parent;
      continue;
    }

    // 3. 新規ノード > 親 かつ 親 < 祖父 の場合、親で左回転したのち、祖父で右回転を行い、元々の親ノードを黒色にする。
    if (newToParent == Less && parentToGrandParent == Greater)
    {
      newNode->parent->color = Black;
      rotateNodes(newNode->parent, ToLeft);
      rotateNodes(newNode->parent, ToRight);
      // 親に対しても修正を行う
      newNode = newNode->parent;
      continue;
    }

    // 4. 新規ノード > 親 > 祖父 の場合、左x回転を行い、新規ノードを黒色にする。
    if (newToParent == Greater && parentToGrandParent == Greater)
    {
      newNode->color = Black;
      RedBlackNode *source = newNode->parent->parent;
      RedBlackNode *newSource = rotateNodes(source, ToLeft);
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

int main(void)
{
  // 10, 20, 30, 40, 50, 60を順に挿入する。
  RedBlackNode *root = NULL;
  printf("\n# 10 を挿入します。\n");
  root = insertRedBlackNode(root, 10);
  printRedBlackTree(root, 0);
  printf("\n# 20 を挿入します。\n");
  root = insertRedBlackNode(root, 20);
  printRedBlackTree(root, 0);
  printf("\n# 30 を挿入します。\n");
  root = insertRedBlackNode(root, 30);
  printRedBlackTree(root, 0);
  printf("\n# 40 を挿入します。\n");
  root = insertRedBlackNode(root, 40);
  printRedBlackTree(root, 0);
  printf("\n# 50 を挿入します。\n");
  root = insertRedBlackNode(root, 50);
  printRedBlackTree(root, 0);
  printf("\n# 60 を挿入します。\n");
  root = insertRedBlackNode(root, 60);
  printRedBlackTree(root, 0);

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
    printf("探索に失敗しました。\n");
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
    printf("探索に失敗しました。\n");
  }
}