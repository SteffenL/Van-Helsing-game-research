#include <vanhelsing/engine/io/GameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelperReader.h>

namespace vanhelsing { namespace engine { namespace io {

GameSaveReader::GameSaveReader(GameSave& gameSave, std::istream& inStream) : m_gameSave(gameSave), m_inStream(inStream)
{
    StreamHelperReader stream(getStream());
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

}}}
