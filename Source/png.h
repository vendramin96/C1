#ifndef PNG_H
#define PNG_H

#include "Header/definitions.h"
#include "Library/platform.h"

#pragma pack(push, 1)
typedef struct
{
    union
    {
        u8 Signature[8];
        u64 SignatureU64;
    };
} png_header;

typedef struct
{
    u32 Length;
    union
    {
        u32 Type;        
        char TypeChar[4];
    };
} png_chunk_header;

typedef struct
{
    u32 CRC;
} png_chunk_footer;

typedef struct
{
    u32 Width;
    u32 Height;
    u8 BitDepth;
    u8 ColorType;
    u8 CompressionMethod;
    u8 FilterMethod;
    u8 InterlaceMethod;
} png_ihdr;

#pragma pack(pop)

#endif