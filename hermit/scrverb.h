// Scrollable Verb Listing class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef SCRVERB_H
#define SCRVERB_H

#include "console/scrollable.h"
#include "console/screen.h"


class ScrollVerbs : public Scrollable {
    int mCursor;
    int mVerbC;
    const char ** mVerbV;

public:
    ScrollVerbs (int verbc, const char *verbv[]);
    virtual ~ScrollVerbs ();

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
};


#endif
