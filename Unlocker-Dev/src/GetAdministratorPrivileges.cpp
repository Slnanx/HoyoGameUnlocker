#include "../include/GetAdministratorPrivileges.h"
#include "../include/SYSHeader.h"

// 使用CreateProcess()函数执行cmd命令
bool ExecuteCmdCommand(const std::string &command)
{
    // 使用Unicode版本的STARTUPINFO结构体
    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};

    si.cb = sizeof(si);

    // 构造完整的命令行
    const std::string FullCommand = "cmd /c " + command;

    // 将std::string转换为宽字符串
    const int WideLength = MultiByteToWideChar(CP_UTF8, 0, FullCommand.c_str(), -1, nullptr, 0);
    std::vector<wchar_t> WideCommand(WideLength);
    MultiByteToWideChar(CP_UTF8, 0, FullCommand.c_str(), -1, WideCommand.data(), WideLength);

    // 创建进程
    if (!CreateProcessW(nullptr,               // 应用程序名称
                        WideCommand.data(), // 命令行（宽字符串）
                        nullptr,               // 进程句柄不可继承
                        nullptr,               // 线程句柄不可继承
                        FALSE,              // 设置句柄继承选项
                        0,                  // 没有创建标志
                        nullptr,               // 使用父进程的环境块
                        nullptr,               // 使用父进程的当前目录
                        &si,                // 指向STARTLING结构的指针
                        &pi)                // 指向PROCESS_INFORMATION结构的指针
    )
    {
        std::cerr << "CreateProcessW failed (" << GetLastError() << ").\n";
        return false;
    }

    // 等待子进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 关闭进程和线程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

// 尝试自动获取管理员权限，获取失败则提示用户手动选择以管理员模式运行（供调用）
void GetAdministratorPrivilege()
{
    constexpr size_t InitialSize = 65536; // 防止文件名特别长，增加数组大小
    auto *TempProgramPath = new wchar_t[InitialSize];
    // 使用Unicode版本的GetModuleFileName函数
    GetModuleFileNameW(nullptr, TempProgramPath, InitialSize); // 获取程序的完整路径名（宽字符串）
    const std::filesystem::path ProgramPath(TempProgramPath);     // 使用std::filesystem::path处理宽字符串路径

    delete[] TempProgramPath; // 释放数组内存

    if (!std::filesystem::exists(ProgramPath))
    {
        MessageBoxW(nullptr, L"自动获取管理员权限失败，请手动选择以管理员模式运行！", L"错误", MB_ICONWARNING);
        exit(1);
    }

    DetectAdministratorStatus(ProgramPath);
}
// 检测当前是否为管理员，如果不是，就调用RestartAsAdministrator(std::wstring)尝试以管理员身份重启Unlocker
std::optional<int> DetectAdministratorStatus(const std::filesystem::path& ProgramPath)
{
    BOOL IsAdmin = FALSE; // 用于存储当前管理员状态
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID pAdminGroup = nullptr;
    if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminGroup))
    {
        std::cerr << "AllocateAndInitializeSid failed (" << GetLastError() << ").\n";
        return std::nullopt;
    }
    else
    {
        if (!CheckTokenMembership(nullptr, pAdminGroup, &IsAdmin))
        {
            std::cerr << "CheckTokenMembership failed (" << GetLastError() << ").\n";
            return std::nullopt;
        }
        FreeSid(pAdminGroup);
    }

    if (!IsAdmin) // 检测是否为管理员
    {
        RestartAsAdministrator(ProgramPath.wstring());
        exit(0);
    }
    return 0;
}
// 以管理员身份重启Unlocker
void RestartAsAdministrator(const std::wstring &AppPath)
{
    SHELLEXECUTEINFOW sei = {sizeof(SHELLEXECUTEINFOW)};
    sei.lpVerb = L"runas";
    sei.lpFile = AppPath.c_str();
    sei.hwnd = nullptr;
    sei.nShow = SW_NORMAL;

    if (!ShellExecuteExW(&sei))
    {
        if (const DWORD dwError = GetLastError(); dwError == ERROR_CANCELLED)
        {
            std::wcout << L"User refused to grant administrative privileges." << std::endl;
        }
        else
        {
            std::cerr << "Failed to execute: " << dwError << std::endl;
        }
    }
}
