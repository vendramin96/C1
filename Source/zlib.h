#ifndef ZLIB_H
#define ZLIB_H

#include "Header/definitions.h"
#include "Library/platform.h"

#pragma pack(push, 1)
typedef struct
{
    u8 CMF;
    u8 FLG;
} zlib_header;

#pragma pack(pop)

#endif