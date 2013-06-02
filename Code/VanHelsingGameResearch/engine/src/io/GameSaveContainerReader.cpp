#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>

namespace vanhelsing { namespace engine { namespace io {

GameSaveContainerReader::GameSaveContainerReader(std::istream& inStream) : m_inStream(inStream)
{
    StreamHelper stream(getStream());
    if (stream.ReadString(5) != std::string("C2AR\0", 5)) {
        throw std::runtime_error("Invalid signature");
    }

    m_unknown1 = stream.Read<unsigned int>();
    stream.Read<int>();

    if (stream.Read<char>() != 0) {
        throw std::runtime_error("Not supported");
    }
}

GameSaveContainerReader::~GameSaveContainerReader() {}

std::istream& GameSaveContainerReader::getStream() const
{
    return m_inStream;
}

}}} // namespace
