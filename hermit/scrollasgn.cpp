// Scrollable Assigned Command Listing class
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "scrollasgn.h"
#include "registry.h"

#define HKCU_SUBKEY_COMMANDS	HKCU_SUBKEY_HERMIT "\\Commands"

const int ScrollAssignCommands::mnCmds = 26 + 10;    // 26 letters and 10 digits

ScrollAssignCommands::~ScrollAssignCommands ()
{
}

int ScrollAssignCommands::isAtHome () const
{
    if (mCursor == 0)
	return 1;
    return 0;
}

int ScrollAssignCommands::isAtEnd () const
{
    if (mCursor == mnCmds - 1)
	return 1;
    return 0;
}

int ScrollAssignCommands::moveCursor (int count)
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

int ScrollAssignCommands::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollAssignCommands::end ()
{
    int where = mCursor;
    mCursor = mnCmds - 1;
    return mCursor - where;
}

int ScrollAssignCommands::getText (int pos, char *line, int width) const
{
    // This produces lines like
    // A  attrib
    // B  brief (f2)
    // 012345678901234567890123456789012345678901234567890
    //           1         2         3         4

    if (line == 0  ||  width < MIN_CMD_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    const char *pStartOfLine = line;

    // Check for out of bounds
    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > mnCmds - 1)
	return 0;

    // Get key
    // digits are in 48 to 57, letters are 65 to 90
    char key = ' ';
    if (mCursor + pos < 26)
	key = mCursor + pos + 65;
    else
	key = mCursor + pos - 26 + 48;

    // Get command text
    char str[256];
    str[0] = '\0';
    char s[2];
    s[0] = key;
    s[1] = '\0';

    try {
	// Open registry key
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_COMMANDS, KEY_READ);

	// try to read the current value
	char *value = 0;
	DWORD type;
	try {
	    value = k.queryValue (s, type);
	    if (type == REG_SZ  &&  value != 0  &&  value[0] != '\0') {
		strncpy (str, value, 255);
		str[255] = '\0';
	    }
	    delete [] value;
	}
	catch (const std::exception&) {
	    // strcpy (str, "(unset)");
	}
    }
    catch (const std::exception&) {
	// strcpy (str, "(unset)");
    }

    // Format Command
    char fmt[20];
    line += wsprintf (line, "%c ", key);
    wsprintf (fmt, "%%-%d.%ds", width - 2, width - 2);
    line += wsprintf (line, fmt, str);

    return line - pStartOfLine;
}

void ScrollAssignCommands::getAttributes (int pos, WORD *attrs, int width) const
{
    // This produces lines like
    // A  attrib
    // B  brief (f2)
    // 012345678901234567890123456789012345678901234567890
    //           1         2         3         4

    // So the following is the color map for keys:
    // 0 to 0	   keyText
    // 1 to end    description

    // And the following is the color map for headings:
    // 0 to end	    keyText

    if (attrs == 0  ||  width < MIN_CMD_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    WORD clrKey = (pos == 0 ? getColor (ceAssignKeyCursor) : getColor (ceAssignKey));
    WORD clrText = (pos == 0 ? getColor (ceAssignCommandCursor) : getColor (ceAssignCommand));

    int i;

    for (i = 0; i < 1; i++)
	attrs[i] = clrKey;
    for (i = 1; i < width; i++)
	attrs[i] = clrText;
}

// End
