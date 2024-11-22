#include "main.h"

typedef struct
{
    bool IsLast;
    int Distance;
    int Length;
    int Character;
} lz77;

#include "Code/string.c"

internal_function void
LZ77Compress(lz77 *LZ77, uptr LZ77Count, char *String)
{
    char *LookaheadBuffer = String;
    char *SearchBuffer = LookaheadBuffer;
    int SearchBufferLength = 0;
    int LZ77Index = 0;
    // loop through the input string
    while(*LookaheadBuffer != '\0')
    {
        bool Match = 0;
        int Distance = 0;
        int Length = 0;
        char Character = '\0';
        // loop through the SearchBuffer backwards
        for(int I = SearchBufferLength - 1; I >= 0; I--)
        {
            int distance = 0;
            int length = 0;
            int J = I;
            int K = 0;
            // Loop forward now until there is not match
            int SearchBufferLengthExtended = SearchBufferLength;
            while((J < SearchBufferLengthExtended) && (K < StringLength(LookaheadBuffer)) && (SearchBuffer[J] == LookaheadBuffer[K]))
            {
                Match = 1;

                if(!length)
                {
                    distance = SearchBufferLengthExtended - J;
                }
                SearchBufferLengthExtended++;
                length++;
                J++;
                K++;
            }

            if(length > Length)
            {
                Distance = distance;
                Length = length;
            }
        }
        
        if(!Match)
        {
            Distance = 0;
            Length = 0;
            Character = LookaheadBuffer[0];
            SearchBufferLength++;
            LookaheadBuffer++;
        }
        else
        {
            Character = LookaheadBuffer[Length];
            SearchBufferLength += Length + 1;
            LookaheadBuffer += Length;
            if(Character != '\0')
            {
                LookaheadBuffer++;
            }
            else
            {
                Character = '-';
            }
        }        

        //Print("> (%d, %d, %c)\n", Distance, Length, Character);
        if((Character == '\0') || (Character == '-'))
        {
            LZ77[LZ77Index].IsLast = 1;
        }
        LZ77[LZ77Index].Distance = Distance;
        LZ77[LZ77Index].Length = Length;
        LZ77[LZ77Index].Character = Character;
        LZ77Index++;
    }
}

internal_function void
LZ77Decompress(lz77 *LZ77, int LZ77Count)
{
    char Output[512] = {0};
    uptr OutputLength = 0;
    for(int I = 0; I < LZ77Count; I++)
    {
        if(!LZ77[I].Distance && !LZ77[I].Length)
        {
            OutputLength += CopyCharacterToBuffer(Output + OutputLength, sizeof(Output) - OutputLength, LZ77[I].Character);
            Output[OutputLength] = '\0';
            continue;
        }

        int K = (int)StringLength(Output);
        for(int J = 0; J < LZ77[I].Length; J++)
        {
            OutputLength += CopyCharacterToBuffer(Output + OutputLength, sizeof(Output) - OutputLength, Output[K - LZ77[I].Distance]);
            Output[OutputLength] = '\0';
            K++;
        }        

        OutputLength += CopyCharacterToBuffer(Output + OutputLength, sizeof(Output) - OutputLength, LZ77[I].Character);
        Output[OutputLength] = '\0';

        if(LZ77[I].IsLast)
        {
            break;
        }
    }

    if(Output[OutputLength - 1] == '-')
    {
        Output[OutputLength - 1] = '\0';
    }
    
    Print("%s\n", Output);
    Print("\n\n");
}

int main(int ArgumentCount, char **Argument)
{
    file File = {0};
    PlatformReadFile(&File, "a.txt");
    PlatformFreeMemory(File.Memory);

    lz77 LZ77[512] = {0};
    char *String = "ABRACADABRA";
    char *String2 = "abcbbcbaaaaaa";
    char *String3 = "ABABABABA";
    char *String4 = "ABABABABACDABABABA";
    LZ77Compress(LZ77, ArrayCount(LZ77), String4); 
    LZ77Decompress(LZ77, ArrayCount(LZ77));

    #if 0
    Print("\n====================\n\n");
    LZ77Compress(LZ77, ArrayCount(LZ77), String2);
    Print("\n====================\n\n");
    LZ77Compress(LZ77, ArrayCount(LZ77), String3);
    Print("\n====================\n\n");
    
    //(0, 0, A), (0, 0, B), (1, 1, B), (2, 2, A), (3, 3, B), (4, 4, A), (5, 5, B), (6, 6, A), (7, 7, B)

    
    
    return 0;
    #endif
}