// Error stuff
// Copyright (C) 1996 Michael D. Lore


#include <cstdarg>
#include <cstdio>
#include "err.h"

using namespace std;

int AppException::debug = 0;
char AppException::buffer[1024];
int AppException::freezeCount = 0;

void AppException::setDebug (int b)
{
    debug = b;
}

int AppException::Freeze ()
{
    freezeCount++;
    return freezeCount;
}

int AppException::Thaw ()
{
    if (freezeCount > 0)
	freezeCount--;
    return freezeCount;
}

AppException::AppException(const char* file, int line, int _errCode, ...)
{
    int s = 0;
    errCode = _errCode;
    if (freezeCount == 0  &&  debug  &&  file != 0)
        s = sprintf (buffer, "%s (%d): ", file, line);
    va_list ap;
    va_start (ap, _errCode);
    if (freezeCount == 0)
        vsprintf (&buffer[s], getErrMsg (errCode), ap);
    va_end (ap);
    if (freezeCount == 0)
        errMsg = buffer;
    else
        errMsg = "no message - error is frozen";
}

AppException::AppException(const char* file, int line, const char* msg, ...)
{
    int s = 0;
    errCode = -1;
    if (freezeCount == 0  &&  debug  &&  file != 0)
        s = sprintf (buffer, "%s (%d): ", file, line);
    va_list ap;
    va_start (ap, msg);
    if (freezeCount == 0 && msg != 0)
        vsprintf (&buffer[s], msg, ap);
    va_end (ap);
    if (freezeCount == 0)
        errMsg = buffer;
    else
        errMsg = "no message - error is frozen";
}

AppException::~AppException()
{
}

const char* AppException::what() const
{
    return errMsg;
}


