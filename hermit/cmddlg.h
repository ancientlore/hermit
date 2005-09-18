// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef CMDDLG_H
#define CMDDLG_H

#include "console/screen.h"
#include "cmdscr.h"


class CommandDialog : public Popup {
    CommandScroller *mpScroller;
    void draw ();
public:
    CommandDialog (Screen& screen);
    virtual ~CommandDialog ();
    virtual void run ();    // replace event loop
};


#endif
