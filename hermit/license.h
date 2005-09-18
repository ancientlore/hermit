// License Screen class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef LICENSE_H
#define LICENSE_H

#include "console/screen.h"
#include "console/scroller.h"
#include "console/scrollmsg.h"


class LicenseScreen : public Screen {
    ScrollMessage mSM;
    static const MessageInfo mMsgs[];
    static int mnMsgs;
    Scroller* mScroller;
    void handleKbEsc (const Event& event);

public:

    LicenseScreen ();
    virtual ~LicenseScreen ();

    // Screen / event related
    virtual int processEvent (const Event& event);
};

#endif
