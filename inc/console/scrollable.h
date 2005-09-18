// Scrollable class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef SCROLLABLE_H
#define SCROLLABLE_H

#define STRICT
#include <windows.h>

// Class Scrollable defines the interface to the data being scrolled.
class Scrollable {

public:

    Scrollable () {}
    virtual ~Scrollable () {}

    // it might be interesting to make a cursor-relative thing.  i.e., the Scroller 
    // never needs to know how many lines are present; it just moves the cursor and
    // asks for data relative to it.

    // the reason to do this is to avoid having to look forward in any type of file to
    // get them all.  (for some files, this takes too long.)  Then we can look at any size
    // object (fitting in 32-bit boundaries, anyway).

    virtual int isAtHome () const = 0;	// returns whether cursor is at beginning
    virtual int isAtEnd () const = 0;	// returns whether cursor is at end

    virtual int moveCursor (int count) = 0;     // returns amount it actually moved; negative to go up
    virtual int home () = 0;    // returns how much the cursor moved to get there
    virtual int end () = 0;	    // returns how much the cursor moved to get there

    // The convention with these is to return "empty" values when the position is bogus
    // Get Text returns number of characters written (excluding '\0')
    virtual int getText (int pos, char *line, int width) const = 0;	// pos relative to cursor
    virtual void getAttributes (int pos, WORD *attrs, int width) const = 0; // pos relative to cursor
};


#endif
