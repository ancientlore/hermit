// Scrollable Verb Listing class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "scrverb.h"

ScrollVerbs::ScrollVerbs (int verbc, const char *verbv[]) :
    mCursor (0), mVerbC (verbc), mVerbV (verbv)
{
    if (mVerbC <= 0  ||  mVerbV == 0)
	throw AppException (WHERE, ERR_INTERNAL);
}

ScrollVerbs::~ScrollVerbs ()
{
}

int ScrollVerbs::isAtHome () const
{
    if (mCursor == 0)
	return 1;
    return 0;
}

int ScrollVerbs::isAtEnd () const
{
    if (mCursor == mVerbC - 1)
	return 1;
    return 0;
}

int ScrollVerbs::moveCursor (int count)
{
    // If a negative count is more than mCursor, we must limit it to neg mCursor
    if (-count > mCursor)
	count = -mCursor;

    // If a positive count is more than size - cursor - 1, we must limit it too.
    if (count > mVerbC - 1 - mCursor)
	count = mVerbC - 1 - mCursor;

    // Set the cursor and return actual amount
    mCursor += count;
    return count;
}

int ScrollVerbs::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollVerbs::end ()
{
    int where = mCursor;
    mCursor = mVerbC - 1;
    return mCursor - where;
}

int ScrollVerbs::getText (int pos, char *line, int width) const
{
    if (line == 0  ||  width < 1)
	throw AppException (WHERE, ERR_INTERNAL);

    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > mVerbC - 1)
	return 0;

    strncpy (line, mVerbV[mCursor + pos], width);
    line[width] = '\0';

    return (int) strlen (line);
}

void ScrollVerbs::getAttributes (int pos, WORD *attrs, int width) const
{
    if (attrs == 0  ||  width < 1)
	throw AppException (WHERE, ERR_INTERNAL);

    WORD clr = (pos == 0 ? getColor (ceVerbCursor) : getColor (ceVerb));

    for (int i = 0; i < width; i++)
	attrs[i] = clr;
}

// End
