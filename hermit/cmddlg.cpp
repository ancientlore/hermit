// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "cmddlg.h"


CommandDialog::CommandDialog (Screen& screen) :
    Popup (screen, 57, 21)
{
    draw ();
    mpScroller = new CommandScroller (screen, mX + 2, mY + 5, 53, 14);
    if (mpScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

CommandDialog::~CommandDialog ()
{
    delete mpScroller;
}

void CommandDialog::draw ()
{
    drawBackground ();
    drawBorder ("HERMIT Commands");
    writeText (2, 2, "Select a command with the cursor and press Enter");
    writeText (2, 3, "to activate it, or press Esc to cancel.");
}

void CommandDialog::run ()
{
    mpScroller->run ();
    mExitKey = mpScroller->getExitCode ();
}

// End
