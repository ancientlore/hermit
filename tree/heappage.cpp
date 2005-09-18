/* ---------------------------------------------------------------------------
  Function:	HeapPage class

  Include:	heap.h

  Description:	The heap class implements a fixed-size heap allocation
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

#include <cstdlib>
#include "tree/heap.h"


// -------------------------------------------------------------------------
//	HeapPage Object
// -------------------------------------------------------------------------

HeapPage::HeapPage(unsigned size, unsigned count) :
	Size(size), Count(count), next(0), allocBits(count), Ptr(0)
{
    try {
	Ptr = (char *) std::malloc(Size * Count);
    }
    catch (...) {
	Ptr = NULL;
    }
    if (Ptr == NULL)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

HeapPage::~HeapPage()
{
    if (Ptr)
	std::free(Ptr);
}

char *HeapPage::alloc()
{
    try {
	unsigned element = allocBits.setNext();
	return Ptr + (unsigned(Size) * element);
    }
    catch (const std::exception&) {
    }

    return 0;
}

bool HeapPage::free(char *ptr)
{
    if (ptr >= Ptr)
    {
	unsigned element = (ptr - Ptr) / Size;

	try {
	    allocBits.clear(element);
	    return true;
	}
	catch (const std::exception&) {
	}
    }
    return false;
}

void HeapPage::freeAll()
{
    allocBits.clearAll();
}

unsigned HeapPage::avail() const
{
    unsigned avail = 0;
    try {
	for (unsigned i = 0; i < Count; i++)
	    if (!allocBits.isSet(i))
		avail += Size;
    }
    catch (const std::exception&) {
    }
    return avail;
}

unsigned HeapPage::used() const
{
    unsigned used = 0;
    try {
	for (unsigned i = 0; i < Count; i++)
	    if (allocBits.isSet(i))
		used += Size;
    }
    catch (const std::exception&) {
    }
    return used;
}

// End


