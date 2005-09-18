// Command Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef PICKCMDDLG_H
#define PICKCMDDLG_H

#include "console/screen.h"
#include "pickcmdscr.h"


class PickCommandsDialog : public Popup {
    PickCommandScroller *mpScroller;
    int mMulti;
    void draw ();
public:
    PickCommandsDialog (Screen& screen, int multi = 0);
    virtual ~PickCommandsDialog ();
    virtual void run ();    // replace event loop
};


#endif
