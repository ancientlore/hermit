// Color tables code
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "console/colors.h"
#include "registry.h"

#define FG_WHITE    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_YELLOW   (FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_VIOLET   (FOREGROUND_RED | FOREGROUND_BLUE)
#define FG_CYAN	    (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FG_RED	    (FOREGROUND_RED)
#define FG_GREEN    (FOREGROUND_GREEN)
#define FG_BLUE	    (FOREGROUND_BLUE)
#define FG_HIGH	    (FOREGROUND_INTENSITY)

#define BG_WHITE    (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_YELLOW   (BACKGROUND_RED | BACKGROUND_GREEN)
#define BG_VIOLET   (BACKGROUND_RED | BACKGROUND_BLUE)
#define BG_CYAN	    (BACKGROUND_BLUE | BACKGROUND_GREEN)
#define BG_RED	    (BACKGROUND_RED)
#define BG_GREEN    (BACKGROUND_GREEN)
#define BG_BLUE	    (BACKGROUND_BLUE)
#define BG_HIGH	    (BACKGROUND_INTENSITY)

// Number of different color tables
const int COLOR_TABLES = 4;

/**********************************************************************/

static WORD defaultColorTable[ceNumEntries] = {

    // StatusBar
    FG_WHITE | FG_HIGH | BG_VIOLET,		    // ceStatusText
    FG_YELLOW | FG_HIGH | BG_VIOLET,		    // ceStatusTail

    // ScrollDir file listing
    FG_CYAN | FG_HIGH,				    // ceFileTag
    FG_YELLOW | FG_HIGH,   			    // ceFileAttributes
    FG_WHITE | FG_HIGH,				    // ceFileSize
    FG_YELLOW | FG_HIGH,			    // ceFileDate
    FG_WHITE | FG_HIGH,				    // ceFileName
    FG_CYAN | FG_HIGH,				    // ceFileNameExt

    // ScrollDir file listing - under cursor!
    FG_CYAN | FG_HIGH | BG_BLUE, 		    // ceFileTagCursor
    FG_YELLOW | FG_HIGH | BG_BLUE,		    // ceFileAttributesCursor
    FG_WHITE | FG_HIGH | BG_BLUE, 		    // ceFileSizeCursor
    FG_YELLOW | FG_HIGH | BG_BLUE, 		    // ceFileDateCursor
    FG_WHITE | FG_HIGH | BG_BLUE,  		    // ceFileNameCursor
    FG_CYAN | FG_HIGH | BG_BLUE, 		    // ceFileNameExtCursor

    // TitleBar
    FG_YELLOW | FG_HIGH | BG_VIOLET,		    // ceTitleText
    FG_WHITE | FG_HIGH | BG_VIOLET,		    // ceTitleTail

    // Popup Windows
    FG_WHITE | FG_HIGH | BG_BLUE,		    // cePopupBackground
    FG_WHITE | FG_HIGH | BG_BLUE | BG_HIGH,		    // cePopupBorder

    // Fields in Popup windows
    FG_CYAN | FG_HIGH | BG_BLUE,		    // ceField
    FG_YELLOW | FG_HIGH | BG_BLUE,		    // ceFieldKey
    FG_CYAN | FG_HIGH,				    // ceSelectedField
    FG_YELLOW | FG_HIGH,			    // ceSelectedKey

    // Edit Window
    FG_WHITE | FG_HIGH | BG_CYAN,		    // ceEditWindow

    // CommandDir file listing
    FG_BLUE | BG_CYAN,  			    // ceCmdHeading
    FG_YELLOW | FG_HIGH | BG_CYAN,		    // ceCmdKey
    FG_WHITE | FG_HIGH | BG_CYAN,		    // ceCmdText

    // CommandDir file listing - under cursor!
    FG_BLUE | FG_HIGH,				    // ceCmdHeadingCursor
    FG_YELLOW | FG_HIGH,			    // ceCmdKeyCursor
    FG_WHITE | FG_HIGH,				    // ceCmdTextCursor

    // License Screen colors
    FG_WHITE,					    // ceLicenseNormal
    FG_YELLOW | FG_HIGH,			    // ceLicenseHighlight
    FG_WHITE | BG_RED,				    // ceLicenseNormalSel
    FG_YELLOW | FG_HIGH | BG_RED,   		    // ceLicenseHighlightSel

    // Help Screen colors
    FG_WHITE,					    // ceHelpNormal
    FG_YELLOW | FG_HIGH,			    // ceHelpHighlight
    FG_WHITE | BG_BLUE,				    // ceHelpNormalSel
    FG_YELLOW | FG_HIGH | BG_BLUE,   		    // ceHelpHighlightSel

    // Command text on user screen
    FG_YELLOW | FG_HIGH,			    // ceCommandText - background always black
						    // because it's merged in later!

    // Verb Scroller
    FG_YELLOW | FG_HIGH | BG_CYAN,		    // ceVerb
    FG_YELLOW | FG_HIGH,     			    // ceVerbCursor

    // Assigned Commands Scroller
    FG_YELLOW | FG_HIGH | BG_CYAN, 		    // ceAssignKey
    FG_WHITE | FG_HIGH | BG_CYAN,    		    // ceAssignCommand
    FG_YELLOW | FG_HIGH,			    // ceAssignKeyCursor
    FG_WHITE | FG_HIGH,				    // ceAssignCommandCursor

    // Info Window
    FG_WHITE | FG_HIGH | BG_BLUE,		    // ceInfoNormal
    FG_YELLOW | FG_HIGH | BG_BLUE,		    // ceInfoHighlight
    FG_WHITE | FG_HIGH | BG_CYAN		    // ceInfoNormalSel
};

