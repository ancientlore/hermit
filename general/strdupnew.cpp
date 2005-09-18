// str dup new files
// Copyright (C) 1997 Michael D. Lore

#include <cstring>
#include "common.h"

using namespace std;

char *strDupNew (const char *str)
{
    if (str == 0)
	return 0;

    char *s = new char [strlen (str) + 1];
    if (s != 0)
	strcpy (s, str);

    return s;
}

// End

