/* ---------------------------------------------------------------------------
  Function:	AVL Tree Object

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
//	Node Object
// --------------------------------------------------------------------------

void Node::init(const Sortable *obj)
{
  ptr = obj;
  left = right = 0;
  bal = Equal;
  size = 1;
}

void Node::kill()
{
  delete (Sortable *) ptr;
}


// --------------------------------------------------------------------------
//	Tree Object
// --------------------------------------------------------------------------

int Tree::SortableCompare(const Sortable& a, const Sortable& b)
{
  return a.compare(b);
}

Tree::Tree(int flags, CompareFunc func, unsigned pageCount) :
	nodeHeap(sizeof(Node), pageCount),
	treeFlags(flags), compare(func),
	root(0), currentLevel(0), maxLevel(0)
{
}

Tree::~Tree()
{
  flush();
}

const Sortable *Tree::add(Node *&node, int& taller)
{
  taller = 0;	// default: not taller
  const Sortable *ptr = 0;

  if (!node)
  {
    node = newNode(lookingFor);
    taller = 1;
    if (node)
      ptr = node->ptr;
  }
  else
  {
    int tallerSubTree = 0;

    int code = compare(*lookingFor, *(node->ptr));

    if (code == 0  &&  (treeFlags & AllowDuplicates))
      code = -1;	// go left for duplicates

    if (code < 0)
    {
      ptr = add(node->left, tallerSubTree);
      if (ptr)
      {
	if (tallerSubTree)
	  switch (node->bal)
	  {
	    case LeftHigh:
	      leftBalance(node, taller);
	      break;

	    case Equal:
	      node->bal = LeftHigh;
	      taller = 1;
	      break;

	    case RightHigh:
	      node->bal = Equal;
	      taller = 0;
	      break;
	  }
      }
    }
    else if (code > 0)
    {
      ptr = add(node->right, tallerSubTree);
      if (ptr)
      {
	if (tallerSubTree)
	  switch (node->bal)
	  {
	    case LeftHigh:
	      node->bal = Equal;
	      taller = 0;
	      break;

	    case Equal:
	      node->bal = RightHigh;
	      taller = 1;
	      break;

	    case RightHigh:
	      rightBalance(node, taller);
	      break;
	  }
      }
    }
    else
      duplicate = node->ptr;	// this node is the duplicate

    if (ptr)
      node->size = node->leftSize() + node->rightSize() + 1;
  }

  return ptr;
}

void Tree::rightBalance(Node *&node, int& taller)
{
  Node *x,		// right subtree of node
       *w;		// left subtree of x

  x = node->right;

  switch (x->bal)
  {
    case RightHigh:
      node->bal = Equal;
      x->bal = Equal;
      rotateLeft(node);
      taller = 0;
      break;

    case Equal:		// impossible
      break;

    case LeftHigh:
      w = x->left;
      switch (w->bal)
      {
	case Equal:
	  node->bal = Equal;
	  x->bal = Equal;
	  break;

	case LeftHigh:
	  node->bal = Equal;
	  x->bal = RightHigh;
	  break;

	case RightHigh:
	  node->bal = LeftHigh;
	  x->bal = Equal;
	  break;
      }
      w->bal = Equal;
      rotateRight(x);
      node->right = x;
      rotateLeft(node);
      taller = 0;
      break;
  }
}

void Tree::leftBalance(Node *&node, int& taller)
{
  Node *x,		// left subtree of node
       *w;		// right subtree of x

  x = node->left;

  switch (x->bal)
  {
    case LeftHigh:
      node->bal = Equal;
      x->bal = Equal;
      rotateRight(node);
      taller = 0;
      break;

    case Equal:		// impossible
      break;

    case RightHigh:
      w = x->right;
      switch (w->bal)
      {
	case Equal:
	  node->bal = Equal;
	  x->bal = Equal;
	  break;

	case RightHigh:
	  node->bal = Equal;
	  x->bal = LeftHigh;
	  break;

	case LeftHigh:
	  node->bal = RightHigh;
	  x->bal = Equal;
	  break;
      }
      w->bal = Equal;
      rotateLeft(x);
      node->left = x;
      rotateRight(node);
      taller = 0;
      break;
  }
}

void Tree::rotateLeft(Node *&node)
{
  if (node  &&  node->right)
  {
    Node *ptr = node->right;
    node->size = node->leftSize() + ptr->leftSize() + 1;
    ptr->size = node->size + ptr->rightSize() + 1;
    node->right = ptr->left;
    ptr->left = node;
    node = ptr;
  }
}

void Tree::rotateRight(Node *&node)
{
  if (node  &&  node->left)
  {
    Node *ptr = node->left;
    node->size = node->rightSize() + ptr->rightSize() + 1;
    ptr->size = node->size + ptr->leftSize() + 1;
    node->left = ptr->right;
    ptr->right = node;
    node = ptr;
  }
}

const Sortable *Tree::add(const Sortable *ptr, const Sortable **dupl)
{
  if (dupl) *dupl = 0;

  if (ptr)
  {
    lookingFor = ptr;
    duplicate = 0;
    int taller;
    const Sortable *found = add(root, taller);
    if (dupl) *dupl = duplicate;
    return found;
  }

  return 0;
}

void Tree::flush()
{
  if (root  &&  (treeFlags & OwnsElements))
    flusher(root);
  root = 0;
  maxLevel = 0;
  nodeHeap.freeAll();
}

void Tree::flusher(Node *node)
{
  if (node->left)
    flusher(node->left);
  if (node->right)
    flusher(node->right);
  node->kill();
}

Node *Tree::newNode(const Sortable *obj)
{
  if (obj)
  {
    Node *node = (Node *) nodeHeap.alloc();

    if (node)
    {
      node->init(obj);
      return node;
    }
  }
  return 0;
}

void Tree::levelCalc(Node *node) const
{
  const_cast<Tree*>(this)->currentLevel++;

  if (node->left)
    levelCalc(node->left);

  if (node->right)
    levelCalc(node->right);

  if (currentLevel > maxLevel)
    const_cast<Tree*>(this)->maxLevel = currentLevel;

  const_cast<Tree*>(this)->currentLevel--;
}

unsigned Tree::getMaxLevel() const
{
  const_cast<Tree*>(this)->currentLevel = 0;
  const_cast<Tree*>(this)->maxLevel = 0;

  if (root)
    levelCalc(root);

  return maxLevel;
}

Node *Tree::indexer(Node *node, unsigned long index) const
{
  if (index < node->leftSize())
    return indexer(node->left, index);
  else if (index == node->leftSize())
    return node;
  else if (node->right)
    return indexer(node->right, index - (node->leftSize() + 1));

  return 0;
}

const Sortable *Tree::operator [] (unsigned long index) const
{
  if (root  &&  index < root->size)
  {
    Node *node = indexer(root, index);
    if (node)
      return node->ptr;
  }

  return 0;
}


// End


