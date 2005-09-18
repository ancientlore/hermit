// Scrollable Directory Listing class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "scrolldir.h"
#include "console/colors.h"


ScrollDir::~ScrollDir ()
{
}

int ScrollDir::isAtHome () const
{
    if (mCursor == 0)
	return 1;
    return 0;
}

const File& ScrollDir::getCurrentFile () const
{
    return getFiles ()->operator [] (mCursor);
}

File& ScrollDir::getCurrentFile ()
{
    return getFiles ()->operator [] (mCursor);
}

int ScrollDir::readDir (const char *pattern)
{
    mCursor = 0;
    return Directory::readDir (pattern);
}

int ScrollDir::isAtEnd () const
{
    if (mCursor == int (getFiles ()->size ()) - 1)
	return 1;
    return 0;
}

int ScrollDir::moveCursor (int count)
{
    // If a negative count is more than mCursor, we must limit it to neg mCursor
    if (-count > mCursor)
	count = -mCursor;

    // If a positive count is more than size - cursor - 1, we must limit it too.
    if (count > int (getFiles ()->size ()) - 1 - mCursor)
	count = int (getFiles ()->size ()) - 1 - mCursor;

    // Set the cursor and return actual amount
    mCursor += count;
    return count;
}

int ScrollDir::home ()
{
    int where = mCursor;
    mCursor = 0;
    return -where;
}

int ScrollDir::end ()
{
    int where = mCursor;
    mCursor = int (getFiles ()->size ()) - 1;
    return mCursor - where;
}

int ScrollDir::getText (int pos, char *line, int width) const
{
    // This produces lines like
    //   D------ <DIR>      Sa Aug 31 19:57 .
    //   D------ <DIR>      Sa Aug 31 19:57 ..
    // * --A----      14244 Mo Sep  9 23:08 status.obj

    if (line == 0  ||  width < FILE_INFO_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    const char *pStartOfLine = line;

    // Check for out of bounds
    line[0] = 0;
    if (mCursor + pos < 0  ||  mCursor + pos > int (getFiles ()->size ()) - 1)
	return 0;

    const File& file = getFiles ()->operator [] (mCursor + pos);

    // Tag
    line += wsprintf (line, "%c ", (file.isTagged () ? '*' : (file.isSortOfTagged () ? '-' : ' ')));

    // Attributes
    line += wsprintf (line, "%c%c%c%c%c%c%c",
		     (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY  ? 'D' : '-'),
		     (file.getAttributes () & FILE_ATTRIBUTE_READONLY   ? 'R' : '-'),
		     (file.getAttributes () & FILE_ATTRIBUTE_ARCHIVE    ? 'A' : '-'),
		     (file.getAttributes () & FILE_ATTRIBUTE_COMPRESSED ? 'C' : '-'),
		     (file.getAttributes () & FILE_ATTRIBUTE_SYSTEM     ? 'S' : '-'),
		     (file.getAttributes () & FILE_ATTRIBUTE_HIDDEN     ? 'H' : '-'),
		     (file.getAttributes () & FILE_ATTRIBUTE_TEMPORARY  ? 'T' : '-'));

    // Size
    if (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
	line += wsprintf (line, " <DIR>     ");
    else if (file.getFileSizeHigh () != 0)
	line += wsprintf (line, " **********");
    else
	line += wsprintf (line, " %10lu", file.getFileSizeLow ());

    // Time
	// FILETIME mData.ftCreationTime;
	// FILETIME mData.ftLastAccessTime;
    line += File::writeTime (line, file.getLastWriteTime ());

    // Name
    // printf (" %12s", mData.cAlternateFileName);
    // gee, wsprintf does not support formats like %-*.*s
    char fmt[20];
    wsprintf (fmt, " %%-%d.%ds", 42 + (width - FILE_INFO_LENGTH),
	      42 + (width - FILE_INFO_LENGTH));
    line += wsprintf (line, fmt, file.getName ());
    if (int (strlen (file.getName ())) > 42 + (width - FILE_INFO_LENGTH))
	line += wsprintf (line, "+");

    return line - pStartOfLine;
}

void ScrollDir::getAttributes (int pos, WORD *attrs, int width) const
{
    // This produces attributes for lines like
    //   D------ <DIR>      Sa Aug 31 19:57 .
    //   D------ <DIR>      Sa Aug 31 19:57 ..
    // * --A----      14244 Mo Sep  9 23:08 status.obj
    // 012345678901234567890123456789012345678901234567890
    //           1         2         3         4

    // So the following is the color map:
    // 0 to 1	    Tag
    // 2 to 9	    Attributes
    // 10 to 20	    Directory tag or file size
    // 21 to 36	    Date / Time
    // 37 to end-1  File name
    // end	    File name too long tag >

    if (attrs == 0  ||  width < FILE_INFO_LENGTH)
	throw AppException (WHERE, ERR_INTERNAL);

    int haveFiles = 1;
    if (getFiles ()->size () <= 0)
	haveFiles = 0;

    WORD clrFileTag = (haveFiles && pos == 0 ? getColor (ceFileTagCursor) : getColor (ceFileTag));
    WORD clrFileAttributes = (haveFiles && pos == 0 ? getColor (ceFileAttributesCursor) : getColor (ceFileAttributes));
    WORD clrFileSize = (haveFiles && pos == 0 ? getColor (ceFileSizeCursor) : getColor (ceFileSize));
    WORD clrFileDate = (haveFiles && pos == 0 ? getColor (ceFileDateCursor) : getColor (ceFileDate));
    WORD clrFileName = (haveFiles && pos == 0 ? getColor (ceFileNameCursor) : getColor (ceFileName));
    WORD clrFileNameExt = (haveFiles && pos == 0 ? getColor (ceFileNameExtCursor) : getColor (ceFileNameExt));
    int i;

    for (i = 0; i < 2; i++)
	attrs[i] = clrFileTag;
    for (i = 2; i < 10; i++)
	attrs[i] = clrFileAttributes;
    for (i = 10; i < 21; i++)
	attrs[i] = clrFileSize;
    for (i = 21; i < 37; i++)
	attrs[i] = clrFileDate;
    for (i = 37; i < width - 1; i++)
	attrs[i] = clrFileName;
    attrs[width - 1] = clrFileNameExt;
}

// End
