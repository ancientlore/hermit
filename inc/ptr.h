/******************************************************************************

    Auto-deleting, reference-counting, pointer management template classes.

******************************************************************************/

#ifndef PTR_INCLUDED
#define PTR_INCLUDED


// Base class for use by Ptr<> and ArrayPtr<> templates
// It manages a doubly-linked list of references to the same pointer

class PtrBase {
public:

    int isOnlyRef () const {
	return (mpNext == 0 && mpPrev == 0 ? 1 : 0);
    }

protected:

    PtrBase ()		            { clear (); }
    PtrBase (const PtrBase& pb)	{ add (const_cast<PtrBase&> (pb)); }
    ~PtrBase ()		            { remove (); }

    void clear () {
	    mpNext = mpPrev = 0;
    }

    void add (PtrBase& pb) {
        mpNext = pb.mpNext;
        if (mpNext != 0) mpNext->mpPrev = this;
        mpPrev = &pb;
        pb.mpNext = this;
    }

    void remove () {
        if (mpNext != 0) mpNext->mpPrev = mpPrev;
        if (mpPrev != 0) mpPrev->mpNext = mpNext;
    }

    // Data members

    PtrBase*	mpNext;
    PtrBase*	mpPrev;

}; // PtrBase


// Define the body of the template class as a macro so the Ptr<> and
// ArrayPtr<> templates can be defined in general and for several built-in
// types without having multiple copies of otherwise identical code.

#define PTR_BODY(NAME,T,DELETE)		\
public:					\
					\
    NAME (T* p = 0) :			\
	PtrBase (), mp (p) {		\
    }					\
					\
    NAME (const NAME<T>& p) :		\
	PtrBase (p), mp (p.mp) {	\
    }					\
					\
    ~NAME () {				\
	deref ();			\
    }					\
					\
    void operator = (T* p) {		\
	deref ();			\
	remove ();			\
	clear ();			\
	mp = p;				\
    }					\
					\
    void operator = (const NAME<T>& p) {\
	if (&p != this) {		\
	    deref ();			\
	    remove ();			\
	    add (const_cast<NAME<T>&>(p));\
	    mp = p.mp;			\
	}				\
    }					\
					\
    T* detach () {			\
	T* p = mp;			\
	mp = 0;				\
	remove ();			\
	clear ();			\
	return p;			\
    }					\
					\
    operator T* () const {		\
	    return mp;			\
    }					\
					\
    T& operator * () const {		\
	    return *mp;			\
    }					\
					\
protected:				\
					\
    void deref () {			\
	if (isOnlyRef ())		\
	    DELETE mp;			\
    }					\
					\
    T*	mp

// Expand the macros to define Ptr<> and ArrayPtr<> templates in general

template <class T>
class Ptr : public PtrBase {
    PTR_BODY (Ptr, T, delete);
public:
    T* operator -> () const {
	    return mp;
    }

    friend operator < (const Ptr<T>& a, const Ptr<T>& b);
    friend operator > (const Ptr<T>& a, const Ptr<T>& b);
};

template <class T>
class ArrayPtr : public PtrBase {
    PTR_BODY (ArrayPtr, T, delete[]);
public:
    T& operator [] (int i) const {
	    return mp[i];
    }
};

// Redefine templates for some of the built-in types since they can't support
// the arrow operator.
#if 0
class Ptr<int> : public PtrBase {
    PTR_BODY (Ptr, int, delete);
};

class Ptr<char> : public PtrBase {
    PTR_BODY (Ptr, char, delete);
};
#endif

#endif
