#pragma once

#include "../GameSave.h"

namespace vanhelsing { namespace engine { namespace io {

class GameSaveWriter
{
public:
    GameSaveWriter(GameSave& gameSave);
    virtual ~GameSaveWriter();

private:
    GameSave& m_gameSave;
};

}}}
