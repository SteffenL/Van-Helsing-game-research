#ifndef __vanhelsing_engine_io_GameSaveWriter__
#define __vanhelsing_engine_io_GameSaveWriter__

#include "../GameSave.h"

namespace vanhelsing { namespace engine { namespace io {

class GameSaveWriter
{
public:
    GameSaveWriter(GameSave& gameSave);

private:
    GameSave& m_gameSave;
};

}}} // namespace
#endif // guard
