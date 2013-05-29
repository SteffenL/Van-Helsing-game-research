#ifndef __vanhelsing_engine_io_StorageGameSaveReader__
#define __vanhelsing_engine_io_StorageGameSaveReader__

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveReader.h>
#include <vanhelsing/engine/log.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class StreamHelper;

class StorageGameSaveReader : public GameSaveReader
{
public:
    StorageGameSaveReader(GameSave& gameSave, std::istream& inStream);
    virtual ~StorageGameSaveReader();

private:
    void readItems(StreamHelper& stream);
    void readItem(StreamHelper& stream);
    void readItemStats1(StreamHelper& stream);
    void readItemStats2(StreamHelper& stream);

    Log m_logger;
};

}}} // namespace
#endif // guard
