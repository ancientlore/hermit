// Edit Window
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

//#include <ctype>
#include <cstring>
#include "console/edit.h"
#include "registry.h"


History::History (const char *name) : mName (name)
{
    if (name == 0)
	throw AppException (WHERE, ERR_INTERNAL);

    for (int i = 0; i < HIST_SIZE; i++)
	mpText[i] = 0;

    char subKey[256];
    strcpy (subKey, HKCU_SUBKEY_HERMIT "\\");
    strcat (subKey, mName);
    try {
	RegistryKey k (HKEY_CURRENT_USER, subKey, KEY_READ | KEY_WRITE);
	DWORD type;
	char str[3];
	for (int j = 0; j < HIST_SIZE; j++) {
	    wsprintf (str, "%d", j);
	    char *value = k.queryValue (str, type);
	    if (type == REG_SZ  &&  value != 0  &&  value[0] != '\0') {
		mpText[j] = strdup (value);
		delete [] value;
		if (mpText[j] == 0)
		    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	    }
	    else {
		delete [] value;
		break;
	    }
	}
    }
    catch (const std::exception&) {
	// oh well, it's only the history
    }
}

History::~History ()
{
    char subKey[256];
    strcpy (subKey, HKCU_SUBKEY_HERMIT "\\");
    strcat (subKey, mName);
    try {
	RegistryKey k (HKEY_CURRENT_USER, subKey, KEY_READ | KEY_WRITE);
	char str[3];
	for (int j = 0; j < HIST_SIZE; j++) {
	    wsprintf (str, "%d", j);
	    const char *value = mpText[j];
	    if (value == 0)
		value = "";
	    k.setValue (str, value);
	}
    }
    catch (const std::exception&) {
	// oh well, it's only the history
    }

    for (int i = 0; i < HIST_SIZE; i++)
	if (mpText[i] != 0)
	    free (mpText[i]);
}

