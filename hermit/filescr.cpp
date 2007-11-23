// File Scroller class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include <string.h>
#include "filescr.h"
#include "sortdlg.h"
#include "bookdlg.h"
#include "gotodlg.h"
#include "filtdlg.h"
#include "registry.h"
#include "ptr.h"
#include "shstuff.h"


FileScroller::FileScroller (Screen& screen, int x, int y, int w, int h) :
    Scroller (screen, x, y, w, h), mHist ("Goto History"), mFilterHist ("Filter History")
{
    for (int i = 0; i < 10; i++) {
	mBookmarkFileCursor[i] = 0;
	mBookmarkScrCursor[i] = 0;
	mpBookmark[i] = 0;
    }
    loadBookmarks ();
    clrPrevCursor ();
    mpLastDir = 0;
    mLastFileCursor = 0;
    mLastScrCursor = 0;
    mFilter[0] = '\0';
    mScrollDir.setCancelProc (FileScroller::cancelProc, this);
    myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) "Reading directory... press F10 to stop"));
    int stopped = mScrollDir.readDir (mFilter[0] == '\0' ? 0 : mFilter);
    myScreen.sendEvent (Event (EV_TITLE_MSG, (DWORD) mScrollDir.getDirName ()));
    setScrollable (&mScrollDir);
    int c = getFirstRealFile ();
    if (c > 0)
	move (c);	// put cursor past . and ..
    if (stopped)
	myScreen.postEvent (Event (EV_STATUS_MSG, (DWORD) "Read aborted - directory listing incomplete!"));
}

FileScroller::~FileScroller ()
{
    saveBookmarks ();

    for (int i = 0; i < 10; i++) {
	if (mpBookmark[i] != 0)
	    free (mpBookmark[i]);
    }
    if (mpLastDir != 0)
	free (mpLastDir);
}

void FileScroller::loadBookmarks ()
{
    try {
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Bookmarks", KEY_READ | KEY_WRITE);
	DWORD type;
	char str[3];
	for (int j = 0; j < 10; j++) {
	    wsprintf (str, "%d", j);
	    char *value = k.queryValue (str, type);
	    if (type == REG_SZ  &&  value != 0  &&  value[0] != '\0') {
		mpBookmark[j] = _strdup (value);
		delete [] value;
		if (mpBookmark[j] == 0)
		    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	    }
	    else
		delete [] value;
	}
    }
    catch (const std::exception&) {
	// oh well, it's only the history
    }
}

void FileScroller::saveBookmarks ()
{
    try {
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Bookmarks", KEY_READ | KEY_WRITE);
	char str[3];
	for (int j = 0; j < 10; j++) {
	    wsprintf (str, "%d", j);
	    const char *value = mpBookmark[j];
	    if (value == 0)
		value = "";
	    k.setValue (str, value);
	}
    }
    catch (const std::exception&) {
	// oh well, it's only the bookmarks
    }
}

void FileScroller::toggle ()
{
    try {
	File& file = mScrollDir.getCurrentFile ();
	if (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
	    return;
	if (file.isTagged ()  ||  file.isSortOfTagged ())
	    file.unTag ();
	else
	    file.tag ();
	drawLine (mCursor);
	down ();
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
    }
}

void FileScroller::tag ()
{
    try {
	File& file = mScrollDir.getCurrentFile ();
	if (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
	    return;
	file.tag ();
	drawLine (mCursor);
	down ();
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
    }
}

void FileScroller::unTag ()
{
    try {
	File& file = mScrollDir.getCurrentFile ();
	if (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
	    return;
	file.unTag ();
	drawLine (mCursor);
	down ();
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
    }
}

void FileScroller::tagAll ()
{
    FileIterator *iter = mScrollDir.getIterator ();
    if (iter != 0) {
	while (!iter->done ()) {
	    File& file = *iter;
	    if (!(file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY))
		file.tag ();
	    (*iter)++;
	}
	delete iter;
	draw ();
	myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) "Tagged all files"));
    }
}

