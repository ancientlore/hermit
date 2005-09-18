// Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef SCROLLER_H
#define SCROLLER_H

#include "console/screen.h"
#include "console/scrollable.h"


class Scroller : public Window {
protected:
    Scrollable * mpScrollable;	    // item to be "scrolled", NOT OWNED!
    int		mX;		    // X origin of scroller
    int		mY;		    // Y origin of scroller
    int		mWidth;		    // width of scroller
    int		mHeight;	    // height of scroller
    int		mCursor;	    // Current cursor location within height
    char *	mpLineBuf;	    // Line drawing buffer
    WORD *	mpAttrBuf;	    // Attribute drawing buffer
    int		mCursorPost;	    // Cursor changes are posted to screen if set

    void draw ();
    void drawLine (int line);	    // Line relative to the origin

    void pageUp ();
    void pageDown ();
    void up ();
    void down ();
    void center ();
    void move (int pos);
    void home ();
    void end ();

    void handleMovement (int realAmt);

public:
    Scroller (Screen& screen, int x, int y, int w, int h, int cursorPost = 1);
    virtual ~Scroller ();

    void setScrollable (Scrollable *pScrollable);
    Scrollable* getScrollable () const { return mpScrollable; }

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed
};


#endif
