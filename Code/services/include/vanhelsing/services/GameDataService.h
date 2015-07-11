#pragma once

#include <string>


namespace vanhelsing { namespace services {

class GameDataService
{
public:
    GameDataService();
    virtual ~GameDataService();
    void Load(const std::string& desiredGameDir);
};

}}
