
#include "shared.c"
#include "os.c"

local void Main(void)
{
    char Message[] = "Hello, world!\n";

    WriteStdOut(Message, sizeof(Message) - 1);
}

