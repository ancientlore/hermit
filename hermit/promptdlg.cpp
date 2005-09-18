// Prompt Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "promptdlg.h"
#include "runedit.h"


PromptDialog::PromptDialog (Screen& screen, const char *cmd, int hilite, char *pText, int length, History *pHist) :
    Popup (screen, 80, 13), mpCmd (cmd), mHiLite (hilite)
{
    draw ();
    mpEdit = new RunEdit (screen, mX + 2, mY + 5, 76, pText, length, pHist);
    if (mpEdit == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mpEdit->setNoPrompt ();	// so F4 doesn't work
}

PromptDialog::~PromptDialog ()
{
    delete mpEdit;
}

void PromptDialog::draw ()
{
    drawBackground ();
    drawBorder ("Enter Parameter");
    writeText (2, 2, "Enter the highlighted parameter and press Enter, or Esc to cancel.");
    writeText (2, 3, "Command:");
    const char* txt = mpCmd;
    int count = (int) strlen (txt);

    writeText (11, 3, mpCmd, (count > 67 ? 67 : count));
    if (mHiLite < 67)
	fillColors (11 + mHiLite, 3, getColor (ceFieldKey), (mHiLite == 66 ? 1 : 2));

    if (count > 67) {
        count -= 67;
        writeText (11, 4, &mpCmd[67], (count > 67 ? 67 : count));
        if (mHiLite >= 66)
            fillColors (11 + mHiLite - 67 + (mHiLite < 67 ? 1 : 0), 4, getColor (ceFieldKey),
                ((mHiLite - 67) == 66 || mHiLite < 67 ? 1 : 2));
    }

    writeText (2, 6, "Hints: Alt-F !f  Add the selected file name (file.ext)");
    writeText (2, 7, "       Alt-D !d  Add the current directory (c:\\dir\\subdir)");
    writeText (2, 8, "       Alt-B !b  Add the base file name, no extension (file)");
    writeText (2, 9, "       Alt-V !v  Add the drive leter (c:)");
    writeText (2, 10, "       Alt-T !t  Add the path only (\\dir\\subdir)");
}

void PromptDialog::run ()
{
    mpEdit->run ();
    mExitKey = mpEdit->getExitCode ();
}

// End
