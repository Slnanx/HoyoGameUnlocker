#include "../include/ConfigurationIO.h"
#include "../include/ConsoleShow.h"
#include "../include/GameRunningStatus.h"
#include "../include/GetAdministratorPrivileges.h"
int main()
{
    SetConsoleTitleW(L"HoyoFPSUnlocker");

    if (CheckGameRunningStatus())
    {
        MessageBoxW(nullptr,L"游戏已经在运行，请先关闭游戏再启动解锁器！",L"提示",MB_ICONWARNING);
        exit(0);
    }

    GetAdministratorPrivilege(); // 尝试自动获取管理员权限，效果就是程序打开的时候会闪一下
    DisableConsoleScroll();      // 禁止控制台滚动

    const std::filesystem::path GamePath = LoadConfiguration()->string();
    std::cout << "Game Path: " << GamePath << std::endl;

    ExecuteCmdCommand("pause");
    return 0;
}
