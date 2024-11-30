#ifndef MEMORY_H
#define MEMORY_H

#include "Header/definitions.h"
#include "Library/platform.h"

#define GetBit(value, index) (((value) & (1 << (index))) != 0)
#define SetBit2(value, index, bit) ((!(bit)) ? ((value) & (1 << (index))) : ((value) | (1 << (index))))
#define SetBit(value, index, bit) ((value) = ((GetBit(value, index) == bit) ? (value) : (SetBit2(value, index, bit))))

#endif