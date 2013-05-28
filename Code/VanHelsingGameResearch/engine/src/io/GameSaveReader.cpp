#include <vanhelsing/engine/io/GameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>

namespace vanhelsing { namespace engine { namespace io {

GameSaveReader::GameSaveReader(GameSave& gameSave, std::istream& inStream) : m_gameSave(gameSave), m_inStream(inStream)
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

GameSaveReader::~GameSaveReader() {}

std::istream& GameSaveReader::getStream() const
{
    return m_inStream;
}

GameSave& GameSaveReader::getGameSave()
{
    return m_gameSave;
}

}}} // namespace
