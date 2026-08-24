
#pragma once

local usize PrintCharacter(char Character)
{
    usize Result = WriteStdOut(&Character, 1);
    return (Result);
}

local usize PrintNewLine(void)
{
    usize Result = PrintCharacter('\n');
    return (Result);
}

local usize Print(string Message)
{
    usize Result = WriteStdOut(Message.Data, Message.Size);
    return (Result);
}

local usize Println(string Message)
{
    usize Result = 0;

    Result += Print(Message);
    Result += PrintNewLine();

    return (Result);
}

local usize PrintUSize(usize Value)
{
    char Digits[64] = {0};
    usize DigitIndex = ArrayCount(Digits);
    usize DigitCount = 0;

    do
    {
        char Digit = '0' + (char)(Value % 10);
        Value /= 10;

        DigitIndex--;
        DigitCount++;

        Digits[DigitIndex] = Digit;
    } while (Value);

    usize Written = Print(StrData(Digits + DigitIndex, DigitCount));
    return (Written);
}

local usize PrintSSize(ssize Value)
{
    usize Written = 0;

    if (Value < 0)
    {
        Written += PrintCharacter('-');
        Value = -Value;
    }

    Written += PrintUSize(Value);
    return (Written);
}