void FileScroller::unTagAll ()
{
    FileIterator *iter = mScrollDir.getIterator ();
    if (iter != 0) {
	while (!iter->done ()) {
	    File& file = *iter;
	    if (!(file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY))
		file.unTag ();
	    (*iter)++;
	}
	delete iter;
	draw ();
	myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) "Un-tagged all files"));
    }
}

void FileScroller::reTagAll ()
{
    FileIterator *iter = mScrollDir.getIterator ();
    if (iter != 0) {
	while (!iter->done ()) {
	    File& file = *iter;
	    if (!(file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)) {
		if (file.isSortOfTagged ())
		    file.tag ();
	    }
	    (*iter)++;
	}
	delete iter;
	draw ();
	myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) "Re-tagged all files"));
    }
}

void FileScroller::addPrevCursor (int fileCursor, int scrCursor)
{
    for (int i = 9; i > 0; i--) {
	mPrevFileCursor[i] = mPrevFileCursor[i - 1];
	mPrevScrCursor[i] = mPrevScrCursor[i - 1];
    }
    mPrevFileCursor[0] = fileCursor;
    mPrevScrCursor[0] = scrCursor;
}

void FileScroller::remPrevCursor (int& fileCursor, int& scrCursor)
{
    fileCursor = mPrevFileCursor[0];
    scrCursor = mPrevScrCursor[0];
    for (int i = 1; i < 10; i++) {
	mPrevFileCursor[i - 1] = mPrevFileCursor[i];
	mPrevScrCursor[i - 1] = mPrevScrCursor[i];
	if (mPrevFileCursor[i] == -1  &&  mPrevScrCursor[i] == -1)
	    return;
    }
    mPrevFileCursor[9] = -1;
    mPrevScrCursor[9] = -1;
}

void FileScroller::clrPrevCursor ()
{
    for (int i = 0; i < 10; i++) {
	mPrevFileCursor[i] = -1;
	mPrevScrCursor[i] = -1;
    }
}

void FileScroller::left ()
{
    int fileCursor, scrCursor;
    remPrevCursor (fileCursor, scrCursor);
    gotoLocation ("..", fileCursor, scrCursor);
}

void FileScroller::right ()
{
    try {
	File& f = mScrollDir.getCurrentFile ();
	if (f.getAttributes () & FILE_ATTRIBUTE_DIRECTORY) {
	    addPrevCursor (mScrollDir.getCursor (), mCursor);
	    gotoLocation (f.getName ());
	}
	// else bring up the file viewer!!!
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
    }
}

void FileScroller::gotoLocation (const char *where, int fileCursor, int screenCursor)
{
    mLastFileCursor = mScrollDir.getCursor ();
    mLastScrCursor = mCursor;
    char *lastDir = mpLastDir;
    mpLastDir = _strdup (mScrollDir.getDirName ());
    if (mpLastDir == 0)
        throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) "Changing directories..."));

    // Do something to allow screen to update (three cycles should do it)
#if 1
    Sleep (40);
#else
    Sleep (0);
    Sleep (0);
    Sleep (0);
#endif

    if (SetCurrentDirectory (where) == TRUE)
	scan (fileCursor, screenCursor);
    else
        myScreen.sendWinErrStatusMsg ("An error occurred while changing directories");

    if (lastDir != 0)	    // freed last because it could be the same ptr as 'where'
        free (lastDir);
}

void FileScroller::scan (int fileCursor, int screenCursor)
{
    myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) "Reading directory... press F10 to stop"));
    int stopped = mScrollDir.readDir (mFilter[0] == '\0' ? 0 : mFilter);
    myScreen.sendEvent (Event (EV_TITLE_MSG, (DWORD) mScrollDir.getDirName ()));
    if (fileCursor < 0)
        mCursor = mScrollDir.moveCursor (getFirstRealFile ());	// put cursor past . and ..
    else {
        mCursor = mScrollDir.moveCursor (fileCursor);	    // put it where desired
	if (screenCursor >= 0)
	    mCursor = screenCursor;
    }
    if (mCursor >= mHeight)
        mCursor = mHeight - 1;
    if (mCursor > mScrollDir.getCursor ())
	mCursor = mScrollDir.getCursor ();
    draw ();
    myScreen.postEvent (Event (EV_CURSOR_CHANGE, mCursor));
    updateDriveCurrentDir ();
    if (stopped)
	myScreen.postEvent (Event (EV_STATUS_MSG, (DWORD) "Read aborted - directory listing incomplete!"));
}

