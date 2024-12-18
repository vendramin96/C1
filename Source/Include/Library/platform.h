#ifndef PLATFORM_LIBRARY_H
#define PLATFORM_LIBRARY_H

#include "Header/definitions.h"

typedef struct
{
    uptr Buffer;
    int Count;
    int Consumed;
} bit_stream;

typedef struct
{
    void *Memory;
    uptr Size;
    bit_stream BitStream;
} file;

uptr PlatformFormatString(char *Buffer, uptr BufferSize, char *Format, ...);
void PlatformWriteConsole(char *String);
void *PlatformAllocateMemory(uptr Size);
bool PlatformFreeMemory(void *Memory);
bool PlatformReadFile(file *File, char *FileName);

#define Print(format, ...) do { char b_[1024]; PlatformFormatString(b_, sizeof(b_), format, __VA_ARGS__); PlatformWriteConsole(b_); } while(0)
#define Log Print
#endif