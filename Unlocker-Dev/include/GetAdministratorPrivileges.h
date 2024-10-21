// 尝试自动获取管理员权限
#pragma once
#ifndef GET_ADMINISTRATORPRIVILEGES_H
#define GET_ADMINISTRATORPRIVILEGES_H

#include "SYSHeader.h"

bool ExecuteCmdCommand(const std::string &command);
void GetAdministratorPrivilege();
std::optional<int> DetectAdministratorStatus(const std::filesystem::path& ProgramPath);
void RestartAsAdministrator(const std::wstring &AppPath);

#endif