int FileScroller::cancelProc (void *pData)
{
    FileScroller *pThis = (FileScroller *) pData;
    if (pThis != 0) {
	while (pThis->myScreen.hasKey ()) {	// only checking real key events
	    Event e;
	    pThis->myScreen.getKey (e);
	    if (e.key == KB_F10) {
		pThis->myScreen.clearPostedKeys ();
		return 1;
	    }
	    else if (!isMouse (e))
	    	pThis->myScreen.postEvent (e);  // don't lose it! (unless mouse)
	}
    }
    return 0;
}

void FileScroller::gotoDrive (const Event& event)
{
    int bit = event.key - KB_CTRL_A;
    DWORD drivesMask = GetLogicalDrives ();
    if ((drivesMask >> bit) & 0x1) {

	char str[3];
	str[0] = bit + KB_A;
	str[1] = ':';
	str[2] = '\0';

	gotoLocation (str);
	clrPrevCursor ();
    }
    else {
	char str[40];
	wsprintf (str, "Drive %c: does not exist!", bit + KB_A);
	myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) str));
    }
}

void FileScroller::gotoPrevious ()
{
    if (mpLastDir != 0) {
	gotoLocation (mpLastDir, mLastFileCursor, mLastScrCursor);
	clrPrevCursor ();
    }
}

void FileScroller::gotoBookmark (const Event& event)
{
    int bit = event.key - KB_CTRL_0;

    if (mpBookmark[bit] != 0) {
	gotoLocation (mpBookmark[bit], mBookmarkFileCursor[bit], mBookmarkScrCursor[bit]);
	clrPrevCursor ();
    }
    else {
	char str[40];
	wsprintf (str, "No bookmark %c", bit + KB_0);
	myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) str));
    }
}

void FileScroller::setBookmark (const Event& event)
{
    int bit = event.key - KB_ALT_0;

    if (bit < 10) {
	if (mpBookmark[bit] != 0) {
	    if (!myScreen.ask ("Bookmarks", "Do you want to overwrite the existing bookmark?"))
		return;
	}
        mBookmarkFileCursor[bit] = mScrollDir.getCursor ();
	mBookmarkScrCursor[bit] = mCursor;
	if (mpBookmark[bit] != 0)
    	    free (mpBookmark[bit]);
	mpBookmark[bit] = _strdup (mScrollDir.getDirName ());
	if (mpBookmark[bit] == 0)
	    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	char str[40];
	wsprintf (str, "Bookmark %c set", bit + KB_0);
	myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) str));
    }
}

void FileScroller::selectBookmark ()
{
    int exitCode = KB_ESC;
    {
	BookmarkDialog book(myScreen, (const char **) mpBookmark);
	book.run ();
	exitCode = book.getExitCode ();
    }
    if (exitCode != KB_ESC)
	gotoBookmark (Event (KB_CTRL_0 + exitCode - '0'));
}

int FileScroller::getFirstRealFile ()
{
    // this relies on the fact that . and .. always comes first
    int c = 0;
    FileIterator *iter = mScrollDir.getIterator ();
    if (iter != 0) {
	while (!iter->done ()) {
	    const File& file = *iter;
	    if (strcmp (file.getName (), ".") == 0  ||  strcmp (file.getName (), "..") == 0)
		c++;
	    else
		break;
	    (*iter)++;
	}
	delete iter;
    }
    return c;
}

