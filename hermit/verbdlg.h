// Verb Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef VERBDLG_H
#define VERBDLG_H

#include "console/screen.h"
#include "verbscr.h"
#include "files.h"
#include "ptr.h"


class VerbDialog : public Popup {
    VerbScroller *mpScroller;
    void draw ();
    int mVerbC;
    const char ** mVerbV;
    int mIndex;
    void buildDirVector ();
    void buildFileVector (const File& file);
    void killVector ();
public:
    VerbDialog (Screen& screen, const File& file);
    virtual ~VerbDialog ();
    virtual void run ();    // replace event loop
    const char *getVerb () const;
};


#endif
