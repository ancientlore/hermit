// Scrollable Configuration Listing class
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "scrollcfg.h"
#include "registry.h"


ScrollConfig::~ScrollConfig ()
{
}

int ScrollConfig::isAtHome () const
{
    if (mCursor == 0)
		return 1;
    return 0;
}

int ScrollConfig::isAtEnd () const
{
    if (mCursor == mnCfg - 1)
	return 1;
    return 0;
}

int ScrollConfig::moveCursor (int count)
{
    // If a negative count is more than mCursor, we must limit it to neg mCursor
    if (-count > mCursor)
	count = -mCursor;

    // If a positive count is more than size - cursor - 1, we must limit it too.
    if (count > mnCfg - 1 - mCursor)
	count = mnCfg - 1 - mCursor;

    // Set the cursor and return actual amount
    mCursor += count;
    return count;
}

int ScrollConfig::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollConfig::end ()
{
    int where = mCursor;
    mCursor = mnCfg - 1;
    return mCursor - where;
}

int ScrollConfig::getText (int pos, char *line, int width) const
{
    if (line == 0)
	throw AppException (WHERE, ERR_INTERNAL);

    // Check for out of bounds
    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > mnCfg - 1)
	return 0;

    strncpy (line, mCfg[mCursor + pos], width);
    line[width] = '\0';
    return strlen (line);
}

void ScrollConfig::getAttributes (int pos, WORD *attrs, int width) const
{
    if (attrs == 0)
	throw AppException (WHERE, ERR_INTERNAL);

    WORD clr = (pos == 0 ? getColor (ceVerbCursor) : getColor (ceVerb));

    for (int i = 0; i < width; i++)
	attrs[i] = clr;
}

const char *ScrollConfig::mCfg[] = {
    "Select Color Scheme",
    "Export Custom Commands",
    "Import Custom Commands",
    "Register DLL Server",
    "Unregister DLL Server"
};

const int ScrollConfig::mnCfg = sizeof (mCfg) / sizeof (const char *);

// End
