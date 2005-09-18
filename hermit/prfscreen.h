// Printf Screen class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef PRFSCREEN_H
#define PRFSCREEN_H

#include "console/screen.h"
#include "console/scroller.h"
#include "scrollprf.h"


class PrintfScreen : public Screen {
    ScrollPrintf mPR;
    Scroller* mScroller;
    void handleKbEsc (const Event& event);

public:

    PrintfScreen (const char *caption, ColorEntry norm, ColorEntry sel);
    virtual ~PrintfScreen ();

    // Screen / event related
    virtual int processEvent (const Event& event);

    void printLine (ColorEntry color, const char *fmt, ...);	// \n is ignored!
    void draw ();   // used to make sure new lines visible
};

#endif
