// Scrollable Message Viewer class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "console/scrollmsg.h"


ScrollMessage::ScrollMessage (const MessageInfo* pMsgs, int count) :
    mCursor (0), mMsgs (pMsgs), mnMsgs (count)
{
    if (mMsgs == 0  ||  mnMsgs <= 0)
	throw AppException (WHERE, ERR_INTERNAL);
}

ScrollMessage::~ScrollMessage ()
{
}

int ScrollMessage::isAtHome () const
{
    if (mCursor == 0)
	return 1;
    return 0;
}

int ScrollMessage::isAtEnd () const
{
    if (mCursor == mnMsgs - 1)
	return 1;
    return 0;
}

int ScrollMessage::moveCursor (int count)
{
    // If a negative count is more than mCursor, we must limit it to neg mCursor
    if (-count > mCursor)
	count = -mCursor;

    // If a positive count is more than size - cursor - 1, we must limit it too.
    if (count > mnMsgs - 1 - mCursor)
	count = mnMsgs - 1 - mCursor;

    // Set the cursor and return actual amount
    mCursor += count;
    return count;
}

int ScrollMessage::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollMessage::end ()
{
    int where = mCursor;
    mCursor = mnMsgs - 1;
    return mCursor - where;
}

int ScrollMessage::getText (int pos, char *line, int width) const
{
    if (line == 0  ||  width < MIN_MSG_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    const char *pStartOfLine = line;

    // Check for out of bounds
    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > mnMsgs - 1)
	return 0;

    line += wsprintf (line, "%-80.80s", mMsgs[mCursor + pos].text);

    return line - pStartOfLine;
}

void ScrollMessage::getAttributes (int pos, WORD *attrs, int width) const
{
    if (attrs == 0  ||  width < MIN_MSG_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    WORD clr;
    int i;

    if (mCursor + pos < 0  ||  mCursor + pos > mnMsgs - 1)
	clr = getColor (mMsgs[mCursor].color);
    else
	clr = (pos == 0 ? getColor (mMsgs[mCursor + pos].colorSelected) :
		 	  getColor (mMsgs[mCursor + pos].color));

    for (i = 0; i < width; i++)
	attrs[i] = clr;
}

// Sample
/*
MessageInfo msgs[] = {
    { ceNormal,	    ceSelected,	    "This is one of the lines." },
    { ceNormal2,    ceSelected2,    "This is another of the lines." }
};

int nMsgs = sizeof (msgs) / sizeof (MessageInfo);
*/

// End
