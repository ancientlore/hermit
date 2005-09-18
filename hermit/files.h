// File objects for HERMIT
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef FILES_H
#define FILES_H

#define STRICT
#include <windows.h>
#include "tree_tpl.h"

#define FILE_INFO_LENGTH 80

// Sorting Info class

class FileSortInfo {

    int mSortType;
    int mDirection;

public:
    enum { NameSort = 1, ExtSort, DateSort, SizeSort };
    enum { Forward, Reverse };

    FileSortInfo () : mSortType (FileSortInfo::NameSort), mDirection (FileSortInfo::Forward) {}

    int getSortType () const { return mSortType; }
    int getDirection () const { return mDirection; }

    void setSortType (int type) {
	if (type >= NameSort  &&  type <= SizeSort)
	    mSortType = type;
    }

    void setDirection (int dir) {
	if (dir == Forward  ||  dir == Reverse)
	    mDirection = dir;
    }
};

// File Information class

class File {

    WIN32_FIND_DATA mData;
    const FileSortInfo& mrSortInfo;
    int mTagged;

    int compareName (const File& file) const;
    int compareExt (const File& file) const;
    int compareDate (const File& file) const;
    int compareDir (const File& file) const;
    int compareSize (const File& file) const;

public:
    
    static FileSortInfo defaultSort;

    File (const WIN32_FIND_DATA& data, const FileSortInfo& info) :
	mData (data), mrSortInfo (info), mTagged (0) {}
    File ();

    int isTagged () const { return (mTagged == 1); }
    int isSortOfTagged () const { return (mTagged == -1); }
    void tag () { mTagged = 1; }
    void unTag () { mTagged = 0; }
    void unTagSortOf () { if (isTagged ()) mTagged = -1; }

    // This routine is intended for testing purposes
    // line must be at least FILE_INFO_LENGTH + 1 chars
    void getInfoLine (char *line) const;
    void getStatusLine (char *line) const;

    // These access basic properties of the file
    const char *getName () const { return mData.cFileName; }
    const char *getAltName () const { return mData.cAlternateFileName; }
    DWORD getAttributes () const { return mData.dwFileAttributes; }
    DWORD getFileSizeHigh () const { return mData.nFileSizeHigh; }
    DWORD getFileSizeLow () const { return mData.nFileSizeLow; }
    const FILETIME& getCreationTime () const { return mData.ftCreationTime; }
    const FILETIME& getLastAccessTime () const { return mData.ftLastAccessTime; }
    const FILETIME& getLastWriteTime () const { return mData.ftLastWriteTime; }

    File& operator = (const File& file);

    static int writeTime (char *line, const FILETIME& time);

    int compare (const File& file) const;
};

inline int operator == (const File& a, const File& b)
{
    return (a.compare (b) == 0);
}

inline int operator > (const File& a, const File& b)
{
    return (a.compare (b) > 0);
}

inline int operator < (const File& a, const File& b)
{
    return (a.compare (b) < 0);
}

inline int operator >= (const File& a, const File& b)
{
    return (a.compare (b) >= 0);
}

inline int operator <= (const File& a, const File& b)
{
    return (a.compare (b) <= 0);
}

inline int operator != (const File& a, const File& b)
{
    return (a.compare (b) != 0);
}

// Directory listing class

typedef TTree<File> FileTree;
typedef TTreeIterator<File> FileIterator;

typedef int (*CancelProc)(void *data);	    // should return non-zero to cancel the read

class Directory {

    FileSortInfo mSortInfo;
    FileTree *mpFiles;
    char *mpDirName;

    CancelProc mpfCancelProc;
    void *mpCancelData;

    void getCurrentDir ();

public:

    Directory ();
    ~Directory ();

    int getSortType () const { return mSortInfo.getSortType (); }
    int getDirection () const { return mSortInfo.getDirection (); }

    void setSorting (int type, int dir); // can throw exceptions
    virtual int readDir (const char *pattern = 0); // can throw exeptions

    FileIterator *getIterator ();   // can throw exceptions
    FileTree *getFiles ();   // can throw exceptions
    const FileTree *getFiles () const;   // can throw exceptions
    const char *getDirName () const;	// can throw exceptions

    void setCancelProc (CancelProc pfProc, void *pData) { 
	mpfCancelProc = pfProc;
	mpCancelData = pData;
    }

    void clearCancelProc () { mpfCancelProc = 0; mpCancelData = 0; }

    int invokeCancelProc () const;
};

#endif
