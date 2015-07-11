#pragma once

#include <string>

namespace common {

class SteamPaths
{
public:
    SteamPaths();
    virtual ~SteamPaths();
    static std::string GetAppInstallPath(unsigned int steamId);
    static std::string GetGetGameSavePath();
};

}
