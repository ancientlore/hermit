// Input Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef INPUTDLG_H
#define INPUTDLG_H

#include "console/screen.h"
#include "console/edit.h"


class InputDialog : public Popup {
    Edit *mpEdit;
    void draw (const char *pCaption, const char *pPrompt);
public:
    InputDialog (Screen& screen, const char *pCaption, const char *pPrompt,
	         char *pText, int length, History *pHist = 0);
    virtual ~InputDialog ();
    virtual void run ();    // replace event loop
};


#endif
