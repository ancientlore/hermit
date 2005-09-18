/* ---------------------------------------------------------------------------
  Function:	Heap class

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
//	Heap Object
// -------------------------------------------------------------------------

Heap::Heap(unsigned pageSiz, unsigned pageCnt) :
	   pageSize(pageSiz), _pageCount(pageCnt)
{
    fullPages = normPages = 0;
}

Heap::~Heap()
{
   freeAll();
}

HeapPage *Heap::newHeapPage()
{
    try {
	HeapPage *page = new HeapPage(pageSize, _pageCount);
	return page;
    }
    catch (const std::exception&) {
    }
    catch (...) {
    }
    return 0;
}

char *Heap::alloc()
{
    char *ptr = 0;

    // make sure a free page exists

    if (!normPages)
	normPages = newHeapPage();

    // allocate from free pages list

    if (normPages)
    {
	// allocate from a free page

	ptr = normPages->alloc();

	// if it failed, page is full.  move to full list

	if (!ptr)
	{
	    HeapPage *page = normPages;
	    normPages = page->next;
	    page->next = fullPages;
	    fullPages = page;

	    // try to allocate from next page (create if needed)

	    if (!normPages)
		normPages = newHeapPage();

	    if (normPages)
		ptr = normPages->alloc();
	}
    }

    return ptr;
}

void Heap::free(char *ptr)
{
    HeapPage *page;
    HeapPage *prev;

    // note: we try to free from all pages because there is no way to tell
    // where a pointer was allocated from.  The page object will verify
    // that the pointer points to its page memory area and only free it
    // if it does.

    // search non-full pages first

    page = normPages;
    prev = 0;

    while (page)
    {
	if (page->free(ptr))
	{			// got it! page->Free returns 1 when it worked
	    // if page is empty, reclaim its space

	    if (page->used() == 0)
	    {
		if (prev == 0)	// we are the root (page == normPages)
		    normPages = page->next;
		else
		    prev->next = page->next;

		delete page;
	    }

	    return;
	}

	prev = page;
	page = page->next;
    }

    // now search full pages

    page = fullPages;
    prev = 0;

    while (page)
    {
	if (page->free(ptr))
	{			// got it! page->Free returns 1 when it worked
	    // remove the page

	    if (prev == 0)	// we are the root (page == fullPages)
		fullPages = page->next;
	    else
		prev->next = page->next;

	    // put it in the norm list

	    page->next = normPages;
	    normPages = page;

	    return;
	}

	prev = page;
	page = page->next;
    }
}

void Heap::freeAll()
{
    HeapPage *page = normPages;
    HeapPage *next;

    while (page)
    {
	next = page->next;
	delete page;
	page = next;
    }

    page = fullPages;

    while (page)
    {
	next = page->next;
	delete page;
	page = next;
    }

    normPages = fullPages = 0;
}

unsigned long Heap::avail() const
{
    unsigned long avail = 0;

    HeapPage *page = normPages;
    while (page)
    {
	avail += page->avail();
	page = page->next;
    }

    return avail;
}

unsigned long Heap::used() const
{
    unsigned long used = 0;

    HeapPage *page = normPages;
    while (page)
    {
	used += page->used();
	page = page->next;
    }

    page = fullPages;
    while (page)
    {
	used += page->used();
	page = page->next;
    }

    return used;
}

unsigned Heap::pageCount() const
{
    unsigned count = 0;

    HeapPage *page = normPages;
    while (page)
    {
	count++;
	page = page->next;
    }

    page = fullPages;
    while (page)
    {
	count++;
	page = page->next;
    }

    return count;
}

// End


