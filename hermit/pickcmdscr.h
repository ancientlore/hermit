// Command Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef PICKCMDSCR_H
#define PICKCMDSCR_H

#include "console/scroller.h"
#include "scrollasgn.h"


class PickCommandScroller : public Scroller {
protected:
    ScrollAssignCommands mScrollCmds;
    int mExitKey;
public:
    PickCommandScroller (Screen& screen, int x, int y, int w, int h);
    virtual ~PickCommandScroller ();

    int getExitCode () const { return mExitKey; }

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed
};


#endif
