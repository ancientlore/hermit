// Shell Extension stuff
// Copyright (C) 1997 Michael D. Lore

#ifndef SHSTUFF_H
#define SHSTUFF_H

#define STRICT
#include <shlobj.h>

// YOU must free the returned string
char * getSpecialFolder (int nFolder, HWND hWnd = NULL);

#endif
