// Color tables code
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef COLORS_H
#define COLORS_H

#define STRICT
#include <windows.h>

// Type of color entries
enum ColorEntry {
    ceFirstEntry = 0,

    // Status bar
    ceStatusText = ceFirstEntry,
    ceStatusTail,

    // ScrollDir file listing
    ceFileTag,
    ceFileAttributes,
    ceFileSize,
    ceFileDate,
    ceFileName,
    ceFileNameExt,

    // ScrollDir file listing - under cursor!
    ceFileTagCursor,
    ceFileAttributesCursor,
    ceFileSizeCursor,
    ceFileDateCursor,
    ceFileNameCursor,
    ceFileNameExtCursor,

    // Title bar
    ceTitleText,
    ceTitleTail,

    // Popup windows
    cePopupBackground,
    cePopupBorder,

    // Fields in Popup windows
    ceField,
    ceFieldKey,
    ceSelectedField,
    ceSelectedKey,

    // Edit Window
    ceEditWindow,

    // CommandDir file listing
    ceCmdHeading,
    ceCmdKey,
    ceCmdText,

    // CommandDir file listing - under cursor!
    ceCmdHeadingCursor,
    ceCmdKeyCursor,
    ceCmdTextCursor,

    // License Window
    ceLicenseNormal,
    ceLicenseHighlight,
    ceLicenseNormalSel,
    ceLicenseHighlightSel,

    // Help Window
    ceHelpNormal,
    ceHelpHighlight,
    ceHelpNormalSel,
    ceHelpHighlightSel,

    // Command text on user screen
    ceCommandText,

    // Verb scroller
    ceVerb,
    ceVerbCursor,

    // Assigned Commands scroller
    ceAssignKey,
    ceAssignCommand,
    ceAssignKeyCursor,
    ceAssignCommandCursor,

    // Info Window
    ceInfoNormal,
    ceInfoHighlight,
    ceInfoNormalSel,
    
    ceNumEntries
};

void selectColorTable (int table);  // it does a mod so you can't pick an invalid one
int getColorTable ();

WORD getColor (ColorEntry entry);

#endif
