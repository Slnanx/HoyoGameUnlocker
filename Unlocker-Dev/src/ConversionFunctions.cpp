#include "../include/ConversionFunctions.h"

// 将多字节字符转换为宽字符
std::wstring Utf8ToWide(const std::string &str)
{
    // 获取需要的宽字符字符串的长度
    const int wideLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (wideLength == 0)
    {
        // 处理错误情况
        return {};
    }

    // 创建宽字符字符串
    std::wstring wide_str(wideLength, L'\0');
    int convertedLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wide_str[0], wideLength);

    if (convertedLength == 0)
    {
        // 处理错误情况
        wide_str.clear();
    }

    return wide_str;
}