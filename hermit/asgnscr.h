// Command Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef ASGNSCR_H
#define ASGNSCR_H

#include "console/scroller.h"
#include "scrollasgn.h"


class AssignCommandScroller : public Scroller {
protected:
    ScrollAssignCommands mScrollCmds;
    int mExitKey;
    void assignCommand (int key);
public:
    AssignCommandScroller (Screen& screen, int x, int y, int w, int h);
    virtual ~AssignCommandScroller ();

    int getExitCode () const { return mExitKey; }

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed
};


#endif
