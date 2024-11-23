#ifndef PLATFORM_H
#define PLATFORM_H

typedef struct
{
    void *Memory;
    int Width;
    int Height;
    int BytesPerPixel;
    int Pitch;
} platform_video;

#endif