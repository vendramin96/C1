#ifndef MEMORY_H
#define MEMORY_H

#include "Header/definitions.h"
#include "Library/platform.h"

#define GetBit(value, index) (int)(((value) & ((uptr)1 << (index))) != 0)
#define ToggleBit(value, index) ((value) = ((value) ^ ((uptr)1 << (index))))
#define SetBit(value, index, bit) (int)(GetBit(((value) = ((GetBit(value, index) == (bit)) ? (value) : (ToggleBit(value, index)))), index))
//#define SetBit(value, index, bit) (((value) = ((GetBit(value, index) == (bit)) ? (value) : (ToggleBit(value, index)))))
#define PeekFile(file, type) (type *)PeekFileSize(file, sizeof(type))
#define ConsumeFile(file, type) (type *)ConsumeFileSize(file, sizeof(type))

#endif