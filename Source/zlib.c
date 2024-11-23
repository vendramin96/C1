#include "zlib.h"

internal_function bool
ParseZLib(void *Data, uptr DataSize)
{
    bool Result = 0;

    file At = {0};
    At.Memory = Data;
    At.Size = DataSize;

    zlib_header *ZLibHeader = ConsumeFile(&At, zlib_header);

    if((((ZLibHeader->CMF*256) + ZLibHeader->FLG) % 31) != 0)
    {
        Log("ZLib header mismatch\n");
        return Result;
    }

    u8 CM = ZLibHeader->CMF & ((1 << 4) - 1);
    u8 CINFO = (ZLibHeader->CMF & 0xF0) >> 4;

    if((CM != 8) || (CINFO > 7))
    {
        Log("ZLib header mismatch\n");
        return Result;
    }

    u8 FCHECK = ZLibHeader->FLG & ((1 << 5) - 1);
    u8 FDICT = (ZLibHeader->FLG >> 5) & 1;
    u8 FLEVEL = (ZLibHeader->FLG >> 6) & ((1 << 2) - 1);

    if(FDICT)
    {
        Log("FDICT is set for the data\n");
        return Result;
    }

    // compressed data...

    ConsumeFile(&At, u32);

    Result = 1;
    return Result;
}