// theap class

#include "tree_tpl.h"


THeapBase* THeapBase::mpHeaps = 0;

THeapBase::THeapBase (unsigned size, unsigned count) : Heap (size, count) {
    mpNext = mpHeaps;
    mpHeaps = this;
}
    
THeapBase::~THeapBase () {
    if (this == mpHeaps) {
	mpHeaps = mpHeaps->mpNext;
    }
    else {
	THeapBase *heap = mpHeaps;
	do {
	    if (heap->mpNext == this) {
		heap->mpNext = mpNext;
		break;
	    }
	    heap = heap->mpNext;
	}
	while (heap);
    }
}

void THeapBase::freeAllHeaps () {
    THeapBase* heap = mpHeaps;
    while (heap) {
	heap->freeAll ();
	heap = heap->mpNext;
    }
}
