#include "zlib.h"

internal_function int
ZLibGetLengthFromSymbol(file *File, int Symbol)
{
    uptr Result = 0;

    if(Symbol == 257)
    {
        Result = 3;
    }
    else if(Symbol == 258)
    {
        Result = 4;
    }
    else if(Symbol == 259)
    {
        Result = 5;
    }
    else if(Symbol == 260)
    {
        Result = 6;
    }
    else if(Symbol == 261)
    {
        Result = 7;
    }
    else if(Symbol == 262)
    {
        Result = 8;
    }
    else if(Symbol == 263)
    {
        Result = 9;
    }
    else if(Symbol == 264)
    {
        Result = 10;
    }
    else if(Symbol == 265)
    {
        uptr ExtraBits = ConsumeFileBits(File, 1);
        Result =  11 + ExtraBits;
    }
    else if(Symbol == 266)
    {
        uptr ExtraBits = ConsumeFileBits(File, 1);
        Result =  13 + ExtraBits;
    }    
    else if(Symbol == 267)
    {
        uptr ExtraBits = ConsumeFileBits(File, 1);
        Result =  15 + ExtraBits;
    }
    else if(Symbol == 268)
    {
        uptr ExtraBits = ConsumeFileBits(File, 1);
        Result =  17 + ExtraBits;
    }
    else if(Symbol == 269)
    {
        uptr ExtraBits = ConsumeFileBits(File, 2);
        Result =  19 + ExtraBits;
    }
    else if(Symbol == 270)
    {
        uptr ExtraBits = ConsumeFileBits(File, 2);
        Result =  23 + ExtraBits;
    }
    else if(Symbol == 271)
    {
        uptr ExtraBits = ConsumeFileBits(File, 2);
        Result =  27 + ExtraBits;
    }
    else if(Symbol == 272)
    {
        uptr ExtraBits = ConsumeFileBits(File, 2);
        Result =  31 + ExtraBits;
    }
    else if(Symbol == 273)
    {
        uptr ExtraBits = ConsumeFileBits(File, 3);
        Result =  35 + ExtraBits;
    }
    else if(Symbol == 274)
    {
        uptr ExtraBits = ConsumeFileBits(File, 3);
        Result =  43 + ExtraBits;
    }
    else if(Symbol == 275)
    {
        uptr ExtraBits = ConsumeFileBits(File, 3);
        Result =  51 + ExtraBits;
    }
    else if(Symbol == 276)
    {
        uptr ExtraBits = ConsumeFileBits(File, 3);
        Result =  59 + ExtraBits;
    }
    else if(Symbol == 277)
    {
        uptr ExtraBits = ConsumeFileBits(File, 4);
        Result =  67 + ExtraBits;
    }
    else if(Symbol == 278)
    {
        uptr ExtraBits = ConsumeFileBits(File, 4);
        Result =  83 + ExtraBits;
    }
    else if(Symbol == 279)
    {
        uptr ExtraBits = ConsumeFileBits(File, 4);
        Result =  99 + ExtraBits;
    }
    else if(Symbol == 280)
    {
        uptr ExtraBits = ConsumeFileBits(File, 4);
        Result =  115 + ExtraBits;
    }
    else if(Symbol == 280)
    {
        uptr ExtraBits = ConsumeFileBits(File, 4);
        Result =  115 + ExtraBits;
    }
    else if(Symbol == 281)
    {
        uptr ExtraBits = ConsumeFileBits(File, 5);
        Result =  131 + ExtraBits;
    }
    else if(Symbol == 282)
    {
        uptr ExtraBits = ConsumeFileBits(File, 5);
        Result =  163 + ExtraBits;
    }
    else if(Symbol == 283)
    {
        uptr ExtraBits = ConsumeFileBits(File, 5);
        Result =  195 + ExtraBits;
    }
    else if(Symbol == 284)
    {
        uptr ExtraBits = ConsumeFileBits(File, 5);
        Result =  227 + ExtraBits;
    }
    else if(Symbol == 285)
    {
        Result =  258;
    }

    return (int)Result;
}

internal_function int
ZLibDecodeHuffmanTree(file *File, huffman_tree *HuffmanTree)
{
    int Result = 0;

    huffman_tree *Node = HuffmanTree;

    for(;;)
    {
        uptr Bit = ConsumeFileBits(File, 1);
        if(!Bit)
        {
            Node = (huffman_tree *)Node->Left;
        }
        else
        {
            Node = (huffman_tree *)Node->Right;
        }

        if(!Node->Left && !Node->Right)
        {
            Result = Node->Symbol;
            break;
        }
    }

    return Result;
}

