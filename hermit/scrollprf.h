// Scrollable Printf class
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#ifndef SCROLLPRF_H
#define SCROLLPRF_H

#include "tree_tpl.h"
#include "console/scrollable.h"
#include <stdarg.h>

class ScrollPrintfElem {
    int		line;
    ColorEntry	color;
    char *	text;
public:
    ScrollPrintfElem () : line (0), color (ceFirstEntry), text (0) {}
    virtual ~ScrollPrintfElem ();
    ScrollPrintfElem (const ScrollPrintfElem& elem);
    ScrollPrintfElem& operator = (const ScrollPrintfElem& elem);
    int compare (const ScrollPrintfElem& elem) const;
    void set (int l, ColorEntry c, const char *t);
    const char *getLine () const { return text; }
    ColorEntry getColor () const { return color; }
};

inline int operator == (const ScrollPrintfElem& a, const ScrollPrintfElem& b)
{
    return (a.compare (b) == 0);
}

inline int operator > (const ScrollPrintfElem& a, const ScrollPrintfElem& b)
{
    return (a.compare (b) > 0);
}

inline int operator < (const ScrollPrintfElem& a, const ScrollPrintfElem& b)
{
    return (a.compare (b) < 0);
}

inline int operator >= (const ScrollPrintfElem& a, const ScrollPrintfElem& b)
{
    return (a.compare (b) >= 0);
}

inline int operator <= (const ScrollPrintfElem& a, const ScrollPrintfElem& b)
{
    return (a.compare (b) <= 0);
}

inline int operator != (const ScrollPrintfElem& a, const ScrollPrintfElem& b)
{
    return (a.compare (b) != 0);
}


class ScrollPrintf : public Scrollable {
    int mCursor;
    TTree<ScrollPrintfElem> mLines;
    int mNextLine;
    ColorEntry mcNorm;
    ColorEntry mcSel;
public:

    ScrollPrintf (ColorEntry normal, ColorEntry sel) : 
	mCursor (0), mNextLine (0), mcNorm (normal), mcSel (sel) {}
    virtual ~ScrollPrintf ();

    virtual int isAtHome () const;	// returns whether cursor is at beginning
    virtual int isAtEnd () const;	// returns whether cursor is at end

    virtual int moveCursor (int count);     // returns amount it actually moved; negative to go up
    virtual int home ();    // returns how much the cursor moved to get there
    virtual int end ();	    // returns how much the cursor moved to get there

    // The convention with these is to return "empty" values when the position is bogus
    // The lines do NOT need to be padded on the right, but the attributes must fill the whole thing
    virtual int  getText (int pos, char *line, int width) const;	// pos relative to cursor
    virtual void getAttributes (int pos, WORD *attrs, int width) const; // pos relative to cursor

    int getCursor () const { return mCursor; }

    void printLine (ColorEntry color, const char *fmt, ...);	// \n is ignored!
    void vPrintLine (ColorEntry color, const char *fmt, va_list list);	// \n is ignored!
};


#endif
