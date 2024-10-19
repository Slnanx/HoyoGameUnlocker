#ifndef GET_ADMINISTRATORPRIVILEGES_H
#define GET_ADMINISTRATORPRIVILEGES_H
#include "..\include\SYSHeader.h"

bool ExecuteCmdCommand(const std::string &command);
void GetAdministratorPrivilege();
int DetectAdministratorStatus(std::filesystem::path ProgramPath);
void RestartAsAdministrator(const std::wstring &AppPath);

#endif