/**********************************************************************/

static WORD colorfulColorTable[ceNumEntries] = {

    // StatusBar
    FG_WHITE | FG_HIGH | BG_CYAN,		    // ceStatusText
    FG_YELLOW | FG_HIGH | BG_CYAN,		    // ceStatusTail

    // ScrollDir file listing
    FG_CYAN | FG_HIGH | BG_BLUE,				    // ceFileTag
    FG_YELLOW | FG_HIGH | BG_BLUE,   			    // ceFileAttributes
    FG_WHITE | FG_HIGH | BG_BLUE,				    // ceFileSize
    FG_YELLOW | FG_HIGH | BG_BLUE,			    // ceFileDate
    FG_WHITE | FG_HIGH | BG_BLUE,				    // ceFileName
    FG_CYAN | FG_HIGH | BG_BLUE,				    // ceFileNameExt

    // ScrollDir file listing - under cursor!
    FG_CYAN | BG_WHITE, 		    // ceFileTagCursor
    FG_BLUE | BG_WHITE,		    // ceFileAttributesCursor
    BG_WHITE, 		    // ceFileSizeCursor
    FG_BLUE | BG_WHITE, 		    // ceFileDateCursor
    BG_WHITE,  		    // ceFileNameCursor
    FG_CYAN | BG_WHITE, 		    // ceFileNameExtCursor

    // TitleBar
    FG_YELLOW | FG_HIGH | BG_CYAN,		    // ceTitleText
    FG_WHITE | FG_HIGH | BG_CYAN,		    // ceTitleTail

    // Popup Windows
    FG_YELLOW | FG_HIGH | BG_CYAN,		    // cePopupBackground
    FG_BLUE | BG_WHITE | BG_HIGH,		    // cePopupBorder

    // Fields in Popup windows
    FG_YELLOW | FG_HIGH | BG_CYAN,		    // ceField
    FG_WHITE | FG_HIGH | BG_CYAN,		    // ceFieldKey
    FG_BLUE | BG_WHITE,				    // ceSelectedField
    BG_WHITE,			    // ceSelectedKey

    // Edit Window
    FG_WHITE,		    // ceEditWindow

    // CommandDir file listing
    FG_CYAN | FG_HIGH | BG_BLUE,  			    // ceCmdHeading
    FG_YELLOW | FG_HIGH | BG_BLUE,		    // ceCmdKey
    FG_WHITE | FG_HIGH | BG_BLUE,		    // ceCmdText

    // CommandDir file listing - under cursor!
    FG_BLUE | FG_HIGH | BG_WHITE,				    // ceCmdHeadingCursor
    BG_WHITE,			    // ceCmdKeyCursor
    FG_BLUE | BG_WHITE,				    // ceCmdTextCursor

    // License Screen colors
    FG_WHITE | FG_HIGH | BG_GREEN,					    // ceLicenseNormal
    FG_YELLOW | FG_HIGH | BG_GREEN,			    // ceLicenseHighlight
    BG_WHITE | BG_HIGH,				    // ceLicenseNormalSel
    FG_GREEN | BG_WHITE | BG_HIGH,   		    // ceLicenseHighlightSel

    // Help Screen colors
    FG_WHITE,					    // ceHelpNormal
    FG_YELLOW | FG_HIGH,			    // ceHelpHighlight
    FG_WHITE | BG_BLUE,				    // ceHelpNormalSel
    FG_YELLOW | FG_HIGH | BG_BLUE,   		    // ceHelpHighlightSel

    // Command text on user screen
    FG_GREEN | FG_HIGH,			    // ceCommandText - background always black
						    // because it's merged in later!

    // Verb Scroller
    FG_YELLOW | FG_HIGH | BG_BLUE,		    // ceVerb
    BG_WHITE,     			    // ceVerbCursor

    // Assigned Commands Scroller
    FG_YELLOW | FG_HIGH | BG_BLUE, 		    // ceAssignKey
    FG_WHITE | FG_HIGH | BG_BLUE,    		    // ceAssignCommand
    FG_BLUE | BG_WHITE,			    // ceAssignKeyCursor
    BG_WHITE,				    // ceAssignCommandCursor

    // Info Window
    FG_WHITE | FG_HIGH | BG_BLUE,		    // ceInfoNormal
    FG_YELLOW | FG_HIGH | BG_BLUE,		    // ceInfoHighlight
    FG_WHITE | FG_HIGH | BG_CYAN		    // ceInfoNormalSel
};

