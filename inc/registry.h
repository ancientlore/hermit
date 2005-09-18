// Registry class
// Copyright (C) 1996 Michael D. Lore  All Rights Reserved

#ifndef REGISTRY_H
#define REGISTRY_H

#define STRICT
#include <windows.h>
#include "common.h"

#define HKCU_SUBKEY_HERMIT	"Software\\Lore\\Hermit"
#define HKLM_SUBKEY_HERMIT	"SOFTWARE\\Lore\\Hermit"

const int CLSID_STRING_SIZE = 39;

class RegistryKey {

    HKEY mhKey;
    static char mSubKey[256];
    void privateRemove (HKEY hKey, const char *pSubKey);

public:

    RegistryKey (HKEY hKey, const char *pSubKey, REGSAM samDesired = KEY_READ);
    RegistryKey (HKEY hKey, const char *pSubKey, REGSAM samDesired, DWORD options);
    virtual ~RegistryKey ();

    void reOpen (HKEY hKey, const char *pSubKey, REGSAM samDesired = KEY_READ);

    // you must delete the returned value
    char *queryValue (const char *pName, DWORD& type) const;

    void setValue (const char *pName, const char *value);
    void setValue (const char *pName, DWORD value);

    const char *enumerateKey (int index) const;

    void remove (const char *pSubKey);
    void setKeyAndValue (const char *pSubKey, const char *pName, const char *pValue);

    operator HKEY () const { return mhKey; }

    static void CLSIDtochar (const CLSID& clsid, char *pCLSID, int length);
};

#endif
