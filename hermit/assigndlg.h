// Run Dialog
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#ifndef ASSIGNDLG_H
#define ASSIGNDLG_H

#include "console/screen.h"
#include "runedit.h"


class AssignCommandDialog : public Popup {
    RunEdit *mpEdit;
    char mKey;
    void draw ();
public:
    AssignCommandDialog (Screen& screen, char *pText, int length, char key);
    virtual ~AssignCommandDialog ();
    virtual void run ();    // replace event loop
};


#endif
