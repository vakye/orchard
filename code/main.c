
#include "shared.c"
#include "os.c"
#include "print.c"

typedef ssize program_main(void);

local b32 IsDigit(char Character)
{
    b32 Result = (Character >= '0') && (Character <= '9');
    return (Result);
}

local ssize CompileAndRun(string Code)
{
    u64 ValueA = 0;
    char Operator = 0;
    u64 ValueB = 0;

    usize Index = 0;

    for (; Index < Code.Size; Index++)
    {
        if (!IsDigit(Code.Data[Index]))
            break;

        ValueA *= 10;
        ValueA += (Code.Data[Index] - '0');
    }

    Operator = Code.Data[Index++];

    for (; Index < Code.Size; Index++)
    {
        if (!IsDigit(Code.Data[Index]))
            break;

        ValueB *= 10;
        ValueB += (Code.Data[Index] - '0');
    }

    u8 Assembly[] =
    {
        // NOTE(vak):
        // 48 b8 (Imm64)    mov rax, Imm64

        0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        // NOTE(vak):
        // 48 b9 (Imm64)    mov rcx, Imm64

        0x48, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        // NOTE(vak):
        // 48 .. c1         Inst rax, rcx

        0x48, 0x00, 0xc1,

        // NOTE(vak):
        // c3               ret

        0xc3,
    };

    CopyMemory(Assembly + 2,  &ValueA, 8);
    CopyMemory(Assembly + 12, &ValueB, 8);

    switch (Operator)
    {
        default:
        case '+': Assembly[21] = 0x03; break; // NOTE(vak): add r64, r/m64 opcode
        case '-': Assembly[21] = 0x2b; break; // NOTE(vak): sub r64, r/m64 opcode
    }

    program_main* ProgramMain = (program_main*)MapExecutable(Assembly, sizeof(Assembly));
    ssize ProgramResult = ProgramMain();

    return (ProgramResult);
}

local void Test(ssize Expected, string Code)
{
    if (Expected != CompileAndRun(Code))
        Print(Str("FAILED: "));
    else
        Print(Str("PASSED: "));

    Print(Str("'"));
    Print(Code);
    Print(Str("'"));
    PrintNewLine();
}

void Main(void)
{
    Test(0,     Str("0"));
    Test(123,   Str("123"));
    Test(10,    Str("3+7"));
    Test(1234,  Str("1000+234"));
    Test(5,     Str("7-2"));
    Test(766,   Str("1000-234"));
}

