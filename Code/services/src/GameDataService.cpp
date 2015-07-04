#include <vanhelsing/services/GameDataService.h>

#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/GamePaths.h>


namespace vanhelsing { namespace services {

GameDataService::GameDataService()
{
}


GameDataService::~GameDataService()
{
}

void GameDataService::Load()
{
    using namespace vanhelsing::engine;

    const auto& gameDir = GamePaths::GetInstallPath();
    if (gameDir.empty()) {
        // TODO: Handle this
        return;
    }

    auto& gameData = GameData::Get();
    gameData.Load(gameDir);
}

}}
