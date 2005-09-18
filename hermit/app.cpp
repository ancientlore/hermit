// Main Hermit application file
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

// todo: [X] Shift-tab size bug
// todo: [X] parameters scroll into view for !p
// todo: [X] some key should always go to windows home dir
// todo: [X] shift also view file info
// todo: [X] list streams in file, FindFirstStreamW
// todo: [ ] SetFileApisToANSI, SetConsoleCP, SetConsoleOutputCP
// todo: [ ] create hard link
// todo: [ ] process viewer

#include "app.h"
#include "cmddlg.h"
#include "rundlg.h"
#include "license.h"
#include "help.h"
#include "verbdlg.h"
#include "asgndlg.h"
#include "pickcmddlg.h"
#include "registry.h"
#include "ptr.h"
#include "promptdlg.h"
#include "console/inputdlg.h"
#include "console/askdlg.h"
#include "waitfix.h"
#include "cfgdlg.h"


#define HKCU_SUBKEY_COMMANDS	HKCU_SUBKEY_HERMIT "\\Commands"

#define COMMAND_SIZE	    1024
#define ENV_VAR_NAME_SIZE   80
#define ENV_VAR_VALUE_SIZE  2048

inline int ispcmd (const char *p, char c)
{
    if (p[0] == '!'  &&  (p[1] == tolower (c)  ||  p[1] == toupper (c)))
	return 1;
    else
	return 0;
}

Application * Application::theApp = 0;
volatile int Application::breakLoop = 0;

BOOL Application::CtrlCHandler (DWORD dwCtrlType)
{
    if (dwCtrlType == CTRL_C_EVENT) {	    // Ignore it
	if (Application::theApp != 0)
	    Application::theApp->postEvent (Event (KB_CTRL_C), 1);
	else
	    breakLoop = 1;
	return TRUE;
    }
    else if (dwCtrlType == CTRL_BREAK_EVENT) {	// ignore these
	if (Application::theApp != 0)
	    Application::theApp->postEvent (Event (EV_STATUS_MSG, (DWORD) "Use Alt-X to quit the program"), 1);
	else
	    breakLoop = 1;
	return TRUE;
    }
    else
	return FALSE;
}

Application::Application () : mHist ("Command History"), mParamHist ("Parameter History"),
		mEnvVarNameHist ("Env Var History"), mEnvVarValueHist ("Env Value History")
{
    int w, h;	// screen size

    // Set the global app pointer
    theApp = this;

    // SET CONSOLE TITLE TO OUR COPYRIGHT TITLE
    SetConsoleTitle (getTitle ());

    // Initialize the mHwnd
    initializeHWND ();

    // Set Ctrl-C handler
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) Application::CtrlCHandler, TRUE) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleCtrlHandler", GetLastError ());

    // REMEMBER THE CURRENT CONSOLE
    mPrevOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    if (mPrevOutput == INVALID_HANDLE_VALUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "GetStdHandle", GetLastError ());

    // Add a handler for quit
    addHandler (KB_ALT_X, this, (Handler) &Application::handleKbAltX);
    addHandler (KB_F1, this, (Handler) &Application::handleKbF1);
    addHandler (KB_ALT_C, this, (Handler) &Application::handleKbAltC);
    addHandler (KB_F2, this, (Handler) &Application::handleKbAltC);
    addHandler (KB_ALT_U, this, (Handler) &Application::handleKbAltU);
    addHandler (KB_ALT_Z, this, (Handler) &Application::handleKbAltZ);
    addHandler (KB_ALT_A, this, (Handler) &Application::handleKbAltA);
    addHandler (KB_ALT_Y, this, (Handler) &Application::handleKbAltY);
    addHandler (KB_ALT_M, this, (Handler) &Application::handleKbAltM);
    addHandler ('$', this, (Handler) &Application::handleKbDollar);
#if 0
    addHandler (KB_TAB, this, (Handler) &Application::handleKbRight);
#endif
    addHandler (KB_ALT_L, this, (Handler) &Application::handleKbAltL);
    addHandler (KB_ALT_I, this, (Handler) &Application::handleKbAltI);
    addHandler (KB_ALT_T, this, (Handler) &Application::handleKbAltT);
    addHandler (KB_BACKTAB, this, (Handler) &Application::handleKbBackTab);
    addHandler (KB_TAB, this, (Handler) &Application::handleKbBackTab);

    // setColors (BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    mTitleBar = new TitleBar (*this);
    if (mTitleBar == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    mStatusBar = new Status (*this);
    if (mStatusBar == 0)
    {
        delete mTitleBar;
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    }
    getSize (w, h);
    mScroller = new FileScroller (*this, 0, 1, w, h - 2);
    if (mScroller == 0) {
        delete mStatusBar;
        delete mTitleBar;
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    }
    mStatusBar->setStatusTail ("F1:Help F2:Cmds");
}

Application::~Application ()
{
    // wipeout global app pointer
    theApp = 0;

    // RESTORE THE OLD SCREEN BUFFER!
    SetConsoleActiveScreenBuffer (mPrevOutput);

    // Remove Ctrl-C handler (I don't think this is strictly necessary)
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) Application::CtrlCHandler, FALSE) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleCtrlHandler", GetLastError ());
}

