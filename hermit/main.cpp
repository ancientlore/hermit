// Main Hermit application file
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#define STRICT
// #define _WIN32_DCOM	    // allows us to use CoInitializeEx
#include <windows.h>
#include <string.h>
#include <objbase.h>

#include <io.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "app.h"
#include "scrolldir.h"
#include "license.h"
#include "resource.h"
#include "registry.h"
#include "cfgscr.h"


#ifdef _WIN32_DCOM
typedef HRESULT (*coInitEx) (void *, DWORD);
#endif

int main (int /*argc*/, const char * /*argv*/[])
{
#if 0
    _CrtSetReportMode (_CRT_WARN, _CRTDBG_MODE_FILE );
    _CrtSetReportFile (_CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode (_CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile (_CRT_ERROR, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode (_CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile (_CRT_ASSERT, _CRTDBG_FILE_STDOUT );
    _CrtMemState st;
    _CrtMemCheckpoint (&st);
#endif

    {
    try {
	RegistryKey debugKey (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Debug", KEY_READ);
	// if the key does not exist, we won't get here. We'll be in the catch block.
	// DWORD type;
	// char *value = debugKey.queryValue (str, type);
	AppException::setDebug (true);    // enables File/Line info in errors
    }
    catch (const std::exception&) {
	// okay, fine. We don't display debug information.
    }

    // BEGIN MAIN STUFF FOR HERMIT APPLICATION
    try {
#ifdef _WIN32_DCOM
        // Try to initialize it multi-threaded. If you can, then other applications launch
	// fast and CodeWright doesn't hang.  If it fails, just use CoInitialize.
	HMODULE m = GetModuleHandle ("ole32.dll");
	if (m == NULL)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "GetModuleHandle", GetLastError ());
	coInitEx func = (coInitEx) GetProcAddress (m, "CoInitializeEx");
	if (func == NULL  ||  FAILED ((*func) (NULL, COINIT_MULTITHREADED))) {
	    if (FAILED (CoInitialize (NULL)))
		throw AppException (WHERE, ERR_WINDOWS_FMT, "CoInitialize", GetLastError ());
	}
#else
        if (FAILED (CoInitialize (NULL)))
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "CoInitialize", GetLastError ());
#endif

#if 0
    _CrtMemDumpAllObjectsSince (&st);
#endif

	// Show license screen
	{
	    LicenseScreen license;
	    license.activate ();
	    license.run ();
	}

#if 0
    _CrtMemDumpAllObjectsSince (&st);
#endif

#if 0
        if (LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (IDI_HERMIT)) == NULL)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "LoadIcon", GetLastError ());
#endif

	// Show main application screen
	Application mainScr;

	// Activate main window
	mainScr.activate ();

	// Check for the startup command file
	{
	    try {
		RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Commands", KEY_READ);
	    }
	    catch (const std::exception&) {
		char path[512];
		if (GetModuleFileName (GetModuleHandle (NULL), path, 512) > 0) {
		    char *ext = strrchr (path, '.');
		    if (ext != 0)
			*ext = '\0';
			if (GetVersion() < 0x80000000)
				strcat(path, ".nt");
			else
				strcat (path, ".9x");
		    if (_access (path, 4) == 0) {
			if (mainScr.ask ("Import Startup Command File",
			    "Would you like to import an initial command set?") != 0) {
			    const char *msg = ConfigScroller::importCommands (path);
			    mainScr.postEvent (Event (EV_STATUS_MSG, (DWORD) msg));
			}
		    }
		}
	    }
	}

	// Run main event loop
	mainScr.run ();
    }
    // CATCH ALL ERRORS (this should usually never execute)
    catch (const std::exception& s) {
        DWORD charsWritten;
        WriteConsole(GetStdHandle (STD_OUTPUT_HANDLE), s.what(), strlen (s.what()), &charsWritten, 0);
        WriteConsole(GetStdHandle (STD_OUTPUT_HANDLE), "\n", 1, &charsWritten, 0);
    }

    // Uninitialize COM
    CoUninitialize ();

    }

    // char *p = new char [20];	// test memory leak checker

    THeapBase::freeAllHeaps ();

#ifdef MSVC
#ifdef _DEBUG
    _CrtSetReportMode (_CRT_WARN, _CRTDBG_MODE_FILE );
    _CrtSetReportFile (_CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode (_CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile (_CRT_ERROR, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode (_CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile (_CRT_ASSERT, _CRTDBG_FILE_STDOUT );
    _CrtDumpMemoryLeaks( );
#endif
#endif

    return 0;
}

// End
