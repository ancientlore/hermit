/* ---------------------------------------------------------------------------
  Heap class

        The heap class implements a fixed-size heap allocation
		scheme.  It consists of two objects, the HeapPage and
		the Heap.  Normally, you will use the Heap object and
		allow it to allocate pages on an as-needed basis.

		Although these objects provide Alloc and Free methods,
		their power is in their efficiency and ability to
		destroy themselves quickly.  Rather than freeing
		all individually allocated items, you can just free
		the whole Heap.

  Copyright (C) 1993 Michael D. Lore
  All Rights Reserved.
--------------------------------------------------------------------------- */

#ifndef HEAP_H
#define HEAP_H

#include "common.h"
#include "tree/bitarray.h"

// NOTE: You should keep your page sizes such that they are not bigger
// than 32K or so.  The default page size (number of elements), results
// in a page that is HEAP_DEF_PAGE_SIZE * sizeof(element) big.  For an
// element that is 500 bytes big, a page is about 32K.  SO... you should
// override the default when your elements are bigger than 512 bytes
// or if you wish to have a smaller page for some other reason.

const int HEAP_DEF_PAGE_SIZE = 64;	// default num elements in a page

// ------------------------------------------------------------------------
//	Heap Page Object
// ------------------------------------------------------------------------

class HeapPage
{
  char *     Ptr;		// memory page pointer
  BitArray   allocBits;		// keep track of allocated blocks
  unsigned   Size;		// size of a data element
  unsigned   Count;		// number of data elements on a page
  HeapPage * next;		// next page pointer

  // No copy
  HeapPage (const HeapPage&);
  HeapPage& operator = (const HeapPage&);

public:

  HeapPage(unsigned size, unsigned count = HEAP_DEF_PAGE_SIZE); // throws ERR_OUT_OF_MEMORY
  ~HeapPage();

  // avail and used on one page

  unsigned avail() const;
  unsigned used() const;

  // alloc/free routines

  char *alloc();
  bool  free(char *ptr);
  void freeAll();

  // friend

  friend class Heap;
};

// ------------------------------------------------------------------------
//	Heap Object
// ------------------------------------------------------------------------

class Heap
{
  HeapPage *fullPages;			// list of heap pages that are full
  HeapPage *normPages;			// list of normal pages (not full)
  unsigned pageSize;			// size of a data element
  unsigned _pageCount;			// number of data elements per page

  HeapPage *newHeapPage();

  // No copy of a heap
  Heap (const Heap&);
  Heap& operator = (const Heap&);

public:

  Heap(unsigned size, unsigned count = HEAP_DEF_PAGE_SIZE);
  ~Heap();

  // avail and used on CURRENTLY active pages (NOT SYSTEM MEMORY!)
  // i.e., avail of zero means no pages are currently allocated
  // OR all pages currently allocated are completely full

  unsigned long avail() const;
  unsigned long used() const;
  unsigned pageCount() const;

  // Alloc/free routines

  char *alloc();		// allocate from the Heap
  void free(char *ptr);		// free an item from the Heap
  void freeAll();		// delete all heap pages (frees malloc mem)
};


#endif

/* End */

