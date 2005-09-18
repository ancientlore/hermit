// Tree templates

#ifndef TREE_TPL_H
#define TREE_TPL_H

#include "tree/tree.h"

// This class is used to release the memory from the static heaps at
// the END of the program. Make sure there is nothing in them!!!
// Call freeAllHeaps only at the very end
class THeapBase : public Heap {
    static THeapBase*   mpHeaps;
    THeapBase*		mpNext;
public:
    THeapBase (unsigned size, unsigned count = HEAP_DEF_PAGE_SIZE);
    ~THeapBase ();
    static void freeAllHeaps ();
};

template <class T> class TSortable;

template <class T>
class THeap : public THeapBase {

public:
    THeap () : THeapBase (sizeof (TSortable<T>)) {}
};

template <class T>
class TSortable : public Sortable {
    static THeap<T> mHeap;
public:
    T mT;
    TSortable() { }
    TSortable(const T& t) : mT(t) {}

    virtual ~TSortable() {}

    virtual int compare(const Sortable& obj) const {
        if (mT < (((const TSortable<T>&)obj).mT))
            return -1;
        else if (mT > (((const TSortable<T>&)obj).mT))
            return 1;
        else
            return 0;
    }

    void *operator new (size_t size) { return mHeap.alloc (); }
    void operator delete (void *ptr) { mHeap.free ((char*)ptr); }
};

template <class T> THeap<T> TSortable<T>::mHeap;

template <class T> class TTreeIterator;

template <class T>
class TTree {
    Tree mTree;
public:
    enum { None = 0, AllowDuplicates = 2 };

    TTree (int flags = None) : mTree (flags | Tree::OwnsElements) {}

    void add (const T& t) {
        const Sortable* dupl = 0;
        if (mTree.add (new TSortable<T> (t), &dupl) == 0) {
            if (dupl != 0)
                throw AppException (WHERE, ERR_DUPLICATE);
            else
                throw AppException (WHERE, ERR_OUT_OF_MEMORY);
        }
    }

    void remove (const T& t) {
        Sortable* pTO = mTree.remove (&(TSortable<T>(t)));
        if (pTO == 0)
	    throw AppException (WHERE, ERR_NOT_FOUND);
        delete pTO;
    }

    void remove (unsigned long index) {
        Sortable* pTO = mTree.remove (index);
        if (pTO == 0)
	    throw AppException (WHERE, ERR_NOT_FOUND);
        delete pTO;
    }

    void flush() { mTree.flush (); }

    T& find (const T& key) const {
        TSortable<T> Key (key);
        const Sortable* Found = ((TTree<T>*) this)->mTree.find (&Key);
        if (Found == 0)
	    throw AppException (WHERE, ERR_NOT_FOUND);
        return ((TSortable<T>*)Found)->mT;
    }

    T& operator [] (unsigned long index) const {
        const Sortable* pTO = mTree[index];
        if (pTO == 0)
	    throw AppException (WHERE, ERR_NOT_FOUND);
        return ((TSortable<T>*)pTO)->mT;
    }

    unsigned long size () const { return ((TTree<T>*)this)->mTree.size (); }

	friend TTreeIterator<T>;
};

template <class T>
class TTreeIterator {
    TreeIterator mIterator;
public:
    enum { Forward, Reverse };

    TTreeIterator(const TTree<T>& aTree, int direc = Forward) :
        mIterator (const_cast<Tree&>(aTree.mTree), direc)
    {
    }   // throws OutOfMemory

    void reset() { mIterator.reset (); }    // throws OutOfMemory
    int done() const { return mIterator.done (); }

    operator T& () const {
        const Sortable* pTO = mIterator;
        if (pTO == 0)
	    throw AppException (WHERE, ERR_NOT_FOUND);
        return ((TSortable<T>*)pTO)->mT;
    }

    TTreeIterator<T>& operator ++ (int) { mIterator++; return *this; }
};

#endif

// End
