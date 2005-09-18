// Config Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef CFGDLG_H
#define CFGDLG_H

#include "console/screen.h"
#include "cfgscr.h"


class ConfigDialog : public Popup {
    ConfigScroller *mpScroller;
    void draw ();
public:
    ConfigDialog (Screen& screen);
    virtual ~ConfigDialog ();
    virtual void run ();    // replace event loop

    friend class ConfigScroller;
};


#endif
