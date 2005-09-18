// Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/scroller.h"


Scroller::Scroller (Screen& screen, int x, int y, int w, int h, int cursorPost) :
    Window (screen), mpScrollable (0), mX (x), mY (y), mWidth (w), mHeight (h),
    mCursor (0), mCursorPost (cursorPost)
{
    // Check values for being "reasonable"
    if (x < 0  ||  y < 0  ||  w <= 0  ||  h <= 0)
	throw AppException (WHERE, ERR_INTERNAL);

    // Get screen size
    int scrWidth, scrHeight;
    myScreen.getSize (scrWidth, scrHeight);  // size of screen

    // Make sure it fits on the screen
    if (x + w > scrWidth  ||  y + h > scrHeight)
	throw AppException (WHERE, ERR_INTERNAL);

    // Allocate drawing buffers
    mpLineBuf = new char [mWidth + 1];
    if (mpLineBuf == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mpAttrBuf = new WORD [mWidth];
    if (mpAttrBuf == 0) {
	delete [] mpLineBuf;
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    }

    // Draw it
    draw ();
    if (mCursorPost)
	myScreen.postEvent (Event (EV_CURSOR_CHANGE, mCursor));
}

Scroller::~Scroller ()
{
    delete [] mpLineBuf;
    delete [] mpAttrBuf;
}

void Scroller::setScrollable (Scrollable *pScrollable)
{
    mpScrollable = pScrollable;
    mCursor = 0;
    draw ();
    if (mCursorPost)
	myScreen.postEvent (Event (EV_CURSOR_CHANGE, mCursor));
}

int Scroller::processEvent (const Event& event)
{
    int mouseX;
    int mouseY;
#if 0
    static int lastY = -1;
#endif

    switch (event.key) {

    case KB_UP:
    case KB_KP_UP:
	up ();
	break;

    case KB_DOWN:
    case KB_KP_DOWN:
	down ();
	break;

    case KB_PGUP:
    case KB_KP_PGUP:
	pageUp ();
	break;

    case KB_PGDN:
    case KB_KP_PGDN:
	pageDown ();
	break;

    case KB_HOME:
    case KB_KP_HOME:
	home ();
	break;

    case KB_END:
    case KB_KP_END:
	end ();
	break;

    case KB_KP_CENTER:
	center ();
	break;

    case MS_FLAG | MS_LEFTBUTTON | MS_MOVED:
	mouseX = LOWORD (event.data);
	mouseY = HIWORD (event.data);
	if (mouseX >= mX  &&  mouseX < mX + mWidth  &&
	    mouseY >= mY  &&  mouseY < mY + mHeight) {
	    move (mouseY - mY  - mCursor);
	}
	else
	    return Window::processEvent (event);
	break;

    case MS_FLAG | MS_LEFTBUTTON | MS_BUTTON:
	mouseX = LOWORD (event.data);
	mouseY = HIWORD (event.data);
	if (mouseX >= mX  &&  mouseX < mX + mWidth  &&
	    mouseY >= mY  &&  mouseY < mY + mHeight) {
	    move (mouseY - mY  - mCursor);
	}
	else
	    return Window::processEvent (event);
	break;

#if 0
    case MS_FLAG | MS_MIDDLEBUTTON | MS_BUTTON:
	mouseX = LOWORD (event.data);
	mouseY = HIWORD (event.data);
	if (mouseX >= mX  &&  mouseX < mX + mWidth  &&
	    mouseY >= mY  &&  mouseY < mY + mHeight) {
	    lastY = HIWORD (event.data);
	    move (mouseY - mY  - mCursor);
	}
	else
	    return Window::processEvent (event);
	break;

    case MS_FLAG | MS_MIDDLEBUTTON | MS_MOVED:
	mouseX = LOWORD (event.data);
	mouseY = HIWORD (event.data);
	if (mouseX >= mX  &&  mouseX < mX + mWidth  &&
	    mouseY >= mY  &&  mouseY < mY + mHeight) {
	    if (mouseY > lastY)
		move (mHeight / 3);
	    else if (mouseY < lastY)
		move (- mHeight / 3);
	}
	else
	    return Window::processEvent (event);
	break;

    case MS_FLAG | MS_BUTTON:
        lastY = -1;
        return Window::processEvent (event);
#endif

    case EV_COLOR_CHANGE:
	draw ();
	return Window::processEvent (event);

    default:
        return Window::processEvent (event);
    }

    return 1;
}

void Scroller::pageUp ()
{
    move (-(mHeight - 1));
}

void Scroller::pageDown ()
{
    move (mHeight - 1);
}

void Scroller::up ()
{
    move (-1);
}

void Scroller::down ()
{
    move (1);
}

void Scroller::center ()
{
    move (mHeight / 2 - mCursor);
}

void Scroller::move (int pos)
{
    // Make sure we really have a scrollable
    if (mpScrollable == 0)
	return;

    // Move the Scrollable's cursor by the designated amount
    int amt = mpScrollable->moveCursor (pos);

    // Handle the movement
    handleMovement (amt);
}

void Scroller::handleMovement (int realAmt)
{
    if (realAmt == 0)
	return;	    // nothing changed

    SMALL_RECT clip;
    SMALL_RECT scroll;

    // Save the Scroller's cursor and adjust it for the movement
    int oldCursor = mCursor;
    mCursor += realAmt;

    // Test the new cursor to see if it's on the screen
    if (mCursor < 0) {
	// Cursor is too high
  	int amt = -mCursor;	// number of lines we need to scroll DOWN
	mCursor = 0;
	if (amt < mHeight) {	// scroll lines if possible
	    // Set up clip rectangle
	    clip.Left = mX;
	    clip.Top = mY;
	    clip.Right = mX + mWidth - 1;
	    clip.Bottom = mY + mHeight - 1;

	    // Set up scroll rectangle
	    scroll.Left = mX;
	    scroll.Top = mY;
	    scroll.Right = mX + mWidth - 1;
	    scroll.Bottom = mY + mHeight - amt - 1;

	    // Scroll the screen
	    myScreen.scroll (mX, mY + amt, scroll, &clip);

	    // Fill in remaining holes
	    for (int i = 0; i < amt; i++)
		drawLine (i);

	    // Make sure highlights are correct
	    if (oldCursor + amt < mHeight)
		drawLine (oldCursor + amt);
	}
	else
	    draw ();
    }
    else if (mCursor >= mHeight) {
	// Cursor is too low
	int amt = mCursor - (mHeight - 1);	// Number of lines we need to scroll UP
	mCursor = mHeight - 1;
	if (amt < mHeight) {	// scroll lines if possible
	    // Set up clip rectangle
	    clip.Left = mX;
	    clip.Top = mY;
	    clip.Right = mX + mWidth - 1;
	    clip.Bottom = mY + mHeight - 1;

	    // Set up scroll rectangle
	    scroll.Left = mX;
	    scroll.Top = mY + amt;
	    scroll.Right = mX + mWidth - 1;
	    scroll.Bottom = mY + mHeight - 1;

	    // Scroll the screen
	    myScreen.scroll (mX, mY, scroll, &clip);

	    // Fill in remaining holes
	    for (int i = mHeight - amt; i < mHeight; i++)
		drawLine (i);

	    // Make sure highlights are correct
	    if (oldCursor - amt >= 0)
		drawLine (oldCursor - amt);
        }
	else
	    draw ();
    }
    else {
	// Cursor fits!  We just need to redraw the lines with the new and old cursor
	drawLine (oldCursor);
	drawLine (mCursor);
    }
    if (mCursorPost)
	myScreen.postEvent (Event (EV_CURSOR_CHANGE, mCursor));
}

void Scroller::home ()
{
    // Make sure we really have a scrollable
    if (mpScrollable == 0)
	return;

    // Move the Scrollable's cursor by the designated amount
    int amt = mpScrollable->home ();

    // Handle the movement
    handleMovement (amt);
}

void Scroller::end ()
{
    // Make sure we really have a scrollable
    if (mpScrollable == 0)
	return;

    // Move the Scrollable's cursor by the designated amount
    int amt = mpScrollable->end ();

    // Handle the movement
    handleMovement (amt);
}

void Scroller::drawLine (int line)
{
    if (line < 0  ||  line >= mHeight)
	throw AppException (WHERE, ERR_INTERNAL);

    if (mpScrollable != 0) {

	// Get text line and attributes
	int written = mpScrollable->getText (line - mCursor, mpLineBuf, mWidth);
	mpScrollable->getAttributes (line - mCursor, mpAttrBuf, mWidth);

	// Draw attributes to screen
	myScreen.writeColors (mX, mY + line, mpAttrBuf, mWidth);

	// Draw text to screen
	if (written > 0)
	    myScreen.writeText (mX, mY + line, mpLineBuf, written);

	// Fill end of text with spaces
	if (mWidth - written > 0)
	    myScreen.fillChar (mX + written, mY + line, ' ', mWidth - written);
    }
    else {  // no Scrollable!
	myScreen.fillChar (mX, mY + line, ' ', mWidth);
	myScreen.fillColors (mX, mY + line, 0, mWidth);
    }
}

void Scroller::draw ()
{
    for (int i = 0; i < mHeight; i++)
	drawLine (i);
}

// End
