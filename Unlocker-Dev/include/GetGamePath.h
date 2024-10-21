#pragma once

#ifndef HEADER_H
#define HEADER_H

#include "SYSHeader.h"

DWORD GetProcessIdByName(const wchar_t *GameName);
std::wstring Utf8ToWide(const std::string &str);
std::filesystem::path GetGamePath(const std::string& GameName);

#endif