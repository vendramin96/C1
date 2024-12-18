#ifndef WINDOWS_MAIN_H
#define WINDOWS_MAIN_H

#include "Header/definitions.h"
#include "Library/platform.h"
#include "platform.h"
#include <windows.h>

#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define ENGINE_DLL_FILE_NAME "main.dll"

typedef struct
{
    HMODULE Library;
    update_and_render *UpdateAndRender;
} windows_engine_dll;

typedef struct
{
    int X;
    int Y;
    int Width;
    int Height;
} window_dimension;

typedef struct
{
    platform_video State;
    BITMAPINFO Information;
} windows_video;

#endif