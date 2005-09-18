// LicenseScreen class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "license.h"
#include "version.h"

LicenseScreen::LicenseScreen () : mSM (mMsgs, mnMsgs)
{
    int w, h;	// screen size

    // SET CONSOLE TITLE TO OUR COPYRIGHT TITLE
#ifdef HERMIT_PRO
    SetConsoleTitle ("HERMIT PRO Licensing Information");
#else
    SetConsoleTitle ("HERMIT Licensing Information");
#endif

    // Handle escape key
    addHandler (KB_ESC, this, (Handler) &LicenseScreen::handleKbEsc);

    // Build the scroller
    getSize (w, h);
    mScroller = new Scroller (*this, 0, 0, w, h);
    if (mScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mScroller->setScrollable (&mSM);
}

LicenseScreen::~LicenseScreen ()
{
}

void LicenseScreen::handleKbEsc (const Event&)
{
    postEvent (Event (EV_QUIT));
}

int LicenseScreen::processEvent (const Event& event)
{
    // char str[80];
    // wsprintf (str, "my key = %d\n", key);
    // writeText (str);
    // return 1;
    return Screen::processEvent (event);
}

const MessageInfo LicenseScreen::mMsgs[] = {	  // .........1.........2.........3.........4.........5.........6.........7.........8
						  // 12345678901234567890123456789012345678901234567890123456789012345678901234567890

    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
#ifdef HERMIT_PRO
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "                 \xD1   \xD1   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xB8 \xD5\xCD\xD1   \xCD\xD1\xCD   \xD5\xCD\xD1\xCD\xB8   PRO       \xDA\xBF" },
#else
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "                 \xD1   \xD1   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xB8 \xD5\xCD\xD1   \xCD\xD1\xCD   \xD5\xCD\xD1\xCD\xB8             \xDA\xBF" },
#endif
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "            \xDA\xBF   \xB3   \xB3   \xB3       \xB3   \xB3   \xB3 \xB3 \xB3 \xB3    \xB3      \xB3          \xDA\xBF   \xC0\xD9" },
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "       \xDA\xBF   \xC0\xD9   \xC6\xCD\xCD\xCD\xB5   \xC6\xCD\xB5     \xC6\xCD\xCD\xD1\xBE   \xB3 \xD4\xCD\xBE \xB3    \xB3      \xB3     \xDA\xBF   \xC0\xD9" },
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "  \xDA\xBF   \xC0\xD9        \xB3   \xB3   \xB3       \xB3  \xC0\xBF   \xB3     \xB3    \xB3      \xB3     \xC0\xD9" },
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "  \xC0\xD9             \xCF   \xCF   \xCF\xCD\xCD\xCD\xBE   \xCF   \xCF   \xCF     \xCF   \xCD\xCF\xCD     \xCF" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    VER_PROGINFO },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " Use the arrow and page keys to scroll the screen." },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " HERMIT likes to get in your shell and help you out.  HERMIT is a text-mode" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " File Manager and Command Assistant, making it easy to move around directories" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " and run built-in and custom commands on your files.  HERMIT is also very well" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " integrated into 32-bit Windows* operating systems." },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseHighlight,   ceLicenseHighlightSel,  " Licensing Information" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " Good news! HERMIT is FREEware. You may use it for any legal purpose provided" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " that the program is not modified in any way and is distributed in its" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " entirety with all copyright notices intact." },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " You agree that HERMIT has no warranty and in no cases shall the author be" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " held liable for any problems encountered as a result of using HERMIT." },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " Additionally, you agree that your use of HERMIT does not entitle you to" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " technical support, free upgrades, or any other services from the author." },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " Send suggestions or contributions to Michael D. Lore, hermit@skyegg.com" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
	{ ceLicenseNormal,	    ceLicenseNormalSel,	    " http://software.skyegg.com/" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseHighlight,   ceLicenseHighlightSel,  " Footnotes" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    " * Windows is a trademark of Microsoft Corporation." },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" },
    { ceLicenseHighlight,   ceLicenseHighlightSel,  "                     *** PRESS ESC TO RETURN TO HERMIT ***" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    "" }

/*
     PFM is distributed under the USER-SUPPORTED concept.
 You are  encouraged to copy and share  this program with
 other users.   If you find PFM useful, your contribution
 ($45 suggested) will be appreciated.  Any comments, bugs
 or suggestions will also be appreciated.

 USER-SUPPORTED software is based on these three beliefs:

   1.  The  value of software is best assessed by the
       user on his own system.
   2.  Creation of personal computer software can and
       should be supported by computing community.
   3.  That copying of programs  should be encourged,
       rather than restricted.
*/

};

int LicenseScreen::mnMsgs = sizeof (mMsgs) / sizeof (MessageInfo);

// End
