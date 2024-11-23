#ifndef PLATFORM_H
#define PLATFORM_H

typedef struct
{
    void *PermanentMemory;
    uptr PermanentMemorySize;    
} platform_memory;

typedef struct
{
    void *Memory;
    int Width;
    int Height;
    int BytesPerPixel;
    int Pitch;
} platform_video;

#define UPDATE_AND_RENDER(name) void name(platform_memory *Memory, platform_video *Video)
typedef UPDATE_AND_RENDER(update_and_render);

#endif