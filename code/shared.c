
#pragma once

#define local static
#define persist static

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define Minimum(A, B) ((A) < (B) ? (A) : (B))
#define Maximum(A, B) ((A) > (B) ? (A) : (B))

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef s64 ssize;
typedef u64 usize;

typedef float f32;
typedef double f64;

typedef u8 b8;
typedef u32 b32;

#define true (1)
#define false (0)

// NOTE(vak): Need to provide memcpy/memset for compiler since we don't
// rely on the CRT.

void* memset(void* DestInit, s32 Byte, usize Size)
{
    u8* Dest = (u8*)DestInit;

    while (Size--)
        *Dest++ = Byte;

    return (DestInit);
}

void* memcpy(void* DestInit, void* SourceInit, usize Size)
{
    u8* Dest = (u8*)DestInit;
    u8* Source = (u8*)SourceInit;

    while (Size--)
        *Dest++ = *Source++;

    return (DestInit);
}

#define ZeroType(Pointer)           ZeroMemory(Pointer, sizeof(*(Pointer)))
#define ZeroArray(Pointer, Count)   ZeroMemory(Pointer, sizeof(*(Pointer)) * (Count))

local void ZeroMemory(void* DestInit, usize Size)                   { memset(DestInit, 0, Size); }
local void FillMemory(void* DestInit, u8 Byte, usize Size)          { memset(DestInit, Byte, Size); }
local void CopyMemory(void* DestInit, void* SourceInit, usize Size) { memcpy(DestInit, SourceInit, Size); }

typedef struct
{
    char* Data;
    usize Size;
} string;

#define StaticStr(Literal) {Literal, sizeof(Literal) - 1}
#define StaticStrData(Data, Size) {Data, Size}

#define Str(Literal) (string){Literal, sizeof(Literal) - 1}
#define StrData(Data, Size) (string){Data, Size}

