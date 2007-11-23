// Screen handling
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef SCREEN_H
#define SCREEN_H

#define STRICT
#include <windows.h>
#include "common.h"
#include "tree_tpl.h"
#include "console/keys.h"
#include "console/events.h"
#include "console/colors.h"

// --------------------------------------------------------------------------------
//	EVENT PROCESSING BASE CLASS
// --------------------------------------------------------------------------------

struct Event {
    int key;
    DWORD data;
    Event () : key (0), data (0) {}
    Event (int k, DWORD d = 0) : key (k), data (d) {}
};

// keys can be processed by overriding processKey or by adding handler functions and setting them in
// the current screen

// The Processor is just a class that can process a key, nothing more
class Processor {
public:
    virtual int processEvent (const Event& event);	    // 0 for not processed, 1 for processed
					    // always invoke parent's method for unhandled keys
    virtual ~Processor ();
};

// This defines a handler function
typedef void (Processor::*Handler)(const Event& event);

// Internal info used to set up handlers
struct HandlerInfo {
    int		key;
    Processor*	handlerObject;
    Handler	handlerFunc;
    int compare (const HandlerInfo& info) const {
	if (key < info.key)
	    return -1;
	else if (key > info.key)
	    return 1;
	else
	    return 0;
    }
};

inline int operator == (const HandlerInfo& a, const HandlerInfo& b) {
    return (a.compare (b) == 0);
}

inline int operator <= (const HandlerInfo& a, const HandlerInfo& b) {
    return (a.compare (b) <= 0);
}

inline int operator >= (const HandlerInfo& a, const HandlerInfo& b) {
    return (a.compare (b) >= 0);
}

inline int operator < (const HandlerInfo& a, const HandlerInfo& b) {
    return (a.compare (b) < 0);
}

inline int operator > (const HandlerInfo& a, const HandlerInfo& b) {
    return (a.compare (b) > 0);
}

inline int operator != (const HandlerInfo& a, const HandlerInfo& b) {
    return (a.compare (b) != 0);
}

// Define storage for the handler info
typedef TTree<HandlerInfo> HandlerTree;
typedef TTreeIterator<HandlerInfo> HandlerIterator;

// Depth of posted keys
const int POST_DEPTH = 10;

// Handy functions
inline int isKey (const Event& event) {
    if (event.key > 0  &&  event.key < EV_FIRST)
	return 1;
    return 0;
}

inline int isEvent (const Event& event) {
    if (event.key < 0  ||  event.key >= EV_FIRST)
	return 1;
    return 0;
}

inline int isMouse (const Event& event) {
    if (event.key & MS_FLAG)
	return 1;
    return 0;
}

inline int isNull (const Event& event) {
    if (event.key == 0)
	return 1;
    return 0;
}

//  Runable is a class that can read key events and execute the handlers.  Handlers can be
// invoked via a handler method or by overriding processKey ().
class Runable : public Processor {
    HandlerTree mHandlers;	    // tree of event handlers to check
    Event mPostedKey[POST_DEPTH];	    // posted key (like posting an event)
    void dispatchEvent (const Event& event);
    void getPostedEvent (Event& event);	// returns next posted key or zero
    void getAnyEvent (Event& event, int key_only);
    int isUsefulEvent (INPUT_RECORD& record) const;
    void createInputHandle ();
    void destroyInputHandle ();
    static INPUT_RECORD mLastRecord;
    static HANDLE mInputHandle;
    static int mInputRefCount;
public:
    Runable ();
    virtual ~Runable ();

    // event-related stuff
    virtual void run ();        // event loop

    int hasEvent () const; 	// returns non-zero if an event is ready
    int hasKey () const; 	// returns non-zero if a new key is ready

    void getEvent (Event& event) { getAnyEvent (event, 0); }
    void getKey (Event& event) { getAnyEvent (event, 1); }

    void clearPostedKeys ();

    void addHandler (int key, Processor* object, Handler func);
    void removeHandler (int key);

    void postEvent (const Event& event, int overwrite = 0);
    void sendEvent (const Event& event) { dispatchEvent (event); }

    HANDLE getInputHandle () const { return mInputHandle; }
};

//  A class to change the standard input mode
class InputModeSwitcher {
    DWORD mOldMode;
    DWORD mNewMode;
    const Runable& mObj;
public:
    InputModeSwitcher (const Runable& obj, DWORD newMode = 0) : mObj (obj), mNewMode (newMode) {
	Sleep (40);		// wait for keydown to appear
	mObj.hasKey ();	// clears useless events like keyup before we get here
	if (GetConsoleMode (mObj.getInputHandle (), &mOldMode) != TRUE)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "GetConsoleMode", GetLastError ());
	if (SetConsoleMode (mObj.getInputHandle (), mNewMode) != TRUE)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleMode", GetLastError ());
    }

    ~InputModeSwitcher () {
	mObj.hasKey ();	// clears useless events like keyup before we go back
	if (SetConsoleMode (mObj.getInputHandle (), mOldMode) != TRUE)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleMode", GetLastError ());
    }

    DWORD getOldMode () {
	return mOldMode;
    }

    DWORD getNewMode () {
	return mNewMode;
    }
};

