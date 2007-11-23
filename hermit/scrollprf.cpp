// Scrollable Printf class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "scrollprf.h"
#include <stdio.h>
#include <stdarg.h>


ScrollPrintfElem::~ScrollPrintfElem ()
{
    if (text != 0)
	free (text);
}

ScrollPrintfElem::ScrollPrintfElem (const ScrollPrintfElem& elem) :
    line (0), color (ceFirstEntry), text (0)
{
    set (elem.line, elem.color, elem.text);
}

ScrollPrintfElem& ScrollPrintfElem::operator = (const ScrollPrintfElem& elem)
{
    set (elem.line, elem.color, elem.text);
    return *this;
}

int ScrollPrintfElem::compare (const ScrollPrintfElem& elem) const
{
    if (line < elem.line)
	return -1;
    else if (line > elem.line)
	return 1;
    else
	return 0;
}

void ScrollPrintfElem::set (int l, ColorEntry c, const char *t)
{
    line = l;
    color = c;
    if (text)
	free (text);
    text = _strdup (t);
    if (text == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

ScrollPrintf::~ScrollPrintf ()
{
}

int ScrollPrintf::isAtHome () const
{
    if (mCursor == 0)
	return 1;
    return 0;
}

int ScrollPrintf::isAtEnd () const
{
    if (mCursor == (int) mLines.size () - 1)
	return 1;
    return 0;
}

int ScrollPrintf::moveCursor (int count)
{
    // If a negative count is more than mCursor, we must limit it to neg mCursor
    if (-count > mCursor)
	count = -mCursor;

    // If a positive count is more than size - cursor - 1, we must limit it too.
    if (count > (int) mLines.size () - 1 - mCursor)
	count = (int) mLines.size () - 1 - mCursor;

    // Set the cursor and return actual amount
    mCursor += count;
    return count;
}

int ScrollPrintf::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollPrintf::end ()
{
    int where = mCursor;
    mCursor = (int) mLines.size () - 1;
    return mCursor - where;
}

int ScrollPrintf::getText (int pos, char *line, int width) const
{
    if (line == 0)
	throw AppException (WHERE, ERR_INTERNAL);

    // Check for out of bounds
    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > (int) mLines.size () - 1)
	return 0;

    strncpy (line, mLines[mCursor + pos].getLine (), width);
    line[width] = 0;
    return (int) strlen (line);
}

void ScrollPrintf::getAttributes (int pos, WORD *attrs, int width) const
{
    if (attrs == 0)
	throw AppException (WHERE, ERR_INTERNAL);

    WORD cBack = (pos == 0 ? getColor (mcSel) : getColor (mcNorm));
    WORD color;

    // handle out of bounds case
    if (mCursor + pos < 0  ||  mCursor + pos > (int) mLines.size () - 1)
	color = cBack;
    else
	color = (cBack & (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)) |
		(getColor (mLines[mCursor + pos].getColor ()) &
		 (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));

    for (int i = 0; i < width; i++)
        attrs[i] = color;
}

void ScrollPrintf::printLine (ColorEntry color, const char *fmt, ...)
{
    va_list list;

    va_start (list, fmt);
    vPrintLine (color, fmt, list);
    va_end (list);
}

void ScrollPrintf::vPrintLine (ColorEntry color, const char *fmt, va_list list)
{
    ScrollPrintfElem elem;
    char text [256];	    // 80 is real limit but let's play it safe
    vsprintf (text, fmt, list);
    elem.set (mNextLine, color, text);
    mLines.add (elem);
    mNextLine++;
}

// End
