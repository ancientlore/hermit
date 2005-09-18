// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "asgndlg.h"


AssignCommandsDialog::AssignCommandsDialog (Screen& screen) :
    Popup (screen, 57, 21)
{
    draw ();
    mpScroller = new AssignCommandScroller (screen, mX + 2, mY + 5, 53, 14);
    if (mpScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

AssignCommandsDialog::~AssignCommandsDialog ()
{
    delete mpScroller;
}

void AssignCommandsDialog::draw ()
{
    drawBackground ();
    drawBorder ("Assign Custom Commands");
    writeText (2, 2, "Select a key with the cursor and press Enter to");
    writeText (2, 3, "assign a command to it.  Press Esc when done.");
}

void AssignCommandsDialog::run ()
{
    mpScroller->run ();
    mExitKey = mpScroller->getExitCode ();
}

// End
