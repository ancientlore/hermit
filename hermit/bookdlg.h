// Bookmarks Dialog
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef BOOKDLG_H
#define BOOKDLG_H

#include "console/screen.h"


class BookmarkDialog : public Popup {
    const char **mpBookmarks;
    void draw ();
public:
    BookmarkDialog (Screen& screen, const char **bookmarks);
    virtual int processEvent (const Event& event);
};


#endif
