// Config Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef CFGSCR_H
#define CFGSCR_H

#include "console/scroller.h"
#include "scrollcfg.h"
#include "console/edit.h"


class ConfigDialog;

class ConfigScroller : public Scroller {
protected:
    ScrollConfig mScrollCfg;
    int mExitKey;
    ConfigDialog* mDialog;  // so we can make it draw
    History mDllHist;
    History mRegHist;
    
    void registerServer ();
    void unregisterServer ();

    void exportReg ();
    void importReg ();

public:
    ConfigScroller (Screen& screen, int x, int y, int w, int h);
    virtual ~ConfigScroller ();

    void setDialog (ConfigDialog* dlg) { mDialog = dlg; }

    int getExitCode () const { return mExitKey; }

    // Key handler for most keys - handled here instead of through a Handler function
    virtual int processEvent (const Event& event);    // 0 for not processed, 1 for processed

    static const char *importCommands (const char *file);
};


#endif
