#include "memory.h"

internal_function int
GetBit(uptr Value, int Index)
{
    int Result = 0;

    Result = (Value & ((uptr)1 << Index)) != 0;
    return Result;
}

internal_function void
ToggleBit(uptr *Value, int Index)
{
    int Bit = GetBit(*Value, Index);

    if(!Bit)
    {
        *Value |= ((uptr)1 << Index);
        return;
    }

    *Value ^= ((uptr)1 << Index);
}

internal_function uptr
SwapBits(uptr Value, int I, int J)
{
    uptr Result = 0;

    int Bit1 = GetBit(Value, I);
    int Bit2 = GetBit(Value, J);

    if(Bit1 != Bit2)
    {
        ToggleBit(&Value, I);
        ToggleBit(&Value, J);
    }

    Result = Value;
    return Result;
}

internal_function uptr
ReverseBitsFromRange(uptr Value, int MinRange, int MaxRange)
{
    uptr Result = 0;

    for(int Index = MinRange; Index < ((MinRange + MaxRange + 1) / 2); Index++)
    {
        Value = SwapBits(Value, Index, MaxRange - Index);
    }

    Result = Value;
    return Result;
}

internal_function uptr
ConsumeFileBits(file *File, int Size)
{
    uptr Result = 0;

    bit_stream *BitStream = &File->BitStream;    
    if(BitStream->Count > BitStream->Consumed)
    {
        Result = BitStream->Buffer;
        BitStream->Consumed += Size;
        return Result;
    }

    BitStream->Consumed += Size;
    while(Size > 0)
    {
        if(BitStream->Count >= (BitStream->Buffer * 8))
        {
            BitStream->Buffer = 0;
            BitStream->Count = 0;
            BitStream->Consumed = 0;
        }

        u8 Byte = *(u8 *)File->Memory;
        BitStream->Buffer |= (uptr)Byte << BitStream->Count;
        BitStream->Count += 8;
        File->Memory = (u8 *)File->Memory + 1;
        File->Size--;

        Size -= (Size < 8) ? (Size) : (8);
    }

    Result = BitStream->Buffer;
    return Result;
}


internal_function uptr
ConsumeFileBitsMSB(file *File, int Size)
{
    uptr Result = ConsumeFileBits(File, Size);

    return Result;
}

internal_function uptr
ConsumeFileBitsMSBReversed(file *File, int Size)
{    
    uptr Result = ConsumeFileBitsMSB(File, Size);
    
    for(int Index = 0; Index < (File->BitStream.Count / 8); Index++)
    {
        Result = ReverseBitsFromRange(Result, Index * 8, (Index * 8) + 7);
    }

    return Result;
}

internal_function void *
PeekFileSize(file *File, uptr Size)
{
    void *Result = 0;

    Result = File->Memory;
    return Result;
}

internal_function void *
ConsumeFileSize(file *File, uptr Size)
{
    void *Result = 0;

    Result = File->Memory;
    File->Memory = (u8 *)File->Memory + 1;
    File->Size--;

    return Result;
}

internal_function void
MSBToLSBU32(u32 *Value)
{
    u32 V = *Value;

    *Value = (V >> 24) | ((V >> 8) & 0xFF00) | ((V & 0xFF00) << 8) | (V << 24);
}