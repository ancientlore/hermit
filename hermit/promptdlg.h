// Prompt Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef PROMPTDLG_H
#define PROMPTDLG_H

#include "console/screen.h"
#include "runedit.h"


class PromptDialog : public Popup {
    RunEdit *mpEdit;
    const char *mpCmd;
    int mHiLite;
    void draw ();
public:
    PromptDialog (Screen& screen, const char *cmd, int hilite,
		  char *pText, int length, History *pHist);
    virtual ~PromptDialog ();
    virtual void run ();    // replace event loop
};


#endif
