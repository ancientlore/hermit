// Popup class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <string.h>
#include <ctype.h>
#include "console/screen.h"


void Popup::init ()
{
    COORD bufSize;
    COORD bufCoord;
    SMALL_RECT rect;

    // Check values for being "reasonable"
    if (mX < 0  ||  mY < 0  ||  mWidth <= 0  ||  mHeight <= 0)
	throw AppException (WHERE, ERR_INTERNAL);

    // Get screen size
    int scrWidth, scrHeight;
    myScreen.getSize (scrWidth, scrHeight);  // size of screen

    // Make sure it fits on the screen
    if (mX + mWidth > scrWidth  ||  mY + mHeight > scrHeight)
	throw AppException (WHERE, ERR_INTERNAL);

    // Allocate background buffer
    mpBackground = new CHAR_INFO [mWidth * mHeight];
    if (mpBackground == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    // Setup to copy
    bufSize.X = mWidth;
    bufSize.Y = mHeight;
    bufCoord.X = 0;
    bufCoord.Y = 0;
    rect.Left = mX;
    rect.Top = mY;
    rect.Right = mX + mWidth - 1;
    rect.Bottom = mY + mHeight - 1;

    // Save background
    if (ReadConsoleOutput (myScreen, mpBackground, bufSize, bufCoord, &rect) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "ReadConsoleOutput", GetLastError ());
}

Popup::Popup (Screen& screen, int x, int y, int w, int h) :
    Window (screen), mX (x), mY (y), mWidth (w), mHeight (h), mpBackground (0), mExitKey (KB_ESC)
{
    init ();
}

Popup::Popup (Screen& screen, int w, int h) :
    Window (screen), mX (0), mY (0), mWidth (w), mHeight (h), mpBackground (0), mExitKey (KB_ESC)
{
    // Get screen size
    int scrWidth, scrHeight;
    myScreen.getSize (scrWidth, scrHeight);  // size of screen

    // Compute the centers
    mX = scrWidth / 2 - (mWidth / 2 + mWidth % 2);
    mY = scrHeight / 2 - (mHeight / 2 + mHeight % 2);

    init ();
}

Popup::~Popup ()
{
    COORD bufSize;
    COORD bufCoord;
    SMALL_RECT rect;

    if (mpBackground != 0) {
	// setup for restore
	bufSize.X = mWidth;
	bufSize.Y = mHeight;
	bufCoord.X = 0;
	bufCoord.Y = 0;
	rect.Left = mX;
	rect.Top = mY;
	rect.Right = mX + mWidth - 1;
	rect.Bottom = mY + mHeight - 1;

	// restore background
	if (WriteConsoleOutput (myScreen, mpBackground, bufSize, bufCoord, &rect) != TRUE)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsoleOutput", GetLastError ());
    }

    // delete background buffer
    delete [] mpBackground;
}

void Popup::drawBorder (const char *title)
{
    if (mWidth < 3  ||  mHeight < 3)
	throw AppException (WHERE, ERR_INTERNAL);

    for (int y = mY; y < mY + mHeight; y++) {
	if (y == mY) {				// First row
	    myScreen.fillColors (mX, y, getColor (cePopupBorder), mWidth);
	    myScreen.fillChar (mX, y, ' ', 1); // \xC9
	    myScreen.fillChar (mX + 1, y, ' ', mWidth - 2); // \xCD
	    myScreen.fillChar (mX + mWidth - 1, y, ' ', 1); // \xBB
	    if (title) {
		int len = (int) strlen (title);
		if (len > mWidth - 2)
		    len = mWidth - 2;
		int x = mX + mWidth / 2 - (len / 2 + len % 2);
		myScreen.writeText (x, y, title, len);
	    }
    	}
	else if (y == mY + mHeight - 1) {	// Last row
	    myScreen.fillColors (mX, y, getColor (cePopupBorder), mWidth);
	    myScreen.fillChar (mX, y, ' ', 1); // \xC8
	    myScreen.fillChar (mX + 1, y, ' ', mWidth - 2); // \xCD
	    myScreen.fillChar (mX + mWidth - 1, y, ' ', 1); // \xBC
	}
	else {					// Normal rows
	    myScreen.fillChar (mX, y, ' ', 1); // \xBA
	    myScreen.fillColors (mX, y, getColor (cePopupBorder), 1);
	    myScreen.fillChar (mX + mWidth - 1, y, ' ', 1); // \xBA
	    myScreen.fillColors (mX + mWidth - 1, y, getColor (cePopupBorder), 1);
	}
    }
}

void Popup::drawBackground ()
{
    for (int y = mY; y < mY + mHeight; y++) {
	myScreen.fillChar (mX, y, ' ', mWidth);
	myScreen.fillColors (mX, y, getColor (cePopupBackground), mWidth);
    }
}

void Popup::drawField (int x, int y, const char *name, int sel)
{
    if (name == 0  ||  x < 0  ||  y < 0)
	throw AppException (WHERE, ERR_INTERNAL);

    myScreen.writeText (mX + x, mY + y, name);

    int normColor = getColor (ceField);
    int highColor = getColor (ceFieldKey);
    if (sel) {
	normColor = getColor (ceSelectedField);
	highColor = getColor (ceSelectedKey);
    }

    int l = (int) strlen (name);
    for (int i = 0; i < l; i++) {
	if (isupper (name[i])  ||  isdigit (name[i])) {
	    myScreen.fillColors (mX + x + i, mY + y, highColor, 1);
	    if (i < l - 1)
		myScreen.fillColors (mX + x + i + 1, mY + y, normColor, l - (i + 1));
	    break;
	}
	else
	    myScreen.fillColors (mX + x + i, mY + y, normColor, 1);
    }
}

// End
