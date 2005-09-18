// Run Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef RUNDLG_H
#define RUNDLG_H

#include "console/screen.h"
#include "runedit.h"


class RunDialog : public Popup {
    RunEdit *mpEdit;
    void draw ();
public:
    RunDialog (Screen& screen, char *pText, int length, History *pHist = 0);
    virtual ~RunDialog ();
    virtual void run ();    // replace event loop
};


#endif
