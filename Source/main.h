#ifndef MAIN_H
#define MAIN_H

#include "Header/definitions.h"
#include "Library/platform.h"
#include "platform.h"

typedef struct
{
    bool IsInitialized;
    file RawPNG;
} engine_state;

#include "memory.c"
#include "zlib.c"
#include "png.c"

#endif