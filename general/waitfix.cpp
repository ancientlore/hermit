// wait with message loop
// Copyright (C) 1997 Michael D. Lore


#include "waitfix.h"


// CoInitialize bug fix wait function
BOOL WaitWithMessageLoop (HANDLE hInput)
{
    DWORD dwRet;

    while (1) {
	dwRet = MsgWaitForMultipleObjects (1, &hInput, FALSE, INFINITE, QS_ALLINPUT);
	if (dwRet == WAIT_OBJECT_0)
	    return TRUE;
	else if (dwRet == WAIT_OBJECT_0 + 1) {
	    // there is a window message
	    ProcessWindowMessages ();
	}
	else
	    return FALSE;   // not supposed to happen
    }
}

void ProcessWindowMessages ()
{
    MSG msg;
    while (PeekMessage (&msg, NULL, NULL, NULL, PM_REMOVE)) {
	TranslateMessage (&msg);
	DispatchMessage (&msg);
    }
}

