/* ---------------------------------------------------------------------------
  AVL Tree Object

        An AVL tree (Adel'son-Vel'skii & Landis) is a binary search
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

#ifndef TREE_H
#define TREE_H

#include "common.h"
#include "tree/heap.h"
#include "tree/sortable.h"


// NOTE: The default heap page size works well for the Tree.  The node
// size is currently 14 bytes, which results in pages of 64 * 14 bytes
// (896 bytes).  This is not overly wasteful for small trees (less than
// 300 elements) and still improves performance significantly on large
// trees (up to 5000 elements).  You may use the following constants to
// determine the correct heap page size:

const int HEAP_TREE_PAGE_SIZE = HEAP_DEF_PAGE_SIZE; // default, < 5,000 elem.
const int HEAP_TREE_GT_5K = 256;  	// 5,000 to 50,000 elements
const int HEAP_TREE_GT_50K = 512;	// more than 50,000 elements



// --------------------------------------------------------------------------
//	Node Object
// --------------------------------------------------------------------------

enum BalanceFactor { LeftHigh, Equal, RightHigh };

struct Node
{
  const Sortable *ptr;	// pointer to data in tree

  unsigned char bal;		// balance factor
  unsigned long size;		// number of nodes in subtree
  Node *left;			// left node pointer
  Node *right;			// right node pointer

  // Node cannot get an error so no Error() is provided.

  unsigned long leftSize()
  {
    if (left) return left->size;
    return 0;
  }

  unsigned long rightSize()
  {
    if (right) return right->size;
    return 0;
  }

  void init(const Sortable *obj);
  void kill();
};


// --------------------------------------------------------------------------
//	Function Types
// --------------------------------------------------------------------------

typedef int  (*CompareFunc)(const Sortable&, const Sortable&);
typedef void (*IterateFunc)(const Sortable&);


// --------------------------------------------------------------------------
//	Tree Object
// --------------------------------------------------------------------------

class Tree
{
  // Pointer to data set:

  Node 		*root;			// root node
  Heap          nodeHeap;		// for node allocation
  int		treeFlags;		// whether tree owns elements

  // These help us keep from passing lots of stuff on the stack:

  const Sortable *lookingFor;		// pointer to search/insert data
  CompareFunc 	compare;		// compare function
  IterateFunc   _iterate;		// iterate function
  unsigned	lineDepth;		// used in printing the tree
  unsigned	itemSize;		// used in printing the tree
  char 		*array;			// used in printing the tree
  const Sortable *duplicate;		// points to duplicate found in add

  // Maintain maximum level we encounter during add:

  unsigned	currentLevel;		// current level number (root = 1)
  unsigned	maxLevel;		// maximum encountered level

  // Internal routines used to add to the tree:

  Node *newNode(const Sortable *obj);
  const Sortable *add(Node *&node, int& taller);
  void leftBalance(Node *&node, int& taller);
  void rightBalance(Node *&node, int& taller);

  // Internal routines

  void rotateRight(Node *&node);
  void rotateLeft(Node *&node);

  // Internal routines used to delete from the tree:

  Sortable *remove(Node *&node, int& shorter);
  Node *findPredecessor(Node *node);
  void removePredecessor(Node *&node, int& shorter);
  void remLeftSubBalance(Node *&node, int& shorter);
  void remRightSubBalance(Node *&node, int& shorter);
  void remLeftBalance(Node *&node, int& shorter);
  void remRightBalance(Node *&node, int& shorter);
  void remNode(Node *&node);
  Sortable *remove(Node *&node, unsigned long index, int& shorter);

  // Internal routines used for search/iterate/flush:

  Node *finder(Node *node) const;
  void iterator(Node *node) const;
  void flusher(Node *node);
  void printer(Node *node) const;
  void levelCalc(Node *node) const;
  Node *indexer(Node *node, unsigned long index) const;

  // standard Sortable compare function

  static int SortableCompare(const Sortable&, const Sortable&);

  // No copy of a tree
  Tree (const Tree&);
  Tree& operator = (const Tree&);

public:

  enum { OwnsElements = 1, AllowDuplicates = 2 };

  // Public Routines:

  Tree(int flags = Tree::OwnsElements,
       CompareFunc func = Tree::SortableCompare,
       unsigned pageCnt = HEAP_TREE_PAGE_SIZE);
  ~Tree();

  // You must give Add a pointer that will exist for the lifetime of the tree
  // If Add returns 0, it failed.  if you set dupl, it will point to the
  // node where we would have added the ptr but found a duplicate (useful
  // for updating records)

  const Sortable *add(const Sortable *ptr, const Sortable **dupl = 0);

  // Remove returns a pointer to the object removed which YOU MUST DELETE!
  // The ptr is just used for comparison

  Sortable *remove(const Sortable *ptr);
  Sortable *remove(unsigned long index);

  // Flush will delete elements if ownsElements is true

  void flush();

  // Find returns a pointer to an element in the tree; do not delete it
  // key is just used for comparison

  const Sortable *find(const Sortable *key) const;

  const Sortable *operator [] (unsigned long index) const;

  void iterate(IterateFunc func) const;

  // determine number of levels in tree and number of elements

  unsigned getMaxLevel() const;
  unsigned long size() const
  {
    if (root) return root->size;
    return 0;
  }
  unsigned long getMaxItems() const;

  // determine amount of node space wasted

  unsigned long nodeUsed() const { return nodeHeap.used(); }
  unsigned long nodeAvail() const { return nodeHeap.avail(); }
  unsigned nodePages() const { return nodeHeap.pageCount(); }

  // print the tree format (for debugging)

  void printTree(IterateFunc func, unsigned itemSiz) const;

  friend class TreeIterator;
};


// --------------------------------------------------------------------------
//	TreeIterator Object
// --------------------------------------------------------------------------

// ----- Node visited enumeration -----

enum NodeVisited
{
  VisitNone = 0,
  VisitLeft = 1,
  VisitThis = 2,
  VisitRight = 4
};

// ----- Node Holder (item on the node stack) -----

struct NodeHolder
{
  Node *node;
  int visited;
  NodeHolder(Node *nod = 0);

  // NodeHolder cannot get an error so no Error() is provided.
};

// ----- Tree Iterator -----

class TreeIterator
{
  Tree& tree;			    // tree to iterate
  NodeHolder *stack;		// stack
  unsigned stackSize;		// size of stack
  unsigned stackPtr;		// ptr to next item to add to stack
  NodeHolder empty;		// returned for errors in stack
  int direction;		// direction of iteration

  void push(NodeHolder& node);
  NodeHolder& pop();
  NodeHolder& top() const;

  // No copy of a tree iterator
  TreeIterator (const TreeIterator&);
  TreeIterator& operator = (const TreeIterator&);

public:

  enum { Forward, Reverse };

  TreeIterator(Tree& aTree, int direc = Forward);   // throws ERR_OUT_OF_MEMORY
  ~TreeIterator();

  // int Error() { return (stack == 0  &&  tree.GetMaxLevel() > 0); }

  void reset();         // throws OutOfMemory
  int done() const;

  // do not delete elements returned from operator const Sortable * ; they
  // are in the tree

  operator const Sortable * () const;
  TreeIterator& operator ++ (int);
};


#endif

/* End */