void FileScroller::selectSorting ()
{
    int sortType = File::defaultSort.getSortType ();
    int sortDir = File::defaultSort.getDirection ();
    int exitCode = KB_ESC;
    {
	SortDialog sort(myScreen, sortType, sortDir);
	sort.run ();
	exitCode = sort.getExitCode ();
    }
    if (exitCode == KB_ENTER) {
	File::defaultSort.setSortType (sortType);
	File::defaultSort.setDirection (sortDir);
	mScrollDir.setSorting (sortType, sortDir);
	draw ();
	myScreen.postEvent (Event (EV_CURSOR_CHANGE, mCursor));
    }
}

void FileScroller::askLocation ()
{
    char text[256];
    int exitCode = KB_ESC;
    {
	*text = '\0';
	GotoDialog gotoDlg (myScreen, text, 255, &mHist);
	gotoDlg.run ();
	exitCode = gotoDlg.getExitCode ();
    }
    if (exitCode == KB_ENTER) {
	gotoLocation (text);
	clrPrevCursor ();
    }
}

void FileScroller::goHome (bool useEnvVars)
{
    char buffer[MAX_PATH];

    // Get the HOME directory
    if (useEnvVars == false || GetEnvironmentVariable ("HERMIT_HOME", buffer, MAX_PATH) <= 0) {
	if (useEnvVars == false || GetEnvironmentVariable ("HOME", buffer, MAX_PATH) <= 0) {
	    ArrayPtr<char> p = getSpecialFolder (CSIDL_PERSONAL);
	    if (p == 0) {
		myScreen.sendWinErrStatusMsg ("Could not locate home directory");
		return;	// we tried all three!!!
	    }
	    strncpy (buffer, p, MAX_PATH - 1);
	    buffer[MAX_PATH - 1] = '\0';
	}
    }

    gotoLocation (buffer);
    clrPrevCursor ();
}

