// Application - process get system info
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#if 0
#define _WIN32_WINNT 0x0502
#define STRICT
#include <windows.h>
#endif

#include <string.h>
#include "app.h"
#include "prfscreen.h"
#include "console/colors.h"
#include "ptr.h"
#include "registry.h"

#if 1
#if 0
typedef enum _STREAM_INFO_LEVELS {
    FindStreamInfoStandard,
    FindStreamInfoMaxInfoLevel
} STREAM_INFO_LEVELS;

typedef struct _WIN32_FIND_STREAM_DATA {
    LARGE_INTEGER StreamSize;
    WCHAR cStreamName[MAX_PATH + 36];
} WIN32_FIND_STREAM_DATA, *PWIN32_FIND_STREAM_DATA;
#endif

typedef HANDLE (WINAPI *FindFirstStreamWFunc) (
    LPCWSTR lpFileName,
    STREAM_INFO_LEVELS InfoLevel,
    LPVOID lpFindStreamData,
    DWORD dwFlags);

typedef BOOL (APIENTRY *FindNextStreamWFunc) (
    HANDLE hFindStream,
    LPVOID lpFindStreamData);
#endif

#ifdef HERMIT_PRO
    #define INFO_CONSOLE_MSG  "HERMIT PRO - System Information"
#else
    #define INFO_CONSOLE_MSG  "HERMIT - System Information"
#endif

#ifdef HERMIT_PRO
    #define FILE_INFO_CONSOLE_MSG  "HERMIT PRO - File Information"
#else
    #define FILE_INFO_CONSOLE_MSG  "HERMIT - File Information"
#endif


