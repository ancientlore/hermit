// Verb Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef VERBSCR_H
#define VERBSCR_H

#include "console/scroller.h"
#include "scrverb.h"


class VerbScroller : public Scroller {
protected:
    ScrollVerbs mScrollVerbs;
    int mExitKey;
    int mIndex;
public:
    VerbScroller (Screen& screen, int x, int y, int w, int h, int verbc, const char *verbv[]);
    virtual ~VerbScroller ();

    int getExitCode () const { return mExitKey; }
    int getIndex () const { return mIndex; }

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed
};


#endif
