// Input Dialog
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#include "console/inputdlg.h"


InputDialog::InputDialog (Screen& screen, const char *pCaption, const char *pPrompt,
			  char *pText, int length, History *pHist) :
    Popup (screen, 80, 7)
{
    draw (pCaption, pPrompt);
    mpEdit = new Edit (screen, mX + 2, mY + 3, 76, pText, length, pHist);
    if (mpEdit == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

InputDialog::~InputDialog ()
{
    delete mpEdit;
}

void InputDialog::draw (const char *pCaption, const char *pPrompt)
{
    drawBackground ();
    drawBorder (pCaption);
    writeText (2, 2, pPrompt);
    writeText (2, 5, "Press Enter to accept or Esc to cancel.");
}

void InputDialog::run ()
{
    mpEdit->run ();
    mExitKey = mpEdit->getExitCode ();
}

// End
