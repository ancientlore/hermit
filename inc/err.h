// Error stuff
// Copyright (C) 1996 Michael D. Lore

#ifndef ERR_H
#define ERR_H

#include <exception>

// Append _FMT if the error is formatted

const int ERR_OUT_OF_MEMORY =		0;
const int ERR_INTERNAL =		1;
const int ERR_FILE_ERROR =		2;
const int ERR_NOT_FOUND =		3;
const int ERR_DUPLICATE =		4;
const int ERR_RANGE =			5;
const int ERR_WINDOWS_FMT = 		6;  // %s func name, %d extended error code
const int ERR_USER_CANCEL =		7;
const int ERR_EXTENSION_FMT =		8;  // error in a component

const char *getErrMsg (int errCode);

#define WHERE	__FILE__, __LINE__

class AppException : public std::exception {
    int errCode;
    const char *errMsg;

    static int debug;
    static char buffer[];
    static int freezeCount;
public:
    AppException(const char* file, int line, int errCode, ...);
    AppException(const char* file, int line, const char* msg, ...);
    virtual ~AppException();

    virtual const char* what() const;
    int code() const { return errCode; }

    static int Freeze();
    static int Thaw();

    static void setDebug(int b);

    class AutoFreezer {
    public:
        AutoFreezer () { AppException::Freeze (); }
        ~AutoFreezer () { AppException::Thaw (); }
    };
};

#endif
