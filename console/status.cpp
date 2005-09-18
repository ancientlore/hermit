// Status Bar
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <string.h>
#include "console/screen.h"


Status::Status (Screen& screen) : Window (screen), mText (0), mTail (0), mTailWidth (0)
{
    myScreen.getSize (mWidth, mY);
    mY--;
    draw ();
}

Status::~Status ()
{
    if (mTail != 0)
	free (mTail);
    if (mText != 0)
	free (mText);
}

void Status::setStatusText (const char *txt)
{
    if (mText != 0) {
	free (mText);
	mText = 0;
    }
    if (txt != 0) {
	mText = strdup (txt);
	if (mText == 0)
	    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    }
    draw ();
}

void Status::setStatusTail (const char *txt)
{
    if (mTail != 0) {
	free (mTail);
	mTail = 0;
	mTailWidth = 0;
    }
    if (txt != 0) {
	mTail = strdup (txt);
	if (mTail == 0)
	    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	mTailWidth = (int) strlen (mTail);
	if (mTailWidth > 20)
	    mTailWidth = 20;
    }
    draw ();
}

void Status::draw ()
{
    // write text fill colors
    myScreen.fillColors (0, mY, getColor (ceStatusText), mWidth - mTailWidth);

    // Write tail fill colors
    if (mTailWidth > 0)
	myScreen.fillColors (mWidth - mTailWidth, mY, getColor (ceStatusTail), mTailWidth);

    // define some vars
    int written = 0;			    // number of characters written so far
    int toWrite = mWidth - mTailWidth;	    // number of characters we need to write right now

    // Write text portion
    if (mText != 0) {
	int c = (int) strlen (mText);
	if (c >= toWrite)
	    c = toWrite - 1;
	written = myScreen.writeText (0, mY, mText, c);
    }

    // Write padding
    if (toWrite - written > 0) {
	myScreen.fillChar (written, mY, ' ', toWrite - written);
	written = toWrite;
    }

    // adjust number to write by the tail
    toWrite += mTailWidth;

    // write tail
    if (mTail != 0) {
	int c = (int) strlen (mTail);
	if (c > toWrite - written)
	    c = toWrite - written;
	written += myScreen.writeText (written, mY, mTail, c);
    }

    // it looks like we should write tail padding, but mTailWidth is always the size of the
    // text and is always zero if mTail is NULL.
}

int Status::processEvent (const Event& event)
{
    if (event.key == EV_STATUS_MSG) {
	setStatusText ((const char *)event.data);
	return 1;
    }
    else if (event.key == EV_STATUS_EXTRA) {
	setStatusTail ((const char *)event.data);
	return 1;
    }
    else if ((event.key == (MS_FLAG | MS_LEFTBUTTON | MS_BUTTON)  ||
	     (event.key == (MS_FLAG | MS_LEFTBUTTON | MS_DBLCLICK)))  &&
	     HIWORD (event.data) == mY) {
	myScreen.postEvent (Event (KB_PGDN));
	return 1;
    }
    else if ((event.key == (MS_FLAG | MS_RIGHTBUTTON | MS_BUTTON)  ||
	     (event.key == (MS_FLAG | MS_RIGHTBUTTON | MS_DBLCLICK)))  &&
	     HIWORD (event.data) == mY) {
	myScreen.postEvent (Event (KB_PGUP));
	return 1;
    }
    else if (event.key == EV_COLOR_CHANGE)
	draw ();

    return Window::processEvent (event);
}


// End
