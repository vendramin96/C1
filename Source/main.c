#include "main.h"

int main(int ArgumentCount, char **Argument)
{
    file File = {0};
    PlatformReadFile(&File, "a.txt");
    PlatformFreeMemory(File.Memory);

    return 0;
}