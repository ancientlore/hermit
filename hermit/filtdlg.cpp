// Filter Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "filtdlg.h"


FilterDialog::FilterDialog (Screen& screen, char *pText, int length, History *pHist) :
    Popup (screen, 51, 8)
{
    draw ();
    mpEdit = new Edit (screen, mX + 2, mY + 4, 47, pText, length, pHist);
    if (mpEdit == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

FilterDialog::~FilterDialog ()
{
    delete mpEdit;
}

void FilterDialog::draw ()
{
    drawBackground ();
    drawBorder ("Filter Files");
    writeText (2, 2, "Enter the new filter using wildcards (* and ?).");
    writeText (2, 3, "An empty filter gets all files, ';' for many.");
    writeText (2, 6, "Then press Enter to accept or Esc to cancel.");
}

void FilterDialog::run ()
{
    mpEdit->run ();
    mExitKey = mpEdit->getExitCode ();
}

// End
