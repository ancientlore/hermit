// Application class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef APP_H
#define APP_H

#define STRICT
#include <objbase.h>

#include "console/screen.h"
#include "filescr.h"
#include "version.h"

class PrintfScreen;

class Application : public Screen {
    History mHist;			    // the history for the run command window
    History mParamHist;			    // the history for the parameter prompt window
    History mEnvVarNameHist;	       	    // env var name history
    History mEnvVarValueHist;	       	    // env var value history
    HANDLE  mPrevOutput;
	HWND	mHWnd;
    static BOOL CtrlCHandler (DWORD dwCtrlType);
    void waitKeyMsg ();
    char *getBufferWithPrompts (const char *cmdBuf, int lCmd, int& multiFiles, int& noWaitMsg, int& autoRefresh);
    void runCommand (const char *curDir, File& file, const char *command, int tSkip, int multiFile);
    void writeCommand (const char *cmd);
    static volatile int breakLoop;
    void lookupViewer (const char *filename, CLSID& classId, char *friendlyName, int size);
    const char *formatGiant (const LONGLONG& value, char *str);
    void writeFileInfo (const File& file, PrintfScreen& prf);
    char *getProcessedDirectory ();
	void initializeHWND ();
	BOOL GetDiskInfo(const char* root, LONGLONG& freeSpace, LONGLONG& totalSpace, LONGLONG& usedSpace);
public:
    Status* mStatusBar;
    TitleBar* mTitleBar;
    FileScroller* mScroller;

    static Application * theApp;	// pointer to the application

    const char *getShell () const;

	HWND getWindowHandle () {
		if (mHWnd == NULL)
			initializeHWND ();
		return mHWnd;
	}

    Application ();
    virtual ~Application ();

    // Screen / event related
    virtual int processEvent (const Event& event);
    void handleKbAltX (const Event& event);
    void handleKbF1 (const Event& event);
    void handleKbAltC (const Event& event);
    void handleKbAltU (const Event& event);
    void handleKbAltZ (const Event& event);
    void handleKbAltA (const Event& event);
    void handleKbAltY (const Event& event);
    void handleKbAltM (const Event& event);
    void handleKbDollar (const Event& event);
    void handleKbTab (const Event& event);
    void handleKbAltL (const Event& event);
    void handleKbAltI (const Event& event);
    void handleKbAltT (const Event& event);
    void handleKbBackTab (const Event& event);

    void enter ();
    void print ();
    void runVerb ();
    void runCmd (const char *cmd = 0, int multi = 0);

    void runSavedCmd (int key, int multi);

    // Name info
    const char *getName () { 
    #ifdef HERMIT_PRO
	return "HERMIT PRO"; 
    #else
	return "HERMIT"; 
    #endif
    }
    const char *getTitle () { return VER_TITLEBAR; }

    // Old output window
    HANDLE getPrevOutput () { return mPrevOutput; }
};

#endif
