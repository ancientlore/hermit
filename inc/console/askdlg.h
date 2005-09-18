// Ask Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef ASKDLG_H
#define ASKDLG_H

#include "console/screen.h"


class AskDialog : public Popup {
public:
    AskDialog (Screen& screen, const char *title, const char *text);
    virtual int processEvent (const Event& event);
};

#endif
