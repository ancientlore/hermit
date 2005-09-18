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
//	Tree Object
// --------------------------------------------------------------------------

Node *Tree::finder(Node *node) const
{
  if (node)
  {
    int code = compare(*lookingFor, *(node->ptr));

    if (code < 0)
      return finder(node->left);
    else if (code > 0)
      return finder(node->right);

    return node;
  }

  return 0;
}

const Sortable *Tree::find(const Sortable *key) const
{
  if (key  &&  root)
  {
    const_cast<Tree*>(this)->lookingFor = key;
    Node *node = finder(root);
    if (node)
      return node->ptr;
  }

  return 0;
}

void Tree::iterator(Node *node) const
{
  if (node->left)
    iterator(node->left);

  _iterate(*(node->ptr));

  if (node->right)
    iterator(node->right);
}

void Tree::iterate(IterateFunc func) const
{
  if (func  &&  root)
  {
    const_cast<Tree*>(this)->_iterate = func;
    iterator(root);
  }
}


// End


