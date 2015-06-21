#include <vanhelsing/engine/GamePaths.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <misc/registry_value.h>
#include <steffenl/common/Registry.h>
#include <nowide/convert.hpp>

namespace vanhelsing { namespace engine {

GamePaths::GamePaths() {}

GamePaths::~GamePaths() {}

std::string GamePaths::GetInstallPath()
{
    using namespace steffenl::common;
    const std::string regPath("HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 215530\\InstallLocation");
    if (Registry::ValueExists(regPath)) {
        std::string installLocation;
        Registry::GetValue(regPath, installLocation, RegWow64Flag::_32bit);
        if (boost::filesystem::is_directory(installLocation)) {
            boost::filesystem::path p(installLocation);
            return p.string();
        }
    }

    return std::string();
}

std::string GamePaths::GetGetGameSavePath()
{
    return std::string();
}

}} // namespace
