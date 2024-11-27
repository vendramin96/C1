#include "dsa.h"

internal_function void
PrintHuffmanTree(huffman_tree *Node, char *String, int Index)
{
    if(!Node)
    {
        return;
    }

    if(!Node->Left && !Node->Right)
    {
        String[Index] = '\0';
        Print("Symbol: %d : %s\n", Node->Symbol, String);
    }

    if(Node->Left != 0)
    {
        String[Index] = '0';
        PrintHuffmanTree((huffman_tree *)Node->Left, String, Index + 1);
    }
    
    if(Node->Right != 0)
    {
        String[Index] = '1';
        PrintHuffmanTree((huffman_tree *)Node->Right, String, Index + 1);
    }
}

internal_function huffman_tree *
CreateHuffmanNode(int Symbol, int Frequency, huffman_tree *Left, huffman_tree *Right)
{
    huffman_tree *Result = 0;

    Result = PlatformAllocateMemory(sizeof(huffman_tree));
    if(!Result)
    {
        Log("Failed to allocate memory for huffman_tree node\n");
        return Result;
    }

    Result->Symbol = Symbol;
    Result->Frequency = Frequency;
    Result->Left = (struct huffman_tree *)Left;
    Result->Right = (struct huffman_tree *)Right;

    return Result;
}

internal_function void
HuffmanPriorityQueueHeapifyUp(huffman_priority_queue *Queue, int Index)
{
    int ParentIndex = (Index - 1) / 2;

    while((Index > 0) && (Queue->Heap[ParentIndex]->Frequency > Queue->Heap[Index]->Frequency))
    {
        huffman_tree *Temporary = Queue->Heap[ParentIndex];
        Queue->Heap[ParentIndex] = Queue->Heap[Index];
        Queue->Heap[Index] = Temporary;
        HuffmanPriorityQueueHeapifyUp(Queue, ParentIndex);
    }
}

internal_function void
HuffmanPriorityQueueHeapifyDown(huffman_priority_queue *Queue, int Index)
{
    int LeftIndex = (2 * Index) + 1;
    int RightIndex = (2 * Index) + 2;
    int Smallest = Index;

    if((LeftIndex < Queue->Size) && (Queue->Heap[LeftIndex]->Frequency < Queue->Heap[Smallest]->Frequency))
    {
        Smallest = LeftIndex;
    }

    if((RightIndex < Queue->Size) && (Queue->Heap[RightIndex]->Frequency < Queue->Heap[Smallest]->Frequency))
    {
        Smallest = RightIndex;
    }

    if(Smallest != Index)
    {
        huffman_tree *Temporary = Queue->Heap[Index];
        Queue->Heap[Index] = Queue->Heap[Smallest];
        Queue->Heap[Smallest] = Temporary;
        HuffmanPriorityQueueHeapifyDown(Queue, Smallest);
    }
}

internal_function void
HuffmanPriorityQueueHeapify(huffman_priority_queue *Queue, int Index)
{
    HuffmanPriorityQueueHeapifyDown(Queue, Index);
}

internal_function huffman_tree *
HuffmanPriorityQueueExtractMin(huffman_priority_queue *Queue)
{
    huffman_tree *Result = 0;

    Result = Queue->Heap[0];
    Queue->Heap[0] = Queue->Heap[Queue->Size - 1];
    Queue->Size--;
    HuffmanPriorityQueueHeapify(Queue, 0);
    
    return Result;
}

internal_function void
HuffmanPriorityQueueInsert(huffman_priority_queue *Queue, huffman_tree *Node)
{
    if(Queue->Size >= Queue->Capacity)
    {
        return;
    }

    Queue->Heap[Queue->Size++] = Node;
    HuffmanPriorityQueueHeapifyUp(Queue, Queue->Size - 1);
}

internal_function bool
HuffmanPriorityQueueAllocate(huffman_priority_queue *Queue, int Capacity)
{
    bool Result = 0;

    
    Queue->Heap = PlatformAllocateMemory(sizeof(void *) * Capacity);
    if(!Queue->Heap)
    {
        Log("Failure to allocate priority_queue memory\n");
        return Result;
    }

    Queue->Capacity = Capacity;
    
    Result = 1;
    return Result;
}

internal_function bool
HuffmanPriorityQueueFree(huffman_priority_queue *Queue)
{
    //@TODO:
    return 0;
}

internal_function huffman_tree *
BuildHuffmanTree(int *Array, int Count)
{
    huffman_tree *Result = 0;

    int Frequency[256] = {0};
    for(int Index = 0; Index < Count; Index++)
    {
        Frequency[Array[Index]]++;
    }
    
    huffman_priority_queue Queue = {0};
    if(!HuffmanPriorityQueueAllocate(&Queue, 4096))
    {
        Log("Failed to initialize priority queue\n");
        return Result;
    }

    for(int Index = 0; Index < ArrayCount(Frequency); Index++)
    {
        if(Frequency[Index] > 0)
        {
            huffman_tree *Node = CreateHuffmanNode(Index, Frequency[Index], 0, 0);
            HuffmanPriorityQueueInsert(&Queue, Node);
        }
    }

    while(Queue.Size > 1)
    {
        huffman_tree *Left = HuffmanPriorityQueueExtractMin(&Queue);
        huffman_tree *Right = HuffmanPriorityQueueExtractMin(&Queue);

        huffman_tree *Node = CreateHuffmanNode(-1, Left->Frequency + Right->Frequency, Left, Right);
        HuffmanPriorityQueueInsert(&Queue, Node);
    }

    Result = HuffmanPriorityQueueExtractMin(&Queue);
    HuffmanPriorityQueueFree(&Queue);
    return Result;
}