/**********************************************************************/

static WORD greyColorTable[ceNumEntries] = {

    // StatusBar
    FG_WHITE | FG_HIGH | BG_HIGH,		    // ceStatusText
    BG_WHITE,		    // ceStatusTail

    // ScrollDir file listing
    FG_WHITE | FG_HIGH,				    // ceFileTag
    FG_WHITE,   			    // ceFileAttributes
    FG_WHITE | FG_HIGH,				    // ceFileSize
    FG_WHITE,			    // ceFileDate
    FG_WHITE | FG_HIGH,				    // ceFileName
    FG_WHITE,				    // ceFileNameExt

    // ScrollDir file listing - under cursor!
    BG_WHITE, 		    // ceFileTagCursor
    BG_WHITE,		    // ceFileAttributesCursor
    BG_WHITE, 		    // ceFileSizeCursor
    BG_WHITE, 		    // ceFileDateCursor
    BG_WHITE,  		    // ceFileNameCursor
    BG_WHITE, 		    // ceFileNameExtCursor

    // TitleBar
    FG_WHITE| FG_HIGH | BG_HIGH,		    // ceTitleText
    BG_WHITE,		    // ceTitleTail

    // Popup Windows
    FG_WHITE | FG_HIGH | BG_HIGH,		    // cePopupBackground
    BG_WHITE | BG_HIGH,		    // cePopupBorder

    // Fields in Popup windows
    FG_WHITE | FG_HIGH | BG_HIGH,		    // ceField
    BG_WHITE,		    // ceFieldKey
    BG_WHITE,				    // ceSelectedField
    BG_WHITE,			    // ceSelectedKey

    // Edit Window
    FG_WHITE | FG_HIGH,		    // ceEditWindow

    // CommandDir file listing
    FG_WHITE | FG_HIGH,  			    // ceCmdHeading
    FG_WHITE,		    // ceCmdKey
    FG_WHITE | FG_HIGH,		    // ceCmdText

    // CommandDir file listing - under cursor!
    BG_WHITE,				    // ceCmdHeadingCursor
    BG_WHITE,			    // ceCmdKeyCursor
    BG_WHITE,				    // ceCmdTextCursor

    // License Screen colors
    FG_WHITE | BG_HIGH,					    // ceLicenseNormal
    FG_WHITE | FG_HIGH | BG_HIGH,			    // ceLicenseHighlight
    BG_WHITE,				    // ceLicenseNormalSel
    BG_WHITE,   		    // ceLicenseHighlightSel

    // Help Screen colors
    FG_WHITE | BG_HIGH,					    // ceHelpNormal
    FG_WHITE | FG_HIGH | BG_HIGH,			    // ceHelpHighlight
    BG_WHITE,				    // ceHelpNormalSel
    BG_WHITE,   		    // ceHelpHighlightSel

    // Command text on user screen
    FG_WHITE | FG_HIGH,			    // ceCommandText - background always black
						    // because it's merged in later!

    // Verb Scroller
    FG_WHITE | FG_HIGH,		    // ceVerb
    BG_WHITE,     			    // ceVerbCursor

    // Assigned Commands Scroller
    FG_WHITE, 		    // ceAssignKey
    FG_WHITE | FG_HIGH,    		    // ceAssignCommand
    BG_WHITE,			    // ceAssignKeyCursor
    BG_WHITE,				    // ceAssignCommandCursor

    // Info Window
    FG_WHITE,		    // ceInfoNormal
    FG_WHITE | FG_HIGH,		    // ceInfoHighlight
    BG_HIGH		    // ceInfoNormalSel
};

