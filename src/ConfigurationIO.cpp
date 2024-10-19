#include "..\include\ConfigurationIO.h"
#include "..\include\Simpleini.h"
#include "..\include\GetGamePath.h"
#include "..\include\GameInfo.h"
#include "..\include\GetAdministratorPrivileges.h"

std::optional<std::filesystem::path> LoadConfiguration()
{
    CSimpleIniA ini;
    // 如果加载成功，继续执行
    if (ini.LoadFile(ConfigurationFileName) == SI_OK)
    {
        const char *GamePathCStr = ini.GetValue("GamePath", "HK4E", "-1");
        // 如果路径有效且包含游戏名，返回路径
        if (GamePathCStr && std::string(GamePathCStr).find(GameName) != std::string::npos)
        {
            return std::filesystem::path(GamePathCStr);
        }
        // 如果路径无效，就调用DetectAndWriteConfiguration()进行路径探测
        else
        {
            std::filesystem::remove(ConfigurationFileName); // 清理可能存在的残留文件
            return DetectAndWriteConfiguration();
        }
    }
    // 如果加载失败，就调用DetectAndWriteConfiguration()进行路径探测
    else
    {
        std::filesystem::remove(ConfigurationFileName);
        return DetectAndWriteConfiguration();
    }
}

// 将游戏路径写入到配置文件中
void WriteConfiguration(const std::filesystem::path &GamePath, const char *Section, const char *Key)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(ConfigurationFileName);
    ini.SetValue(Section, Key, GamePath.string().c_str());
    ini.SaveFile(ConfigurationFileName);
}

// 每两秒探测一次游戏路径，直到探测成功后返回游戏路径
std::optional<std::filesystem::path> DetectAndWriteConfiguration()
{
    std::optional<std::filesystem::path> GamePath;

    while (true)
    {
        GamePath = GetGamePath(GameName);
        if (GamePath && GamePath->string().find(GameName) != std::string::npos)
        {
            WriteConfiguration(*GamePath, "GamePath", "HK4E");
            std::println("已探测到游戏路径: {}", GamePath->string());
            std::println("游戏路径已经被写入到配置文件中！即将开始注入...");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            ExecuteCmdCommand("cls");
            return GamePath;
        }
        else
        {
            std::println("未找到有效配置文件，正在尝试探测游戏路径...");
            std::println("Game path detection failed. Retrying in 2 seconds...");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            ExecuteCmdCommand("cls");
        }
    }

    return DetectAndWriteConfiguration();
}
