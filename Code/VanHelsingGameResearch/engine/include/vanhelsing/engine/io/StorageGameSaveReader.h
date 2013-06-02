#ifndef __vanhelsing_engine_io_StorageGameSaveReader__
#define __vanhelsing_engine_io_StorageGameSaveReader__

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/log.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace inventory { class Item; } } }

namespace vanhelsing { namespace engine { namespace io {

class StreamHelper;

class StorageGameSaveReader : public GameSaveContainerReader
{
public:
    StorageGameSaveReader(StorageGameSave& gameSave, std::istream& inStream);
    virtual ~StorageGameSaveReader();

private:
    void readArtifacts(StreamHelper& stream);
    inventory::Item* readItem(StreamHelper& stream);
    void readEnchantments(StreamHelper& stream, inventory::Item& item);
    void readUnknownMaybeEnchantments(StreamHelper& stream);

    Log m_logger;
    StorageGameSave& m_gameSave;
};

}}} // namespace
#endif // guard
