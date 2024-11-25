#include "zlib.h"

internal_function huffman_tree *
ZLibCreateHuffmanNode(int Symbol, int Frequency, huffman_tree *Left, huffman_tree *Right)
{
    huffman_tree *Result = 0;

    Result = PlatformAllocateMemory(sizeof(huffman_tree));
    if(!Result)
    {
        return Result;
    }

    Result->Symbol = Symbol;
    Result->Frequency = Frequency;
    Result->Left = (struct huffman_tree *)Left;
    Result->Right = (struct huffman_tree *)Right;

    return Result;
}

internal_function huffman_tree *
ZLibBuildHuffmanTree(priority_queue *Queue, int *CL, int CLCount)
{
    huffman_tree *Result = 0;

    for(int Index = 0; Index < CLCount; Index++)
    {
        Queue->Array[Index].Frequency;
    }

    return Result;
}

internal_function bool
ZLibInitializePriorityQueue(priority_queue *Queue, int Capacity)
{
    bool Result = 0;

    Queue->Array = PlatformAllocateMemory(sizeof(priority_queue) * Capacity);
    if(!Queue->Array)
    {
        return Result;
    }

    Queue->Capacity = Capacity;

    Result = 1;
    return Result;
}

internal_function bool
ZLibParse(void *Data, uptr Size)
{
    bool Result = 0;

    file At = {0};
    At.Memory = Data;
    At.Size = Size;

    zlib_header *ZLibHeader = ConsumeFile(&At, zlib_header);
    u8 CM = ZLibHeader->CMF & 0xF;
    u8 CINFO = (ZLibHeader->CMF >> 4) & 0xF;
    if(CM != 8)
    {
        Log("Compression method is %d\n", CM);
        return Result;
    }
    else if(CINFO > 7)
    {
        Log("Compression info is %d\n", CINFO);
        return Result;
    }

    u8 FCHECK = ZLibHeader->FLG & 0x1F;
    u8 FDICT = (ZLibHeader->FLG >> 5) & 0x1;
    u8 FLEVEL = (ZLibHeader->FLG >> 6) & 0x3;
    if((((ZLibHeader->CMF * 256) + ZLibHeader->FLG) % 31) != 0)
    {
        Log("(CMF*256) + FLG is not a multiple of 31\n");
        return Result;
    }

    if(FDICT)
    {
        ConsumeFile(&At, u32); //@TODO: Test this with ConsumeFileBits
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

            if(!((HLIT >= 257) && (HLIT <= 286)))
            {
                Log("HLIT is %d", HLIT);
                return Result;
            }
            else if(!((HDIST >= 1) && (HDIST <= 32)))
            {
                Log("HDIST is %d", HDIST);
                return Result;
            }
            else if(!((HCLEN >= 4) && (HCLEN <= 19)))
            {
                Log("HCLEN is %d", HCLEN);
                return Result;
            }

            uptr CLCL[ZLIB_CLCL_COUNT] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
            int CL[ArrayCount(CLCL)] = {0};
            for(int Index = 0; Index < HCLEN; Index++)
            {
                CL[CLCL[Index]] = (int)ConsumeFileBits(&At, 3);
            }

            priority_queue PriorityQueue = {0};
            ZLibInitializePriorityQueue(&PriorityQueue, 4096);            
            ZLibBuildHuffmanTree(&PriorityQueue, CL, ArrayCount(CL));

            int j = 0;
        }
        else
        {
            Log("BTYPE is %d", BTYPE);
            return Result;
        }
    }

    ConsumeFile(&At, u32);

    return Result;
}