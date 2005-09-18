/* ---------------------------------------------------------------------------
  Function:	AVL Tree Object

  Service:      SUBSYSTEM

  Include:	tree.h

  Description:  An AVL tree (Adel'son-Vel'skii & Landis) is a binary search
		tree in which the heights of the left and right subtrees
		of the root differ by at most one and in which the left
		and right subtrees are again AVL trees.

		With each node of an AVL tree is associated a balance factor
		that is Left High, Equal, or Right High according,
		respectively, as the left subtree has height greater than,
		equal to, or less than that of the right subtree.

		The AVL tree is, in practice, balanced quite well.  It can
		(at the worst case) become skewed to the left or right,
		but never so much that it becomes inefficient.  The
		balancing is done as items are added or deleted.

		You will use two classes:

			Tree
			TreeIterator

  See also:	Robert L. Kruse, Data Structures and Program Design, 2nd Ed.
		Prentice-Hall

  Copyright (C) 1993 Michael D. Lore
  All Rights Reserved.
--------------------------------------------------------------------------- */

#include <cstring>
#include <cstdio>
#include "tree/tree.h"

using namespace std;

// --------------------------------------------------------------------------
//	Tree Object
// --------------------------------------------------------------------------


Node *Tree::findPredecessor(Node *node)
{
  if (node)
  {
    Node *pred = node->left;
    if (pred)
      while (pred->right)
	pred = pred->right;
    return pred;
  }
  return 0;
}

void Tree::remLeftSubBalance(Node *&node, int& shorter)
{
  Node *q = node->right;		// q: root of taller subtree
  Node *w;

  switch (q->bal)
  {
    case Equal:
      node->bal = RightHigh;
      q->bal = LeftHigh;	// q will be the new root node
      rotateLeft(node);
      shorter = 0;		// next level not shorter
      break;

    case RightHigh:
      node->bal = Equal;
      q->bal = Equal;   	// q will be the new root node
      rotateLeft(node);
      break;

    case LeftHigh:
      w = q->left;
      if (w->bal == LeftHigh)
	q->bal = RightHigh;
      else
	q->bal = Equal;
      if (w->bal == RightHigh)
	node->bal = LeftHigh;
      else
	node->bal = Equal;
      w->bal = Equal;		// w will be the new root node
      rotateRight(q);
      node->right = q;
      rotateLeft(node);
      break;
  }
}

void Tree::remRightSubBalance(Node *&node, int& shorter)
{
  Node *q = node->left;		// q: root of taller subtree
  Node *w;

  switch (q->bal)
  {
    case Equal:
      node->bal = LeftHigh;
      q->bal = RightHigh;	// q will be the new root node
      rotateRight(node);
      shorter = 0;		// next level not shorter
      break;

    case LeftHigh:
      node->bal = Equal;
      q->bal = Equal;   	// q will be the new root node
      rotateRight(node);
      break;

    case RightHigh:
      w = q->right;
      if (w->bal == RightHigh)
	q->bal = LeftHigh;
      else
	q->bal = Equal;
      if (w->bal == LeftHigh)
	node->bal = RightHigh;
      else
	node->bal = Equal;
      w->bal = Equal;		// w will be the new root node
      rotateLeft(q);
      node->left = q;
      rotateRight(node);
      break;
  }
}

void Tree::removePredecessor(Node *&node, int& shorter)
{
  if (node->right)
  {
    removePredecessor(node->right, shorter);

    if (shorter)	// left subtree was shortened
      remRightBalance(node, shorter);

    node->size = node->leftSize() + node->rightSize() + 1;
  }
  else
    remNode(node);
}

void Tree::remLeftBalance(Node *&node, int& shorter)
{
  switch (node->bal)
  {
    case Equal:			// one subtree shortened
      node->bal = RightHigh;		// now it's right high
      shorter = 0;			// overall tree same height
      break;

    case LeftHigh:		// taller subtree shortened
      node->bal = Equal;			// now it's equal
      break;

    case RightHigh:		// shorter subtree shortened
      remLeftSubBalance(node, shorter);
      break;
  }
}

void Tree::remRightBalance(Node *&node, int& shorter)
{
  switch (node->bal)
  {
    case Equal:			// one subtree shortened
      node->bal = LeftHigh;		// now it's left high
      shorter = 0;			// overall tree same height
      break;

    case RightHigh:		// taller subtree shortened
      node->bal = Equal;			// now it's equal
      break;

    case LeftHigh:		// shorter subtree shortened
      remRightSubBalance(node, shorter);
      break;
  }
}

void Tree::remNode(Node *&node)
{
  Node *temp = node;
  if (node->left)
    node = node->left;
  else if (node->right)
    node = node->right;
  else
    node = 0;
  nodeHeap.free((char *)temp);
}

Sortable *Tree::remove(Node *&node, int& shorter)
{
  shorter = 1;		// default: shorter
  Sortable *ptr = 0;

  int code = compare(*lookingFor, *(node->ptr));

  if (code < 0)
  {
    if (node->left)
    {
      ptr = remove(node->left, shorter);

      if (shorter  &&  ptr)	// left subtree was shortened
	remLeftBalance(node, shorter);
    }
  }
  else if (code > 0)
  {
    if (node->right)
    {
      ptr = remove(node->right, shorter);

      if (shorter  &&  ptr)	// left subtree was shortened
	remRightBalance(node, shorter);
    }
  }
  else
  {
    ptr = (Sortable *) node->ptr;

    if (node->left  &&  node->right)	// do the switch to find the prev.
    {					// node with only one subtree
      Node *pred = findPredecessor(node);
      node->ptr = pred->ptr;
      removePredecessor(node->left, shorter);

      if (shorter)	// left subtree was shortened
	remLeftBalance(node, shorter);
    }
    else				// we found the node; it has 1 subtree
      remNode(node);
  }

  if (ptr  &&  node)
    node->size = node->leftSize() + node->rightSize() + 1;

  return ptr;
}

Sortable *Tree::remove(const Sortable *ptr)
{
  if (ptr  &&  root)
  {
    lookingFor = ptr;
    int shorter;
    return remove(root, shorter);
  }

  return 0;
}

Sortable *Tree::remove(Node *&node, unsigned long index, int& shorter)
{
  shorter = 1;		// default: shorter
  Sortable *ptr = 0;

  if (index < node->leftSize())
  {
    if (node->left)
    {
      ptr = remove(node->left, index, shorter);

      if (shorter  &&  ptr)	// left subtree was shortened
	remLeftBalance(node, shorter);
    }
  }
  else if (index == node->leftSize())
  {
    ptr = (Sortable *) node->ptr;

    if (node->left  &&  node->right)	// do the switch to find the prev.
    {					// node with only one subtree
      Node *pred = findPredecessor(node);
      node->ptr = pred->ptr;
      removePredecessor(node->left, shorter);

      if (shorter)	// left subtree was shortened
	remLeftBalance(node, shorter);
    }
    else				// we found the node; it has 1 subtree
      remNode(node);
  }
  else
  {
    if (node->right)
    {
      ptr = remove(node->right, index - (node->leftSize() + 1), shorter);

      if (shorter  &&  ptr)	// left subtree was shortened
	remRightBalance(node, shorter);
    }
  }

  if (ptr  &&  node)
    node->size = node->leftSize() + node->rightSize() + 1;

  return ptr;
}

Sortable *Tree::remove(unsigned long index)
{
  if (root  &&  index < root->size)
  {
    int shorter;
    return remove(root, index, shorter);
  }

  return 0;
}


// End