void Application::handleKbAltI (const Event& /*event*/)
{
    {
	PrintfScreen prf (INFO_CONSOLE_MSG, ceInfoNormal, ceInfoNormalSel);
	prf.activate ();
	prf.draw ();

	// Computer Information
        prf.printLine (ceInfoNormal, "");
	prf.printLine (ceInfoHighlight, " Computer Information");

	// Computer name
	{
	    char compName [MAX_COMPUTERNAME_LENGTH + 1];
	    DWORD csize = MAX_COMPUTERNAME_LENGTH + 1;
	    if (GetComputerName (compName, &csize) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "GetComputerName", GetLastError ());
	    prf.printLine (ceInfoNormal, "        Computer name:    %s", compName);
	}

	// User name
	{
	    char userName [256];
	    DWORD size = 256;
	    if (GetUserName (userName, &size) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "GetUserName", GetLastError ());
	    prf.printLine (ceInfoNormal, "        User name:        %s", userName);
	}

	// Operating system
	{
	    OSVERSIONINFO info;
	    info.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	    info.dwBuildNumber = 0;
	    char *str = "<unknown>";
	    if (GetVersionEx (&info) != TRUE)
		throw AppException (WHERE, ERR_WINDOWS_FMT, "GetVersionEx", GetLastError ());
	    if (info.dwPlatformId == VER_PLATFORM_WIN32s)
		str = "Win32s on Windows 3.1";
	    else if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		str = "Windows95";
	    else if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
		str = "Windows NT";
	    prf.printLine (ceInfoNormal, "        Operating System: %s Version %u.%u, Build %u",
			   str, info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber);
	}

	// Disk Information
        prf.printLine (ceInfoNormal, "");
	prf.printLine (ceInfoHighlight, " File System Information");

        // AVAILABLE DRIVES
	UINT oldMode = SetErrorMode (SEM_FAILCRITICALERRORS);	// don't prompt
	DWORD drivesMask = GetLogicalDrives ();
	char drive = 'A';
	char root[10];
	for (int i = 0; i < 26; i++) {
	    if (drivesMask & 0x00000001) {
		wsprintf (root, "%c:\\", drive);

		// Get type of drive
		UINT dtype = GetDriveType (root);
		char *typ = "<unknown>";
		switch (dtype) {
		case 1:			typ = "<invalid root dir>"; break;  // The root directory does not exist.
		case DRIVE_REMOVABLE:	typ = "removeable disk";    break;  // The drive can be removed from the drive.
		case DRIVE_FIXED:	typ = "fixed disk";	    break;  // The disk cannot be removed from the drive.
		case DRIVE_REMOTE:	typ = "network drive";	    break;  // The drive is a remote (network) drive.
		case DRIVE_CDROM:	typ = "CD-ROM drive";	    break;  // The drive is a CD-ROM drive.
		case DRIVE_RAMDISK:	typ = "RAM disk";	    break;  // The drive is a RAM disk.
		case 0:
		default:		typ = "<unknown>";	    break;  // The drive type cannot be determined.
		}

		// Print drive heading
		prf.printLine (ceInfoHighlight, "        Drive %2.2s (%s)", root, typ);

		// Get volume information
		char volume[256];
		DWORD serial;
		DWORD maxFilenameLen;
		DWORD sysFlags;
		char fileSystem[256];
		volume[0] = 0;
		serial = maxFilenameLen = sysFlags = 0;
		fileSystem[0] = 0;
		if (GetVolumeInformation (root,		// address of root directory of the file system
					  volume,	// address of name of the volume
					  256,		// length of lpVolumeNameBuffer
					  &serial,	// address of volume serial number
					  &maxFilenameLen,	// address of system's maximum filename length
					  &sysFlags,	// address of file system flags
					  fileSystem,	// address of name of file system
					  256		// length of lpFileSystemNameBuffer
					 ) == TRUE) {

		    prf.printLine (ceInfoNormal, "                File System:      %s", fileSystem);
		    prf.printLine (ceInfoNormal, "                Volume Name:      %s", volume);
		    prf.printLine (ceInfoNormal, "                Serial Number:    %u", serial);
		    prf.printLine (ceInfoNormal, "                Compressed:       %s", (sysFlags & FS_VOL_IS_COMPRESSED ? "Yes" : "No"));
		    prf.printLine (ceInfoNormal, "                Preserves Case:   %s", (sysFlags & FS_CASE_IS_PRESERVED ? "Yes" : "No"));
		    prf.printLine (ceInfoNormal, "                Case Sensitive:   %s", (sysFlags & FS_CASE_SENSITIVE ? "Yes" : "No"));
		    prf.printLine (ceInfoNormal, "                Unicode Support:  %s", (sysFlags & FS_UNICODE_STORED_ON_DISK ? "Yes" : "No"));
		    prf.printLine (ceInfoNormal, "                Preserves ACLs:   %s", (sysFlags & FS_PERSISTENT_ACLS ? "Yes" : "No"));
		    prf.printLine (ceInfoNormal, "                File Compression: %s", (sysFlags & FS_FILE_COMPRESSION ? "Yes" : "No"));
		    prf.printLine (ceInfoNormal, "                Filename Length:  %u", maxFilenameLen);

			LONGLONG freeSpace, totalSpace, usedSpace;
		    char str[80];
			if (GetDiskInfo(root, freeSpace, totalSpace, usedSpace) == TRUE) {
			    prf.printLine (ceInfoNormal, "                Free Space:       %s bytes (%1.0f%%)",
					   formatGiant (freeSpace, str), ((float) freeSpace) * 100.0 / totalSpace);
			    prf.printLine (ceInfoNormal, "                Used Space:       %s bytes (%1.0f%%)",
					   formatGiant (usedSpace, str), ((float) usedSpace) * 100.0 / totalSpace);
			    prf.printLine (ceInfoNormal, "                Total Space:      %s bytes",
					   formatGiant (totalSpace, str));
		    }
		}
		else
		    prf.printLine (ceInfoNormal, "                <could not read disk>");

	    }
	    drive++;
	    drivesMask >>= 1;
	}
	SetErrorMode (oldMode);

	// Last line
        prf.printLine (ceInfoNormal, "");
	prf.printLine (ceInfoHighlight, "                     *** PRESS ESC TO RETURN TO HERMIT ***");

	prf.draw ();
	prf.run ();
    }
    activate ();
    SetConsoleTitle (getTitle ());
}

