// Run Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "rundlg.h"


RunDialog::RunDialog (Screen& screen, char *pText, int length, History *pHist) :
    Popup (screen, 80, 17)
{
    draw ();
    mpEdit = new RunEdit (screen, mX + 2, mY + 3, 76, pText, length, pHist);
    if (mpEdit == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

RunDialog::~RunDialog ()
{
    delete mpEdit;
}

void RunDialog::draw ()
{
    drawBackground ();
    drawBorder ("Run Command");
    writeText (2, 2, "Type the command to run and press Enter, or press Esc to cancel.");
    writeText (2, 4, "Hints: Alt-F !f  Add the selected file name (file.ext)");
    writeText (2, 5, "       Alt-D !d  Add the current directory (c:\\dir\\subdir)");
    writeText (2, 6, "       Alt-B !b  Add the base file name, no extension (file)");
    writeText (2, 7, "       Alt-V !v  Add the drive leter (c:)");
    writeText (2, 8, "       Alt-T !t  Add the path only (\\dir\\subdir)");
    writeText (2, 9, "       Alt-P !p  Prompt for a string at this location");
    writeText (2, 10,"       Alt-M !m  Insert all tagged files here (file.ext)");
    writeText (2, 11,"       Alt-Q !q  Insert all tagged files quoted (\"file.ext\")");
    writeText (2, 12,"       Alt-I !i  Immediate return - no \"Press Esc\" message");
    writeText (2, 13,"       Alt-A !a  Automatic refresh of file listing");
    writeText (2, 14, "Type Ctrl-Enter to run command once for each tagged file.");
}

void RunDialog::run ()
{
    mpEdit->run ();
    mExitKey = mpEdit->getExitCode ();
}

// End
