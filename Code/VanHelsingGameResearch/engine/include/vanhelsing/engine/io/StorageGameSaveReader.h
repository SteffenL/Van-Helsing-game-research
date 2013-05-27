#ifndef __vanhelsing_engine_io_StorageGameSaveReader__
#define __vanhelsing_engine_io_StorageGameSaveReader__

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveReader.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class StreamHelper;

class StorageGameSaveReader : GameSaveReader
{
public:
    StorageGameSaveReader(GameSave& gameSave, std::istream& inStream);

private:
    void readItems(StreamHelper& stream);
    void readItem(StreamHelper& stream);
    void readItem2(StreamHelper& stream);
    void readItem3(StreamHelper& stream);
};

}}} // namespace
#endif // guard
