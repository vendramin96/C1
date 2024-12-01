#ifndef MEMORY_H
#define MEMORY_H

#include "Header/definitions.h"
#include "Library/platform.h"

#define PeekFile(file, type) (type *)PeekFileSize(file, sizeof(type))
#define ConsumeFile(file, type) (type *)ConsumeFileSize(file, sizeof(type))

#endif