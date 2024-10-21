#include "../include/ModuleFinder.h"

bool ModuleFinder::GetModule(DWORD Pid, const std::wstring &ModuleName, MODULEENTRY32W &Entry)
{
    if (ModuleName.empty())
    {
        return false;
    }

    // 初始化MODULEENTRY32W结构体，准备获取模块信息
    MODULEENTRY32W Mod32{};
    Mod32.dwSize = sizeof(MODULEENTRY32W);

    // 创建一个快照，包含指定进程ID的所有模块
    // 使用std::unique_ptr来自动管理快照句柄的生命周期
    std::unique_ptr<void, decltype(&CloseHandle)> Snap(
        CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid),
        CloseHandle);

    // 如果快照创建失败，就返回false
    if (!Snap)
    {
        return false;
    }

    // 初始化一个标志，用于标记是否找到模块
    bool Found = false;

    // 使用Module32FirstW函数获取快照中的第一个模块信息
    if (Module32FirstW(Snap.get(), &Mod32))
    {
        // 遍历快照中的所有模块
        do
        {
            // 使用wcscmp比较宽字符串
            if (Mod32.th32ProcessID == Pid && wcscmp(Mod32.szModule, ModuleName.c_str()) == 0)
            {
                // 将找到的模块信息复制到传入的Entry参数中
                Entry = Mod32;
                Found = true;
                break; // 跳出循环
            }
        } while (Module32NextW(Snap.get(), &Mod32));
    }

    return Found;
}
