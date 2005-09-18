// Verb Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "verbscr.h"


VerbScroller::VerbScroller (Screen& screen, int x, int y, int w, int h, 
			    int verbc, const char *verbv[]) : 
    Scroller (screen, x, y, w, h, 0), mExitKey (KB_ESC), mIndex (-1), mScrollVerbs (verbc, verbv)
{
    setScrollable (&mScrollVerbs);
}

VerbScroller::~VerbScroller ()
{
}

int VerbScroller::processEvent (const Event& event)
{
    switch (event.key) {

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	mExitKey = KB_ENTER;
	mIndex = mScrollVerbs.getCursor ();
	postEvent (Event (EV_QUIT));
	break;

    default:
        return Scroller::processEvent (event);
    }

    return 1;
}


// End
