// Command Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "cmdscr.h"


CommandScroller::CommandScroller (Screen& screen, int x, int y, int w, int h) :
    Scroller (screen, x, y, w, h, 0), mExitKey (KB_ESC)
{
    setScrollable (&mScrollCmds);
}

CommandScroller::~CommandScroller ()
{
}

int CommandScroller::processEvent (const Event& event)
{
    int key;

    // Look for key ranges
    if ((event.key >= KB_CTRL_0  &&  event.key <= KB_CTRL_9)  ||
	(event.key >= KB_CTRL_A  &&  event.key <= KB_CTRL_Z)  ||
	(event.key >= KB_ALT_0  &&  event.key <= KB_ALT_9)) {
	mExitKey = event.key;
	postEvent (Event (EV_QUIT));
	return 1;
    }

    switch (event.key) {

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	key = mScrollCmds.mCmds[mScrollCmds.getCursor ()].key;
	if (key != 0  &&  key != -1) {
	    mExitKey = key;
	    postEvent (Event (EV_QUIT));
	}
	break;

    case KB_F1:		    // help
    case KB_ALT_G:	    // goto dir
    case KB_F6:		    // goto dir
    case KB_ALT_V:	    // prev dir
    case KB_F4:		    // prev dir
    case KB_ALT_B:	    // show bookmarks
    case KB_ALT_F:	    // filter files
    case KB_ALT_S:	    // sort files
    case KB_ALT_R:	    // refresh dir
    case KB_F5:		    // refresh dir
    case KB_PLUS:	    // tag file
    case KB_MINUS:	    // untag file
    case KB_SPACE:	    // toggle tag
    case KB_ALT_U:	    // show user screen
    case KB_ALT_Z:	    // launch shell
    case KB_ALT_X:	    // quit
    case KB_ALT_PLUS:	    // tag all files
    case KB_ALT_MINUS:	    // untag all files
    case '~':		    // goto home dir
    case '$':		    // show license screen
    case KB_ALT_H:	    // goto home dir
    case KB_ALT_P:	    // print file
    case KB_ALT_N:	    // run command
    case KB_ALT_O:	    // open file
    case KB_ALT_A:	    // assign custom commands
    case KB_ALT_Y:	    // run Your commands
    case KB_ALT_M:	    // run commands on Multiple files
    case KB_ALT_E:	    // retag files
    case KB_F3:		    // run command
    case KB_TAB:	    // file info
    case KB_ALT_L:	    // set env variabLe
    case KB_ALT_T:	    // setup hermit
    case KB_ALT_I:	    // system info
    case KB_BACKTAB:	    // file info
	mExitKey = event.key;
	postEvent (Event (EV_QUIT));
	break;

    default:
        return Scroller::processEvent (event);
    }

    return 1;
}


// End
