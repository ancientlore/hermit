// Screen handling
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/screen.h"
#include "console/askdlg.h"


#pragma warning (disable : 4355)
Screen::Screen () :
    mModeSwitcher (static_cast<Runable&>(*this), ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)
{
    mScreenBuf = CreateConsoleScreenBuffer (GENERIC_READ | GENERIC_WRITE, 0, 0,
					    CONSOLE_TEXTMODE_BUFFER, 0);
    if (mScreenBuf == INVALID_HANDLE_VALUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "CreateConsoleScreenBuffer", GetLastError ());
    setCursor ();   // disable cursor
}
#pragma warning (default : 4355)

Screen::~Screen ()
{
    int s = mWindows.size ();
    for (int i = 0; i < s; i++)
	delete mWindows[0];	// they get removed while we process!!!  So we keep deleting 0

    CloseHandle (mScreenBuf);	// if it fails, so what?  Nothing we can do
}

void Screen::activate ()
{
    if (SetConsoleActiveScreenBuffer (mScreenBuf) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleActiveScreenBuffer", GetLastError ());
}

void Screen::setCursor (int size)
{
    CONSOLE_CURSOR_INFO info;
    if (size <= 0) {
	info.bVisible = FALSE;
	info.dwSize = 1;
    }
    else {
	info.bVisible = TRUE;
	info.dwSize = (size <= 100 ? size : 100);
    }
    if (SetConsoleCursorInfo (mScreenBuf, &info) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleCursorInfo", GetLastError ());
}

void Screen::setCursorPosition (int x, int y)
{
    COORD pos;
    pos.X = (SHORT) x;
    pos.Y = (SHORT) y;
    if (SetConsoleCursorPosition (mScreenBuf, pos) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleCursorPosition", GetLastError ());
}

void Screen::setColors (int attrs)
{
    if (SetConsoleTextAttribute (mScreenBuf, (WORD) attrs) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleTextAttribute", GetLastError ());
}

void Screen::getSize (int& x, int& y)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo (mScreenBuf, &info) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "GetConsoleScreenBufferInfo", GetLastError ());
    x = info.dwSize.X;
    y = info.dwSize.Y;
}

int Screen::writeText (const char *text)
{
    DWORD charsWritten;
    if (text == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    if (WriteConsole (mScreenBuf, text, (DWORD) strlen (text), &charsWritten, 0) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsole", GetLastError ());
    return charsWritten;
}

int Screen::writeText (int x, int y, const char *text, int count)
{
    DWORD charsWritten;
    if (text == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    if (count <= 0)
	count = (int) strlen (text);
    COORD c;
    c.X = (SHORT) x;
    c.Y = (SHORT) y;
    if (WriteConsoleOutputCharacter (mScreenBuf, text, count, c, &charsWritten) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsoleOutputCharacter", GetLastError ());
    return charsWritten;
}

int Screen::writeColors (int x, int y, const WORD *attrs, int count)
{
    DWORD attrsWritten;
    if (attrs == 0  ||  count <= 0)
	throw AppException (WHERE, ERR_INTERNAL);
    COORD c;
    c.X = (SHORT) x;
    c.Y = (SHORT) y;
    if (WriteConsoleOutputAttribute (mScreenBuf, attrs, count, c, &attrsWritten) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsoleOutputAttribute", GetLastError ());
    return attrsWritten;
}

void Screen::fillChar (int x, int y, int c, int count)
{
    DWORD charsWritten;
    if (count <= 0)
	throw AppException (WHERE, ERR_INTERNAL);
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    if (FillConsoleOutputCharacter (mScreenBuf, c, count, coord, &charsWritten) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "FillConsoleOutputCharacter", GetLastError ());
}

void Screen::fillColors (int x, int y, WORD attr, int count)
{
    DWORD attrsWritten;
    if (count <= 0)
	throw AppException (WHERE, ERR_INTERNAL);
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    if (FillConsoleOutputAttribute (mScreenBuf, attr, count, coord, &attrsWritten) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "FillConsoleOutputAttribute", GetLastError ());
}

void Screen::write (int x, int y, const CHAR_INFO *pBuf, int count)
{
    if (pBuf == 0  ||  count <= 0)
	throw AppException (WHERE, ERR_INTERNAL);

    COORD bufSize, bufOrigin;
    bufSize.X = (SHORT) count;
    bufSize.Y = 1;
    bufOrigin.X = bufOrigin.Y = 0;
    SMALL_RECT rect;
    rect.Left = (SHORT) x;
    rect.Top = (SHORT) y;
    rect.Right = (SHORT) (x + count - 1);
    rect.Bottom = (SHORT) y;
    if (WriteConsoleOutput (mScreenBuf, pBuf, bufSize, bufOrigin, &rect) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsoleOutput", GetLastError ());
}

void Screen::scroll (int destX, int destY, const SMALL_RECT& scrollRect, const SMALL_RECT *pClipRect)
{
    CHAR_INFO fill;
    fill.Char.AsciiChar = ' ';
    fill.Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
    COORD dest;
    dest.X = destX;
    dest.Y = destY;
    if (ScrollConsoleScreenBuffer (mScreenBuf, &scrollRect, pClipRect, dest, &fill) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "ScrollConsoleScreenBuffer", GetLastError ());
}

void Screen::addWindow (Window* pWindow)
{
    mWindows.add (pWindow);	    // could throw out of mem
}

void Screen::removeWindow (Window* pWindow)
{
    AppException::AutoFreezer a; // freeze and thaw errors so this function cannot register any!
    try {
	mWindows.remove (pWindow);
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)
	    throw;
    }
}

int Screen::processEvent (const Event& event)
{
    int handled = 0;
    WindowPtrIterator iter (mWindows);
    while (!iter.done ()) {
	Window *pw = iter;
	handled += pw->processEvent (event);
	iter++;
    }
    handled += Runable::processEvent (event);
    return handled;
}

int Screen::ask (const char *title, const char *msg)
{
    if (title == 0  ||  msg == 0)
	throw AppException (WHERE, ERR_INTERNAL);

    AskDialog dlg (*this, title, msg);
    dlg.run ();
    if (dlg.getExitCode () == KB_ENTER)
	return 1;
    else
	return 0;
}

void Screen::sendWinErrStatusMsg (const char *alternate)
{
    Event e;
    e.key = EV_STATUS_MSG;
    LPVOID lpMsgBuf = 0;
    if (FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		       0, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		       (LPTSTR) &lpMsgBuf, 0, 0) > 0) {
	e.data = (DWORD) lpMsgBuf;
	char *ptr = (char *) lpMsgBuf;
	int l = (int) strlen (ptr);
	for (int i = l - 1; i >= l - 2; i--) {	// trim cr-lf
	    if (i >= 0  &&  (ptr[i] == '\r'  ||  ptr[i] == '\n'))
		ptr[i] = '\0';
	}
	sendEvent (e);
	LocalFree (lpMsgBuf);
    }
    else {
	e.data = (DWORD) alternate;
	postEvent (e, 1);
    }
}

// End
