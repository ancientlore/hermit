// PrintfScreen class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "prfscreen.h"

PrintfScreen::PrintfScreen (const char *caption, ColorEntry norm, ColorEntry sel) :
    mPR (norm, sel)
{
    int w, h;	// screen size

    // SET CONSOLE TITLE TO OUR COPYRIGHT TITLE
    SetConsoleTitle (caption);

    // Handle escape key
    addHandler (KB_ESC, this, (Handler) &PrintfScreen::handleKbEsc);

    // Build the scroller
    getSize (w, h);
    mScroller = new Scroller (*this, 0, 0, w, h);
    if (mScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mScroller->setScrollable (&mPR);
}

PrintfScreen::~PrintfScreen ()
{
}

void PrintfScreen::handleKbEsc (const Event& )
{
    postEvent (Event (EV_QUIT));
}

int PrintfScreen::processEvent (const Event& event)
{
    // char str[80];
    // wsprintf (str, "my key = %d\n", key);
    // writeText (str);
    // return 1;
    return Screen::processEvent (event);
}

void PrintfScreen::printLine (ColorEntry color, const char *fmt, ...)
{
    va_list list;
    va_start (list, fmt);
    mPR.vPrintLine (color, fmt, list);
    va_end (list);
}

void PrintfScreen::draw ()
{
    mScroller->setScrollable (&mPR);
}

// End
