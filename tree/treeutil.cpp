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
#include <cmath>
#include "tree/tree.h"

using namespace std;

// --------------------------------------------------------------------------
//	Tree Object
// --------------------------------------------------------------------------

void Tree::printer(Node *node) const
{
  _iterate(*(node->ptr));
  printf("-%03lu", node->size);
  if (node->bal == Equal)
    putchar('=');
  else if (node->bal == LeftHigh)
    putchar('/');
  else
    putchar('\\');

  const_cast<Tree*>(this)->lineDepth += (itemSize + 5);

  if (node->left  ||  node->right)
  {
    printf("-+-");

    size_t index = strlen(array);

    if (node->left)
    {
      const_cast<Tree*>(this)->lineDepth += 3;
      array[index] = '|';
      array[index + 1] = 0;
      printer(node->left);
      array[index] = 0;
      const_cast<Tree*>(this)->lineDepth -= 3;
    }
    else
      putchar('\n');

    for (unsigned i = 0; i < lineDepth; i++)
    {
      if (((i + 2) % (itemSize + 3 + 5)) == 0)
	putchar(array[((i + 2) / (itemSize + 3 + 5)) - 1]);
      else
	putchar(' ');
    }

    printf(" +-");

    if (node->right)
    {
      const_cast<Tree*>(this)->lineDepth += 3;
      array[index] = ' ';
      array[index + 1] = 0;
      printer(node->right);
      array[index] = 0;
      const_cast<Tree*>(this)->lineDepth -= 3;
    }
    else
      putchar('\n');
  }
  else
    putchar('\n');

  const_cast<Tree*>(this)->lineDepth -= (itemSize + 5);
}

void Tree::printTree(IterateFunc func, unsigned itemSiz) const
{
  puts("Node-+-Left \t / Left High");
  puts("     |      \t = Equal");
  puts("     +-Right\t \\ Right High\n");

  getMaxLevel();	// Make sure it is updated

  if (func  &&  root)
  {
    try {
      const_cast<Tree*>(this)->array = new char[maxLevel + 1];
    }
    catch (...) {
      const_cast<Tree*>(this)->array = NULL;
    }
    if (array)
    {
      array[0] = 0;
      const_cast<Tree*>(this)->_iterate = func;
      const_cast<Tree*>(this)->lineDepth = 0;
      const_cast<Tree*>(this)->itemSize = itemSiz;
      printer(root);
      delete array;
    }
    else
      puts("No memory to print tree.");
  }
}

unsigned long Tree::getMaxItems() const
{
  unsigned long count = 0;

  getMaxLevel();	// make sure it's updated

  for (unsigned i = 0; i < maxLevel; i++)
    count += (unsigned long) pow(double(2), double(i));

  return count;
}

// End


