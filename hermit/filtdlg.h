// Filter Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef FILTDLG_H
#define FILTDLG_H

#include "console/screen.h"
#include "console/edit.h"


class FilterDialog : public Popup {
    Edit *mpEdit;
    void draw ();
public:
    FilterDialog (Screen& screen, char *pText, int length, History *pHist = 0);
    virtual ~FilterDialog ();
    virtual void run ();    // replace event loop
};


#endif
