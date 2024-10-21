#pragma once

#ifndef CONVERSIONFUNCTIONS_H
#define CONVERSIONFUNCTIONS_H

#include "SYSHeader.h"

// 将多字节字符转换为宽字符

std::wstring Utf8ToWide(const std::string& str);

#endif // CONVERSIONFUNCTIONS_H
