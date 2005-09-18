// Edit Window
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef EDIT_H
#define EDIT_H

#include "console/screen.h"


#define HIST_SIZE  20

class History {
    char *mpText[HIST_SIZE];
    void add (const char *text);
    const char *mName;
public:
    History (const char *name);
    ~History ();
    friend class Edit;
};


class Edit : public Popup {
    History *mpHist;
    char *mpText;
    char *mpRealText;
    int mLength;
    int mMaxLength;
    int mbInsMode;
    int mWindowCursor;
    int mTextCursor;
    int mHistIndex;
protected:
    void draw ();
    void drawText ();
    void processText (const Event& event);
    void up ();
    void down ();
    void left ();
    void right ();
    void backSpace ();
    void tab ();
    void home ();
    void end ();
    void ins ();
    void del ();
public:
    Edit (Screen& screen, int x, int y, int w, char *text, int length, History *pHist = 0);
    virtual ~Edit ();
    virtual int processEvent (const Event& event);
};


#endif
