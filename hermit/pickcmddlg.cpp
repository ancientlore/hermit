// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "pickcmddlg.h"


PickCommandsDialog::PickCommandsDialog (Screen& screen, int multi) :
    Popup (screen, 57, 21), mMulti (multi)
{
    draw ();
    mpScroller = new PickCommandScroller (screen, mX + 2, mY + 5, 53, 14);
    if (mpScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

PickCommandsDialog::~PickCommandsDialog ()
{
    delete mpScroller;
}

void PickCommandsDialog::draw ()
{
    drawBackground ();
    if (mMulti)
	drawBorder ("Custom Command on ALL Tagged Files");
    else
	drawBorder ("Run a Custom Command");
    writeText (2, 2, "Select the command with the cursor and press Enter to");
    if (mMulti)
	writeText (2, 3, "run it on all tagged files, or press Esc to cancel.");
    else
	writeText (2, 3, "run it, or press Esc to cancel.");
}

void PickCommandsDialog::run ()
{
    mpScroller->run ();
    mExitKey = mpScroller->getExitCode ();
}

// End
