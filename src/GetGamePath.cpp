#include "..\include\GetGamePath.h"

// 将多字节字符转换为宽字符

std::wstring Utf8ToWide(const std::string &str)
{
    // 获取需要的宽字符字符串的长度
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (wideLength == 0)
    {
        // 处理错误情况
        return std::wstring();
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

// 根据游戏名称获取进程ID
DWORD GetProcessIdByName(const wchar_t *GameName)
{
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W pe; // 使用宽字符版本的结构体
        pe.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(hSnapshot, &pe))
        {
            do
            {
                if (_wcsicmp(pe.szExeFile, GameName) == 0)
                { // 使用不区分大小写的比较
                    pid = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe)); // 使用宽字符版本的函数
        }
        CloseHandle(hSnapshot);
    }
    return pid;
}

// 获取游戏的可执行文件路径
std::filesystem::path GetGamePath(std::string GameName)
{
    // 获得游戏的PID
    DWORD GamePid = GetProcessIdByName(Utf8ToWide(GameName).c_str());
    if (GamePid == 0)
    {
        return {}; // 如果游戏未启动，返回一个空的path对象
    }

    // 通过游戏PID获取进程句柄
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GamePid);
    if (hProcess == NULL)
    {
        std::cerr << "无法打开进程！错误代码：" << GetLastError() << std::endl;
        return {}; // 如果无法打开进程，返回一个空的path对象
    }

    std::filesystem::path executablePath;
    HMODULE hMod;
    DWORD cbNeeded;

    // 遍历进程中的所有模块
    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
    {
        // 获取模块的完整路径
        TCHAR szFileName[MAX_PATH] = {0};
        if (GetModuleFileNameEx(hProcess, hMod, szFileName, MAX_PATH))
        {
            executablePath = szFileName;
        }
        else
        {
            std::cerr << "无法获取模块文件名！错误代码：" << GetLastError() << std::endl;
        }
    }
    else
    {
        std::cerr << "无法枚举进程模块！错误代码：" << GetLastError() << std::endl;
    }

    // 关闭进程句柄
    CloseHandle(hProcess);
    return executablePath;
}