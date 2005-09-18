// Goto Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef GOTODLG_H
#define GOTODLG_H

#include "console/screen.h"
#include "console/edit.h"


class GotoDialog : public Popup {
    Edit *mpEdit;
    void draw ();
public:
    GotoDialog (Screen& screen, char *pText, int length, History *pHist = 0);
    virtual ~GotoDialog ();
    virtual void run ();    // replace event loop
};


#endif
