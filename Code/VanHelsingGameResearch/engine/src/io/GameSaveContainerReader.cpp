#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StreamHelper.h>

namespace vanhelsing { namespace engine { namespace io {

GameSaveContainerReader::GameSaveContainerReader(ContainerInfoType& containerInfo, std::istream& inStream)
    : m_containerInfo(containerInfo), m_inStream(inStream)
{
    StreamHelper stream(getInStream());
    const std::string& signature = stream.ReadString(5);
    if (signature != std::string("C2AR\0", 5)) {
        throw std::runtime_error("Invalid signature");
    }

    m_containerInfo.Signature = signature;
    m_containerInfo.Version = stream.Read<unsigned int>();
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

}}} // namespace
