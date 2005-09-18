// HelpScreen class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "help.h"
#include "license.h"
#include "version.h"

#ifdef HERMIT_PRO
    #define HELP_CONSOLE_MSG  "HERMIT PRO Help"
#else
    #define HELP_CONSOLE_MSG  "HERMIT Help"
#endif
HelpScreen::HelpScreen () : mSM (mMsgs, mnMsgs)
{
    int w, h;	// screen size

    // SET CONSOLE TITLE TO OUR COPYRIGHT TITLE
    SetConsoleTitle (HELP_CONSOLE_MSG);

    // Handle escape key
    addHandler (KB_ESC, this, (Handler) &HelpScreen::handleKbEsc);
    addHandler (KB_SPACE, this, (Handler) &HelpScreen::handleKbDollar);
    addHandler (KB_ENTER, this, (Handler) &HelpScreen::handleKbDollar);
    addHandler ('$', this, (Handler) &HelpScreen::handleKbDollar);

    // Build the scroller
    getSize (w, h);
    mScroller = new Scroller (*this, 0, 0, w, h);
    if (mScroller == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mScroller->setScrollable (&mSM);
}

HelpScreen::~HelpScreen ()
{
}

void HelpScreen::handleKbEsc (const Event&)
{
    postEvent (Event (EV_QUIT));
}

void HelpScreen::handleKbDollar (const Event&)
{
    {
	LicenseScreen license;
	license.activate ();
	license.run ();
    }
    activate ();
    SetConsoleTitle (HELP_CONSOLE_MSG);
}

int HelpScreen::processEvent (const Event& event)
{
    // char str[80];
    // wsprintf (str, "my key = %d\n", key);
    // writeText (str);
    // return 1;
    return Screen::processEvent (event);
}

const MessageInfo HelpScreen::mMsgs[] = {	  // .........1.........2.........3.........4.........5.........6.........7.........8
						  // 12345678901234567890123456789012345678901234567890123456789012345678901234567890

    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
#ifdef HERMIT_PRO
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "                 \xD1   \xD1   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xB8 \xD5\xCD\xD1   \xCD\xD1\xCD   \xD5\xCD\xD1\xCD\xB8   PRO       \xDA\xBF" },
#else
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "                 \xD1   \xD1   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xCD\xCD\xB8   \xD1\xCD\xB8 \xD5\xCD\xD1   \xCD\xD1\xCD   \xD5\xCD\xD1\xCD\xB8             \xDA\xBF" },
#endif
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "            \xDA\xBF   \xB3   \xB3   \xB3       \xB3   \xB3   \xB3 \xB3 \xB3 \xB3    \xB3      \xB3          \xDA\xBF   \xC0\xD9" },
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "       \xDA\xBF   \xC0\xD9   \xC6\xCD\xCD\xCD\xB5   \xC6\xCD\xB5     \xC6\xCD\xCD\xD1\xBE   \xB3 \xD4\xCD\xBE \xB3    \xB3      \xB3     \xDA\xBF   \xC0\xD9" },
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "  \xDA\xBF   \xC0\xD9        \xB3   \xB3   \xB3       \xB3  \xC0\xBF   \xB3     \xB3    \xB3      \xB3     \xC0\xD9" },
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "  \xC0\xD9             \xCF   \xCF   \xCF\xCD\xCD\xCD\xBE   \xCF   \xCF   \xCF     \xCF   \xCD\xCF\xCD     \xCF" },
    { ceLicenseNormal,	    ceLicenseNormalSel,	    VER_PROGINFO },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
#ifdef HERMIT_PRO
    { ceHelpHighlight,	    ceHelpHighlightSel,	    " H E R M I T   P R O   H E L P" },
#else
    { ceHelpHighlight,	    ceHelpHighlightSel,	    " H E R M I T   H E L P" },
