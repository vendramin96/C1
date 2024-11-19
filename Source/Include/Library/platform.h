#ifndef PLATFORM_LIBRARY_H
#define PLATFORM_LIBRARY_H

#include "Header/definitions.h"

uptr PlatformFormatString(char *Buffer, uptr BufferSize, char *Format, ...);
void PlatformWriteConsole(char *String);

#define Print(format, ...) do { char b_[1024]; PlatformFormatString(b_, sizeof(b_), format, __VA_ARGS__); PlatformWriteConsole(b_); } while(0)

#endif