#include "main.h"

internal_function void
DrawPixel(platform_video *Video, int X, int Y, u32 Color)
{
    //@NOTE: I don't know about this Color...
    if(X < 0)
    {
        X = 0;
    }
    else if(X >= Video->Width)
    {
        X = Video->Width - 1;
    }

    if(Y < 0)
    {
        Y = 0;
    }
    else if(Y >= Video->Height)
    {
        Y = Video->Height - 1;
    }

    u8 *Row = (u8 *)Video->Memory + (X * Video->BytesPerPixel) + (Y * Video->Pitch);
    u32 *Pixel = (u32 *)Row;
    *Pixel = Color;
}

UPDATE_AND_RENDER(UpdateAndRender)
{
    engine_state *EngineState = (engine_state *)Memory->PermanentMemory;
    if(!EngineState->IsInitialized)
    {
        #if 0
        file PNGFile = {0};
        PlatformReadFile(&PNGFile, "a.png");
        ParsePNG(&PNGFile);

        u16 Value = 0x1234;
        file At = {0};
        At.Memory = &Value;
        At.Size = sizeof(u16);
        while(At.Size > 0)
        {
            Print("%d ", ConsumeFileBits(&At, 1));
        }

        Print("\n");
        #endif

        int Array[] = {97, 98, 98, 99, 99, 99, 100, 100, 100, 100, 101, 101, 101, 101, 101, 101, 102, 102, 102, 102, 102, 102, 102};
        char S[100] = {0};
        huffman_tree *Root = BuildHuffmanTree(Array, ArrayCount(Array));
        PrintHuffmanTree(Root, S, 0);

        EngineState->IsInitialized = 1;
    }
}