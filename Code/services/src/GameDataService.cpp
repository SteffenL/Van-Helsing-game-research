#include <vanhelsing/services/GameDataService.h>

#include <vanhelsing/engine/GameData.h>
#include <common/SteamPaths.h>
#include <common/Log.h>


namespace vanhelsing { namespace services {

GameDataService::GameDataService()
{
}


GameDataService::~GameDataService()
{
}

void GameDataService::Load(const std::string& desiredGameDir)
{
    // TODO: Decouple this
    unsigned int steamAppId = 215530;

    using namespace common;
    using namespace vanhelsing::engine;

    std::string effectiveGameDir;
    if (desiredGameDir.empty()) {
        const auto& installPath = SteamPaths::GetAppInstallPath(steamAppId);
        if (installPath.empty()) {
            Log(LogLevel::Error) << "Unable to locate the game install path." << std::endl;
            return;
        }

        effectiveGameDir = installPath;
    }
    else {
        effectiveGameDir = desiredGameDir;
    }

    auto& gameData = GameData::Get();
    gameData.Load(effectiveGameDir);
}

}}
