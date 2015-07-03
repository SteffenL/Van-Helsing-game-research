#include <vanhelsing/engine/GamePaths.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <misc/registry_value.h>
#include <common/Registry.h>
#include <nowide/convert.hpp>

namespace vanhelsing { namespace engine {

GamePaths::GamePaths() {}

GamePaths::~GamePaths() {}

std::string GamePaths::GetInstallPath()
{
    using namespace common;
    namespace fs = boost::filesystem;

    const std::string regPath("HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 215530\\InstallLocation");
    if (Registry::ValueExists(regPath)) {
        std::string installLocation;
        Registry::GetValue(regPath, installLocation, RegWow64Flag::_32bit);
        if (fs::is_directory(installLocation)) {
            fs::path p(installLocation);
            return p.string();
        }
    }

    return std::string();
}

std::string GamePaths::GetGetGameSavePath()
{
    return std::string();
}

}}
