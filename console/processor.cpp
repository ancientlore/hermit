// Screen handling
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/screen.h"
#include "waitfix.h"

// return -1 for error, 0 for not processed, 1 for processed
// always invoke parent's method for unhandled keys
int Processor::processEvent (const Event&)
{
    return 0;	    // no keys processed in root class
}

// virtual destructor
Processor::~Processor ()
{
}

HANDLE Runable::mInputHandle = INVALID_HANDLE_VALUE;
int Runable::mInputRefCount = 0;

Runable::Runable ()
{
    if (mInputRefCount == 0)
	createInputHandle ();
    mInputRefCount++;
}

Runable::~Runable ()
{
    if (mInputRefCount > 0)
	mInputRefCount--;
    if (mInputRefCount == 0)
	destroyInputHandle ();
}

void Runable::createInputHandle ()
{
    mInputHandle = CreateFile ("CONIN$",
			       GENERIC_READ | GENERIC_WRITE,
			       FILE_SHARE_READ | FILE_SHARE_WRITE,
			       NULL,
			       OPEN_EXISTING,
			       0,
			       NULL);
    if (mInputHandle == INVALID_HANDLE_VALUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "CreateFile", GetLastError ());
}

void Runable::destroyInputHandle ()
{
    CloseHandle (mInputHandle);	    // cannot do much if we fail
    mInputHandle = INVALID_HANDLE_VALUE;
}

void Runable::postEvent (const Event& event, int overwrite)
{
    // Check for overwrite -- only post one of these
    if (overwrite) {
	for (int i = POST_DEPTH - 1; i >= 0; i--) {
	    if (mPostedKey[i].key == event.key) {
		mPostedKey[i] = event;
		return;
	    }
	}
    }
    // See if the post depth has been exceeded
    if (mPostedKey [POST_DEPTH - 1].key != 0) {
	// throw errorMsg (WHERE, ERR_INTERNAL);
	MessageBeep (MB_OK);
	return;
    }
    // Move the other events
    for (int i = POST_DEPTH - 1; i > 0; i--)
	mPostedKey[i] = mPostedKey[i - 1];
    // Set the new event
    mPostedKey[0] = event;
}

void Runable::getPostedEvent (Event& event)
{
    int i = POST_DEPTH - 1;
    event.key = 0;
    event.data = 0;
    while (i >= 0) {
	if (mPostedKey[i].key != 0) {
	    event = mPostedKey[i];
	    mPostedKey[i].key = 0;
	    mPostedKey[i].data = 0;
	    break;
	}
	i--;
    }
}

void Runable::clearPostedKeys ()
{
    for (int i = 0; i < POST_DEPTH; i++) {
	if (isKey (mPostedKey[i])) {
	    for (int j = i; j < POST_DEPTH - 1; j++)
		mPostedKey[j] = mPostedKey[j + 1];
        }
    }
}

int Runable::isUsefulEvent (INPUT_RECORD& record) const
{
    return ( (record.EventType == KEY_EVENT  &&  record.Event.KeyEvent.bKeyDown == TRUE) ||
	     (record.EventType == MOUSE_EVENT) ||
	     (record.EventType == WINDOW_BUFFER_SIZE_EVENT) );
}

INPUT_RECORD Runable::mLastRecord;

int Runable::hasKey () const
{
    // Check for multi-key events
    if (mLastRecord.Event.KeyEvent.wRepeatCount > 1)
	return 1;

    // Check for events in the queue
    DWORD numRead = 0;
    INPUT_RECORD record;
    HANDLE input = getInputHandle ();

    // translate window messages (CoInitialize bug fix)
    ProcessWindowMessages ();

    // Peek the event (and flush out the ones that don't count)
    do {
	if (PeekConsoleInput (input, &record, 1, &numRead) != TRUE)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "PeekConsoleInput", GetLastError ());
	if (numRead > 0) {
	    if (!isUsefulEvent (record)) {
		if (ReadConsoleInput (input, &record, 1, &numRead) != TRUE)
		    throw AppException (WHERE, ERR_WINDOWS_FMT, "ReadConsoleInput", GetLastError ());
	    }
	    else
		return 3;	// there is an event
	}
    } while (numRead > 0);

    // There are no events ready
    return 0;
}

int Runable::hasEvent () const
{
    // Check for events that are posted
    int i = POST_DEPTH - 1;
    while (i >= 0) {
        if (mPostedKey[i].key != 0)
    	    return 2;
        i--;
    }

    return hasKey ();
}

