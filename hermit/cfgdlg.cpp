// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "cfgdlg.h"


ConfigDialog::ConfigDialog (Screen& screen) :
    Popup (screen, 57, 13)
{
    draw ();
    mpScroller = new ConfigScroller (screen, mX + 2, mY + 5, 53, 6);
    if (mpScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mpScroller->setDialog (this);
}

ConfigDialog::~ConfigDialog ()
{
    delete mpScroller;
}

void ConfigDialog::draw ()
{
    drawBackground ();
    drawBorder ("Setup HERMIT");
    writeText (2, 2, "Select an item with the cursor and press Enter to");
    writeText (2, 3, "configure it.  Press Esc when done.");
}

void ConfigDialog::run ()
{
    mpScroller->run ();
    mExitKey = mpScroller->getExitCode ();
}

// End
