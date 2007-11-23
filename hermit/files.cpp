// File objects for HERMIT
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <stdio.h>
#include <string.h>
#include "files.h"
#include "registry.h"
#include "ptr.h"


FileSortInfo File::defaultSort;

File::File () : mrSortInfo (File::defaultSort), mTagged (0)
{
    memset (&mData, 0, sizeof (WIN32_FIND_DATA));
}

void File::getInfoLine (char *line) const
{
    if (line == 0)
	return;

    // Tag
    line += wsprintf (line, "%c ", (mTagged ? '*' : ' '));

    // Attributes
    line += wsprintf (line, "%c%c%c%c%c%c%c",
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  ? 'D' : '-'),
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_READONLY   ? 'R' : '-'),
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE    ? 'A' : '-'),
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED ? 'C' : '-'),
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM     ? 'S' : '-'),
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN     ? 'H' : '-'),
		     (mData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY  ? 'T' : '-'));

    // Size
    if (mData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	line += wsprintf (line, " <DIR>     ");
    else if (mData.nFileSizeHigh != 0)
	line += wsprintf (line, " **********");
    else
	line += wsprintf (line, " %10lu", mData.nFileSizeLow);

    // Time
	// FILETIME mData.ftCreationTime;
	// FILETIME mData.ftLastAccessTime;
    line += writeTime (line, mData.ftLastWriteTime);

    // Name
    // printf (" %12s", mData.cAlternateFileName);
    line += wsprintf (line, " %-42.42s", mData.cFileName);
    if (strlen (mData.cFileName) > 42)
	line += wsprintf (line, "+");
}

int File::writeTime (char *line, const FILETIME& time)
{
    FILETIME local;
    SYSTEMTIME tm;
    const char *start = line;

    FileTimeToLocalFileTime(&time, &local);
    FileTimeToSystemTime (&local, &tm);
    char *str = "";
    switch (tm.wDayOfWeek) {
	case 0: str = "Su"; break;
	case 1: str = "Mo"; break;
	case 2: str = "Tu"; break;
	case 3: str = "We"; break;
	case 4: str = "Th"; break;
	case 5: str = "Fr"; break;
	case 6: str = "Sa"; break;
    }
    line += wsprintf (line, " %2s", str);
    switch (tm.wMonth) {
	case 1: str = "Jan"; break;
	case 2: str = "Feb"; break;
	case 3: str = "Mar"; break;
	case 4: str = "Apr"; break;
	case 5: str = "May"; break;
	case 6: str = "Jun"; break;
	case 7: str = "Jul"; break;
	case 8: str = "Aug"; break;
	case 9: str = "Sep"; break;
	case 10: str = "Oct"; break;
	case 11: str = "Nov"; break;
	case 12: str = "Dec"; break;
    }
    line += wsprintf (line, " %3s %2d", str, tm.wDay);
    SYSTEMTIME now;
    GetLocalTime (&now);
    int cNow = now.wYear * 12 + now.wMonth - 1;
    int cFile = tm.wYear * 12 + tm.wMonth - 1;
    if (cNow - cFile >= 6  ||  cNow - cFile < 0)
	line += wsprintf (line, "  %4.4d", tm.wYear);
    else
       	line += wsprintf (line, " %2.2d:%2.2d", tm.wHour, tm.wMinute);

    return line - start;
}

void File::getStatusLine (char *line) const
{
    if (line == 0)
	return;
    line += wsprintf (line, "%-12s Created",
		      (mData.cAlternateFileName[0] == '\0' ? mData.cFileName : mData.cAlternateFileName));
    line += writeTime (line, mData.ftCreationTime);
    line += wsprintf (line, " Accessed");
    line += writeTime (line, mData.ftLastAccessTime);
}

