// Assign Command Dialog
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#include "assigndlg.h"


AssignCommandDialog::AssignCommandDialog (Screen& screen, char *pText, int length, char key) :
    Popup (screen, 80, 14), mKey (key)
{
    draw ();
    mpEdit = new RunEdit (screen, mX + 2, mY + 3, 76, pText, length);
    if (mpEdit == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

AssignCommandDialog::~AssignCommandDialog ()
{
    delete mpEdit;
}

void AssignCommandDialog::draw ()
{
    drawBackground ();
    char str[40];
    wsprintf (str, "Assign Command to the %c Key", mKey);
    drawBorder (str);
    writeText (2, 2, "Type the command to assign and press Enter, or press Esc to cancel.");
    writeText (2, 4, "Hints: Alt-F !f  Add the selected file name (file.ext)");
    writeText (2, 5, "       Alt-D !d  Add the current directory (c:\\dir\\subdir)");
    writeText (2, 6, "       Alt-B !b  Add the base file name, no extension (file)");
    writeText (2, 7, "       Alt-P !p  Prompt for a string at this location");
    writeText (2, 8, "       Alt-M !m  Insert all tagged files here (file.ext)");
    writeText (2, 9, "       Alt-Q !q  Insert all tagged files quoted (\"file.ext\")");
    writeText (2, 10,"       Alt-I !i  Immediate return - no \"Press Esc\" message");
    writeText (2, 11,"       Alt-A !a  Automatic refresh of file listing");
}

void AssignCommandDialog::run ()
{
    mpEdit->run ();
    mExitKey = mpEdit->getExitCode ();
}

// End
