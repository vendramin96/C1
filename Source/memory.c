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
ReverseBits(uptr Value, int Size)
{
    uptr Result = Value;

    for(int Index = 0; Index < (Size / 2); Index++)
    {
        int Left = Size - (Index + 1);
        int LeftValue = (Result >> Left) & 1;
        int RightValue = (Result >> Index) & 1;
        
        if(LeftValue != RightValue)
        {
            Result ^= (uptr)(1 << Left);
            Result ^= (uptr)(1 << Index);
        }
    }

    return Result;
}

internal_function uptr
ConsumeFileBitsMSB(file *File, int Size)
{
    uptr Result = 0;

    if(Size < 1)
    {
        return Result;
    }

    // 7654 3210
    // 1010 0101 ... 1010 0101
    // 101.0 0.101 ... 1010 0101 
    Result = Size;
    while(Size > 0)
    {        
        if(File->BitBufferCount == (sizeof(uptr) * 8))
        {
            FlushFileBitBuffer(File);
        }

        int BitsLeft = 8 - (File->BitBufferCount % 8);
        int BitCount = (Size < 8) ? (Size) : (8);
        u8 Byte = *(u8 *)File->Memory;
     
        //BitCount = (BitCount > BitsLeft) ? (BitsLeft) : (BitCount);
        BitsLeft = (BitsLeft > BitCount) ? (BitsLeft - BitCount) : (BitCount - BitsLeft);
        Byte = (Byte >> BitsLeft) & ((1 << BitCount) - 1);
        File->BitBuffer |= (uptr)Byte << (64 - (File->BitBufferCount + BitCount));
        File->BitBufferCount += BitCount;

        if(!BitsLeft)
        {
            File->Memory = (u8 *)File->Memory + 1;
            File->Size--;
        }
        
        Size -= BitCount;
    }

    Assert(!Size);

    Result = (File->BitBuffer >> (64 - File->BitBufferCount)) & ((1 << Result) - 1);
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