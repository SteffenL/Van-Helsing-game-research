#ifndef vanhelsing_engine_io_GameSaveWriter_
#define vanhelsing_engine_io_GameSaveWriter_

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

}}} // namespace
#endif // guard
