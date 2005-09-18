// Command Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef CMDSCR_H
#define CMDSCR_H

#include "console/scroller.h"
#include "scrollcmd.h"


class CommandScroller : public Scroller {
protected:
    ScrollCommands mScrollCmds;
    int mExitKey;
public:
    CommandScroller (Screen& screen, int x, int y, int w, int h);
    virtual ~CommandScroller ();

    int getExitCode () const { return mExitKey; }

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed
};


#endif
