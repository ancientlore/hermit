// Wait on objects with message loop
// Copyright (C) 1997 Michael D. Lore

#ifndef WAITFIX_H
#define WAITFIX_H

#define STRICT
#include <windows.h>

BOOL WaitWithMessageLoop (HANDLE hInput);
void ProcessWindowMessages ();


#endif
