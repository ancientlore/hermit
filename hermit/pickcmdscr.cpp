// Pick Command Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "pickcmdscr.h"


PickCommandScroller::PickCommandScroller (Screen& screen, int x, int y, int w, int h) : 
    Scroller (screen, x, y, w, h, 0), mExitKey (KB_ESC)
{
    setScrollable (&mScrollCmds);
}

PickCommandScroller::~PickCommandScroller ()
{
}

int PickCommandScroller::processEvent (const Event& event)
{
    int key;

    // Look for key ranges
    if ((event.key >= 'A'  &&  event.key <= 'Z')  ||
	(event.key >= 'a'  &&  event.key <= 'z')) {
	// move cursor
	move ((event.key - 'a' < 0 ? event.key - 'A' : event.key - 'a') - mScrollCmds.getCursor ());
	return 1;
    }
    if (event.key >= '0'  &&  event.key <= '9') {
	// move cursor
	move ((26 + event.key - '0') - mScrollCmds.getCursor ());
	return 1;
    }

    switch (event.key) {

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	key = mScrollCmds.getCursor ();
	if (key < 26)
	    key += 65;
	else
	    key += (48 - 26);
	mExitKey = key;
	postEvent (Event (EV_QUIT));
        break;

    default:
        return Scroller::processEvent (event);
    }

    return 1;
}

// End
