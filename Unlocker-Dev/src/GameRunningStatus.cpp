#include "../include/GameRunningStatus.h"

//检测游戏是否被启动
bool CheckGameRunningStatus()
{
    if (const DWORD GamePid = GetProcessIdByName(Utf8ToWide(GameName).c_str()); GamePid != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
