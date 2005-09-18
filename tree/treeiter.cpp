/* ---------------------------------------------------------------------------
  Function:	AVL Tree Iterator Object

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

#include <cstdlib>
#include <cstring>

#include "tree/tree.h"

using namespace std;

// --------------------------------------------------------------------------
//	TreeIterator Object
// --------------------------------------------------------------------------

NodeHolder::NodeHolder(Node *nod) : visited(VisitNone), node(nod)
{
  if (!node)
    visited |= (VisitLeft | VisitThis | VisitRight);
  else
  {
    if (!node->left)
      visited |= VisitLeft;
    if (!node->right)
      visited |= VisitRight;
  }
}

TreeIterator::TreeIterator(Tree& aTree, int direc) :
				tree(aTree), direction(direc),
				stack(0), stackSize(0), stackPtr(0)
{
  reset();
}

TreeIterator::~TreeIterator()
{
  if (stack)
    delete [] stack;
}

void TreeIterator::reset()
{
  // if the # levels has changed, delete current stack if there is one

  if (stackSize != tree.getMaxLevel())
  {
    if (stack)
    {
      delete [] stack;
      stack = 0;
    }
  }

  // build a new stack if one does not exist

  stackPtr = 0;
  stackSize = tree.getMaxLevel();
  if (stackSize  &&  !stack) {
    try {
      stack = new NodeHolder [stackSize];
    }
    catch (...) {
      stack = NULL;
    }
    if (stack == NULL)
		throw AppException (WHERE, ERR_OUT_OF_MEMORY);
  }

  // preset the stack to point to the first tree element

  if (stack)
  {
    if (tree.root)
    {
      NodeHolder nh(tree.root);
      push(nh);
      this->operator ++ (1);
    }
  }
}

int TreeIterator::done() const
{
  return (stackPtr == 0);
}

TreeIterator::operator const Sortable * () const
{
  if (stack)
  {
    NodeHolder& nh = top();
    if (nh.node)
      return nh.node->ptr;
  }
  return 0;
}

TreeIterator& TreeIterator::operator ++ (int)
{
  if (stack)
  {
    int isDone = 0;

    while (!isDone  &&  !done())
    {
      NodeHolder& nh = top();

      if (direction == Forward)
      {
	if (!(nh.visited & VisitLeft))		// push left node to visit
	{
	  NodeHolder nx(nh.node->left);
	  push(nx);
	  nh.visited |= VisitLeft;
	}
	else if (!(nh.visited & VisitThis))	// done! visit this node
	{
	  nh.visited |= VisitThis;
	  isDone = 1;
	}
	else if (!(nh.visited & VisitRight))	// push right node to visit
	{
	  NodeHolder nx(nh.node->right);
	  push(nx);
	  nh.visited |= VisitRight;
	}
	else					// we visited everything; pop
	{
	  pop();
	}
      }
      else
      {
	if (!(nh.visited & VisitRight))		// push right node to visit
	{
	  NodeHolder nx(nh.node->right);
	  push(nx);
	  nh.visited |= VisitRight;
	}
	else if (!(nh.visited & VisitThis))	// done! visit this node
	{
	  nh.visited |= VisitThis;
	  isDone = 1;
	}
	else if (!(nh.visited & VisitLeft))	// push left node to visit
	{
	  NodeHolder nx(nh.node->left);
	  push(nx);
	  nh.visited |= VisitLeft;
	}
	else					// we visited everything; pop
	{
	  pop();
	}
      }
    }
  }

  return *this;
}

void TreeIterator::push(NodeHolder& node)
{
  if (stackPtr < stackSize)
  {
    stack[stackPtr] = node;
    stackPtr++;
  }
}

NodeHolder& TreeIterator::top() const
{
  if (stackPtr > 0)
    return stack[stackPtr - 1];
  return const_cast<TreeIterator*>(this)->empty;
}

NodeHolder& TreeIterator::pop()
{
  if (stackPtr > 0)
  {
    stackPtr--;
    return stack[stackPtr];
  }
  return empty;
}

// End