// --------------------------------------------------------------------------------
//	WINDOWS CLASSES
// --------------------------------------------------------------------------------

class Screen;

class Window : public Runable {
protected:
    Screen& myScreen;
public:
    Window (Screen& screen);
    virtual ~Window ();
};

class Status : public Window {
    int mY;
    int mWidth;
    char *mText;
    char *mTail;
    int mTailWidth;
    void draw ();
public:
    Status (Screen& screen);
    virtual ~Status ();
    void setStatusText (const char *txt);   // called anytime to update
    void setStatusTail (const char *txt);   // shown on very end
    virtual int processEvent (const Event& event);	    // 0 for not processed, 1 for processed
};

class TitleBar : public Window {
    int mY;
    int mWidth;
    char *mText;
    char *mTail;
    int mTailWidth;
    void draw ();
public:
    TitleBar (Screen& screen);
    virtual ~TitleBar ();
    void setTitleText (const char *txt);   // called anytime to update
    void setTitleTail (const char *txt);   // shown on very end
    virtual int processEvent (const Event& event);	    // 0 for not processed, 1 for processed
};

class Popup : public Window {
    void init ();
protected:
    int		mX;		    // X origin of Popup
    int		mY;		    // Y origin of Popup
    int		mWidth;		    // width of Popup
    int		mHeight;	    // height of Popup
    PCHAR_INFO	mpBackground;	    // Background of window
    int		mExitKey;

    void drawBorder (const char *title = 0);
    void drawBackground ();

    // These are all coords relative to the Popup window!
    void drawField (int x, int y, const char *name, int sel = 0);
    void setCursor (int size = 0);	// zero disables the cursor
    void setCursorPosition (int x = 0, int y = 0);
    int writeText (int x, int y, const char *text, int count = -1);
    int writeColors (int x, int y, const WORD *attrs, int count);
    void write (int x, int y, const CHAR_INFO *pBuf, int count);
    void fillChar (int x, int y, int c, int count);
    void fillColors (int x, int y, WORD attr, int count);

public:
    Popup (Screen& screen, int x, int y, int w, int h);
    Popup (Screen& screen, int w, int h);
    virtual ~Popup ();
    int getExitCode () const { return mExitKey; }
};

// --------------------------------------------------------------------------------
//	SCREEN CLASS
// --------------------------------------------------------------------------------

typedef TTree<Window*> WindowPtrTree;
typedef TTreeIterator<Window*> WindowPtrIterator;

class Screen : public Runable {	// a screen can process events too
    HANDLE  mScreenBuf;
    WindowPtrTree mWindows;
protected:
    InputModeSwitcher mModeSwitcher;
public:
    // Constructor / destructor
    Screen ();
    virtual ~Screen ();

    // console-related stuff
    void activate ();
    operator HANDLE () { return mScreenBuf; }

    // cursor stuff
    void setCursor (int size = 0);	// zero disables the cursor
    void setCursorPosition (int x = 0, int y = 0);

    // high-level drawing and info
    void getSize (int& x, int& y);  // size of screen
    int writeText (const char *text);	// this should be avoided in general as it is not the low-level
    int ask (const char *title, const char *msg);   // simple popup question
    void sendWinErrStatusMsg (const char *alternate);	// send windows formatted error status msg

    // drawing-related stuff
    void setColors (int attrs);
    int writeText (int x, int y, const char *text, int count = -1);
    int writeColors (int x, int y, const WORD *attrs, int count);
    void write (int x, int y, const CHAR_INFO *pBuf, int count);
    void fillChar (int x, int y, int c, int count);
    void fillColors (int x, int y, WORD attr, int count);
    void scroll (int destX, int destY, const SMALL_RECT& scrollRect, const SMALL_RECT *pClipRect);

    // window functions
    void addWindow (Window* pWindow);	    // allows window to automatically process events
    void removeWindow (Window* pWindow);
    virtual int processEvent (const Event& event);	    // 0 for not processed, 1 for processed
};

// --------------------------------------------------------------------------------
//	POPUP INLINES
// --------------------------------------------------------------------------------

inline void Popup::setCursor (int size) { myScreen.setCursor (size); }
inline void Popup::setCursorPosition (int x, int y) { myScreen.setCursorPosition (mX + x, mY + y); }
inline int Popup::writeText (int x, int y, const char *text, int count) { return myScreen.writeText (mX + x, mY + y, text, count); }
inline int Popup::writeColors (int x, int y, const WORD *attrs, int count) { return myScreen.writeColors (mX + x, mY + y, attrs, count); }
inline void Popup::write (int x, int y, const CHAR_INFO *pBuf, int count) { myScreen.write (mX + x, mY + y, pBuf, count); }
inline void Popup::fillChar (int x, int y, int c, int count) { myScreen.fillChar (mX + x, mY + y, c, count); }
inline void Popup::fillColors (int x, int y, WORD attr, int count) { myScreen.fillColors (mX + x, mY + y, attr, count); }

#endif
