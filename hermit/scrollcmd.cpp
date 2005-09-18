// Scrollable Command Listing class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "scrollcmd.h"


ScrollCommands::~ScrollCommands ()
{
}

int ScrollCommands::isAtHome () const
{
    if (mCursor == 0)
	return 1;
    return 0;
}

int ScrollCommands::isAtEnd () const
{
    if (mCursor == mnCmds - 1)
	return 1;
    return 0;
}

int ScrollCommands::moveCursor (int count)
{
    // If a negative count is more than mCursor, we must limit it to neg mCursor
    if (-count > mCursor)
	count = -mCursor;

    // If a positive count is more than size - cursor - 1, we must limit it too.
    if (count > mnCmds - 1 - mCursor)
	count = mnCmds - 1 - mCursor;

    // Set the cursor and return actual amount
    mCursor += count;
    return count;
}

int ScrollCommands::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollCommands::end ()
{
    int where = mCursor;
    mCursor = mnCmds - 1;
    return mCursor - where;
}

int ScrollCommands::getText (int pos, char *line, int width) const
{
    // This produces lines like
    // Bookmarks
    //   Ctl-0 to Ctl-9  Goto bookmark 0 to 9
    //   Alt-0 to Alt-9  Set bookmark 0 to 9
    //   Alt-B           Show bookmarks
    // 012345678901234567890123456789012345678901234567890
    //           1         2         3         4

    if (line == 0  ||  width < MIN_CMD_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    const char *pStartOfLine = line;

    // Check for out of bounds
    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > mnCmds - 1)
	return 0;

    if (mCmds[mCursor + pos].key == -1) {
	// Heading
	if (mCmds[mCursor + pos].keyText[0] != '\0') {
	    char fmt[20];
	    wsprintf (fmt, "%%-%d.%ds", width, width);
	    line += wsprintf (line, fmt, mCmds[mCursor + pos].keyText);
	}
    }
    else {
	// Command
	char fmt[20];
	line += wsprintf (line, "  %-14.14s  ", mCmds[mCursor + pos].keyText);
	wsprintf (fmt, "%%-%d.%ds", width - 18, width - 18);
	line += wsprintf (line, fmt, mCmds[mCursor + pos].description);
    }

    return line - pStartOfLine;
}