void Application::handleKbAltX (const Event&)
{
    postEvent (Event (EV_QUIT));
}

void Application::handleKbF1 (const Event&)
{
    {
	HelpScreen help;
	help.activate ();
	help.run ();
    }
    activate ();
    SetConsoleTitle (getTitle ());
}

void Application::handleKbAltC (const Event&)
{
    int exitCode = KB_ESC;
    {
	CommandDialog dlg (*this);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC)
	postEvent (Event (exitCode));
}

void Application::handleKbAltA (const Event&)
{
    AssignCommandsDialog dlg (*this);
    dlg.run ();
}

void Application::handleKbAltT (const Event& /*event*/)
{
    ConfigDialog dlg (*this);
    dlg.run ();
    postEvent (Event (EV_COLOR_CHANGE));    // redraw where dialog was
}

void Application::handleKbAltY (const Event&)
{
    int exitCode = KB_ESC;
    {
	PickCommandsDialog dlg (*this);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC)
	postEvent (Event (EV_RUN_SINGLE_COMMAND, exitCode));
}

void Application::handleKbAltM (const Event&)
{
    int exitCode = KB_ESC;
    {
	PickCommandsDialog dlg (*this, 1);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC)
	postEvent (Event (EV_RUN_MULTI_COMMAND, exitCode));
}

