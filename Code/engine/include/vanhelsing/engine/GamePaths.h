#ifndef vanhelsing_engine_GamePaths_
#define vanhelsing_engine_GamePaths_

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

}} // namespace
#endif // guard
