// Scrollable COM Custom Viewer class
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#ifndef SCROLLCOM_H
#define SCROLLCOM_H

#include "console/scrollable.h"
#include "console/screen.h"
#include "hmtobjs.h"


class ScrollCOM : public Scrollable {
    const CLSID& mClass;
    IHermitScrollable *mpInterface;
    const char *mpCompName;
public:
    // All of these can throw exceptions from the component's failure.
    // The COMScroller class catches these.

    ScrollCOM (const CLSID& classid, const char *compName = "<unknown>");
    virtual ~ScrollCOM ();

    virtual int isAtHome () const;	// returns whether cursor is at beginning
    virtual int isAtEnd () const;	// returns whether cursor is at end

    virtual int moveCursor (int count);     // returns amount it actually moved; negative to go up
    virtual int home ();    // returns how much the cursor moved to get there
    virtual int end ();	    // returns how much the cursor moved to get there

    // The convention with these is to return "empty" values when the position is bogus
    // The lines do NOT need to be padded on the right, but the attributes must fill the whole thing
    virtual int  getText (int pos, char *line, int width) const;	// pos relative to cursor
    virtual void getAttributes (int pos, WORD *attrs, int width) const; // pos relative to cursor

    // Extra methods specific to ScrollCOM
    void showFile (const char *filename);   // should be called after construction
    int processEvent (int key, DWORD data, int& redraw);  // returns 1 if the key was handled
    int getStatusText (char *line, int width) const;	// returns 1 if status to be drawn
    void positionCursor (int absolutePos);  // should redraw
};


#endif
