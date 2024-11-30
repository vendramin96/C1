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
    File->BitBufferCount = 0;
    File->BitsConsumed = 0;
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

    if((Size < 1) || (Size > (sizeof(File->BitBuffer) * 8)))
    {
        Log("Size is %d\n", Size);
        return Result;
    }
    else if(Size > File->Size)
    {
        Log("File underflow\n");
        return Result;
    }

    Result = Size;
    while(Size > 0)
    {
        if(File->BitBufferCount == (sizeof(File->BitBuffer) * 8))
        {
            //@TODO;
            FlushFileBitBuffer(File);
        }

        int BitsConsumed = (Size < 8) ? (Size) : (8);
        u8 Byte = *(u8 *)File->Memory;
        for(int Index = 7; Index >= 0; Index--)
        {
            int Bit = GetBit(Byte, Index);
            File->BitBufferCount++;
            File->BitBuffer |= (uptr)Bit << (64 - File->BitBufferCount);            
        }

        File->Memory = (u8 *)File->Memory + 1;
        File->Size--;
        
        Size -= BitsConsumed;
    }
    
    File->BitsConsumed += (int)Result;    
    Result = File->BitBuffer >> (64 - File->BitsConsumed);

    return Result;
}

internal_function uptr
ConsumeFileBitsMSBReversed(file *File, int Size)
{
    uptr Result = 0;
    
    Result = ConsumeFileBitsMSB(File, Size);    
    Result &= (((uptr)1 << ((Size < 8) ? (Size) : (Size + (((Size - 1) / 8) * 8)))) - 1);
    Result = ReverseBits(Result, Size);    
    
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