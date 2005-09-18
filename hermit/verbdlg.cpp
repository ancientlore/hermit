// Verb Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <string.h>
#include "verbdlg.h"
#include "registry.h"


VerbDialog::VerbDialog (Screen& screen, const File& file) :
    Popup (screen, 49, 13), mVerbC (0), mVerbV (0)
{
    try {
	if (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
	    buildDirVector ();
	else
	    buildFileVector (file);
    }
    catch (const std::exception&) {
		killVector ();
		throw;
    }

    draw ();
    mpScroller = new VerbScroller (screen, mX + 2, mY + 5, 45, 6, mVerbC, mVerbV);
    if (mpScroller == 0) {
	killVector ();
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    }
}

VerbDialog::~VerbDialog ()
{
    delete mpScroller;
    killVector ();
}

void VerbDialog::killVector ()
{
    if (mVerbV) {
	for (int i = 0; i < mVerbC; i++)
	    delete [] const_cast<char*> (mVerbV[i]);
	delete [] mVerbV;
	mVerbV = 0;
	mVerbC = 0;
    }
}

void VerbDialog::buildDirVector ()
{
    mVerbC = 2;
    mVerbV = new const char * [2];
    if (mVerbV == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mVerbV[0] = strDupNew ("explore");
    mVerbV[1] = strDupNew ("open");
    if (mVerbV[0] == 0  ||  mVerbV[1] == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

void VerbDialog::buildFileVector (const File& file)
{
    // Allocate buffer (ten should be enough!)
    mVerbV = new const char * [20];
    if (mVerbV == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    // Get the extension
    const char *ext = strrchr (file.getName (), '.');
    if (ext != 0) {

	try {
	    // Get the top registry key to search
	    RegistryKey topKey (HKEY_CLASSES_ROOT, ext);
	    DWORD type;
	    ArrayPtr<char> val2 = topKey.queryValue (0, type);
	    if (*val2 != '\0') {
		topKey.reOpen (HKEY_CLASSES_ROOT, val2);
	    }

	    RegistryKey key (topKey, "shell");

	    // Get subkeys
	    int i = 0;
	    mVerbC = 0;
	    char *val;
	    do {
		val = (char*) key.enumerateKey (i);
		if (val != 0) {
		    int failed = 0;
		    try {
			RegistryKey anyKey (RegistryKey (key, val), "command");
		    }
		    catch (const std::exception&) {
			failed = 1;
		    }
		    if (!failed  &&  mVerbC < 20  &&  stricmp (val, "printto") != 0) {
			mVerbV[mVerbC] = strDupNew (val);
			mVerbC++;
		    }
		}
		i++;
	    } while (val != 0);
	}
	catch (const std::exception&) {
	    // any failures and we default to processing below
	}
    }

    // When there is no extension or nothing found in registry
#if 1
    if (ext == 0  ||  mVerbC == 0) {
	SetLastError (ERROR_NO_ASSOCIATION);
	throw AppException (WHERE, ERR_WINDOWS_FMT, "VerbDialog::buildFileVector", GetLastError ());
    }
#else
    if (ext == 0  ||  mVerbC == 0) {
	mVerbC = 2;
	mVerbV[0] = strDupNew ("open");
	mVerbV[1] = strDupNew ("print");
	if (mVerbV[0] == 0  ||  mVerbV[1] == 0)
	    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    }
#endif
}

const char *VerbDialog::getVerb () const
{
    if (mIndex < 0  ||  mVerbV == 0)
	return 0;
    else
	return mVerbV[mIndex];
}

void VerbDialog::draw ()
{
    drawBackground ();
    drawBorder ("Choose an Action");
    writeText (2, 2, "Select a verb with the cursor and press Enter");
    writeText (2, 3, "to execute it, or press Esc to cancel.");
}

void VerbDialog::run ()
{
    mpScroller->run ();
    mExitKey = mpScroller->getExitCode ();
    mIndex = mpScroller->getIndex ();
}

// End
