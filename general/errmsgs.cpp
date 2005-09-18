// Error Messages
// Copyright (C) 1996 Michael D. Lore

#include "err.h"


static const char *errors[] = {
    "out of memory",			// ERR_OUT_OF_MEMORY
    "internal error",			// ERR_INTERNAL
    "filer access error",		// ERR_FILE_ERROR
    "item not found",			// ERR_NOT_FOUND
    "duplicate item",			// ERR_DUPLICATE
    "out of range",			// ERR_RANGE
    "Windows API error in function %s: %d", // ERR_WINDOWS_FMT
    "User cancelled operation",		// ERR_USER_CANCEL
    "Error 0x%x in component %s",	// ERR_EXTENSION_FMT
    ""					// end
};

static int numErrors = sizeof (errors) / sizeof (const char *);


const char *getErrMsg (int errCode)
{
    const char *buf = "Error code not found";
    if (errCode >= 0  &&  errCode < numErrors)
	buf = errors[errCode];
    return buf;
}

// End