int File::compare (const File& file) const
{
    int (File::*comp[5])(const File& file) const;

    comp[0] = &File::compareDir;
    //int type = mrSortInfo.getSortType ();

    switch (mrSortInfo.getSortType ()) {
    case FileSortInfo::NameSort:
        comp[1] = &File::compareName;
        comp[2] = &File::compareDate;
        comp[3] = &File::compareSize;
	comp[4] = &File::compareExt;
	break;
    case FileSortInfo::ExtSort:
	comp[1] = &File::compareExt;
        comp[2] = &File::compareName;
        comp[3] = &File::compareDate;
        comp[4] = &File::compareSize;
	break;
    case FileSortInfo::DateSort:
        comp[1] = &File::compareDate;
        comp[2] = &File::compareName;
        comp[3] = &File::compareSize;
	comp[4] = &File::compareExt;
	break;
    case FileSortInfo::SizeSort:
        comp[1] = &File::compareSize;
        comp[2] = &File::compareName;
        comp[3] = &File::compareDate;
	comp[4] = &File::compareExt;
	break;
    default:
        comp[1] = &File::compareName;
        comp[2] = &File::compareDate;
        comp[3] = &File::compareSize;
	comp[4] = &File::compareExt;
	break;
    }

    // Try each compare function and return result if not the same
    for (int i = 0; i < 4; i++) {
	int val = (this->*comp[i])(file);
	if (val != 0) {
	    if (i == 1  &&  mrSortInfo.getDirection () != FileSortInfo::Forward)
		val = -val;
	    return val;
	}
    }

    return 0;
}

int File::compareName (const File& file) const
{
    return _stricmp (mData.cFileName, file.mData.cFileName);
}

int File::compareExt (const File& file) const
{
    const char *ptrA = strrchr (mData.cFileName, '.');
    const char *ptrB = strrchr (file.mData.cFileName, '.');
    if (ptrA == 0  &&  ptrB == 0)
	return 0;
    else if (ptrA == 0)
	return 1;
    else if (ptrB == 0)
	return -1;
    else
	return _stricmp (ptrA, ptrB);
}

int File::compareDate (const File& file) const
{
    DWORD h = mData.ftLastWriteTime.dwHighDateTime - file.mData.ftLastWriteTime.dwHighDateTime;
    if (h != 0)
	return h;
    return mData.ftLastWriteTime.dwLowDateTime - file.mData.ftLastWriteTime.dwLowDateTime;
}

int File::compareSize (const File& file) const
{
    DWORD h = mData.nFileSizeHigh - file.mData.nFileSizeHigh;
    if (h != 0)
	return h;
    return mData.nFileSizeLow - file.mData.nFileSizeLow;
}

