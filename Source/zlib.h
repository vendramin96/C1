#ifndef ZLIB_H
#define ZLIB_H

#include "Header/definitions.h"
#include "Library/platform.h"

#define ZLIB_CLCL_COUNT 19

typedef struct
{
    int Symbol;
    int Frequency;
    struct huffman_tree *Left;
    struct huffman_tree *Right;
} huffman_tree;

typedef struct
{
    huffman_tree *Array;
    int Size;
    int Capacity;
} priority_queue;

#pragma pack(push, 1)
typedef struct
{
    u8 CMF;
    u8 FLG;
} zlib_header;
#pragma pack(pop)

#endif