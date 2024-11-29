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
        file PNGFile = {0};
        PlatformReadFile(&PNGFile, "a.png");
        //ParsePNG(&PNGFile);

        u8 Data[] = 
        {
            0xcb, 0x48,	0xcd,	0xc9,	0xc9,	0x57,	0xc8,	0x40,	0x27,	0xb9,	0x00,
        };
        
        //DeflateDecompress(Data, sizeof(Data));

        file At = {0};
        At.Memory = Data;
        At.Size = sizeof(Data);

        uptr R = ConsumeFileBitsMSB(&At, 1);
        R = ReverseBits(R, 1);
        R = ConsumeFileBitsMSB(&At, 2);
        R = ReverseBits(R, 2);
        R = ConsumeFileBitsMSB(&At, 8);
        R = ReverseBits(R, 8);

        EngineState->IsInitialized = 1;
    }
}