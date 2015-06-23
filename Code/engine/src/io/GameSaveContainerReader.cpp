#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StreamHelperReader.h>

#include <string>

namespace vanhelsing { namespace engine { namespace io {

GameSaveContainerReader::GameSaveContainerReader(ContainerInfoType& containerInfo, std::istream& inStream)
    : m_containerInfo(containerInfo), m_inStream(inStream)
{
    StreamHelperReader stream(getInStream());
    auto& signature = stream.ReadString(5);
    if (signature.compare(std::string("C2AR\0", 5)) != 0) {
        throw std::runtime_error("Invalid signature");
    }

    m_containerInfo.Signature = signature;
    m_containerInfo.Version = stream.Read<unsigned int>();

    checkVersion();

    m_containerInfo.Unknown.v1 = stream.Read<int>();
    
    m_containerInfo.Unknown.v2 = stream.Read<char>();
    if (m_containerInfo.Unknown.v2 != 0) {
        throw std::runtime_error("Not supported");
    }
}

GameSaveContainerReader::~GameSaveContainerReader() {}

std::istream& GameSaveContainerReader::getInStream() const
{
    return m_inStream;
}

void GameSaveContainerReader::checkVersion()
{
    if (m_containerInfo.Version >= MinimumSupportedVersion) {
        return;
    }

    std::string msg(
        "This file is unsupported because it's version (" + std::to_string(m_containerInfo.Version) + ") "
        "is older than the minimum supported version (" + std::to_string(MinimumSupportedVersion) + ")."
    );
    throw std::runtime_error(msg);
}

}
}
} // namespace
