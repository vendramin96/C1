#ifndef DSA_H
#define DSA_H

#include "Header/definitions.h"
#include "Library/platform.h"

typedef struct
{
    int Symbol;
    int Frequency;
    struct huffman_tree *Left;
    struct huffman_tree *Right;
} huffman_tree;

typedef struct
{
    huffman_tree **Heap;
    int Size;
    int Capacity;
} huffman_priority_queue;

#endif