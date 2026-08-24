
#include "shared.c"
#include "os.c"
#include "print.c"

local void Main(void)
{
    Println(Str("Hello, world!"));

    Print(Str("Printing integers:"));
    PrintNewLine();

    Print(Str("    ")); PrintUSize(123456789); PrintNewLine();
    Print(Str("    ")); PrintSSize(-124); PrintNewLine();
    Print(Str("    ")); PrintSSize(52387); PrintNewLine();
    Print(Str("    ")); PrintUSize(1024); PrintNewLine();
}

