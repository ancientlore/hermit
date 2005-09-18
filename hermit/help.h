// Help Screen class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef HELP_H
#define HELP_H

#include "console/screen.h"
#include "console/scroller.h"
#include "console/scrollmsg.h"


class HelpScreen : public Screen {
    ScrollMessage mSM;
    static const MessageInfo mMsgs[];
    static int mnMsgs;
    Scroller* mScroller;
    void handleKbEsc (const Event& event);
    void handleKbDollar (const Event& event);

public:

    HelpScreen ();
    virtual ~HelpScreen ();

    // Screen / event related
    virtual int processEvent (const Event& event);
};

#endif
