#pragma once

#include <string>

namespace vanhelsing { namespace engine {

class GamePaths
{
public:
    GamePaths();
    virtual ~GamePaths();
    static std::string GetInstallPath();
    static std::string GetGetGameSavePath();
};

}}
