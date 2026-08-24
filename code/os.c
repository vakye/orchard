
#pragma once

local usize WriteStdOut(void* Data, usize Size);
local usize WriteStdErr(void* Data, usize Size);

local void Exit(u8 ExitCode);

#if defined(__linux__)

#define STDOUT_FILENO (1)
#define STDERR_FILENO (2)

typedef enum
{
    SyscallNumber_Write = 1,
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

local void Main(void);

__attribute__((force_align_arg_pointer))
void EntryPoint(void)
{
    Main();
    Exit(0);
}

#else
#error Unimplemented OS
#endif

