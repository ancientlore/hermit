// Run Edit Window
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "runedit.h"


RunEdit::RunEdit (Screen& screen, int x, int y, int w, char *text, int length, History *pHist) :
    Edit (screen, x, y, w, text, length, pHist), mNoPrompt (0)
{
}

RunEdit::~RunEdit ()
{
}

void RunEdit::insBaseFile ()
{
    processText (Event ('!'));
    processText (Event ('b'));
}

void RunEdit::insFile ()
{
    processText (Event ('!'));
    processText (Event ('f'));
}

void RunEdit::insDir ()
{
    processText (Event ('!'));
    processText (Event ('d'));
}

void RunEdit::insPrompt ()
{
    if (!mNoPrompt) {
        processText (Event ('!'));
	processText (Event ('p'));
    }
}

void RunEdit::insAllFiles ()
{
    if (!mNoPrompt) {
	processText (Event ('!'));
	processText (Event ('m'));
    }
}
void RunEdit::insAllFilesQuoted ()
{
    if (!mNoPrompt) {
	processText (Event ('!'));
	processText (Event ('q'));
    }
}

void RunEdit::insDrive ()
{
	processText (Event ('!'));
	processText (Event ('v'));
}

void RunEdit::insPathOnly ()
{
	processText (Event ('!'));
	processText (Event ('t'));
}

void RunEdit::insImmediateReturn ()
{
    if (!mNoPrompt) {
	processText (Event ('!'));
	processText (Event ('i'));
    }
}

void RunEdit::insAutoRefresh ()
{
    if (!mNoPrompt) {
	processText (Event ('!'));
	processText (Event ('a'));
    }
}

int RunEdit::processEvent (const Event& event)
{
    switch (event.key) {

    case KB_F1:
    case KB_ALT_B:
	insBaseFile ();
	break;

    case KB_F2:
    case KB_ALT_F:
	insFile ();
	break;

    case KB_F3:
    case KB_ALT_D:
	insDir ();
	break;

    case KB_F4:
    case KB_ALT_P:
	insPrompt ();
	break;

    case KB_F5:
    case KB_ALT_M:
	insAllFiles ();
	break;

    case KB_F6:
    case KB_ALT_Q:
	insAllFilesQuoted ();
	break;

    case KB_F7:
    case KB_ALT_I:
	insImmediateReturn ();
	break;

    case KB_F8:
    case KB_ALT_A:
	insAutoRefresh ();
	break;

	case KB_F9:
	case KB_ALT_V:
	insDrive ();
	break;

	case KB_F10:
	case KB_ALT_T:
	insPathOnly ();
	break;

    case KB_CTRL_ENTER:
	sendEvent (Event (KB_ENTER));
	mExitKey = KB_CTRL_ENTER;
	break;

    default:
        return Edit::processEvent (event);
    }

    return 1;
}

// End
