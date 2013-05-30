#ifndef __vanhelsing_engine_GameData__
#define __vanhelsing_engine_GameData__

#include <string>
#include <map>

namespace vanhelsing { namespace engine {

class GameData
{
public:
    static void Load(const std::string& gameDir);

private:
    static void loadArtifacts();

private:
    static std::string m_gameDir;
};

}} // namespace
#endif // guard
