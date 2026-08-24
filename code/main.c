
#include "shared.c"
#include "os.c"
#include "print.c"

typedef ssize program_main(void);

void Main(void)
{
    string Code = Str("1337");

    u64 ReturnValue = 0;

    for (usize Index = 0; Index < Code.Size; Index++)
    {
        ReturnValue *= 10;
        ReturnValue += (Code.Data[Index] - '0');
    }

    u8 Assembly[] =
    {
        // NOTE(vak):
        // 48 b8 (Imm64)    mov rax, Imm64

        0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        // NOTE(vak):
        // c3               ret

        0xc3,
    };

    CopyMemory(Assembly + 2, &ReturnValue, 8);

    program_main* ProgramMain = (program_main*)MapExecutable(Assembly, sizeof(Assembly));
    ssize ProgramResult = ProgramMain();

    Print(Str("Program result: "));
    PrintSSize(ProgramResult);
    PrintNewLine();
}