void Application::handleKbAltU (const Event&)
{
    Event e;
    if (SetConsoleActiveScreenBuffer (mPrevOutput) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleActiveScreenBuffer", GetLastError ());
    do {
	getKey (e);
    }
    while (e.key != KB_SPACE  &&  e.key != KB_ENTER  &&  e.key != KB_ESC);
    activate ();
    SetConsoleTitle (getTitle ());
}

void Application::handleKbAltL (const Event&)
{
    int exitCode = KB_ESC;
    char var[ENV_VAR_NAME_SIZE];
    ArrayPtr<char> value = new char [ENV_VAR_VALUE_SIZE];
    if (value == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    char str[ENV_VAR_NAME_SIZE + 48];  // must be able to hold env var name and some more
    *var = '\0';
    *value = '\0';
    {
	InputDialog dlg (*this, "Set Environment Variable", "Enter the variable name:",
			 var, ENV_VAR_NAME_SIZE - 1, &mEnvVarNameHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC) {
	if (GetEnvironmentVariable (var, value, ENV_VAR_VALUE_SIZE) <= 0)
	    *value = '\0';
	wsprintf (str, "Enter the value for %s:", var);
	InputDialog dlg (*this, "Set Environment Variable Value", str,
			 value, ENV_VAR_VALUE_SIZE - 1, &mEnvVarValueHist);
	dlg.run ();
	exitCode = dlg.getExitCode ();
    }
    if (exitCode != KB_ESC) {
	int delCode = KB_ESC;
	if (*value == '\0') {
	    {
	    wsprintf (str, "You entered a blank value. Would you like to delete %s?", var);
	    AskDialog d (*this, "Environment Variable", str);
	    d.run ();
	    delCode = d.getExitCode ();
	    }
	    if (delCode != KB_ESC) {
		if (SetEnvironmentVariable (var, NULL) != TRUE)
		    sendWinErrStatusMsg ("Error while deleting environment variable.");
		else
		    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Environment variable deleted."));
		return;
	    }
	}

	if (SetEnvironmentVariable (var, value) != TRUE)
	    sendWinErrStatusMsg ("Error while setting environment variable.");
	else
	    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Environment variable assigned."));
    }
}

#if 0
void Application::handleKbRight (const Event& event)
{
    try {
	File f = mScroller->getCurrentFile ();
	if (f.getAttributes () & FILE_ATTRIBUTE_DIRECTORY) {
	    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Cannot view directories as files."));
	}
	else {
	    try {
		ViewerScreen viewer (CLSID_HermitScrollable, "Hermit.DefaultViewer.1",
				     f.getName ());
		viewer.activate ();
		viewer.run ();
	    }
	    catch (const std::exception& info) {
		errorFreezer a;
		sendEvent (Event (EV_STATUS_MSG, (DWORD) info.errMsg));
	    }
	}
    }
    catch (const std::exception& info) {
        if (info.errCode != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
    	    throw;
    }

    activate ();
    SetConsoleTitle (getTitle ());
}
#endif

const char *Application::getShell () const
{
    OSVERSIONINFO info;
    info.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    info.dwBuildNumber = 0;
    GetVersionEx (&info);
    if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
        return "cmd.exe";
    else // VER_PLATFORM_WIN32_WINDOWS or VER_PLATFORM_WIN32s
        return "command.com";
}

void Application::handleKbAltZ (const Event&)
{
    PROCESS_INFORMATION info;
    STARTUPINFO start;
    char buffer[256];

    // Get the shell to run
    if (GetEnvironmentVariable ("HERMIT_SHELL", buffer, 256) <= 0)
	if (GetEnvironmentVariable ("SHELL", buffer, 256) <= 0)
	    if (GetEnvironmentVariable ("COMSPEC", buffer, 256) <= 0)
    		strcpy (buffer, getShell ());

    // Init structures
    memset (&info, 0, sizeof (PROCESS_INFORMATION));
    memset (&start, 0, sizeof (STARTUPINFO));
    start.cb = sizeof (STARTUPINFO);
    start.lpTitle = buffer;
    start.dwFlags = STARTF_USESTDHANDLES;
    start.hStdInput = GetStdHandle (STD_INPUT_HANDLE);
    start.hStdOutput = mPrevOutput;
    start.hStdError = GetStdHandle (STD_ERROR_HANDLE);

    // Swap screen buffers
    if (SetConsoleActiveScreenBuffer (mPrevOutput) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleActiveScreenBuffer", GetLastError ());

    // Set new title
    SetConsoleTitle (buffer);

    // Write command
    writeCommand (buffer);

    // Create the process
    theApp = 0;
    breakLoop = 0;
    InputModeSwitcher switcher (*this, mModeSwitcher.getOldMode ());
    if (CreateProcess (0, buffer, 0, 0, TRUE, CREATE_NEW_PROCESS_GROUP | NORMAL_PRIORITY_CLASS,
		       0, 0, &start, &info) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "CreateProcess", GetLastError ());

    // Wait for it to end
    if (WaitWithMessageLoop (info.hProcess) != TRUE) {
	CloseHandle (info.hThread);
	CloseHandle (info.hProcess);
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WaitWithMessageLoop", GetLastError ());
    }
    theApp = this;

    CloseHandle (info.hThread);
    CloseHandle (info.hProcess);

    // Write message and wait for key
    // waitKeyMsg ();	    // naaahhh...this is annoying after typing 'exit'

    // Restore where we were
    activate ();
    SetConsoleTitle (getTitle ());
    postEvent (Event (EV_STATUS_MSG, (DWORD) "You may need to refresh the file listing by pressing F5."));
}

void Application::waitKeyMsg ()
{
    Event e;
    int w, h;
    COORD where;
    DWORD written;
    CHAR_INFO buf[29];
    SMALL_RECT r;

    getSize (w, h);
    where.X = 0;
    where.Y = h - 1;
    r.Top = r.Bottom = h - 1;
    r.Left = 0;
    r.Right = 28;
    COORD size;
    size.X = 29;
    size.Y = 1;
    COORD orig;
    orig.X = 0;
    orig.Y = 0;

    if (ReadConsoleOutput (mPrevOutput, buf, size, orig, &r) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "ReadConsoleOutput", GetLastError ());
    if (WriteConsoleOutputCharacter (mPrevOutput, "Press Esc to return to HERMIT",
				     29, where, &written) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsoleOutputCharacter", GetLastError ());
    if (FillConsoleOutputAttribute (mPrevOutput, getColor (ceStatusText), 29, where, &written) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "FillConsoleOutputAttribute", GetLastError ());
    do {
	getKey (e);
    }
    while (e.key != KB_SPACE  &&  e.key != KB_ENTER  &&  e.key != KB_ESC);
    if (WriteConsoleOutput (mPrevOutput, buf, size, orig, &r) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsoleOutput", GetLastError ());
}

void Application::handleKbDollar (const Event& /*event*/)
{
    {
	LicenseScreen license;
	license.activate ();
	license.run ();
    }
    activate ();
    SetConsoleTitle (getTitle ());
}

int Application::processEvent (const Event& event)
{
#if 0
    char str[80];
    wsprintf (str, "my key = %d\n", event.key);
    writeText (str);
#endif

    CoFreeUnusedLibraries();

    // Look for user-defined commands
    if ((event.key >= 'A'  &&  event.key <= 'Z')  ||
	(event.key >= 'a'  &&  event.key <= 'z')  ||
	(event.key >= '0'  &&  event.key <= '9')) {
	postEvent (Event (EV_RUN_SINGLE_COMMAND, event.key));
    }

    switch (event.key) {

    case KB_ENTER:
	runVerb ();
	break;

    case KB_ALT_O:
	enter ();
	break;

    case KB_ALT_P:
	print ();
	break;

    case KB_ALT_N:
    case KB_F3:
	runCmd ();
	break;

    case EV_RUN_SINGLE_COMMAND:
	runSavedCmd (event.data, 0);
	break;

    case EV_RUN_MULTI_COMMAND:
	runSavedCmd (event.data, 1);
	break;

    default:
        return Screen::processEvent (event);
    }

    return 1;
}

void Application::runSavedCmd (int key, int multi)
{
    // translate key
    if (key >= 'a'  &&  key <= 'z')
	key = key - 'a' + 'A';

    ArrayPtr<char> str = new char [COMMAND_SIZE];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    str[0] = '\0';
    char s[2];
    s[0] = key;
    s[1] = '\0';

    try {
	// Open registry key
	RegistryKey k (HKEY_CURRENT_USER, HKCU_SUBKEY_COMMANDS, KEY_READ);

	// try to read the current value
	char *value = 0;
	DWORD type;
	try {
	    value = k.queryValue (s, type);
	    if (type == REG_SZ  &&  value != 0  &&  value[0] != '\0') {
		strncpy (str, value, COMMAND_SIZE - 1);
		str[COMMAND_SIZE - 1] = '\0';
	    }
	    delete [] value;
	}
	catch (const std::exception&) {
	    // no key assigned
	}
    }
    catch (const std::exception&) {
	// no key assigned
    }

    if (str[0] != '\0')
	runCmd (str, multi);
}

void Application::runCmd (const char *cmd, int multi)
{
    ArrayPtr<char> buffer = new char [COMMAND_SIZE];
    if (buffer == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    int lCmd;
    int hmtShell = 0;

    // Get the shell to run
    if (GetEnvironmentVariable ("HERMIT_COMSPEC", buffer, COMMAND_SIZE - 100) <= 0) {
	if (GetEnvironmentVariable ("COMSPEC", buffer, COMMAND_SIZE - 100) <= 0)	// smaller so we can add the command!
	    strcpy (buffer, getShell ());
    }
    else
	hmtShell = 1;

    // Get the command to run
    int exitCode = KB_ESC;
    {
	char tempbuf[10];

	if (hmtShell)
	    strcat (buffer, " ");
	else
	    strcat (buffer, " /c ");	// HERMIT_COMPSEC has its own /c
        if (GetEnvironmentVariable ("HERMIT_QUOTE", tempbuf, 10) > 0)
	    strcat (buffer, tempbuf);	// add leading quote around command
	lCmd = (int) strlen (buffer);
	if (cmd == 0) {
	    RunDialog runDlg (*this, &buffer[lCmd], COMMAND_SIZE - 1 - lCmd, &mHist);
	    runDlg.run ();
	    exitCode = runDlg.getExitCode ();
	}
	else {
	    if (strlen (cmd) > (unsigned) COMMAND_SIZE - 1 - lCmd)
		postEvent (Event (EV_STATUS_MSG, (DWORD) "The command string is too long!"));
	    else {
		strncpy (&buffer[lCmd], cmd, COMMAND_SIZE - 1 - lCmd);
		buffer[COMMAND_SIZE - 1] = '\0';
		exitCode = KB_ENTER;
		if (multi)
		    exitCode = KB_CTRL_ENTER;
	    }
	}
    }
    if (exitCode != KB_ENTER  &&  exitCode != KB_CTRL_ENTER)
	return;

    // Translate prompted entries in the command buffer
    int multiFiles = 0;
    int noWaitMsg = 0;
    int autoRefresh = 0;
    ArrayPtr<char> cmdBuf = getBufferWithPrompts (buffer, lCmd, multiFiles, noWaitMsg, autoRefresh);
    if (cmdBuf == 0)
	return;	    // cancel or error
    if (multiFiles  &&  exitCode == KB_CTRL_ENTER) {
	sendEvent (Event (EV_STATUS_MSG, (DWORD) "Cannot run multi-file command on multiple files."));
	return;
    }

    // Swap screen buffers
    if (SetConsoleActiveScreenBuffer (mPrevOutput) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleActiveScreenBuffer", GetLastError ());

    theApp = 0;
    breakLoop = 0;
    ArrayPtr<char> curDir = getProcessedDirectory ();
    if (exitCode == KB_ENTER) { // single
	try {
	    runCommand (curDir, mScroller->getCurrentFile (), cmdBuf, lCmd, multiFiles);
	}
	catch (const AppException& info) {
	    if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
		throw;
	}
    }
    else {  // Multiple
	FileIterator *iter = mScroller->getIterator ();
	while (!iter->done ()) {
	    if (breakLoop) {
		sendEvent (Event (EV_STATUS_MSG, (DWORD) "Multiple command run stopped"));
		break;
	    }
	    File& file = *iter;
	    if (file.isTagged ())
		runCommand (curDir, file, cmdBuf, lCmd, 0);
	    (*iter)++;
	}
	delete iter;
    }
    // delete [] cmdBuf;
    theApp = this;

    // Write message and wait for key
    if (!noWaitMsg  ||  breakLoop)
	waitKeyMsg ();

    // Restore where we were
    activate ();
    SetConsoleTitle (getTitle ());
    mScroller->sendEvent (Event (EV_REDRAW, 0));
    if (autoRefresh)
	postEvent (Event (KB_F5));
    else if (!breakLoop)
	postEvent (Event (EV_STATUS_MSG, (DWORD) "You may need to refresh the file listing by pressing F5."));
}

char *Application::getProcessedDirectory ()
{
    const char *curDir = mScroller->getDirName ();

    int len = (int) strlen (curDir);	    // length of current directory
    int sepLen = 1;		    // Length of separator
    int sepDef = 0;		    // Whether a separator is defined
    char tmpbuf[10];		    // buffer for env var

    if (GetEnvironmentVariable ("HERMIT_DIRSEP", tmpbuf, 10) > 0) {
		sepLen = (int) strlen (tmpbuf);
		if (sepLen < 1) {
			strcpy(tmpbuf, "\\");
			sepLen = 1;
		}
		sepDef = 1;
    }

    int sepCount = 0;
    for (int i = 0; i < len; i++) {
	if (curDir[i] == '\\')
	    sepCount++;
    }

    char *buffer = new char [len + sepCount * (sepLen - 1) + 1];  // alloc for more than 1 sep
    if (buffer == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    // Formulate dirname
    int j = 0;
    for (int i = 0; i < len; i++) {
	if (curDir[i] == '\\'  &&  sepDef == 1) {
	    strcpy (&buffer[j], tmpbuf);
	    j += sepLen;
	}
	else {
	    buffer[j] = curDir[i];
	    j++;
	    buffer[j] = '\0';
	}
    }

    return buffer;
}

char *Application::getBufferWithPrompts (const char *cmdBuf, int lCmd, int& multiFiles, int& noWaitMsg, int& autoRefresh)
{
    // int cmdSize = 0;
    ArrayPtr<char> prompts[10];
    int size = 0;
    int len = (int) strlen (cmdBuf);

    multiFiles = 0;
    noWaitMsg = 0;
    autoRefresh = 0;

    // Prompt for each one
    int k = 0;
    int m = 0;
    for (int i = 0; i < len; i++) {
	if (ispcmd (&cmdBuf[i], 'p')) {
	    if (k >= 10) {
		postEvent (Event (EV_STATUS_MSG, (DWORD) "You may only have 10 prompts."));
		return 0;
	    }

	    prompts[k] = new char[128];
	    if ((char *) prompts[k] == 0)
		throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	    prompts[k][0] = '\0';

	    // Run dialog

	    int exitCode = KB_ESC;
	    {
		PromptDialog dlg (*this, &cmdBuf[lCmd], i - lCmd, prompts[k], 127, &mParamHist);
		dlg.run ();
		exitCode = dlg.getExitCode ();
	    }
	    if (exitCode == KB_ESC) {
		postEvent (Event (EV_STATUS_MSG, (DWORD) "Operation cancelled."));
		return 0;
	    }

	    size += (int) strlen (prompts[k]);
	    k++;
	    i++;    // skip second char
	}
	else if (ispcmd (&cmdBuf[i], 'm')  ||  ispcmd (&cmdBuf[i], 'q')) {
	    if (m >= 1) {
		postEvent (Event (EV_STATUS_MSG, (DWORD) "You may only insert all tagged files once."));
		return 0;
	    }
	    m++;
	    multiFiles = 1;
	    // See how big to make the string (a whopper!)
	    int tagged = 0;
	    FileIterator *iter = mScroller->getIterator ();
	    while (!iter->done ()) {
		File& file = *iter;
		if (file.isTagged ()) {
		    size += (int) strlen (file.getName ()) + (ispcmd (&cmdBuf[i],'q') ? 3 : 1);
		    tagged = 1;
		}
		(*iter)++;
	    }
	    delete iter;

	    // if no files tagged, use current
	    if (tagged == 0) {
		try {
		    size += (int) strlen (mScroller->getCurrentFile ().getName ()) + (cmdBuf[i] == '\x06' ? 3 : 1);
		}
		catch (const AppException& info) {
		    if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
			throw;
		    else {
			postEvent (Event (EV_STATUS_MSG, (DWORD) "No file to insert."));
			return 0;
		    }
		}
	    }
	    i++;    // skip second char
	}
    }

    ArrayPtr<char> buffer = new char [len + 1 + size + 1];
    if (buffer == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    *buffer = '\0';

    // Formulate real command
    k = 0;
    int j = 0;
    for (int i = 0; i < len; i++) {
	if (ispcmd (&cmdBuf[i], 'p')) {
	    strcpy (&buffer[j], prompts[k]);
	    j += (int) strlen (prompts[k]);
	    k++;
	    i++;
	}
	else if (ispcmd (&cmdBuf[i], 'i')) {
	    noWaitMsg = 1;
	    i++;
	}
	else if (ispcmd (&cmdBuf[i], 'a')) {
	    autoRefresh = 1;
	    i++;
	}
	else if (ispcmd (&cmdBuf[i], 'm')  ||  ispcmd (&cmdBuf[i], 'q')) {
	    // insert all tagged files
	    int tagged = 0;
	    FileIterator *iter = mScroller->getIterator ();
	    while (!iter->done ()) {
		File& file = *iter;
		if (file.isTagged ()) {
		    if (ispcmd (&cmdBuf[i], 'q')) {
			strcpy (&buffer[j], "\"");
			j++;
		    }
		    strcpy (&buffer[j], file.getName ());
		    j += (int) strlen (file.getName ());
		    if (ispcmd (&cmdBuf[i], 'q')) {
			strcpy (&buffer[j], "\"");
			j++;
		    }
		    strcpy (&buffer[j], " ");
		    j++;
		    tagged = 1;
		}
		(*iter)++;
	    }
	    delete iter;

	    // if no files tagged, use current
	    if (tagged == 0) {
		try {
		    if (ispcmd (&cmdBuf[i], 'q')) {
			strcpy (&buffer[j], "\"");
			j++;
		    }
		    strcpy (&buffer[j], mScroller->getCurrentFile ().getName ());
		    j += (int) strlen (mScroller->getCurrentFile ().getName ());
		    if (ispcmd (&cmdBuf[i], 'q')) {
			strcpy (&buffer[j], "\"");
			j++;
		    }
		    strcpy (&buffer[j], " ");
		    j++;
		}
		catch (const AppException& info) {
		    if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
			throw;
		    else {
			postEvent (Event (EV_STATUS_MSG, (DWORD) "No file to insert."));
			return 0;
		    }
		}
	    }
	    i++;
	}
	else {
	    buffer[j] = cmdBuf[i];
	    j++;
	    buffer[j] = '\0';
	}
    }

    return buffer.detach ();
}

// returns pointer to path part (beyond c:)
const char* splitDrivePath(const char *curDir, int& driveLen, int& pathLen)
{
	// first look for :
	const char* pathPtr = strchr(curDir, ':');
	if (pathPtr != 0) {
		pathPtr++;
		driveLen = pathPtr - curDir;
		pathLen = (int) strlen(pathPtr);
	}
	else {
		// look for UNC names
		int sepLen = 1;		    // Length of separator
		char tmpbuf[10];		    // buffer for env var
		if (GetEnvironmentVariable ("HERMIT_DIRSEP", tmpbuf, 10) > 0) {
			sepLen = (int) strlen (tmpbuf);
			if (sepLen < 1) {
				strcpy(tmpbuf, "\\");
				sepLen = 1;
			}
		}
		else {
			strcpy(tmpbuf, "\\");
			sepLen = 1;
		}

		if ((int) strlen(curDir) > 2 * sepLen &&
			strncmp(curDir, tmpbuf, sepLen) == 0  &&
			strncmp(&curDir[sepLen], tmpbuf, sepLen) == 0) {
			pathPtr = strstr(&curDir[sepLen * 2], tmpbuf);
			if (pathPtr != 0)
				pathPtr = strstr(&pathPtr[sepLen], tmpbuf);
			if (pathPtr != 0) {
				driveLen = pathPtr - curDir;
				pathLen = (int) strlen(pathPtr);
			}
			else {
				pathPtr = &curDir[strlen(curDir)];
				pathLen = 0;
				driveLen = (int) strlen(curDir);
			}
		}
		else {
			pathPtr = curDir;
			driveLen = 0;
			pathLen = (int) strlen(pathPtr);
		}
	}
	return pathPtr;
}

void Application::runCommand (const char *curDir, File& file, const char *command,
			      int tSkip, int multiFile)
{
    PROCESS_INFORMATION info;
    STARTUPINFO start;
    int i;

    // See if we quote commands
    char tempbuf[10];
    int quoteLen = 1;
    if (GetEnvironmentVariable ("HERMIT_QUOTE", tempbuf, 10) > 0)
	quoteLen = (int) strlen (tempbuf);
    else
	tempbuf[0] = '\0';

    // Get base file name (no extension)
    ArrayPtr<char> base = strDupNew (file.getName ());
    if (base == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    if (!(file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)) {
	char *dot = strrchr(base, '.');	// remove extension
	if (dot != 0)
	    *dot = '\0';
    }
    int baseLen = (int) strlen (base);

    // See how long the real command is
    int len = (int) strlen (command);
    int dirLen = (int) strlen (curDir);
	int driveLen = 0;
	int pathLen = dirLen;
	const char* pathPtr = splitDrivePath(curDir, driveLen, pathLen);
    int fileLen = (int) strlen (file.getName ());
    int dirCount = 0, fileCount = 0, baseCount = 0, driveCount = 0, pathCount = 0;
    for (i = 0; i < len; i++) {
	if (ispcmd (&command[i], 'd')) {
	    dirCount++;
	    i++;
	}
	else if (ispcmd (&command[i], 'f')) {
	    fileCount++;
	    i++;
	}
	else if (ispcmd (&command[i], 'b')) {
	    baseCount++;
	    i++;
	}
	else if (ispcmd (&command[i], 'v')) {
	    driveCount++;
	    i++;
	}
	else if (ispcmd (&command[i], 't')) {
	    pathCount++;
	    i++;
	}
    }
    ArrayPtr<char> buffer = new char [len + dirCount * dirLen + fileCount * fileLen + baseCount * baseLen + driveCount * driveLen + pathCount * pathLen + quoteLen + 1];
    if (buffer == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    *buffer = '\0';

    // Formulate real command
    int j = 0;
    for (i = 0; i < len; i++) {
	if (ispcmd (&command[i], 'd')) {
	    strcpy (&buffer[j], curDir);
	    j += dirLen;
	    i++;
	}
	else if (ispcmd (&command[i], 'f')) {
	    strcpy (&buffer[j], file.getName ());
	    j += fileLen;
	    i++;
	}
	else if (ispcmd (&command[i], 'b')) {
	    strcpy (&buffer[j], base);
	    j += baseLen;
	    i++;
	}
	else if (ispcmd (&command[i], 'v')) {
	    strncpy (&buffer[j], curDir, driveLen);
	    j += driveLen;
		buffer[j] = '\0';
	    i++;
	}
	else if (ispcmd (&command[i], 't')) {
	    strcpy (&buffer[j], pathPtr);
	    j += pathLen;
	    i++;
	}
	else if (ispcmd (&command[i], '!')) {
	    buffer[j] = command[i];
	    j++;
	    buffer[j] = '\0';
	    i++;
	}
	else {
	    buffer[j] = command[i];
	    j++;
	    buffer[j] = '\0';
	}
    }

    // get new title
    ArrayPtr<char> ttlbuf = new char [strlen (&buffer[tSkip]) + 1];
    if (ttlbuf != 0)
	strcpy (ttlbuf, &buffer[tSkip]);

    // Init structures
    memset (&info, 0, sizeof (PROCESS_INFORMATION));
    memset (&start, 0, sizeof (STARTUPINFO));
    start.cb = sizeof (STARTUPINFO);
    start.lpTitle = (ttlbuf != 0 ? (char *)ttlbuf : &buffer[tSkip]);
    start.dwFlags = STARTF_USESTDHANDLES;
    start.hStdInput = GetStdHandle (STD_INPUT_HANDLE);
    start.hStdOutput = mPrevOutput;
    start.hStdError = GetStdHandle (STD_ERROR_HANDLE);

    // Set new title; Write command to screen
    if (ttlbuf != 0) {			    // this is stupid but works around a Windows95
	SetConsoleTitle (ttlbuf);	    // problem...when we add the quote character after
	writeCommand (ttlbuf);		    // calling SetConsoleTitle Win95 picks up the change!!!
    }
    else {
	SetConsoleTitle (&buffer[tSkip]);
	writeCommand (&buffer[tSkip]);
    }

    // See if we quote commands
    strcat (buffer, tempbuf);

    // Create the process
    InputModeSwitcher switcher (*this, mModeSwitcher.getOldMode ());
    if (CreateProcess (0, buffer, 0, 0, TRUE, CREATE_NEW_PROCESS_GROUP | NORMAL_PRIORITY_CLASS,
			0, 0, &start, &info) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "CreateProcess", GetLastError ());

    // Wait for process to end
    if (WaitWithMessageLoop (info.hProcess) != TRUE) {
	CloseHandle (info.hThread);
	CloseHandle (info.hProcess);
	throw AppException (WHERE, ERR_WINDOWS_FMT, "WaitWithMessageLoop", GetLastError ());
    }

    CloseHandle (info.hThread);
    CloseHandle (info.hProcess);

    if (multiFile) {
	int tagged = 0;
	FileIterator *iter = mScroller->getIterator ();
	while (!iter->done ()) {
	    File& f = *iter;
	    if (f.isTagged ()) {
		f.unTagSortOf ();
		tagged = 1;
	    }
	    (*iter)++;
	}
	delete iter;

	// if no files tagged, use current
	if (tagged == 0)
	    file.unTagSortOf ();
    }
    else
	file.unTagSortOf ();
}

void Application::writeCommand (const char *cmd)
{
    DWORD charsWritten;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo (mPrevOutput, &info) != TRUE)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "GetConsoleScreenBufferInfo", GetLastError ());
    // use same background color but foreground yellow
    WORD attr = getColor (ceCommandText);
    attr |= (info.wAttributes & (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE));
    // just in case color is same, then we do something different
// causes problems
//    if (info.wAttributes == attr) {
//	char* pAttr = (char*) &attr;
//	char a = pAttr[1];
//	pAttr[1] = pAttr[0];
//	pAttr[0] = a;
	// attr &= ~BACKGROUND_INTENSITY;	// turn off intense background
//    }
    if (SetConsoleTextAttribute (mPrevOutput, attr) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleTextAttribute", GetLastError ());
    // It seems contiuously setting the text attribute is necessary...almost like a background
    // thread is resetting it for us!
#if 1
    Sleep (40);
#endif
    if (WriteConsole (mPrevOutput, "--> ", 4, &charsWritten, 0) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsole", GetLastError ());
    if (SetConsoleTextAttribute (mPrevOutput, attr) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleTextAttribute", GetLastError ());
    if (WriteConsole (mPrevOutput, cmd, (DWORD) strlen (cmd), &charsWritten, 0) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsole", GetLastError ());
    if (SetConsoleTextAttribute (mPrevOutput, info.wAttributes) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleTextAttribute", GetLastError ());
    if (WriteConsole (mPrevOutput, " ", 1, &charsWritten, 0) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsole", GetLastError ());
#if 1
    Sleep (40);
#endif
    if (SetConsoleTextAttribute (mPrevOutput, info.wAttributes) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleTextAttribute", GetLastError ());
    if (WriteConsole (mPrevOutput, "\r\n", 2, &charsWritten, 0) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "WriteConsole", GetLastError ());
    if (SetConsoleTextAttribute (mPrevOutput, info.wAttributes) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "SetConsoleTextAttribute", GetLastError ());
#if 1
    Sleep (40);
#endif
}

void Application::enter ()
{
    File f;
    try {
	f = mScroller->getCurrentFile ();
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
	else
	    return;
    }
    int sz = (int) strlen (f.getName ()) + (int) strlen (mScroller->getDirName ()) + 1;
    ArrayPtr<char> str = new char [sz + 1];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    strcpy (str, mScroller->getDirName ());
    strcat (str, "\\");
    strcat (str, f.getName ());
    if (f.getAttributes () & FILE_ATTRIBUTE_DIRECTORY) {
	sendEvent (Event (EV_STATUS_MSG, (DWORD) "Launching Explorer..."));
	if ((int)ShellExecute (NULL, "explore", str, NULL, NULL, SW_SHOWNORMAL) <= 32)
	    sendWinErrStatusMsg ("An error occurred while launching the explorer");
	else
	    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Launch complete"));
    }
    else {
	// Using NULL instead of "open" in the hopes that we pick up stray "run" commands
	sendEvent (Event (EV_STATUS_MSG, (DWORD) "Launching Application..."));
	if ((int)ShellExecute (NULL, NULL, str, NULL, NULL, SW_SHOWNORMAL) <= 32)
	    sendWinErrStatusMsg ("An error occurred while launching the application");
	else
	    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Launch complete"));
    }
}

void Application::print ()
{
    File f;
    try {
	f = mScroller->getCurrentFile ();
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
	else
	    return;
    }
    int sz = (int) strlen (f.getName ()) + (int) strlen (mScroller->getDirName ()) + 1;
    ArrayPtr<char> str = new char [sz + 1];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    strcpy (str, mScroller->getDirName ());
    strcat (str, "\\");
    strcat (str, f.getName ());
    if (f.getAttributes () & FILE_ATTRIBUTE_DIRECTORY) {
	sendEvent (Event (EV_STATUS_MSG, (DWORD) "Cannot print a directory"));
    }
    else {
	sendEvent (Event (EV_STATUS_MSG, (DWORD) "Spooling file..."));
	if ((int)ShellExecute (NULL, "print", str, NULL, NULL, SW_SHOWNORMAL) <= 32)
	    sendWinErrStatusMsg ("An error occurred while launching the application");
	else
	    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Spool complete"));
    }
}

void Application::runVerb ()
{
    File f;
    try {
	f = mScroller->getCurrentFile ();
    }
    catch (const AppException& info) {
	if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    throw;
	else
	    return;
    }
    int sz = (int) strlen (f.getName ()) + (int) strlen (mScroller->getDirName ()) + 1;
    ArrayPtr<char> str = new char [sz + 1];
    if (str == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    strcpy (str, mScroller->getDirName ());
    strcat (str, "\\");
    strcat (str, f.getName ());

    ArrayPtr<char> verb;
    try
    {
	VerbDialog verbDlg (*this, f);
	verbDlg.run ();
	int exitCode = verbDlg.getExitCode ();
	if (exitCode == KB_ENTER) {
	    const char *vb = verbDlg.getVerb ();
	    if (vb != 0) {
		verb = strDupNew (vb);
		if (verb == 0)
		    throw AppException (WHERE, ERR_OUT_OF_MEMORY);
	    }
	}
    }
    catch (const AppException& e) {
	if (e.code() == ERR_WINDOWS_FMT)   // no verbs at all!
	    sendWinErrStatusMsg ("No app is associated with the file, or a registry error occurred");
	else
	    throw;
    }

    if (verb != 0) {
	sendEvent (Event (EV_STATUS_MSG, (DWORD) "Launching program..."));
	if ((int)ShellExecute (NULL, verb, str, NULL, NULL, SW_SHOWNORMAL) <= 32)
	    sendWinErrStatusMsg ("An error occurred while launching the program");
	else
	    sendEvent (Event (EV_STATUS_MSG, (DWORD) "Launch complete"));
    }
}

void Application::initializeHWND ()
{
#define MY_BUFSIZE 100	// buffer size for console window titles

    char pszNewWindowTitle [MY_BUFSIZE];	// contains fabricated WindowTitle
    char pszOldWindowTitle [MY_BUFSIZE];	// contains original WindowTitle

    // fetch current window title
    GetConsoleTitle (pszOldWindowTitle, MY_BUFSIZE);

    // format a "unique" NewWindowTitle
    wsprintf (pszNewWindowTitle,"%s - %d/%d", getTitle (),
			  GetTickCount (), GetCurrentProcessId ());

    // change current window title
    SetConsoleTitle (pszNewWindowTitle);

    // ensure window title has been updated
    Sleep (40);

    // look for NewWindowTitle
    mHWnd = FindWindow (NULL, pszNewWindowTitle);

    // restore original window title
    SetConsoleTitle (pszOldWindowTitle);
}

// End
