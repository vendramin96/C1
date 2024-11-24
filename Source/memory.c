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

internal_function void
FlushFileBitBuffer(file *File)
{
    File->BitBuffer = 0;
    File->BitBufferCount;
}

internal_function uptr
ConsumeFileBitsSize(file *File, int Size)
{
    uptr Result = 0;

    int BitsLeft = Size;
    int BitBufferCount = File->BitBufferCount;
    while(BitsLeft)
    {
        if(File->BitBufferCount >= 64)
        {
            FlushFileBitBuffer(File);
        }

        u8 Byte = *(u8 *)File->Memory;
        int BitsConsumed = 0;
        if(BitsLeft >= 8)
        {
            BitsConsumed = 8;
            File->BitBuffer |= Byte << File->BitBufferCount;
            File->BitBufferCount += BitsConsumed;
            File->Memory = (u8 *)File->Memory + 1;
            File->Size -= 1;            
        }
        else
        {   
            BitsConsumed = BitsLeft;         
            File->BitBuffer |= ((Byte >> (File->BitBufferCount % 8)) & ((1 << BitsConsumed) - 1)) << File->BitBufferCount;
            File->BitBufferCount += BitsConsumed;
        }

        BitsLeft -= BitsConsumed;
    }

    Assert(File->Size >= 0);
    Assert(!BitsLeft);

    Result = (File->BitBuffer >> (BitBufferCount)) & ((1 << Size) - 1);
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