/*
RFC 1951
* SearchBuffer of 32K?
* Bit sequency == code

* The block sizes are arbitrary, except that non-compressible blocks are limited to 65,535 bytes.
* The Huffman trees for each block are independent of those for previous or subsequent blocks;
* pointers to duplicated strings, where a pointer is represented as a pair <length, backward distance>
* <Length, Distance> Length <= 258, Distance <= 32K. No limitation for block size, except uncompressible blocks.
* Each type of value (literals, distances, and lengths) in the compressed data is represented using a Huffman code, using one code
  tree for literals and lengths and a separate code tree for distances. The code trees for each block appear in a compact form just before
  the compressed data for that block.
* since we write numbers with the most-significant digit on the left, we also write bytes with the most-significant bit on the left.
* Within a computer, a number may occupy multiple bytes.  
  All multi-byte numbers in the format described here are stored with the least-significant byte first (at the lower memory address).
* Data elements are packed into bytes in order of increasing bit number within the byte, i.e., starting with the least-significant bit of the byte.
  Data elements other than Huffman codes are packed starting with the least-significant bit of the data element.
  Huffman codes are packed starting with the most-significant bit of the code.
* In other words, if one were to print out the compressed data as
  a sequence of bytes, starting with the first byte at the
  *right* margin and proceeding to the *left*, with the most-
  significant bit of each byte on the left as usual, one would be
  able to parse the result from right to left, with fixed-width
  elements in the correct MSB-to-LSB order and Huffman codes in
  bit-reversed order (i.e., with the first bit of the code in the
  relative LSB position).
* Prefix coding represents symbols from an a priori known
  alphabet by bit sequences (codes), one code for each symbol, in
  a manner such that different symbols may be represented by bit
  sequences of different lengths, but a parser can always parse
  an encoded string unambiguously symbol-by-symbol.
* We define a prefix code in terms of a binary tree in which the
  two edges descending from each non-leaf node are labeled 0 and
  1 and in which the leaf nodes correspond one-for-one with (are
  labeled with) the symbols of the alphabet; then the code for a
  symbol is the sequence of 0's and 1's on the edges leading from
  the root to the leaf labeled with that symbol.
* Additional rules for huffman code: 
  * All codes of a given bit length have lexicographically consecutive values, in the same order as the symbols they represent;
  * Shorter codes lexicographically precede longer codes.
*
*/

#include "zlib.h"

internal_function bool
ZLibPriorityQueueIsEmpty(priority_queue *Queue)
{
    bool Result = 0;

    Result = Queue->Capacity == 0;
    return Result;
}

internal_function huffman_tree *
ZLibPriorityQueueExtractMin(priority_queue *Queue)
{
    huffman_tree *Result = 0;

    if(ZLibPriorityQueueIsEmpty(Queue))
    {
        return Result;
    }

    Result = Queue->Heap[0];

    huffman_tree *LastNode = Queue->Heap[Queue->Size - 1];
    Queue->Heap[0] = LastNode;
    Queue->Size--;

    int Index = 0;
    while(Index < Queue->Size)
    {
        int LeftChildIndex = (2 * Index) + 1;
        int RightChildIndex = (2 * Index) + 2;
        int Smallest = Index;

        if((LeftChildIndex < Queue->Size) && (Queue->Heap[LeftChildIndex]->Length < Queue->Heap[Smallest]->Length))
        {
            Smallest = LeftChildIndex;
        }

        if((RightChildIndex < Queue->Size) && (Queue->Heap[RightChildIndex]->Length < Queue->Heap[Smallest]->Length))
        {
            Smallest = RightChildIndex;
        }

        if(Smallest == Index)
        {
            break;
        }

        huffman_tree *Temporary = Queue->Heap[Index];
        Queue->Heap[Index] = Queue->Heap[Smallest];
        Queue->Heap[Smallest] = Temporary;
        Index = Smallest;
    }
    
    return Result;
}