void Runable::getAnyEvent (Event& event, int key_only)
{
    INPUT_RECORD record;
    DWORD numRead = 0;

    // Check for and return a posted key, if present
    if (!key_only) {
	getPostedEvent (event);
	if (!isNull (event))
	    return;
    }

    // Get standard input
    HANDLE input = getInputHandle ();

    // Read console until we get a suitable event
    if (mLastRecord.Event.KeyEvent.wRepeatCount > 1) {
        record = mLastRecord;
        mLastRecord.Event.KeyEvent.wRepeatCount--;
    }
    else {
	do {
#if 1	// now that console mode is raw, we don't need this to get Control-C, but for CoInitialize fix
	    // Peek until we get an event (check for posted events meanwhile)
	    do {
		// Check for posted events
    		if (!key_only) {
		    getPostedEvent (event);
		    if (event.key != 0)
			return;
		}
		// See how many real events
		if (GetNumberOfConsoleInputEvents (input, &numRead) != TRUE)
    		    throw AppException (WHERE, ERR_WINDOWS_FMT, "GetNumberOfConsoleInputEvents", GetLastError ());
		// If there are none, wait for them
		if (numRead <= 0) {
		    // WaitForSingleObject (input, INFINITE);
		    WaitWithMessageLoop (input);
		}
	    }
	    while (numRead <= 0);
#endif
	    // Read the event (now that we know we have one)
	    if (ReadConsoleInput (input, &record, 1, &numRead) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "ReadConsoleInput", GetLastError ());
	} while (!isUsefulEvent (record));

	if (record.EventType == KEY_EVENT)
	    mLastRecord = record;
    }

    if (record.EventType == KEY_EVENT) {
    // Print debug information
#if 0
    char str[90];
    wsprintf (str, "repeat=%2d vKeyCode=%4d vScanCode=%4d control=%4x",
	      record.Event.KeyEvent.wRepeatCount,
	      record.Event.KeyEvent.wVirtualKeyCode,
	      record.Event.KeyEvent.wVirtualScanCode,
	      record.Event.KeyEvent.dwControlKeyState);
    DWORD charsWritten;
    WriteConsole (GetStdHandle (STD_OUTPUT_HANDLE), str, strlen (str), &charsWritten, 0);
    if (record.Event.KeyEvent.uChar.AsciiChar != 0) {
	wsprintf (str, " ascii=\"%c\" %d", record.Event.KeyEvent.uChar.AsciiChar,
		  (int)record.Event.KeyEvent.uChar.AsciiChar);
	WriteConsole (GetStdHandle (STD_OUTPUT_HANDLE), str, strlen (str), &charsWritten, 0);
    }
#endif

    // Munge the key into what we want
    int flags = 0;
    if (record.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
	flags |= 0x0100;
    if (record.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
	flags |= 0x0200;
    if (record.Event.KeyEvent.dwControlKeyState & (ENHANCED_KEY))
	flags |= 0x0800;
    if (flags == 0  &&  record.Event.KeyEvent.uChar.AsciiChar != 0) {
	event.key = record.Event.KeyEvent.uChar.AsciiChar;
	event.data = 0;
	if ((record.Event.KeyEvent.dwControlKeyState & (SHIFT_PRESSED))  &&
		event.key == 9)
	    event.key = 15;	// backtab
	return;
    }
    if (flags == 0) // because it's not an ascii char we need another flag
	flags |= 0x1000;
    if (record.Event.KeyEvent.dwControlKeyState & (SHIFT_PRESSED))
	flags |= 0x0400;
    event.key = record.Event.KeyEvent.wVirtualKeyCode | flags;
    event.data = 0;

    // Print debug information
#if 0
    char str2[30];
    wsprintf (str2, "key=%d ", event.key);
    DWORD charsWritten2;
    WriteConsole (GetStdHandle (STD_OUTPUT_HANDLE), str2, strlen (str2), &charsWritten2, 0);
#endif
    }
    else if (record.EventType == MOUSE_EVENT) {
	int flags = MS_FLAG;	    // indicates mouse event

	if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
	    flags |= MS_LEFTBUTTON;
	if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
	    flags |= MS_MIDDLEBUTTON;
	if (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
	    flags |= MS_RIGHTBUTTON;

	if (record.Event.MouseEvent.dwControlKeyState & (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED))
	    flags |= MS_ALT;
	if (record.Event.MouseEvent.dwControlKeyState & (RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED))
	    flags |= MS_CTRL;
	if (record.Event.MouseEvent.dwControlKeyState & (SHIFT_PRESSED))
	    flags |= MS_SHIFT;

	if (record.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK)
	    flags |= MS_DBLCLICK;
	if (record.Event.MouseEvent.dwEventFlags & MOUSE_MOVED)
	    flags |= MS_MOVED;
	if (!(record.Event.MouseEvent.dwEventFlags & (DOUBLE_CLICK | MOUSE_MOVED)))
	    flags |= MS_BUTTON;

	event.key = flags;
	event.data = *(reinterpret_cast<DWORD*>(&(record.Event.MouseEvent.dwMousePosition)));
    }
    else if (record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
	event.key = CW_RESIZE;
	event.data = *(reinterpret_cast<DWORD*>(&(record.Event.WindowBufferSizeEvent.dwSize)));
    }
    else
	throw AppException (WHERE, ERR_INTERNAL);	    // should always get a valid event
}

void Runable::run ()
{
    do {
	// Get next key
	Event event;
	getEvent (event);

	// Dispatch the key
	dispatchEvent (event);

	// Check for quit event (we do this after dispatch to allow something to happen first)
	if (event.key == EV_QUIT)
	    return;

    } while (1);
}

void Runable::dispatchEvent (const Event& event)
{
    HandlerInfo tmp;
    int handled = 0;

    // Check for a handler
    tmp.key = event.key;
    tmp.handlerObject = 0;
    tmp.handlerFunc = 0;
    try {
        HandlerInfo info = mHandlers.find (tmp);
        if (info.handlerObject != 0  &&  info.handlerFunc != 0) {
    	    (info.handlerObject->*info.handlerFunc) (event);
    	    handled = 1;
        }
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)
	    throw;	    // rethrow because it isn't the "normal" case
    }

    // Try the processKey mechanism
    if (!handled)
        handled = processEvent (event);

    // if it is still not handled we can do something.  But we are not right now.
}

void Runable::addHandler (int key, Processor* object, Handler func)
{
    HandlerInfo info;
    info.key = key;
    info.handlerObject = object;
    info.handlerFunc = func;
    mHandlers.add (info);	// this could throw out of mem or duplicate
}

void Runable::removeHandler (int key)
{
    HandlerInfo info;
    info.key = key;
    info.handlerObject = 0;
    info.handlerFunc = 0;
    try {
	mHandlers.remove (info);	// I catch the errors because I don't want to fail if
    }					// it's not in there.
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)
	    throw;	    // rethrow
    }
}


// End
