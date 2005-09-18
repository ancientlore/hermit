// Shell Stuff
// Copyright (C) 1997 Michael D. Lore  All Rights Reserved

#include "common.h"
#include "shstuff.h"
#include "ptr.h"

// YOU must free the returned string
char * getSpecialFolder (int nFolder, HWND hWnd)
{
    LPMALLOC pMalloc;
    LPITEMIDLIST pidl;

    ArrayPtr<char> str = new char [MAX_PATH + 30];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    if (FAILED (SHGetMalloc (&pMalloc)))
	return NULL;

    if (FAILED (SHGetSpecialFolderLocation (hWnd, nFolder, &pidl))) {
	pMalloc->Release ();
	return NULL;
    }

    if (SHGetPathFromIDList (pidl, str) != TRUE) {
	pMalloc->Free (pidl);
	pMalloc->Release ();
	return NULL;
    }

    pMalloc->Free (pidl);
    pMalloc->Release ();

    return str.detach ();
}

// End