void Application::handleKbBackTab (const Event& /*event*/)
{
    {
	PrintfScreen prf (FILE_INFO_CONSOLE_MSG, ceInfoNormal, ceInfoNormalSel);
	prf.activate ();
	prf.draw ();

	// File Information
        prf.printLine (ceInfoNormal, "");

	int tagged = 0;
	FileIterator *iter = mScroller->getIterator ();
	while (!iter->done ()) {
	    File& file = *iter;
	    if (file.isTagged ()) {
		writeFileInfo (file, prf);
	        tagged = 1;
    	    }
	    (*iter)++;
	}
	delete iter;

	// if no files tagged, use current
	if (tagged == 0) {
	    try {
		File& file = mScroller->getCurrentFile ();
		writeFileInfo (file, prf);
	    }
	    catch (const AppException& info) {
	        if (info.code() != ERR_NOT_FOUND)  // ERR_NOT_FOUND happens for empty file directories
	    	    throw;
	    }
	}

	// Last line
	prf.printLine (ceInfoHighlight, "                     *** PRESS ESC TO RETURN TO HERMIT ***");

	prf.draw ();
	prf.run ();
    }
    activate ();
    SetConsoleTitle (getTitle ());
}

void Application::writeFileInfo (const File& file, PrintfScreen& prf)
{
    //    DWORD dwFileAttributes;

    char str[80];

    prf.printLine (ceInfoHighlight, " %s", file.getName ());
    prf.printLine (ceInfoNormal, "        Alternate Name: %s", file.getAltName ());

    if (!(file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)) {
	const char *ext = strrchr (file.getName (), '.');
	int written = 0;
	if (ext != NULL  &&  strlen (ext) >= 2) {
	    try {
		RegistryKey key (HKEY_CLASSES_ROOT, ext);
		DWORD type;
		ArrayPtr<char> objt = key.queryValue (NULL, type);
		if (type == REG_SZ  &&  objt != NULL  &&  objt[0] != ' ') {
		    RegistryKey k (HKEY_CLASSES_ROOT, objt);
		    ArrayPtr<char> value = k.queryValue (NULL, type);
		    if (type == REG_SZ  &&  value != NULL  &&  value[0] != ' ') {
			prf.printLine (ceInfoNormal, "        Type:           %s", (char *)value);
			written = 1;
		    }
		}
	    }
	    catch (const std::exception&) {
	    }
	}
	if (!written)
	    prf.printLine (ceInfoNormal, "        Type:");
    }

    LONGLONG size = ((LONGLONG)file.getFileSizeHigh () << 32) + (LONGLONG) file.getFileSizeLow ();

    prf.printLine (ceInfoNormal, "        Size:           %s", formatGiant (size, str));

    File::writeTime (str, file.getLastWriteTime ());
    prf.printLine (ceInfoNormal, "        Modified:      %s", str);

    File::writeTime (str, file.getCreationTime ());
    prf.printLine (ceInfoNormal, "        Created:       %s", str);

    File::writeTime (str, file.getLastAccessTime ());
    prf.printLine (ceInfoNormal, "        Accessed:      %s", str);

    *str = 0;
    if (file.getAttributes () & FILE_ATTRIBUTE_DIRECTORY)
	strcat (str, " Directory");
    if (file.getAttributes () & FILE_ATTRIBUTE_READONLY)
	strcat (str, " Readonly");
    if (file.getAttributes () & FILE_ATTRIBUTE_ARCHIVE)
	strcat (str, " Archive");
    if (file.getAttributes () & FILE_ATTRIBUTE_COMPRESSED)
	strcat (str, " Compressed");
    if (file.getAttributes () & FILE_ATTRIBUTE_SYSTEM)
	strcat (str, " System");
    if (file.getAttributes () & FILE_ATTRIBUTE_HIDDEN)
	strcat (str, " Hidden");
    if (file.getAttributes () & FILE_ATTRIBUTE_TEMPORARY)
	strcat (str, " Temp");
    prf.printLine (ceInfoNormal, "        Flags:         %s", str);

#if 1
    // stream data
    if ((file.getAttributes() & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        HMODULE hMod = LoadLibrary("kernel32.dll");
        if (hMod != NULL) {
            FindFirstStreamWFunc ffs = (FindFirstStreamWFunc) GetProcAddress(hMod, "FindFirstStreamW");
            FindNextStreamWFunc fns = (FindNextStreamWFunc) GetProcAddress(hMod, "FindNextStreamW");
            if (ffs != NULL && fns != NULL) {
                wchar_t filen[MAX_PATH + 1];
                char streamName[(MAX_PATH + 36) * 2 + 1];
                WIN32_FIND_STREAM_DATA data;
                mbstowcs(filen, file.getName(), MAX_PATH);
                filen[MAX_PATH] = L'\0';
                HANDLE h = (*ffs)(filen, FindStreamInfoStandard, &data, 0);
                if (h != INVALID_HANDLE_VALUE) {
                    wcstombs(streamName, data.cStreamName, (MAX_PATH * 36) * 2);
                    streamName[(MAX_PATH + 36) * 2] = '\0';
                    prf.printLine(ceInfoNormal, "        Streams:        %s (%s bytes)", streamName, formatGiant(data.StreamSize.QuadPart, str));
                    while ((*fns)(h, &data) == TRUE) {
                        wcstombs(streamName, data.cStreamName, (MAX_PATH * 36) * 2);
                        streamName[(MAX_PATH + 36) * 2] = '\0';
                        prf.printLine(ceInfoNormal, "                        %s (%s bytes)", streamName, formatGiant(data.StreamSize.QuadPart, str));
                    }
                    FindClose(h);
                }
            }
            FreeLibrary(hMod);
        }
    }
#endif

    prf.printLine (ceInfoNormal, "");
}

const char *Application::formatGiant (const LONGLONG& val, char *str)
{
    *str = '\0';

    LONGLONG value = val;
    // LONGLONG ten = 10;

    int i = 0;
    do {
	if (((i + 1) % 4) == 0  &&  i != 0) {
	    str[i] = ',';
	    str[i + 1] = '\0';
	    i++;
	}
	str[i] = ((char) (value % 10)) + '0';
	str[i + 1] = '\0';
	i++;
	value = value / 10;
    } while (value > 0);

    _strrev (str);

    return str;
}

typedef BOOL (_stdcall *GDFS_FUNC)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

BOOL Application::GetDiskInfo(const char* root, LONGLONG& freeSpace, LONGLONG& totalSpace, LONGLONG& usedSpace)
{
	DWORD freeClusters, totalClusters, sectorsPerCluster, bytesPerSector;
	ULARGE_INTEGER i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
	GDFS_FUNC pGetDiskFreeSpaceEx;

	pGetDiskFreeSpaceEx = (GDFS_FUNC) GetProcAddress(GetModuleHandle("kernel32.dll"),
										 "GetDiskFreeSpaceExA");

	if (pGetDiskFreeSpaceEx) {
		if (pGetDiskFreeSpaceEx (root,
                (PULARGE_INTEGER)&i64FreeBytesToCaller,
                (PULARGE_INTEGER)&i64TotalBytes,
                (PULARGE_INTEGER)&i64FreeBytes) == FALSE)
			return FALSE;

		freeSpace = (LONGLONG) i64FreeBytes.QuadPart;
		totalSpace = (LONGLONG) i64TotalBytes.QuadPart;
		usedSpace = (LONGLONG) (i64TotalBytes.QuadPart - i64FreeBytes.QuadPart);
	}
	else {
		if (GetDiskFreeSpace (root,				// address of root path
					  &sectorsPerCluster,   // address of sectors per cluster
					  &bytesPerSector,	    // address of bytes per sector
					  &freeClusters,		// address of number of free clusters
					  &totalClusters		// address of total number of clusters
					 ) != TRUE)
			return FALSE;

		freeSpace = Int32x32To64 (sectorsPerCluster * bytesPerSector, freeClusters);
		totalSpace = Int32x32To64 (sectorsPerCluster * bytesPerSector, totalClusters);
		usedSpace = Int32x32To64 (sectorsPerCluster * bytesPerSector, totalClusters - freeClusters);
	}

	return TRUE;
}

// End