#endif
    { ceHelpNormal,	    ceHelpNormalSel,	    " Use the arrow and page keys to scroll the screen." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,	    ceHelpHighlightSel,	    " *** Press the space bar to see the licensing screen! ***" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },

// Inser from hermit.txt

    { ceHelpHighlight,  ceHelpHighlightSel,	    " INTRODUCTION" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " HERMIT is a great program for people who spend a lot of time in a console" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " based shell, like command.com under Windows98, cmd.exe under Windows 2000," },
    { ceHelpNormal,	    ceHelpNormalSel,	    " or other shells that run on Win32 like the Korn shell or bash." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " HERMIT makes it easy to move around directories, move back-and-forth" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " between them, and jump around to lots of places. HERMIT constantly" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " displays the directory listing for the directory that you are in." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " In addition, HERMIT contains powerful text substitution that makes it" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " easy to build command lines to operate on multiple files at the touch of" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " a single key. For example, I can bring up a text editor like VIM by" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " \"tagging\" the files in the directory listing and pressing \"V\". A command" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " like \"VIM !q\" could be associated with \"V\", in which the \"!q\" is" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " replaced with all the files I selected." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " HERMIT is a small, easy to learn, and consistent interface. It's like being" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " in your shell, except that you can always see your directory listing and" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " command output, and you can easily build powerful commands with minimal" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " typing." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " This document assumes you have some familiarity with MS-DOS shells and how" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " commands are executed within them." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,  ceHelpHighlightSel,	    " GENERAL USER INTERFACE INFORMATION" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " The HERMIT user interface runs in a command shell under Windows95 and" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Windows NT. The user interface displays a directory listing that you can" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " scroll through, a title bar that shows the current directory and file" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " filtering information, and a status bar that shows additional file" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " information, error and information messages, and keyboard hints." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " The keyboard usage in HERMIT is very simple. Almost all HERMIT commands are" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " accessed by holding the Alt key and pressing a letter or number. The" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " function keys (F1 to F12) serve as shortcuts for many of the commands. Some" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " commands have single character shortcuts that may be more intuitive to some" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " users. The command set was designed to be \"Brief-like,\" which doesn't mean" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " too much because HERMIT is not a text editor." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Pressing the Ctrl (Control) key and a letter or number is used to jump to" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " another directory location.  Letters are used to jump to new drives. Numbers" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " are reserved for bookmarks (saved directory locations)." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Pressing a single letter or digit executes a saved command. HERMIT has" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " highly customizable ways of formulating a command line based on what files" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " are selected. This allows you to save a lot of typing. HERMIT can run a" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " command many times for different files, or pass a list of the selected files" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " to a single command. The section on custom commands describes this" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " capability in more detail." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " The HERMIT user interface consists of dialogs, lists, text-entry fields, and" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " radio options. Only lists can be controlled by the mouse. All user interface" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " elements are controlled by standard, intuitive commands.  Arrow keys are" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " used to move around and scroll lists (in addition to Home, End, PgDn, and" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " PgUp). Enter and Esc are used to confirm or cancel dialogs. Text-entry" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " fields also utilize Backspace and the up-arrow to get a history of what you" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " have typed before." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Radio options appear in dialogs and are simple words presented with a" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " highlighted character that is used to switch the option on. The currently" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " set option presents the entire word in a different color." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " I don't suspect these basic user interface commands will pose any problem" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " for users. However, since HERMIT is a console program instead of a graphical" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " user interface, it may be difficult for novice users to recognize the user" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " interface elements that appear on the screen. In these cases, look for" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " different colors and try using single letters to turn on options and arrow" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " keys to make things move.  In some lists that are presented, pressing Enter" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " brings up a sub-dialog for the selected list item." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,  ceHelpHighlightSel,	    " KEYBOARD SUMMARY" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " This section presents an overview of all the HERMIT commands. Unusual" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " capatilization (like seTup) is used to help you mentally associate" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " a word with a command (like Alt-T)." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   General:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     F1                  displays help." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-C or F2         displays the Command dialog, which lists all" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         commands and a brief description for each." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     $                   displays the licen$ing screen." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-T               displays the seTup options. These options allow" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         you to set the color scheme, import and export" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         your custom commands, and register or unregister" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         Component Object Model DLL servers." },
    { ceHelpNormal,	    ceHelpNormalSel,	    " " },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   Switching Directories:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Ctl-A to Ctl-Z      go to the directory indicated by the letter pressed." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-G or F6         Goto a new directory (prompts you to type it in)." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-V or F4         go back to the preVious directory." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Left Arrow or Esc   change to parent directory." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Right Arrow         change to subdirectory that is highlighted by the" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         cursor." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-H or ~          switch to Home directory (indicated by HERMIT_HOME" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         or HOME environment variables). ~ always goes to the" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                         My Documents directory." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   Bookmarks:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Ctl-0 to Ctl-9      change to directory set in the chosen bookmark." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-0 to Alt-9      assign the current directory to the specified bookmar" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-B               display the Bookmarks and even pick one." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   Directory Listing:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-F           Filter the file listing with Windows-style wildcard" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     characters \"*\" and \"?\"." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-S           displays the Sort dialog to specify how to sort the" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     directory listing." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-R or F5     Refresh the directory listing." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Up Arrow        move the cursor up." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Down Arrow      move the cursor down." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Home            move the cursor to the first item." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     End             move the cursor to the last item." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     PgUp            move the cursor up one page." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     PgDn            move the cursor down one page." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   Tagging Files" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     +               tag the file under the cursor." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     -               un-tag the file under the cursor." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Spacebar        alternate the tagged status of the file under the" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     cursor." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-Plus        tag all files in the directory." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-Minus       un-tag all files in the directory." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-E           rE-tag files that were tagged before a command was" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     executed on them. (Executing a command on a tagged" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     file puts it into a \"sort of tagged\" state; using" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     Alt-E tags all of the files in this state.)" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   Shells and Commands" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Tab or Backtab  display extended information about the currently" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     tagged files." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-U           show the User screen. The user screen is the screen" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     where the output of your commands goes." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-Z           launch the shell specified by the HERMIT_SHELL, SHELL," },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     or COMSPEC environment variables." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Enter           run associated program command. This prompts you for" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     the \"actions\" Windows knows about for the given file" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     type like \"open\" or \"edit\", and allows you to choose" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     one." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-O           Open the file via a Windows association." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-P           Print the file via a Windows association." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-N or F3     ruN a command (prompts you to type it in)." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-A           Assign custom commands to keys." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-Y           display a list of Your custom commands to choose from." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-M           run your custom commands on Multiple files." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-L           set an environment variabLe. Also allows you to unset" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "                     one." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-I           display sytem and disk Information." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   Exiting the Program:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Alt-X           eXit." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,  ceHelpHighlightSel,	    " MOUSE SUMMARY" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Left button click - puts the curson on the file in the list" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Right button click - move up a directory level" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Left button double-click - on a directory, changes to that directory." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "         on a file, launches the associated application" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Shift Left button click - selects file" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Ctrl Left button click - selects or deselects a file" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Left button click in title bar - move cursor to top" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Right button click in title bar - move cursor to bottom" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Left button click in status bar - move cursor to bottom" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "     Right button click in status bar - move cursor to top" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,  ceHelpHighlightSel,	    " RUNNING COMMANDS" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Commands can be entered manually by typing Alt-N or assigned directly to" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " letter or number keys. Pressing a single key can execute commands on a large" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " group of files that you have selected, or on the currently highlighted file." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " There are several special character sequences which, when inserted into your" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " commands, are replaced with information about the tagged files." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !f  Add selected file name (file.ext)" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       When a !f is encountered the current file name is inserted into" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       the command. For instance, the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !f" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might be expanded to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo autoexec.bat" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !d  Add current directory (c:\\dir\\subdir)" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       When a !d is encountered the current directory path is inserted" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       into the command. For instance, the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !d" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might be expanded to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo c:\\" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !b  Add base file name without extension (file)" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       When a !b is encountered, the current file's base name is inserted" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       without the extension. For instance, the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !b" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might be expanded to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo autoexec" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !v  Add the drive leter (c:)" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       When a !v is encountered, the drive letter or first part of a UNC" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       name is inserted. For example, the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !v" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might be expanded to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo c:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       or:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo \\\\kiki\\d$" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !t  Add the path only (\\dir\\subdir)" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       When a !t is encountered, the path without a drive letter or UNC" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       name is inserted. For example, the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !t" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might be expanded to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo \\dev\\hermit" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
	{ ceHelpNormal,	    ceHelpNormalSel,	    "   !p  Prompt me for text at this location" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       When a !p is encountered, HERMIT will prompt you to enter text that" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       is inserted into the command. This is really useful for entering" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       command switches, file names, and so forth. It can also be used" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       as a \"are you sure\" mechanism. On all of my \"dangerous\" commands," },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       I place a !p so that executing the command by accident would still" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       require me to press return to continue." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !m  Insert the names of all the tagged files here" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       Many commands can operate on multiple files. When a !m is" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       encountered, all selected files are inserted, separated by a space." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       For instance, if two files are selected then the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !m" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might expand to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo autoexec.bat config.sys" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !q  Insert the names of all the tagged files here, each in quotes" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       The !q sequence has the same behavior as !m, except that quotes" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       are placed around the command. For instance, the command:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo !q" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       might expand to:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "           echo \"autoexec.bat\" \"config.sys\"" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       This is useful when long file names containing spaces are used." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !i  Return immediately after executing the command" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       A !i does not expand to anything but instead causes HERMIT to" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       return to the file listing immediately after executing the command," },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       instead of waiting for you to press a key." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !a  Refresh the file listing automatically after executing the command" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       A !a sequence does not expand to anything but instead causes HERMIT" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       to refresh the file listing when it is next drawn. This is useful" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       for getting the new file listing after running a command that adds," },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       deletes, or renames files." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   !!  Insert single !" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       If you need a ! character, use two instead so that the parser" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "       will not try to insert anything." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Here is an advanced example of a command that works nice on" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Windows 2000:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   echo off && for %f in (!q) do (echo %f && attrib !p %f)!i!a" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " This command is used to change the attributes of all of the selected" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " files, or the file under the cursor if none are selected. It prompts" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " for the arguments to attrib, which could be -r to make the files" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " writable, and echos the name of each file as it is changed.  The" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " command returns immediately because of the !i and also refreshes the" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " file listing because of the !a (which allows you to see the changed" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " attributes)." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " If you assign this command to the letter A, then you can simply mark" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " all the files that you want to change, press A, type -r or +r, and" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " your attributes will be changed. That sure beats typing:" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "   attrib -r myfile.txt" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " for every file you need to change!" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,  ceHelpHighlightSel,	    " OTHER INFORMATION" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " The best way to get to know HERMIT is to try it out. Most of the things" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " HERMIT does are harmless, but you should experiment with custom commands" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " using commands like ECHO when you are testing things out. This is especially" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " true when building complex scripted commands for deletion and other" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " operations that you may not be able to undo." },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Have fun," },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpNormal,	    ceHelpNormalSel,	    " Michael Lore" },

// End from hermit.txt

    { ceHelpNormal,	    ceHelpNormalSel,	    "" },
    { ceHelpHighlight,	    ceHelpHighlightSel,	    "                     *** PRESS ESC TO RETURN TO HERMIT ***" },
    { ceHelpNormal,	    ceHelpNormalSel,	    "" }
};

int HelpScreen::mnMsgs = sizeof (mMsgs) / sizeof (MessageInfo);

// End
