// Sorting Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "files.h"
#include "sortdlg.h"


SortDialog::SortDialog (Screen& screen, int& sortType, int& sortDir) : 
    Popup (screen, 47, 10), mrSortType (sortType), mrSortDirection (sortDir),
	mSortType (sortType), mSortDirection (sortDir)
{
    draw ();
}

SortDialog::~SortDialog ()
{
}

void SortDialog::draw ()
{
    drawBackground ();
    drawBorder ("Sorting");

    writeText (2, 2, "Type the highlighted letter to select");
    writeText (2, 3, "sorting options.");
    writeText (2, 5, "Sort by:");
    writeText (2, 6, "Direction:");
    writeText (2, 8, "Press Enter when complete or Esc to cancel.");
    drawFields ();
}

void SortDialog::drawFields ()
{
    drawField (14, 5, "Name", (mSortType == FileSortInfo::NameSort ? 1 : 0));
    drawField (20, 5, "Extension", (mSortType == FileSortInfo::ExtSort ? 1 : 0));
    drawField (31, 5, "Size", (mSortType == FileSortInfo::SizeSort ? 1 : 0));
    drawField (37, 5, "Date", (mSortType == FileSortInfo::DateSort ? 1 : 0));
    drawField (14, 6, "Forward", (mSortDirection == FileSortInfo::Forward ? 1 : 0));
    drawField (23, 6, "Reverse", (mSortDirection == FileSortInfo::Reverse ? 1 : 0));
}

int SortDialog::processEvent (const Event& event)
{
    switch (event.key) {

    case 'N':
    case 'n':
	mSortType = FileSortInfo::NameSort;
	drawFields ();
	break;

    case 'E':
    case 'e':
	mSortType = FileSortInfo::ExtSort;
	drawFields ();
	break;

    case 'S':
    case 's':
	mSortType = FileSortInfo::SizeSort;
	drawFields ();
	break;

    case 'D':
    case 'd':
	mSortType = FileSortInfo::DateSort;
	drawFields ();
	break;

    case 'F':
    case 'f':
	mSortDirection = FileSortInfo::Forward;
	drawFields ();
	break;

    case 'R':
    case 'r':
	mSortDirection = FileSortInfo::Reverse;
	drawFields ();
	break;

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	mrSortType = mSortType;
	mrSortDirection = mSortDirection;
	mExitKey = KB_ENTER;
	postEvent (Event (EV_QUIT));
	break;

    default:
        return Popup::processEvent (event);
    }

    return 1;
}

// End