int File::compareDir (const File& file) const
{
    int i = (mData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    int j = (file.mData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    return j - i;
}

File& File::operator = (const File& file)
{
    // we just don't assign the reference member
    mTagged = file.mTagged;
    memcpy (&mData, &file.mData, sizeof (WIN32_FIND_DATA));
    return *this;
}

Directory::Directory () : mpDirName (0), mpFiles (0),
			  mpfCancelProc (0), mpCancelData (0),
			  mSortInfo (File::defaultSort)
{
    try {
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\File Listing", KEY_READ | KEY_WRITE);
	DWORD type;
	char *value = k.queryValue ("Sort Type", type);
	if (type == REG_DWORD  &&  value != 0) {
	    mSortInfo.setSortType (*((DWORD *)value));
	    File::defaultSort.setSortType (*((DWORD *)value));
	}
	delete [] value;
	value = k.queryValue ("Direction", type);
	if (type == REG_DWORD  &&  value != 0) {
	    mSortInfo.setDirection (*((DWORD *)value));
	    File::defaultSort.setDirection (*((DWORD *)value));
	}
	delete [] value;
    }
    catch (const std::exception&) {
	// oh well, it's only the history
    }

}

Directory::~Directory ()
{
    delete mpFiles;
    delete [] mpDirName;

    try {
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\File Listing", KEY_READ | KEY_WRITE);
	k.setValue ("Sort Type", (DWORD) mSortInfo.getSortType ());
	k.setValue ("Direction", (DWORD) mSortInfo.getDirection ());
    }
    catch (const std::exception&) {
	// oh well, it's only the last sort
    }
}

void Directory::setSorting (int type, int dir)
{
    if (type != mSortInfo.getSortType ()  ||  dir != mSortInfo.getDirection ()) {
	mSortInfo.setSortType (type);
	mSortInfo.setDirection (dir);
	if (mpFiles != 0) {
	    FileTree *tree = 0;
	    try {
		tree = mpFiles;
		mpFiles = new FileTree;
		if (mpFiles == 0)
		    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
		FileIterator iter (*tree);
		while (!iter.done ()) {
		    mpFiles->add ((const File&) iter);
		    iter++;
		}
	    }
	    catch (const std::exception&) {
			delete tree;
			throw;
	    }
	    delete tree;
	}
    }
}

void Directory::getCurrentDir ()
{
    delete [] mpDirName;

    try {
	mpDirName = new char [40];
	if (mpDirName == 0)
	    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	DWORD size = GetCurrentDirectory(40, mpDirName);
	if (size == 0)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "GetCurrentDirectory", GetLastError ());
	else if (size > 40) {
	    delete [] mpDirName;
	    mpDirName = new char [size];
	    if (mpDirName == 0)
    		throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	    size = GetCurrentDirectory (size, mpDirName);
	    if (size == 0)
	        throw AppException (WHERE, ERR_WINDOWS_FMT, "GetCurrentDirectory", GetLastError ());
	}
    }
    catch (const std::exception&) {
		delete [] mpDirName;
		mpDirName = 0;
		throw;
    }
}

int Directory::invokeCancelProc () const
{
    if (mpfCancelProc != 0)
	return (*mpfCancelProc) (mpCancelData);
    return 0;
}

int Directory::readDir (const char *pattern)
{
    // Save directory name
    getCurrentDir ();

    // Recreate file tree
    delete mpFiles;
    mpFiles = new FileTree;
    if (mpFiles == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    // Duplicate the pattern
    int pn = (int) strlen ((pattern ? pattern : "*.*"));
    ArrayPtr<char> pat = new char [pn + 1];
    if (pat == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    strcpy (pat, (pattern ? pattern : "*.*"));

    // Find all files specified by the pattern
    WIN32_FIND_DATA data;
    int stopped = 0;
    int gotStarStar = 0;
    HANDLE hFind;
    char *aPat = strtok (pat, ";");
    while (aPat != NULL) {
	if (strcmp (aPat, "*.*") == 0)
	    gotStarStar = 1;
	hFind = FindFirstFile(aPat, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
	    BOOL ret = TRUE;
	    while (ret == TRUE) {
		try {
		    mpFiles->add (File (data, mSortInfo));
		}
		catch (const AppException& info) {
		    if (info.code() != ERR_DUPLICATE)
		        throw;
		}
		// Invoke cancel proc
		if (invokeCancelProc ()) {
		    stopped++;
		    break;
		}
		// get next file
		ret = FindNextFile (hFind, &data);
		if (ret == FALSE) {
		    DWORD err = GetLastError ();
		    if (err != ERROR_NO_MORE_FILES)
			throw AppException (WHERE, ERR_WINDOWS_FMT, "FindNextFile", err);
		}
	    }
	    FindClose (hFind);
	    if (stopped)
		break;
	}
        aPat = strtok (NULL, ";");
    }

    // Find all directories (we always want to show them in the listing even when filtered)
    if (!stopped  &&  pattern != 0  &&  strcmp (pattern, "*.*") != 0  &&  !gotStarStar) {
	hFind = FindFirstFile("*.*", &data);
	if (hFind != INVALID_HANDLE_VALUE) {
	    BOOL ret = TRUE;
	    while (ret == TRUE) {
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		    try {
			mpFiles->add (File (data, mSortInfo));
		    }
		    catch (const AppException& info) {
			if (info.code() != ERR_DUPLICATE)
			    throw;
		    }
		}
		// invoke cancel proc
		if (invokeCancelProc ()) {
		    stopped++;
		    break;
		}
		// get next file
		ret = FindNextFile (hFind, &data);
		if (ret == FALSE) {
		    DWORD err = GetLastError ();
		    if (err != ERROR_NO_MORE_FILES)
			throw AppException (WHERE, ERR_WINDOWS_FMT, "FindNextFile", err);
		}
	    }
	    FindClose (hFind);
	}
    }

    return stopped;
}

FileIterator *Directory::getIterator ()
{
    if (mpFiles == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    FileIterator *pIter = new FileIterator (*mpFiles);
    if (pIter == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    return pIter;
}

FileTree *Directory::getFiles ()
{
    if (mpFiles == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    return mpFiles;
}

const FileTree *Directory::getFiles () const
{
    if (mpFiles == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    return mpFiles;
}

const char *Directory::getDirName () const
{
    if (mpDirName == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    return mpDirName;
}

// End

