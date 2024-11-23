#include "Header/definitions.h"
#include "Library/platform.h"

#include "Code/string.c"

internal_function void
MinHeapify(int *Array, int ArraySize, int Index)
{
}

internal_function void
BuildHuffmanTree()
{

}

internal_function void
HuffmanEncoding(char *StringToEncode, int StringToEncodedSize, char *StringToDecode, int StringToDecodeSize)
{
    // count the frequency of letters
    int Frequency[256] = {0};
    int FrequencyIndex = 0;
    for(int I = 1; I < ArrayCount(Frequency); I++)
    {
        for(int J = 0; J < StringLength(StringToEncode); J++)
        {
            if(I == StringToEncode[J])
            {
                Frequency[I]++;
            }
        }
    }

    // build the huffman tree starting with the lowest letter frequency and adding its frequency count to parent node
    BuildHuffmanTree();
}

internal_function void
HuffmanDecode(char *StringDecoded, int StringDecodedSize)
{
}

int main(int ArgumentCount, char **Argument)
{
    char *StringToEncode = "lossless";
    char StringToDecode[100] = {0};
    HuffmanEncoding(StringToEncode, sizeof(StringToEncode), StringToDecode, sizeof(StringToDecode));

    char StringDecoded[100];
    HuffmanDecode(StringDecoded, sizeof(StringDecoded));

    int Value[] = { 10, 5, 15, 2, 20, 30 };

    Print("Value[]: ");
    for(int Index = 0; Index < ArrayCount(Value); Index++)
    {
        Print("%d ", Value[Index]);
    }
    Print("\n\n");

    for(int Index = (ArrayCount(Value) / 2) - 1; Index >= 0; Index--)
    {
        MinHeapify(Value, ArrayCount(Value), Index);
    }

    Print("Value[]: ");
    for(int Index = 0; Index < ArrayCount(Value); Index++)
    {
        Print("%d ", Value[Index]);
    }
    Print("\n\n");

    return 0;
}