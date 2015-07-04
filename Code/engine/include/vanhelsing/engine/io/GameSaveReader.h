#pragma once

#include "../GameSave.h"
#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveReader
{
public:
    GameSaveReader(GameSave& gameSave, std::istream& inStream);
    virtual ~GameSaveReader();
    std::istream& getStream() const;

protected:
    GameSave& getGameSave();
    unsigned int m_unknown1;

private:
    GameSave& m_gameSave;
    std::istream& m_inStream;
};

}}}
