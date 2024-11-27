#include "deflate.h"

internal_function bool
DeflateDecompress(void *Data, uptr Size)
{
    bool Result = 0;

    file At = {0};
    At.Memory = Data;
    At.Size = Size;

    zlib_header *ZLibHeader  = ConsumeFile(&At, zlib_header);

    u8 CM = ZLibHeader->CMF & 0xF;
    u8 CINFO = (ZLibHeader->CMF >> 4) & 0xF;

    if(CM != 8)
    {
        Log("CM is %d\n", CM);
        return Result;
    }
    else if(CINFO > 7)
    {
        Log("CINFO is %d\n", CINFO);
        return Result;
    }

    u8 FCHECK = ZLibHeader->FLG & 0x1F;
    u8 FDICT = (ZLibHeader->FLG >> 5) & 0x1;
    u8 FLEVEL = (ZLibHeader->FLG >> 6) & 0x3;

    if((((ZLibHeader->CMF * 256) + ZLibHeader->FLG) % 31) != 0)
    {
        Log("((CMF* 256) + FLG) is not a multiple of 31\n");
        return Result;
    }

    if(FDICT)
    {
        ConsumeFile(&At, u32);
    }

    uptr BFINAL = 0;

    while(!BFINAL)
    {
        BFINAL = ConsumeFileBits(&At, 1);
        uptr BTYPE = ConsumeFileBits(&At, 2);

        if(!BTYPE)
        {
            Assert(0);
        }
        else if(BTYPE == 1)
        {
            Assert(0);
        }
        else if(BTYPE == 2)
        {
            uptr HLIT = ConsumeFileBits(&At, 5) + 257;
            uptr HDIST = ConsumeFileBits(&At, 5) + 1;
            uptr HCLEN = ConsumeFileBits(&At, 4) + 4;

            int CLCL[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
            int CL[ArrayCount(CLCL)] = {0};
            for(int Index = 0; Index < HCLEN; Index++)
            {
                CL[CLCL[Index]] = (int)ConsumeFileBits(&At, 3);
            }

            int Index = 0;
        }
        else
        {
            Assert(0);
        }
    }

    ConsumeFile(&At, u32);

    Result = 1;
    return Result;
}