// Registry class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#include "registry.h"
#include "ptr.h"


char RegistryKey::mSubKey[256];

RegistryKey::RegistryKey (HKEY hKey, const char *pSubKey, REGSAM samDesired) :
    mhKey (0)
{
    if (samDesired & KEY_CREATE_SUB_KEY) {
	DWORD disposition;
	if (RegCreateKeyEx (hKey, pSubKey, 0, 0 /*class*/, REG_OPTION_NON_VOLATILE, samDesired,
			    0, &mhKey, &disposition) != ERROR_SUCCESS)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "RegCreateKeyEx", GetLastError ());
    }
    else {
	if (RegOpenKeyEx (hKey, pSubKey, 0, samDesired, &mhKey) != ERROR_SUCCESS)
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "RegOpenKeyEx", GetLastError ());
    }
}

RegistryKey::RegistryKey (HKEY hKey, const char *pSubKey, REGSAM samDesired, DWORD options) :
    mhKey (0)
{
    DWORD disposition;
    if (RegCreateKeyEx (hKey, pSubKey, 0, 0 /*class*/, options, samDesired,
		        0, &mhKey, &disposition) != ERROR_SUCCESS)
        throw AppException (WHERE, ERR_WINDOWS_FMT, "RegCreateKeyEx", GetLastError ());
}

void RegistryKey::reOpen (HKEY hKey, const char *pSubKey, REGSAM samDesired)
{
    RegCloseKey (mhKey);
    mhKey = 0;
    if (RegOpenKeyEx (hKey, pSubKey, 0, samDesired, &mhKey) != ERROR_SUCCESS)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegOpenKeyEx", GetLastError ());
}

RegistryKey::~RegistryKey ()
{
    RegCloseKey (mhKey);
}

char * RegistryKey::queryValue (const char *pName, DWORD& type) const
{
    DWORD size = 0;
    if (RegQueryValueEx (mhKey, pName, 0, &type, 0, &size) != ERROR_SUCCESS)
    	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegQueryValueEx", GetLastError ());
    char *data = new char [size];
    if (data == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);
    if (RegQueryValueEx (mhKey, pName, 0, &type, (BYTE*) data, &size) != ERROR_SUCCESS) {
	delete [] data;
    	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegQueryValueEx", GetLastError ());
    }
    return data;
}

void RegistryKey::setValue (const char *pName, const char *value)
{
    if (value == 0)
	value = "";
    if (RegSetValueEx (mhKey, pName, 0, REG_SZ, (const BYTE *)value, (DWORD) strlen (value) + 1) != ERROR_SUCCESS)
    	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegSetValueEx", GetLastError ());
}

void RegistryKey::setValue (const char *pName, DWORD value)
{
    if (RegSetValueEx (mhKey, pName, 0, REG_DWORD, (const BYTE *)&value, sizeof (DWORD)) != ERROR_SUCCESS)
    	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegSetValueEx", GetLastError ());
}

const char *RegistryKey::enumerateKey (int index) const
{
    *(const_cast<char *>(mSubKey)) = '\0';
    DWORD size = 256;

    LONG rval = RegEnumKeyEx (mhKey, index, const_cast<char *>(mSubKey), &size, 0, 0, 0, 0);
    if (rval != ERROR_SUCCESS  &&  rval != ERROR_NO_MORE_ITEMS)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegEnumKeyEx", GetLastError ());

    if (rval == ERROR_NO_MORE_ITEMS)
	return NULL;

    return mSubKey;
}

void RegistryKey::remove (const char *pSubKey)
{
    privateRemove (mhKey, pSubKey);
}

void RegistryKey::privateRemove (HKEY hKey, const char *pSubKey)
{
    // Open the child.
    HKEY hKeyChild ;
    LONG lRes = RegOpenKeyEx (hKey, pSubKey, 0, KEY_ALL_ACCESS, &hKeyChild);
    if (lRes != ERROR_SUCCESS)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegOpenKeyEx", GetLastError ());

    // Enumerate all of the decendents of this child.
    FILETIME time;
    char szBuffer[256];
    DWORD dwSize = 256;
    LONG rval;
    while ((rval = RegEnumKeyEx (hKeyChild, 0, szBuffer, &dwSize, NULL,
	                 NULL, NULL, &time)) == ERROR_SUCCESS) {
	// Delete the decendents of this child.
	try {
	    privateRemove (hKeyChild, szBuffer);
	}
	catch (const std::exception&) {
	    // Cleanup before exiting.
	    RegCloseKey (hKeyChild);
	    throw;
	}

	dwSize = 256 ;
    }

    if (rval != ERROR_NO_MORE_ITEMS)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegEnumKeyEx", GetLastError ());

    // Close the child.
    RegCloseKey (hKeyChild);

    // Delete this child.
    if (RegDeleteKey (hKey, pSubKey) != ERROR_SUCCESS)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegDeleteKey", GetLastError ());
}

void RegistryKey::setKeyAndValue (const char *pSubKey, const char *pName, const char *pValue)
{
    HKEY hKey;
    if (pSubKey == 0)
	throw AppException (WHERE, ERR_INTERNAL);
    ArrayPtr<char> szKeyBuf = new char [strlen (pSubKey) + (pName ? strlen (pName) : 0) + 2];
    if (szKeyBuf == 0)
	throw AppException (WHERE, ERR_OUT_OF_MEMORY);

    // Copy keyname into buffer.
    strcpy(szKeyBuf, pSubKey);

    // Add subkey name to buffer.
    if (pName != NULL) {
	strcat (szKeyBuf, "\\");
	strcat (szKeyBuf, pName);
    }

    // Create and open key and subkey.
    LONG lResult = RegCreateKeyEx (mhKey, szKeyBuf,
	                           0, NULL, REG_OPTION_NON_VOLATILE,
	                           KEY_ALL_ACCESS, NULL,
	                           &hKey, NULL);
    if (lResult != ERROR_SUCCESS)
	throw AppException (WHERE, ERR_WINDOWS_FMT, "RegCreateKeyEx", GetLastError ());

    // Set the Value.
    if (pValue != NULL) {
	if (RegSetValueEx (hKey, NULL, 0, REG_SZ, (BYTE *) pValue, (DWORD) strlen (pValue) + 1) != ERROR_SUCCESS) {
	    RegCloseKey (hKey);
	    throw AppException (WHERE, ERR_WINDOWS_FMT, "RegSetValueEx", GetLastError ());
	}
    }

    RegCloseKey (hKey) ;
}

void RegistryKey::CLSIDtochar (const CLSID& clsid, char *pCLSID, int length)
{
    if (length < CLSID_STRING_SIZE)
	throw AppException (WHERE, ERR_INTERNAL);

    // Get CLSID
    LPOLESTR wszCLSID = NULL ;
    HRESULT hr = StringFromCLSID(clsid, &wszCLSID) ;
    if (FAILED (hr))
	throw AppException (WHERE, ERR_WINDOWS_FMT, "StringFromCLSID", GetLastError ());

    // Covert from wide characters to non-wide.
    wcstombs (pCLSID, wszCLSID, length) ;

    // Free memory.
    CoTaskMemFree (wszCLSID) ;
}


// End