internal_function void
PrintPriorityQueueHeap(priority_queue *Queue)
{
    for(int Index = 0; Index < Queue->Size; Index++)
    {
        Print("%d ", Queue->Heap[Index]->Frequency);
    }

    Print("\n");
}

internal_function void
ZLibPriorityQueueMinHeapifyUp(priority_queue *Queue, int Index)
{
    int ParentIndex = (Index - 1) / 2;
    if((Index > 0) && (Queue->Heap[ParentIndex]->Frequency > Queue->Heap[Index]->Frequency))
    {
        huffman_tree *Temporary = Queue->Heap[Index];
        Queue->Heap[Index] = Queue->Heap[ParentIndex];
        Queue->Heap[ParentIndex] = Temporary;
        ZLibPriorityQueueMinHeapifyUp(Queue, ParentIndex);
    }
}

internal_function void
ZLibPriorityQueueMinHeapifyDown(priority_queue *Queue, int Index)
{
    int LeftChildIndex = (2 * Index) + 1;
    int RightChildIndex = (2 * Index) + 2;
    int Smallest = Index;

    if((LeftChildIndex < Queue->Size) && (Queue->Heap[LeftChildIndex]->Frequency < Queue->Heap[Smallest]->Frequency))
    {
        Smallest = LeftChildIndex;
    }

    if((RightChildIndex < Queue->Size) && (Queue->Heap[RightChildIndex]->Frequency < Queue->Heap[Smallest]->Frequency))
    {
        Smallest = RightChildIndex;
    }

    if(Smallest != Index)
    {
        huffman_tree *Temporary = Queue->Heap[Index];
        Queue->Heap[Index] = Queue->Heap[Smallest];
        Queue->Heap[Smallest] = Temporary;
        Index = Smallest;
        ZLibPriorityQueueMinHeapifyDown(Queue, Index);
    }
}

internal_function void
ZLibPriorityQueueInsert(priority_queue *Queue, huffman_tree *Node)
{
    Queue->Heap[Queue->Size] = Node;
    Queue->Size++;
    ZLibPriorityQueueMinHeapifyUp(Queue, Queue->Size - 1);
}

internal_function huffman_tree *
ZLibPriorityQueueExtractMin(priority_queue *Queue)
{
    huffman_tree *Result = Queue->Heap[0];

    Queue->Heap[0] = Queue->Heap[Queue->Size - 1];
    Queue->Size--;

    ZLibPriorityQueueMinHeapifyDown(Queue, 0);

    return Result;
}

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
        if(CL[Index] != 0)
        {
            huffman_tree *Node = ZLibCreateHuffmanNode(Index, CL[Index], 0, 0);
            ZLibPriorityQueueInsert(Queue, Node);
        }
    }    

    while(Queue->Size > 1)
    {
        huffman_tree *LeftChildNode = ZLibPriorityQueueExtractMin(Queue);
        huffman_tree *RightChildNode = ZLibPriorityQueueExtractMin(Queue);
        huffman_tree *ParentNode = ZLibCreateHuffmanNode(-1, LeftChildNode->Frequency + RightChildNode->Frequency, LeftChildNode, RightChildNode);
        ZLibPriorityQueueInsert(Queue, ParentNode);
    }

    Result = ZLibPriorityQueueExtractMin(Queue);
    return Result;
}

internal_function bool
ZLibInitializePriorityQueue(priority_queue *Queue, int Capacity)
{
    bool Result = 0;

    Queue->Heap = PlatformAllocateMemory(sizeof(huffman_tree *) * Capacity);
    if(!Queue->Heap)
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
            huffman_tree *CodeLengthTree =  ZLibBuildHuffmanTree(&PriorityQueue, CL, ArrayCount(CL));

            int LengthCode[286] = {0};            
            for(int Index = 0; Index < HLIT; Index++)
            {
                LengthCode[Index] = ZLibDecodeHuffmanTree(&At, CodeLengthTree);
            }

            int DistCode[32] = {0};
            for(int Index = 0; Index < HDIST; Index++)
            {
                DistCode[Index] = ZLibDecodeHuffmanTree(&At, CodeLengthTree);
            }

            PriorityQueue.Size = 0;
            huffman_tree *DistanceTree = ZLibBuildHuffmanTree(&PriorityQueue, DistCode, ArrayCount(DistCode));

            int *Output = PlatformAllocateMemory(Megabytes(32));
            int OutputIndex = 0;
            for(int Index = 0; Index < (HLIT + HDIST); Index++)
            {
                
            }

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