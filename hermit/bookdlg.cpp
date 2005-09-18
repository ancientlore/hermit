// Bookmark Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "bookdlg.h"


BookmarkDialog::BookmarkDialog (Screen& screen, const char **bookmarks) :
    Popup (screen, 80, 15), mpBookmarks (bookmarks)
{
    draw ();
}

void BookmarkDialog::draw ()
{
    drawBackground ();
    drawBorder ("Bookmarks");

    writeText (2, 2, "Type the highlighted number to select a bookmark or Esc to cancel.");

    for (int i = 0; i < 10; i++) {
	char str[3];
	str[0] = '0' + i;
	str[1] = ':';
	str[2] = '\0';
	drawField (2, 4 + i, str);
	if (mpBookmarks[i] != 0) {
	    int l = (int) strlen (mpBookmarks[i]);
	    writeText (5, 4 + i, mpBookmarks[i], (l > 73 ? 73 : l));
	    if (l > 73)
		drawField (78, 4 + i, "+");
	}
	else
	    writeText (5, 4 + i, "(unset)");
    }
}

int BookmarkDialog::processEvent (const Event& event)
{
    if (event.key >= '0'  &&  event.key <= '9') {
	if (mpBookmarks[event.key - '0'] != 0) {
	    mExitKey = event.key;
	    postEvent (Event (EV_QUIT));
	}
	return 1;
    }

    switch (event.key) {

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

#if 0
    case KB_ENTER:
	mExitKey = KB_ENTER;
	postEvent (Event (EV_QUIT));
	break;
#endif

    default:
        return Popup::processEvent (event);
    }

    return 1;
}

// End