void History::add (const char *text)
{
    int i;

    if (text == 0  ||  *text == '\0')
	return;	    // don't add blanks to the history

    // See if the string is already in the list; if so, promote it to front
    for (i = 0; i < HIST_SIZE  &&  mpText[i] != 0; i++) {
	if (strcmp (mpText[i], text) == 0) {
	    char *p = mpText[i];
	    for (int j = i; j > 0; j--)
		mpText[j] = mpText[j - 1];
	    mpText[0] = p;
	    return;
	}
    }

    // Add the string
    if (mpText[HIST_SIZE - 1] != 0)
	free (mpText[HIST_SIZE - 1]);
    for (i = HIST_SIZE - 1; i > 0; i--)
	mpText[i] = mpText[i - 1];
    mpText[0] = strdup (text);
    if (mpText[0] == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
}

Edit::Edit (Screen& screen, int x, int y, int w, char *text, int length, History *pHist) :
    Popup (screen, x, y, w, 1), mpRealText (text), mpHist (pHist), mMaxLength (length),
    mbInsMode (1), mWindowCursor (0), mTextCursor (0), mHistIndex (-1)
{
    if (text == 0  ||  length <= 0)
	throw AppException (WHERE, ERR_INTERNAL);

    mLength = (int) strlen (mpRealText);
    if (mLength > mMaxLength)
	throw AppException (WHERE, ERR_INTERNAL);
    mpText = new char [mMaxLength + 1];
    if (mpText == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    strncpy (mpText, mpRealText, mMaxLength);
    mpText[mMaxLength] = '\0';
    if (mpHist != 0  &&  mpHist->mpText[0] != 0  &&  strcmp (mpText, mpHist->mpText[0]) == 0)
	mHistIndex = 0;
    setCursor (1);	// zero disables the cursor
    setCursorPosition ();
    draw ();
}

Edit::~Edit ()
{
    delete [] mpText;
    setCursor (0);
}

void Edit::draw ()
{
    fillChar (0, 0, ' ', mWidth);
    fillColors (0, 0, getColor (ceEditWindow), mWidth);
    drawText ();
}

void Edit::drawText ()
{
    int textOffset = mTextCursor - mWindowCursor;
    if (textOffset < 0)
	throw AppException (WHERE, ERR_INTERNAL);
    int n = writeText (0, 0, &mpText[textOffset], (mLength > mWidth ? mWidth : mLength));
    if (n < mWidth)
	fillChar (n, 0, ' ', mWidth - n);
}

void Edit::processText (const Event& event)
{
    if ((mbInsMode  &&  mLength < mMaxLength)  ||  (!mbInsMode  &&  mTextCursor < mMaxLength)) {
	if (mbInsMode) {
	    for (int i = mLength; i >= mTextCursor; i--)
		mpText[i + 1] = mpText[i];
	    mLength++;
	}
	else if (mTextCursor == mLength) {  // inserting at end from overwrite mode
	    mpText[mTextCursor + 1] = '\0';
	    mLength++;
	}
	mpText[mTextCursor] = event.key;
	mTextCursor++;
	if (mWindowCursor < mWidth - 1) {
	    mWindowCursor++;
	    setCursorPosition (mWindowCursor);
	}
	drawText ();
	mHistIndex = -1;
    }
    else
	MessageBeep (MB_OK);
}

void Edit::up ()
{
    if (mpHist != 0  &&  mHistIndex < HIST_SIZE - 1  &&  mpHist->mpText[mHistIndex + 1] != 0) {
	mHistIndex++;
	strncpy (mpText, mpHist->mpText[mHistIndex], mMaxLength);
	mpText[mMaxLength] = '\0';
	mLength = (int) strlen (mpText);
	mTextCursor = 0;
	mWindowCursor = 0;
	setCursorPosition (mWindowCursor);
	drawText ();
    }
}

void Edit::down ()
{
    if (mpHist != 0  &&  mHistIndex >= 0) {
	mHistIndex--;
	if (mHistIndex >= 0) {
	    strncpy (mpText, mpHist->mpText[mHistIndex], mMaxLength);
	    mpText[mMaxLength] = '\0';
	    mLength = (int) strlen (mpText);
	}
	else {
	    mpText[0] = '\0';
	    mLength = 0;
	}
	mTextCursor = 0;
	mWindowCursor = 0;
	setCursorPosition (mWindowCursor);
	drawText ();
    }
}

void Edit::left ()
{
    if (mTextCursor > 0) {
    	mTextCursor--;
	if (mWindowCursor > 0) {
	    mWindowCursor--;
	    setCursorPosition (mWindowCursor);
	}
	else
	    drawText ();
    }
}

void Edit::right ()
{
    if (mTextCursor < mLength) {
	mTextCursor++;
	if (mWindowCursor < mWidth - 1) {
	    mWindowCursor++;
	    setCursorPosition (mWindowCursor);
	}
	else
	    drawText ();
    }
}

void Edit::backSpace ()
{
    if (mTextCursor > 0  &&  mLength > 0) {	// can't delete the nothing from the end
	strcpy (&mpText[mTextCursor - 1], &mpText[mTextCursor]);
	mLength--;
	mTextCursor--;
	if (mWindowCursor > 0) {
	    mWindowCursor--;
	    setCursorPosition (mWindowCursor);
	}
	drawText ();
	mHistIndex = -1;
    }
    else
	MessageBeep (MB_OK);
}

void Edit::tab ()
{
    processText (Event (' '));
}

void Edit::home ()
{
    if (mTextCursor > 0) {
    	mTextCursor = 0;
	if (mWindowCursor > 0) {
	    mWindowCursor = 0;
	    setCursorPosition (mWindowCursor);
	}
	drawText ();
    }
}

void Edit::end ()
{
    if (mTextCursor != mLength) {
    	mTextCursor = mLength;
	mWindowCursor = mLength;
	if (mWindowCursor > mWidth - 1)
	    mWindowCursor = mWidth - 1;
	setCursorPosition (mWindowCursor);
	drawText ();
    }
}

void Edit::ins ()
{
    mbInsMode = !mbInsMode;
    setCursor (mbInsMode ? 1 : 99);
}

void Edit::del ()
{
    if (mTextCursor < mLength  &&  mLength > 0) {	// can't delete the nothing from the end
	strcpy (&mpText[mTextCursor], &mpText[mTextCursor + 1]);
	mLength--;
	drawText ();
	mHistIndex = -1;
    }
    else
	MessageBeep (MB_OK);
}

int Edit::processEvent (const Event& event)
{
    if (event.key > 0  &&  event.key < 256  &&  isprint (event.key)) {
	processText (event);
	return 1;
    }

    switch (event.key) {

    case KB_UP:
    case KB_KP_UP:
	up ();
	break;

    case KB_DOWN:
    case KB_KP_DOWN:
	down ();
	break;

    case KB_LEFT:
    case KB_KP_LEFT:
	left ();
	break;

    case KB_RIGHT:
    case KB_KP_RIGHT:
	right ();
	break;

    case KB_BKSPACE:
	backSpace ();
	break;

    case KB_TAB:
	tab ();
	break;

    case KB_HOME:
    case KB_KP_HOME:
	home ();
	break;

    case KB_END:
    case KB_KP_END:
	end ();
	break;

    case KB_INS:
    case KB_KP_INS:
	ins ();
	break;

    case KB_DEL:
    case KB_KP_DEL:
	del ();
	break;

    case KB_ESC:
	mExitKey = KB_ESC;
	postEvent (Event (EV_QUIT));
	break;

    case KB_ENTER:
	strncpy (mpRealText, mpText, mLength);
	mpRealText[mLength] = '\0';
	if (mpHist != 0)
	    mpHist->add (mpText);
	mExitKey = KB_ENTER;
	postEvent (Event (EV_QUIT));
	break;

    case EV_COLOR_CHANGE:
	draw ();
	return Popup::processEvent (event);

    default:
        return Popup::processEvent (event);
    }

    return 1;
}

// End
