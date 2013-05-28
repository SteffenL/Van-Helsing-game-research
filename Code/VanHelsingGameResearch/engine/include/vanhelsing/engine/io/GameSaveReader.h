#ifndef __vanhelsing_engine_io_GameSaveReader__
#define __vanhelsing_engine_io_GameSaveReader__

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

}}} // namespace
#endif // guard
