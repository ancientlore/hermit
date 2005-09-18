// Goto Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "gotodlg.h"


GotoDialog::GotoDialog (Screen& screen, char *pText, int length, History *pHist) :
    Popup (screen, 80, 6)
{
    draw ();
    mpEdit = new Edit (screen, mX + 2, mY + 3, 76, pText, length, pHist);
    if (mpEdit == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

GotoDialog::~GotoDialog ()
{
    delete mpEdit;
}

void GotoDialog::draw ()
{
    drawBackground ();
    drawBorder ("Goto Location");
    writeText (2, 2, "Type the directory name and press Enter, or press Esc to cancel.");
}

void GotoDialog::run ()
{
    mpEdit->run ();
    mExitKey = mpEdit->getExitCode ();
}

// End
