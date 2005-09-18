// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef ASGNDLG_H
#define ASGNDLG_H

#include "console/screen.h"
#include "asgnscr.h"


class AssignCommandsDialog : public Popup {
    AssignCommandScroller *mpScroller;
    void draw ();
public:
    AssignCommandsDialog (Screen& screen);
    virtual ~AssignCommandsDialog ();
    virtual void run ();    // replace event loop
};


#endif