internal_function void
ZLibPriorityQueueInsert(priority_queue *Queue, huffman_tree *Value)
{
    Queue->Heap[Queue->Size++] = Value;
    int Index = (int)(Queue->Size - 1);
    int ParentIndex = (Index - 1) / 2;
    while((Index > 0) && (Queue->Heap[Index] < Queue->Heap[ParentIndex]))
    {
        huffman_tree *Temporary = Queue->Heap[Index];
        Queue->Heap[Index] = Queue->Heap[ParentIndex];
        Queue->Heap[ParentIndex] = Temporary;
        Index = ParentIndex;
        ParentIndex = (Index - 1) / 2;
    }
}

internal_function huffman_tree *
ZLibCreateHuffmanNode(int Symbol, int Length, huffman_tree *Left, huffman_tree *Right)
{
    huffman_tree *Result = 0;

    Result = PlatformAllocateMemory(sizeof(huffman_tree))    ;
    if(!Result)
    {
        return Result;
    }

    Result->Symbol = Symbol;
    Result->Length = Length;
    Result->Left = (struct huffman_tree *)Left;
    Result->Right = (struct huffman_tree *)Right;    

    return Result;
}

internal_function void
ZLibBuildHuffmanTree(huffman_tree *HuffmanTree, uptr *CL, int CLCount)
{
    uptr Max = CL[0];
    for(int Index = 1; Index < CLCount; Index++)
    {
        if(CL[Index] > Max)
        {
            Max = CL[Index];
        }
    }

    uptr Buckets[ZLIB_CLCL_COUNT + 1] = {0};
    for(int Symbol = 0; Symbol < CLCount; Symbol++)
    {
        uptr Length = CL[Symbol];
        if(Length > 0)
        {
            Buckets[Length] = Symbol;
        }
    }

    priority_queue PriorityQueue = {0};
    PriorityQueue.Capacity = 512*4;
    PriorityQueue.Heap = PlatformAllocateMemory(sizeof(priority_queue) * PriorityQueue.Capacity);
    for(int Length = 1; Length <= ZLIB_CLCL_COUNT; Length++)
    {
        for(int Symbol = 0; Symbol < ArrayCount(Buckets); Symbol++)
        {
            huffman_tree *Node = ZLibCreateHuffmanNode(Symbol, Length, 0, 0);
            ZLibPriorityQueueInsert(&PriorityQueue, Node);
            PriorityQueue.Size++;
        }
    }
    
    while(PriorityQueue.Size > 1)
    {
        huffman_tree *LeftNode = ZLibPriorityQueueExtractMin(&PriorityQueue);
        huffman_tree *RightNode = ZLibPriorityQueueExtractMin(&PriorityQueue);

        huffman_tree *ParentNode = ZLibCreateHuffmanNode(0, 0, LeftNode, RightNode);
    }

    int j = 0;
}

internal_function bool
ZLibParse(void *Data, uptr DataSize)
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

    u8 BFINAL = (u8)ConsumeFileBitsSize(&At, 1);
    u8 BTYPE = (u8)ConsumeFileBitsSize(&At, 2);

    while(!BFINAL)
    {
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
            // read representation of code trees
            uptr HLIT = ConsumeFileBitsSize(&At, 5) + 257;
            uptr HDIST = ConsumeFileBitsSize(&At, 5) + 1;
            uptr HCLEN = ConsumeFileBitsSize(&At, 4) + 4;

            Assert((HLIT >= 257) && (HLIT <= 286));
            Assert((HDIST >= 1) && (HDIST <= 32));
            Assert((HCLEN >= 4) && (HCLEN <= 19));

            int CLCL[ZLIB_CLCL_COUNT] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

            uptr CodeLength[ZLIB_CLCL_COUNT] = {0};
            for(int Index = 0; Index < HCLEN; Index++)
            {
                CodeLength[CLCL[Index]] = ConsumeFileBitsSize(&At, 3);
            }

            huffman_tree CLHuffmanTree = {0}; 
            ZLibBuildHuffmanTree(&CLHuffmanTree, CodeLength, ArrayCount(CodeLength));

            int j = 0;
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