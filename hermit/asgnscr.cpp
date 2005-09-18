// Command Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "asgnscr.h"
#include "assigndlg.h"
#include "registry.h"
#include "ptr.h"

#define HKCU_SUBKEY_COMMANDS	HKCU_SUBKEY_HERMIT "\\Commands"
#define COMMAND_SIZE		1024

AssignCommandScroller::AssignCommandScroller (Screen& screen, int x, int y, int w, int h) :
    Scroller (screen, x, y, w, h, 0), mExitKey (KB_ESC)
{
    setScrollable (&mScrollCmds);
}

AssignCommandScroller::~AssignCommandScroller ()
{
}

int AssignCommandScroller::processEvent (const Event& event)
{
    int key;

    // Look for key ranges
    if ((event.key >= 'A'  &&  event.key <= 'Z')  ||
	(event.key >= 'a'  &&  event.key <= 'z')) {
	// move cursor
	move ((event.key - 'a' < 0 ? event.key - 'A' : event.key - 'a') - mScrollCmds.getCursor ());
	return 1;
    }
    if (event.key >= '0'  &&  event.key <= '9') {
	// move cursor
	move ((26 + event.key - '0') - mScrollCmds.getCursor ());
	return 1;
    }

    switch (event.key) {

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	// Launch command edit window
	key = mScrollCmds.getCursor ();
	if (key < 26)
	    key += 65;
	else
	    key += (48 - 26);
	assignCommand (key);
	drawLine (mCursor);
        break;

    default:
        return Scroller::processEvent (event);
    }

    return 1;
}

void AssignCommandScroller::assignCommand (int key)
{
    ArrayPtr<char> str = new char [COMMAND_SIZE];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    str[0] = '\0';
    char s[2];
    s[0] = key;
    s[1] = '\0';

    try {
	// Open registry key
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_COMMANDS, KEY_READ | KEY_WRITE);

	// try to read the current value
	char *value = 0;
	DWORD type;
	try {
	    value = k.queryValue (s, type);
	    if (type == REG_SZ  &&  value != 0  &&  value[0] != '\0') {
		strncpy (str, value, COMMAND_SIZE - 1);
		str[COMMAND_SIZE - 1] = '\0';
	    }
	    delete [] value;
	}
	catch (const std::exception&) {
	    // okay, so it isn't set.
	}

	AssignCommandDialog dlg (myScreen, str, COMMAND_SIZE - 1, key);
	dlg.run ();
	if (dlg.getExitCode () != KB_ESC) {
	    // save command
	    try {
		k.setValue (s, str);
	    }
	    catch (const std::exception&) {
		myScreen.sendWinErrStatusMsg ("Error saving key to registry");
	    }
	}
    }
    catch (const std::exception&) {
	myScreen.sendWinErrStatusMsg ("Could not read Commands key");
    }
}

// End
