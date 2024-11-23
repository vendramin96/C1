#include "memory.h"

#define PeekFile(file, type) (type *)PeekFileSize(file, sizeof(type))
#define ConsumeFile(file, type) (type *)ConsumeFileSize(file, sizeof(type))

internal_function void *
PeekSize(void **Memory, uptr *MemorySize, uptr Size)
{
    void *Result = 0;

    if(Size > *MemorySize)
    {
        Log("File memory overflow\n");
        return Result;
    }

    Result = *Memory;
    return Result;
}

internal_function void *
ConsumeSize(void **Memory, uptr *MemorySize, uptr Size)
{
    void *Result = 0;

    Result = PeekSize(Memory, MemorySize, Size);
    if(!Result)
    {
        return Result;
    }

    *Memory = (u8 *)*Memory + Size;
    *MemorySize -= Size;

    return Result;
}

internal_function uptr
ConsumeBitsSize(void **Memory, uptr *MemorySize, int Size)
{
    uptr Result = 0;

    int BitCount = 0;
    while(Size > 0)
    {
        u8 Byte = *(u8 *)*Memory;
        Result |= Byte << BitCount;

        if(Size > 8)
        {
            BitCount += 8;
            *Memory = (u8 *)*Memory + 1;
        }
        else
        {
            BitCount = Size;
        }

        Size -= BitCount;
    }

    return Result;
}

internal_function void *
PeekFileSize(file *File, uptr Size)
{
    void *Result = 0;

    Result = PeekSize(&File->Memory, &File->Size, Size);
    return Result;
}

internal_function void *
ConsumeFileSize(file *File, uptr Size)
{
    void *Result = 0;

    Result = ConsumeSize(&File->Memory, &File->Size, Size);
    return Result;
}

internal_function void
MSBToLSBU32(u32 *Value)
{
    u32 V = *Value;

    *Value = (V >> 24) | ((V >> 8) & 0xFF00) | ((V & 0xFF00) << 8) | (V << 24);
}