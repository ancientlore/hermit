// Ask Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <cstring>
#include "console/askdlg.h"


AskDialog::AskDialog (Screen& screen, const char *title, const char *text) :
    Popup (screen, (int) strlen (text) + 4, 7)
{
    drawBackground ();
    drawBorder (title);

    writeText (2, 2, text);
    drawField (2, 4, "Yes");
    drawField (7, 4, "No");
}

int AskDialog::processEvent (const Event& event)
{
    switch (event.key) {

    case KB_ESC:
    case 'N':
    case 'n':
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    // case KB_ENTER:
    case 'Y':
    case 'y':
	mExitKey = KB_ENTER;
	postEvent (Event (EV_QUIT));
	break;

    default:
        return Popup::processEvent (event);
    }

    return 1;
}


// End