/**********************************************************************/

static WORD forestColorTable[ceNumEntries] = {

    // StatusBar
    BG_WHITE | BG_HIGH,		    // ceStatusText
    FG_RED | BG_WHITE | BG_HIGH,		    // ceStatusTail

    // ScrollDir file listing
    FG_GREEN | FG_HIGH | BG_GREEN,				    // ceFileTag
    FG_YELLOW | FG_HIGH | BG_GREEN,   			    // ceFileAttributes
    FG_WHITE | FG_HIGH | BG_GREEN,				    // ceFileSize
    FG_YELLOW | FG_HIGH | BG_GREEN,			    // ceFileDate
    FG_WHITE | FG_HIGH | BG_GREEN,				    // ceFileName
    FG_GREEN | FG_HIGH | BG_GREEN,				    // ceFileNameExt

    // ScrollDir file listing - under cursor!
    FG_GREEN | FG_HIGH | BG_YELLOW, 		    // ceFileTagCursor
    FG_YELLOW | FG_HIGH | BG_YELLOW,		    // ceFileAttributesCursor
    FG_WHITE | FG_HIGH | BG_YELLOW, 		    // ceFileSizeCursor
    FG_YELLOW | FG_HIGH | BG_YELLOW, 		    // ceFileDateCursor
    FG_WHITE | FG_HIGH | BG_YELLOW,  		    // ceFileNameCursor
    FG_GREEN | FG_HIGH | BG_YELLOW, 		    // ceFileNameExtCursor

    // TitleBar
    BG_WHITE | BG_HIGH,		    // ceTitleText
    FG_RED | BG_WHITE | BG_HIGH,		    // ceTitleTail

    // Popup Windows
    FG_WHITE | FG_HIGH | BG_RED,		    // cePopupBackground
    FG_WHITE | FG_HIGH,		    // cePopupBorder

    // Fields in Popup windows
    FG_WHITE | FG_HIGH | BG_RED,		    // ceField
    FG_YELLOW | FG_HIGH | BG_RED,		    // ceFieldKey
    FG_WHITE | FG_HIGH,				    // ceSelectedField
    FG_YELLOW | FG_HIGH,			    // ceSelectedKey

    // Edit Window
    FG_YELLOW | FG_HIGH,		    // ceEditWindow

    // CommandDir file listing
    FG_GREEN | FG_HIGH,  			    // ceCmdHeading
    FG_YELLOW | FG_HIGH,		    // ceCmdKey
    FG_WHITE | FG_HIGH,		    // ceCmdText

    // CommandDir file listing - under cursor!
    FG_GREEN | FG_HIGH | BG_GREEN,				    // ceCmdHeadingCursor
    FG_YELLOW | FG_HIGH | BG_GREEN,			    // ceCmdKeyCursor
    FG_WHITE | FG_HIGH | BG_GREEN,				    // ceCmdTextCursor

    // License Screen colors
    FG_WHITE | FG_HIGH | BG_GREEN,					    // ceLicenseNormal
    FG_YELLOW | FG_HIGH | BG_GREEN,			    // ceLicenseHighlight
    BG_WHITE | BG_HIGH,				    // ceLicenseNormalSel
    FG_GREEN | BG_WHITE | BG_HIGH,   		    // ceLicenseHighlightSel

    // Help Screen colors
    FG_WHITE | FG_HIGH | BG_GREEN,					    // ceHelpNormal
    FG_YELLOW | FG_HIGH | BG_GREEN,			    // ceHelpHighlight
    BG_WHITE | BG_HIGH,				    // ceHelpNormalSel
    FG_GREEN | BG_WHITE | BG_HIGH,   		    // ceHelpHighlightSel

    // Command text on user screen
    FG_GREEN | FG_HIGH,			    // ceCommandText - background always black
						    // because it's merged in later!

    // Verb Scroller
    FG_YELLOW | FG_HIGH,		    // ceVerb
    FG_YELLOW | FG_HIGH | BG_GREEN,     			    // ceVerbCursor

    // Assigned Commands Scroller
    FG_YELLOW | FG_HIGH, 		    // ceAssignKey
    FG_WHITE | FG_HIGH,    		    // ceAssignCommand
    FG_YELLOW | FG_HIGH | BG_GREEN,			    // ceAssignKeyCursor
    FG_WHITE | FG_HIGH | BG_GREEN,				    // ceAssignCommandCursor

    // Info Window
    FG_WHITE | FG_HIGH,		    // ceInfoNormal
    FG_GREEN | FG_HIGH,		    // ceInfoHighlight
    FG_WHITE | FG_HIGH | BG_YELLOW		    // ceInfoNormalSel
};


