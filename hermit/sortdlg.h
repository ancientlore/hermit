// Sorting Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef SORTDLG_H
#define SORTDLG_H

#include "console/screen.h"


class SortDialog : public Popup {
    int& mrSortType;
    int& mrSortDirection;
    int  mSortType;
    int  mSortDirection;
    void draw ();
    void drawFields ();
public:
    SortDialog (Screen& screen, int& sortType, int& sortDir);
    virtual ~SortDialog ();
    virtual int processEvent (const Event& event);
};


#endif