void ScrollCommands::getAttributes (int pos, WORD *attrs, int width) const
{
    // This produces lines like
    // Bookmarks
    //   Ctl-0 to Ctl-9  Goto bookmark 0 to 9
    //   Alt-0 to Alt-9  Set bookmark 0 to 9
    //   Alt-B           Show bookmarks
    // 012345678901234567890123456789012345678901234567890
    //           1         2         3         4

    // So the following is the color map for keys:
    // 0 to 17	    keyText
    // 18 to end    description

    // And the following is the color map for headings:
    // 0 to end	    keyText

    if (attrs == 0  ||  width < MIN_CMD_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    WORD clrHeading = (pos == 0 ? getColor (ceCmdHeadingCursor) : getColor (ceCmdHeading));
    WORD clrKey = (pos == 0 ? getColor (ceCmdKeyCursor) : getColor (ceCmdKey));
    WORD clrText = (pos == 0 ? getColor (ceCmdTextCursor) : getColor (ceCmdText));
    int i;

    if (mCmds[mCursor + pos].key == -1) {
	// Heading
	for (i = 0; i < width; i++)
	    attrs[i] = clrHeading;
    }
    else {
	// Command
	WORD clrKeyText = clrKey;
	if (mCmds[mCursor + pos].key == 0)
	    clrKeyText = clrText;
	for (i = 0; i < 18; i++)
	    attrs[i] = clrKeyText;
	for (i = 18; i < width; i++)
	    attrs[i] = clrText;
    }
}

CommandInfo ScrollCommands::mCmds[] = {
//  { 0,	    "Alt-A",		"Description" },

    { -1,	    "General Information", 0 },
    { KB_F1,	    "F1",		"Help" },
    { 0,	    "Alt-C (or F2)", 	"show Commands (this window)" },
    { '$',	    "$",		"show licen$ing information" },
    { KB_ALT_T,	    "Alt-T", 		"seTup HERMIT" },

    { -1,	    "", 0 },
    { -1,	    "Switching directories", 0 },
    { 0,	    "Ctl-A to Ctl-Z",	"go to drive letter A to Z" },
    { KB_ALT_G,	    "Alt-G (or F6)", 	"Go to a new directory" },
    { KB_ALT_V,	    "Alt-V (or F4)", 	"go to preVious directory" },
    { KB_LEFT,	    "\x1B or Esc",  	"go up one directory level" },
    { KB_RIGHT,	    "\x1A",		"go into highlighted directory" },
    { '~',	    "Alt-H (or ~)",	"go to Home directory" },   // set by HERMIT_HOME, HOME

    { -1,	    "", 0 },
    { -1,	    "Bookmarks", 0 },
    { 0,	    "Ctl-0 to Ctl-9",	"go to bookmark 0 to 9" },
    { 0,	    "Alt-0 to Alt-9",	"set bookmark 0 to 9" },
    { KB_ALT_B,	    "Alt-B",		"show Bookmarks" },

    { -1,	    "", 0 },
    { -1,	    "Directory Listing", 0 },
    { KB_ALT_F,	    "Alt-F",		"Filter directory listing" },
    { KB_ALT_S,	    "Alt-S",		"Sort directory listing" },
    { KB_ALT_R,	    "Alt-R (or F5)",	"Refresh directory listing" },
    { KB_UP,	    "\x18",		"move cursor up" },
    { KB_DOWN,	    "\x19",		"move cursor down" },
    { KB_HOME,	    "Home",		"move cursor to first file" },
    { KB_END,	    "End",		"move cursor to last file" },
    { KB_PGUP,	    "PgUp",		"move cursor up one page" },
    { KB_PGDN,	    "PgDn",		"move cursor down one page" },

    { -1,	    "", 0 },
    { -1,	    "Tagging Files", 0 },
    { KB_PLUS,	    "+",		"tag file" },
    { KB_MINUS,	    "-",		"untag file" },
    { KB_SPACE,	    "Spacebar",		"toggle tag" },
    { KB_ALT_PLUS,  "Alt-Plus",		"tag all files" },
    { KB_ALT_MINUS, "Alt-Minus", 	"untag all files" },
    { KB_ALT_E,	    "Alt-E", 		"rE-tag all files" },

    { -1,	    "", 0 },
    { -1,	    "Shells and Commands", 0 },
#if 0
    { KB_TAB,	    "Tab",  		"view selected file" },
#endif
    { KB_BACKTAB,   "Tab/BackTab",	"display file information" },
    { KB_ALT_U,	    "Alt-U",		"view User (output) screen" },
    { KB_ALT_Z,	    "Alt-Z",		"run shell (usually DOS)" },	// shell to run set by HERMIT_SHELL, SHELL, COMSPEC
    { KB_ENTER,	    "Enter",		"run associated program verb" },
    { KB_ALT_O,	    "Alt-O",		"Open file via association" },
    { KB_ALT_P,	    "Alt-P",		"Print file via association" },
    { KB_ALT_N,	    "Alt-N (or F3)",  	"type in command to ruN" },
    { KB_ALT_A,	    "Alt-A",  		"Assign custom commands to keys" },
    { KB_ALT_Y,	    "Alt-Y",  		"run Your custom commands" },
    { KB_ALT_M,	    "Alt-M",  		"run them on Multiple files" },
    { KB_ALT_L,	    "Alt-L",  		"set an environment variabLe" },
    { KB_ALT_I,	    "Alt-I",  		"display system Information" },

    { -1,	    "", 0 },
    { -1,	    "Exiting the program", 0 },
    { KB_ALT_X,	    "Alt-X",		"eXit" }
};

int ScrollCommands::mnCmds = sizeof (ScrollCommands::mCmds) / sizeof (CommandInfo);

// End
