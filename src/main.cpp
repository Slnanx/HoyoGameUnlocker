#include "..\include\SimpleIni.h"
#include "..\include\GetGamePath.h"
#include "..\include\ConfigurationIO.h"
#include "..\include\ConsoleShow.h"
#include "..\include\GetAdministratorPrivileges.h"

int main()
{
    SetConsoleTitleW(L"HoyoFPSUnlocker");
    GetAdministratorPrivilege(); // 尝试自动获取管理员权限，效果就是程序打开的时候会闪一下
    DisableConsoleScroll();      // 禁止控制台滚动
    std::filesystem::path GamePath = LoadConfiguration()->string();

    MessageBoxW(NULL, GamePath.wstring().c_str(), L"文件路径", MB_OK);
}
