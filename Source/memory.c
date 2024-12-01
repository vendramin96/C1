#include "memory.h"

//@TODO: GetBitFromRange
internal_function uptr
SwapBits(uptr *Value, int I, int J)
{
    uptr Result = 0;

    if(I == J)
    {
        Log("Swaping index I, J are the same\n");
        return Result;
    }

    int Bit1 = GetBit(*Value, I);
    int Bit2 = GetBit(*Value, J);

    if(Bit1 != Bit2)
    {
        *Value = ToggleBit(*Value, I);
        *Value = ToggleBit(*Value, J);
    }

    Result = *Value;
    return Result;
}

internal_function uptr
ReverseBits(uptr Value, int MinIndex, int MaxIndex)
{
    uptr Result = 0;

    int I = MinIndex;
    int J = MaxIndex;
    for(int Index = MaxIndex; Index >= ((MaxIndex + MinIndex + 1) / 2); Index--)
    {
        //Print("I: %d, J: %d\n", I, J);
        int MinBit = GetBit(Value, I);
        int MaxBit = GetBit(Value, J);

        if(MinBit != MaxBit)
        {
            ToggleBit(Value, I);
            ToggleBit(Value, J);
        }

        I++;
        J--;
    }

    Result = Value;
    return Result;
}

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
FlushFileBitBuffer(bit_stream *BitStream)
{
    BitStream->Buffer = 0;
    BitStream->Count = 0;
}

internal_function uptr
ConsumeFileBits(file *File, int Size)
{
    uptr Result = 0;

    bit_stream *BitStream = &File->BitStream;
    if((Size < 1) || (Size > (sizeof(BitStream->Buffer) * 8)) || (Size > File->Size))
    {
        Log("Size is %d\n", Size);
        return Result;
    }
    
    BitStream->Consumed += Size;
    while(Size > 0)
    {
        if(BitStream->Count == (sizeof(BitStream->Buffer) * 8))
        {
            FlushFileBitBuffer(BitStream);
        }

        u8 Byte = *(u8 *)File->Memory;
        for(int Index = 0; Index < 8; Index++)
        {
            int Bit = GetBit(Byte, Index);
            BitStream->Buffer |= Bit << BitStream->Count;
            BitStream->Count++;
        }

        File->Memory = (u8 *)File->Memory + 1;
        File->Size--;

        Size -= (Size < 8) ? (Size) : (8);
    }
    
    Result = File->BitStream.Buffer;

    return Result;
}

internal_function uptr
ConsumeFileBitsMSBReversed(file *File, int Size)
{
    uptr Result = 0;
    Result = ConsumeFileBits(File, Size);
#if 0
    for(int I = File->BitStream.Count; I >= 0; I -= 8)
    {
        for(int J = 0; J < 8; J++)
        {
            Result = SwapBits(&Result, I - 1, I - 8 + J);
        }
    }
#endif
    //@TODO: Reconstruct Result such that it is in MSB instead of LSB
    // '1010 0101 '1111 0000 '1010 0101 '1111 0000 ...
    // 32-0, 16,8    
    
    for(int I = File->BitStream.Count / 8; I > 0; I--)
    {
        // 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (8, 8)
        // ^                       ^
        // '63 62 61 60 59 58 57 56 '55 54 53 52 51 50 49 48 '47 46 45 44 43 42 41 40 '39 38 37 36 35 34 33 32 (56,56), (40, 40), (24,24), (8,8) = 16
        // '31 30 29 28 27 26 25 24 '23 22 21 20 19 18 17 16 '15 14 13 12 11 10 09 08 '07 06 05 04 03 02 01 00
        //Result = ((Result << LeftShift) & LeftMask) | ((Result >> RightShift) & RightMask);  
        if(!(I % 2))
        {
            int MinIndex = ((I - 1) * 8) - 1; //File->BitStream.Count - (File->BitStream.Count - 8);
            int MaxIndex = (I * 8) - 1; //File->BitStream.Count - 1;
            for(int J = 0; J < 8; J++)
            {
//                Print("MinIndex: %d, MaxIndex: %d\n", MinIndex, MaxIndex);
                SwapBits(&Result, MaxIndex, MinIndex);
                MaxIndex--;
                MinIndex--;
            }
        }
    }
   
    for(int Index = 0; Index < (File->BitStream.Count / 8); Index++)
    {
        int MinIndex = Index * 8;
        int MaxIndex = MinIndex + 7;
        Print("MinIndex: %d, MaxIndex: %d\n", MinIndex, MaxIndex);
        Result = ReverseBits(Result, MinIndex, MaxIndex); // 0-7, 8-15, 16-23
    }

    Result = (Result >> (File->BitStream.Count - (File->BitStream.Consumed + Size))) & ((1 << Size) - 1);    
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