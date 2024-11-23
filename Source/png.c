#include "png.h"

internal_function bool
ParsePNG(file *PNGFile)
{
    bool Result = 0;

    file At = *PNGFile;
    u64 *Signature = ConsumeFile(&At, u64);

    if(*Signature != 0x0a1a0a0d474e5089)
    {
        Assert(0);
    }

    while(At.Size > 0)
    {
        png_chunk_header *ChunkHeader = ConsumeFile(&At, png_chunk_header);        
        MSBToLSBU32(&ChunkHeader->Length);
        MSBToLSBU32(&ChunkHeader->Type);

        if(ChunkHeader->Length > 0)
        {
            void *ChunkData = ConsumeFileSize(&At, ChunkHeader->Length);
            if(ChunkHeader->Type == 'IHDR')
            {
                png_ihdr *IHDR = (png_ihdr *)ChunkData;
                MSBToLSBU32(&IHDR->Width);
                MSBToLSBU32(&IHDR->Height);
                //@TODO: Allocate memory for the image
            }
            else if(ChunkHeader->Type == 'IDAT')
            {
                ParseZLib(ChunkData, ChunkHeader->Length);
            }
            else if(ChunkHeader->Type == 'IEND')
            {
                break;
            }            
        }

        png_chunk_footer *ChunkFooter = ConsumeFile(&At, png_chunk_footer);
    }

    Assert(!At.Size);

    Result = 1;
    return Result;
}