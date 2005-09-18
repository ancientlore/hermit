// Config Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <stdio.h>
#include <io.h>
#include "cfgscr.h"
#include "registry.h"
#include "console/colors.h"
#include "cfgdlg.h"
#include "console/inputdlg.h"
#include "ptr.h"

#define COMMAND_SIZE	    1024

ConfigScroller::ConfigScroller (Screen& screen, int x, int y, int w, int h) :
    Scroller (screen, x, y, w, h, 0), mExitKey (KB_ESC), mDialog (0),
    mDllHist ("RegSvr History"), mRegHist ("Cmd Export History")
{
    setScrollable (&mScrollCfg);
}

ConfigScroller::~ConfigScroller ()
{
}

int ConfigScroller::processEvent (const Event& event)
{
    int key;
    int table;

    switch (event.key) {

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	// Launch command edit window
	myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) ""));
	key = mScrollCfg.getCursor ();
	switch (key) {
	case 0:		    // "Select Color Scheme"
	    table = getColorTable ();
	    selectColorTable (++table);
	    myScreen.sendEvent (Event (EV_COLOR_CHANGE));
	    if (mDialog != NULL)
		mDialog->draw ();
	    draw ();
	    break;

	case 1:		    // "Export Custom Commands"
	    exportReg ();
	    break;

	case 2:		    // "Import Custom Commands"
	    importReg ();
	    break;

	case 3:		    // "Register DLL Server"
	    registerServer ();
	    break;

	case 4:		    // "Unregister DLL Server"
	    unregisterServer ();
	    break;

	default:
	    break;
	}
	// drawLine (mCursor);
        break;

    default:
        return Scroller::processEvent (event);
    }

    return 1;
}

void ConfigScroller::exportReg ()
{
    int exitCode = KB_ESC;
    char path[512];
    *path = 0;
    {
	InputDialog dlg (myScreen, "Export Custom Commands", "Enter the pathname:",
			 path, 511, &mRegHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC) {

	if (_access (path, 0) == 0)
	    if (myScreen.ask ("Confirm File Replace", "Overwrite the existing file?") == 0)
		return;

	FILE *file = fopen (path, "w");
	if (file == NULL) {
	    myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) "Could not create file."));
	    return;
	}

	for (int i = 0; i < 2; i++) {
	    int st, fn;
	    if (i == 0) {
		st = 'A';
		fn = 'Z';
	    }
	    else if (i == 1) {
		st = '0';
		fn = '9';
	    }

	    for (char c = st; c <= fn; c++) {
		try {
		    // Open registry key
		    RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Commands", KEY_READ);

		    char valname[2];
		    valname[0] = c;
		    valname[1] = '\0';
		    // try to read the current value
		    char *value = 0;
		    DWORD type;
		    try {
			value = k.queryValue (valname, type);
			if (type == REG_SZ  &&  value != 0  &&  value[0] != '\0') {
			    fprintf (file, "%c,%s\n", c, value);
			}
			delete [] value;
		    }
		    catch (const std::exception&) {
		    }
		}
		catch (const std::exception&) {
		}
	    }
	}
	fclose (file);
	myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) "Commands exported."));
    }
}

void ConfigScroller::importReg ()
{
    int exitCode = KB_ESC;
    char path[512];
    *path = 0;
    {
	InputDialog dlg (myScreen, "Import Custom Commands", "Enter the pathname:",
			 path, 511, &mRegHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC) {
	myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) importCommands (path)));
    }
}

const char *ConfigScroller::importCommands (const char *path)
{
    FILE *file = fopen (path, "r");
    if (file == NULL)
	return "Could not open file";

    ArrayPtr<char> str = new char [COMMAND_SIZE + 3];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    char *status = 0;
    while (fgets (str, COMMAND_SIZE + 3, file) != NULL) {
	// Remove trailing whitespace
	int n = (int) strlen (str);
	while (n > 0  &&  (str[n - 1] == '\n'  ||  str[n - 1] == '\t'  ||  str[n - 1] == ' ')) {
	    str[n - 1] = '\0';
	    n--;
	}
	// If string is empty, just ignore it
	if (str[0] == '\0')
	    continue;
	// Uppercase the first letter
	str[0] = toupper (str[0]);
	// Make sure the command format is [A-Z,0-9],
	if ((!(str[0] >= 'A'  &&  str[0] <= 'Z')  &&
	     !(str[0] >= '0'  &&  str[0] <= '9')) ||  str[1] != ',') {
	    status = "A read error occurred.";
	    break;
	}
	// Create the value name string
	char valname[2];
	valname[0] = str[0];
	valname[1] = '\0';
	// Save to registry
	try {
	    // Open registry key
	    RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Commands", KEY_READ | KEY_WRITE);
	    k.setValue (valname, &str[2]);
	}
	catch (const std::exception&) {
	    status = "Error saving key to registry";
	    break;
	}
    }

    if (ferror (file) != 0)
	status = "A read error occurred.";

    if (status == 0) {
	// Set initialized state (actually no reason...we use Commands key)
#if 0
	try {
	    RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Initialized", KEY_READ | KEY_WRITE);
	}
	catch (const std::exception&) {
	}
#endif
	// Set message
	status = "Commands imported.";
    }

    fclose (file);

    return status;
}

void ConfigScroller::registerServer ()
{
    int exitCode = KB_ESC;
    char dllpath[512];
    *dllpath = 0;
    {
	InputDialog dlg (myScreen, "Register DLL Server", "Enter the DLL pathname:",
			 dllpath, 511, &mDllHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC) {

	HINSTANCE hInst = LoadLibrary (dllpath);
	if (hInst == NULL) {
	    myScreen.sendWinErrStatusMsg ("Cannot load DLL");
	    return;
	}

	FARPROC dllEntryPoint;
	dllEntryPoint = GetProcAddress (hInst, "DllRegisterServer");
	if (dllEntryPoint == NULL) {
	    myScreen.sendWinErrStatusMsg ("Cannot load DllRegisterServer");
	    FreeLibrary (hInst);
	    return;
	}

	if (FAILED ((*dllEntryPoint) ()))
	    myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) "The registration function failed."));
	else
	    myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) "Server registered!"));

	FreeLibrary (hInst);
    }
}

void ConfigScroller::unregisterServer ()
{
    int exitCode = KB_ESC;
    char dllpath[512];
    *dllpath = 0;
    {
	InputDialog dlg (myScreen, "Unregister DLL Server", "Enter the DLL pathname:",
			 dllpath, 511, &mDllHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC) {

	HINSTANCE hInst = LoadLibrary (dllpath);
	if (hInst == NULL) {
	    myScreen.sendWinErrStatusMsg ("Cannot load DLL");
	    return;
	}

	FARPROC dllEntryPoint;
	dllEntryPoint = GetProcAddress (hInst, "DllUnregisterServer");
	if (dllEntryPoint == NULL) {
	    myScreen.sendWinErrStatusMsg ("Cannot load DllUnregisterServer");
	    FreeLibrary (hInst);
	    return;
	}

	if (FAILED ((*dllEntryPoint) ()))
	    myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) "The unregistration function failed."));
	else
	    myScreen.sendEvent (Event(EV_STATUS_MSG, (DWORD) "Server unregistered!"));

	FreeLibrary (hInst);
    }
}

// End