static WORD *colorTable =  NULL;	// causes it to be read from registry
static int curTable = 0;

static void assignColorTable (int table)
{
    if (table == 0)
	colorTable = defaultColorTable;
    else if (table == 1)
	colorTable = colorfulColorTable;
    else if (table == 2)
	colorTable = greyColorTable;
    else if (table == 3)
	colorTable = forestColorTable;
    else
	return;
    curTable = table;
}

int getColorTable ()
{
    return curTable;
}

void selectColorTable (int table)
{
    table = table % COLOR_TABLES;
    assignColorTable (table);

    // Save the choice in the registry
    if (table >= 0  &&  table < COLOR_TABLES) {
	try {
	    RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Color",
		KEY_READ | KEY_WRITE);
	    k.setValue ("Scheme", (DWORD) table);
	}
	catch (const std::exception&) {
	    // oh well, it's only the colors
	}
    }
}

WORD getColor (ColorEntry entry)
{
    if (colorTable == NULL) {
	// get the choice from the registry
	try {
	    RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_HERMIT "\\Color",
		KEY_READ);
	    DWORD type;
	    char *value = k.queryValue ("Scheme", type);
	    if (type == REG_DWORD  &&  value != 0)
		assignColorTable (*((DWORD *)value));
	    delete [] value;
	}
	catch (const std::exception&) {
	    // oh well, it's only the colors
	}

	if (colorTable == NULL)
	    colorTable = defaultColorTable;
    }

    if (entry >= ceFirstEntry  &&  entry < ceNumEntries)
	return colorTable[entry];
    else
	return FG_WHITE;
}

// End
