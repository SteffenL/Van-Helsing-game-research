#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StreamHelperReader.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>

#include <string>

namespace vanhelsing { namespace engine { namespace io {

GameSaveContainerReader::GameSaveContainerReader(ContainerInfoType& containerInfo, std::istream& inStream)
    : m_containerInfo(containerInfo), m_inStream(inStream)
{
    StreamHelperReader stream(getInStream());
    auto& signature = stream.ReadString(5);
    if (signature.compare(std::string("C2AR\0", 5)) != 0) {
        throw VanHelsingEngineError("Invalid signature");
    }

    m_containerInfo.Signature = signature;
    stream.Read(m_containerInfo.Version);

    checkVersion();

    stream.Read(m_containerInfo.Unknown.v1);
    
    stream.Read(m_containerInfo.Unknown.v2);
    if (m_containerInfo.Unknown.v2 != 0) {
        throw VanHelsingEngineError("This file must be investigated");
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
    throw VanHelsingEngineError(msg);
}

}
}
}
