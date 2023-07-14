#include <stdio.h>
#include "avl.h"

NodeComparisonResult compareNode(AVLNode *basis, AVLNode *target)
{
  if (target == NULL || basis == NULL)
  {
    printf("compareNode: NULL node is passed\n");
    return Equal;
  }
  if (target->value > basis->value)
  {
    return Greater;
  }
  else if (target->value < basis->value)
  {
    return Less;
  }
  return Equal;
}

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
  return None;
};

NodeDirection getAVLNodeVariant(AVLNode *node)
{
  if (node->parent == NULL)
  {
    return None;
  }
  if (node->parent->left == node)
  {
    return Left;
  }
  if (node->parent->right == node)
  {
    return Right;
  }
  return None;
}

void updateRedBlackNodeChild(AVLNode *parent, AVLNode *newChild, NodeDirection variant)
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

AVLNode *getClosestDecendant(AVLNode *node, NodeDirection variant)
{
  if (node == NULL)
    return NULL;

  if (variant == Left)
  {
    if (node->left == NULL)
      return node;
    AVLNode *closestNode = node->left;
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
    AVLNode *closestNode = node->right;
    while (closestNode->left != NULL)
    {
      closestNode = closestNode->left;
    }
    return closestNode;
  }

  return NULL;
}

void swapAVLNodeValue(AVLNode *node1, AVLNode *node2)
{
  if (node1 == NULL || node2 == NULL)
  {
    printf("swapAVLNodeValue: NULL node is passed\n");
    return;
  }
  int tmp = node1->value;
  node1->value = node2->value;
  node2->value = tmp;
}

AVLNode *rotateNodes(AVLNode *source, NodeDirection direction)
{
  if (source == NULL)
    return source;
  // printf("回転します。\n");
  if (direction == Left)
  {
    // 左回転の場合、右ノードが存在しないと回転できない
    if (source->right == NULL)
    {
      printf("[rotateNodes] 右のノードが存在しないため、左回転できません。\n");
      return source;
    }
    AVLNode *rightNode = source->right;
    AVLNode *rightLeftNode = rightNode != NULL ? rightNode->left : NULL;
    AVLNode *parent = source->parent;
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
    // 高さと平衡定数を更新する
    updateAVLNodeBalance(source);
    updateAVLNodeHeight(source);
    // 右ノードの平衡定数の更新は、すでに行われているため、高さの更新のみ行う
    // 親ノードの高さと平衡定数の更新を強制する
    rightNode->height = -1;
    updateAVLNodeHeight(rightNode);

    return rightNode;
  }

  if (direction == Right)
  {
    // 右回転の場合、左ノードが存在しないと回転できない
    if (source->left == NULL)
    {
      printf("[rotateNodes] 左のノードが存在しないため、右回転できません。\n");
      return source;
    }
    AVLNode *leftNode = source->left;
    AVLNode *leftRightNode = leftNode != NULL ? leftNode->right : NULL;
    AVLNode *parent = source->parent;
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
    // 高さと平衡定数を更新する
    updateAVLNodeBalance(source);
    updateAVLNodeHeight(source);
    // 左ノードの平衡定数の更新は、すでに行われているため、高さの更新のみ行う
    // 親ノードの高さと平衡定数の更新を強制する
    leftNode->height = -1;
    updateAVLNodeHeight(leftNode);

    return leftNode;
  }
  return source;
}

AVLNode *getAVLNodeSibling(AVLNode *node)
{
  if (node == NULL)
  {
    return NULL;
  }

  NodeDirection nodeVariant = getAVLNodeVariant(node);
  if (nodeVariant == Left)
  {
    return node->right;
  }
  if (nodeVariant == Right)
  {
    return node->left;
  }
  return NULL;
}