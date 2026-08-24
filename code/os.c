
#pragma once

local void* MapExecutable(void* Code, usize CodeSize);

local usize WriteStdOut(void* Data, usize Size);
local usize WriteStdErr(void* Data, usize Size);

local void Exit(u8 ExitCode);

#if defined(__linux__)

#define STDOUT_FILENO (1)
#define STDERR_FILENO (2)

#define PROT_NONE       (0x00)
#define PROT_READ       (0x01)
#define PROT_WRITE      (0x02)
#define PROT_EXEC       (0x04)

#define MAP_PRIVATE     (0x02)
#define MAP_ANONYMOUS   (0x20)

typedef struct
{
    u32 ArgCount;
    char** Args;
} linux_state;

local linux_state LinuxState = {0};

typedef enum
{
    SyscallNumber_Write = 1,
    SyscallNumber_MMap  = 9,
    SyscallNumber_Exit  = 60,
} syscall_number;

local usize LinuxSyscall(
    syscall_number SyscallNumber,
    usize Arg0, usize Arg1, usize Arg2,
    usize Arg3, usize Arg4, usize Arg5
)
{
    usize Result = 0;

    register usize R10 __asm__("r10") = Arg3;
    register usize R8 __asm__("r8") = Arg4;
    register usize R9 __asm__("r9") = Arg5;

    __asm__ volatile (
        "syscall" :
        "=a"(Result) :
        "a"(SyscallNumber),
        "D"(Arg0),
        "S"(Arg1),
        "d"(Arg2),
        "r"(R10),
        "r"(R8),
        "r"(R9) :
        "memory", "rcx", "r11"
    );

    return (Result);
}

local void* MapExecutable(void* Code, usize CodeSize)
{
    ssize MapResult = (ssize)LinuxSyscall(
        SyscallNumber_MMap,
        0,
        CodeSize,
        PROT_READ|PROT_WRITE|PROT_EXEC,
        MAP_PRIVATE|MAP_ANONYMOUS,
        -1,
        0
    );


    void* Result = (void*)Maximum(0, MapResult);

    if (Result)
        CopyMemory(Result, Code, CodeSize);

    return (Result);
}

local usize WriteStdOut(void* Data, usize Size)
{
    ssize Written = (ssize)LinuxSyscall(
        SyscallNumber_Write,
        STDOUT_FILENO,
        (usize)Data,
        Size,
        0, 0, 0
    );

    usize Result = Maximum(0, Written);
    return (Result);
}

local usize WriteStdErr(void* Data, usize Size)
{
    ssize Written = (ssize)LinuxSyscall(
        SyscallNumber_Write,
        STDERR_FILENO,
        (usize)Data,
        Size,
        0, 0, 0
    );

    usize Result = Maximum(0, Written);
    return (Result);
}

local void Exit(u8 ExitCode)
{
    LinuxSyscall(SyscallNumber_Exit, ExitCode, 0, 0, 0, 0, 0);
}

void Main(void);

__attribute__((force_align_arg_pointer))
void EntryPoint(void)
{
    Main();
    Exit(0);
}

#else
#error Unimplemented OS
#endif

