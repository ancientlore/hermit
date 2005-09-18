// File Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef FILESCR_H
#define FILESCR_H

#include "console/scroller.h"
#include "scrolldir.h"
#include "console/edit.h"


#define FILTER_LEN	256

class FileScroller : public Scroller {
protected:
    ScrollDir	mScrollDir;
    int		mBookmarkFileCursor[10];    // These maintain the bookmarks
    int		mBookmarkScrCursor[10];
    char *	mpBookmark[10];
    int		mPrevFileCursor[10];	    // These maintain cursor positions when we move in/out dirs
    int		mPrevScrCursor[10];
    char *	mpLastDir;		    // The maintain the last directory visited
    int		mLastFileCursor;
    int		mLastScrCursor;
    History	mHist;			    // the history for the goto drive window
    char	mFilter[FILTER_LEN + 1];    // filter like *.txt
    History	mFilterHist;

    void updateStatusBar ();
    int getFirstRealFile ();
    void updateDriveCurrentDir ();
    void scan (int fileCursor = -1, int screenCursor = -1);
    void gotoLocation (const char *where, int fileCursor = -1, int screenCursor = -1);
    void addPrevCursor (int fileCursor, int scrCursor);
    void remPrevCursor (int& fileCursor, int& scrCursor);
    void clrPrevCursor ();
    static int cancelProc (void *pData);
    void loadBookmarks ();
    void saveBookmarks ();

    void left ();
    void right ();
    void toggle ();
    void tag ();
    void unTag ();
    void tagAll ();
    void unTagAll ();
    void reTagAll ();
    void gotoDrive (const Event& event);
    void gotoBookmark (const Event& event);
    void setBookmark (const Event& event);
    void selectBookmark ();
    void selectSorting ();
    void gotoPrevious ();
    void askLocation ();
    void goHome (bool useEnvVars = true);
    void setFilter ();
    int handleMouse (const Event& event);

public:
    FileScroller (Screen& screen, int x, int y, int w, int h);
    virtual ~FileScroller ();

    File& getCurrentFile ()		{ return mScrollDir.getCurrentFile (); }
    const File& getCurrentFile () const { return mScrollDir.getCurrentFile (); }

    const char *getDirName () const	{ return mScrollDir.getDirName (); }

    FileIterator *getIterator ()	{ return mScrollDir.getIterator (); }	// can throw exceptions

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed
};


#endif
