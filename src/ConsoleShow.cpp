#include "..\include\ConsoleShow.h"

// 确保STD_OUTPUT_HANDLE已定义
#ifndef STD_OUTPUT_HANDLE
#define STD_OUTPUT_HANDLE ((DWORD) - 11)
#endif

// 使鼠标滚轮滚动不会导致文本的滚动
void DisableConsoleScroll()
{
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;

    if (GetConsoleScreenBufferInfo(hand, &info))
    {
        // 直接在设置缓冲区大小时进行类型转换
        COORD size = {static_cast<SHORT>(info.srWindow.Right + 1), static_cast<SHORT>(info.srWindow.Bottom + 1)};
        SetConsoleScreenBufferSize(hand, size);
    }
}