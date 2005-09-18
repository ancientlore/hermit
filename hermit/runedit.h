// Run Edit Window
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef RUNEDIT_H
#define RUNEDIT_H

#include "console/edit.h"


class RunEdit : public Edit {
    int mNoPrompt;
    void insFile ();		// code 2
    void insDir ();		// code 3
    void insBaseFile ();	// no extension, code 1
    void insPrompt ();		// code 4
    void insAllFiles ();	// code 5
    void insAllFilesQuoted ();	// code 6
    void insImmediateReturn ();	// code 6
    void insAutoRefresh ();	// code 6
	void insDrive ();
	void insPathOnly ();
public:
    RunEdit (Screen& screen, int x, int y, int w, char *text, int length, History *pHist = 0);
    virtual ~RunEdit ();
    virtual int processEvent (const Event& event);
    void setNoPrompt () { mNoPrompt = 1; }
};


#endif