void FileScroller::setFilter ()
{
    int exitCode = KB_ESC;
    {
	FilterDialog dlg (myScreen, mFilter, FILTER_LEN, &mFilterHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode == KB_ENTER) {
	if (mFilter[0] == '\0')
	    myScreen.sendEvent (Event (EV_TITLE_EXTRA, (DWORD) 0));
	else
	    myScreen.sendEvent (Event (EV_TITLE_EXTRA, (DWORD) mFilter));
	scan ();
    }
}

int FileScroller::handleMouse (const Event& event)
{
    int mouseX = LOWORD (event.data);
    int mouseY = HIWORD (event.data);
    if (mouseX >= mX  &&  mouseX < mX + mWidth  &&
        mouseY >= mY  &&  mouseY < mY + mHeight) {
	int isDir = 0;
	try {
	    File& f = mScrollDir.getCurrentFile ();
	    if (f.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
		isDir = 1;
	}
	catch (const AppException& info) {
	    if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
		throw;
	}
	switch (event.key) {
	case MS_FLAG | MS_LEFTBUTTON | MS_DBLCLICK:
	    if (isDir)
		right ();   // don't need to move because single click did it
	    else
		myScreen.postEvent (Event (KB_ALT_O));	// for dbl click we are already on the line
	    break;
	case MS_FLAG | MS_LEFTBUTTON | MS_BUTTON | MS_ALT:
	case MS_FLAG | MS_LEFTBUTTON | MS_MOVED | MS_ALT:
	    move (mouseY - mY  - mCursor);
	    break;
	case MS_FLAG | MS_LEFTBUTTON | MS_DBLCLICK | MS_ALT:
	    myScreen.postEvent (Event (KB_TAB));	// don't need to move because single click did it
	    break;
	case MS_FLAG | MS_RIGHTBUTTON | MS_BUTTON:
	case MS_FLAG | MS_RIGHTBUTTON | MS_DBLCLICK:
	    left ();	// position doesn't matter
	    break;
	case MS_FLAG | MS_LEFTBUTTON | MS_BUTTON | MS_CTRL:
	case MS_FLAG | MS_LEFTBUTTON | MS_MOVED | MS_CTRL:
	    move (mouseY - mY  - mCursor);
	    toggle ();
	    move (mouseY - mY  - mCursor);
	    break;
	case MS_FLAG | MS_LEFTBUTTON | MS_BUTTON | MS_SHIFT:
	case MS_FLAG | MS_LEFTBUTTON | MS_MOVED | MS_SHIFT:
	    move (mouseY - mY  - mCursor);
	    tag ();
	    move (mouseY - mY  - mCursor);
	    break;
	default:
	    return 0;	// not handled
	}
	return 1;	// handled
    }
    return 0;		// not handled
}

int FileScroller::processEvent (const Event& event)
{
    if (event.key >= KB_CTRL_A  &&  event.key <= KB_CTRL_Z) {
	gotoDrive (event);
	return 1;
    }

    if (event.key >= KB_ALT_0  &&  event.key <= KB_ALT_9) {
	setBookmark (event);
	return 1;
    }

    if (event.key >= KB_CTRL_0  &&  event.key <= KB_CTRL_9) {
	gotoBookmark (event);
	return 1;
    }

    if (isMouse (event)  &&  handleMouse (event))
	return 1;

    switch (event.key) {

    case KB_LEFT:
    case KB_KP_LEFT:
    case KB_ESC:
	left ();
	break;

    case KB_RIGHT:
    case KB_KP_RIGHT:
	right ();
	break;

    case KB_F5:
    case KB_ALT_R:
	scan (mScrollDir.getCursor (), mCursor);
	break;

    case KB_F4:
    case KB_ALT_V:
	gotoPrevious ();
	break;

    case KB_ALT_S:
	selectSorting ();
	break;

    case KB_ALT_B:
	selectBookmark ();
	break;

    case KB_F6:
    case KB_ALT_G:
	askLocation ();
	break;

    case KB_ALT_F:
	setFilter ();
	break;

    case '~':
        goHome (false);
        break;

    case KB_ALT_H:
	goHome (true);
	break;

    case KB_PLUS:
	tag ();
	break;

    case KB_MINUS:
	unTag ();
	break;

    case KB_SPACE:
	toggle ();
	break;

    case KB_ALT_PLUS:
	tagAll ();
	break;

    case KB_ALT_MINUS:
	unTagAll ();
	break;

    case KB_ALT_E:
	reTagAll ();
	break;

    case EV_CURSOR_CHANGE:
	updateStatusBar ();
	break;

    case EV_REDRAW:
	draw ();
	break;

    default:
        return Scroller::processEvent (event);
    }

    return 1;
}

void FileScroller::updateStatusBar ()
{
    char line[FILE_INFO_LENGTH + 1];
    line[0] = '\0';
    try {
	const File& file = mScrollDir.getCurrentFile ();
	file.getStatusLine (line);
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
    }
    myScreen.sendEvent (Event (EV_STATUS_MSG, (DWORD) line));
}

void FileScroller::updateDriveCurrentDir ()
{
    char env_var[4];
    const char *dirName = mScrollDir.getDirName ();

    /*
     * If the new current directory path is NOT a UNC path, we must
     * update the OS environment variable specifying the current
     * directory for what is now current drive. To do this, get the
     * full current directory, build the environment variable string
     * and call SetEnvironmentVariable(). We need to do this because
     * SetCurrentDirectory does not (i.e., does not update the
     * current-directory-on-drive environment variables) and other
     * functions (fullpath, spawn, etc) need them to be set.
     *
     * If associated with a 'drive', the current directory should
     * have the form of the example below:
     *
     *  D:\nt\private\mytests
     *
     * so that the environment variable should be of the form:
     *
     *  =D:=D:\nt\private\mytests
     *
     */
    if ( !((dirName[0] == '\\' || dirName[0] == '/') && (dirName[0] == dirName[1])) ) {
        env_var[0] = '=';
        env_var[1] = toupper (dirName[0]);
        env_var[2] = ':';
        env_var[3] = '\0';
        if (SetEnvironmentVariable (env_var, dirName) != TRUE)
    	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetEnvironmentVariable", GetLastError ());
    }
}

// End
