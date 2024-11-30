#include "deflate.h"

internal_function huffman_tree *
DeflateBuildHuffmanTree(int *Array, int Count)
{
    return 0;
}

internal_function bool
DeflateDecompress(void *Data, uptr Size)
{
    bool Result = 0;

    file At = {0};
    At.Memory = Data;
    At.Size = Size;
#if 0
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
#endif
    uptr BFINAL = 0;

    while(!BFINAL)
    {
        BFINAL = ConsumeFileBitsMSBReversed(&At, 1);
        uptr BTYPE = ConsumeFileBitsMSBReversed(&At, 2);

        if(!BTYPE)
        {
            ConsumeFileBitsMSBReversed(&At, 5);
            u16 Length = (u16)ConsumeFileBitsMSBReversed(&At, 16);
            u16 NotLength = (u16)ConsumeFileBitsMSBReversed(&At, 16);

            if(Length != (u16)~NotLength)
            {
                Log("Length and one's complement of Length mismatch\n");
                return Result;
            }

            ConsumeFileSize(&At, Length); //@TODO: Copy to output buffer
        }
        else if(BTYPE == 1)
        {
            int LoopCount = 0;
            for(;;)
            {
                uptr Code = ConsumeFileBitsMSBReversed(&At, 8);
                if(Code < 256)
                {
                    int i = 0;
                }
                else if((Code >= 257) && (Code <= 285))
                {
                    int i = 0;
                }
                else if(Code == 256)
                {
                    break;
                }
                else
                {
                    return Result;
                }

                int i = 0;
            }
        }
        else if(BTYPE == 2)
        {
            Assert(0);
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