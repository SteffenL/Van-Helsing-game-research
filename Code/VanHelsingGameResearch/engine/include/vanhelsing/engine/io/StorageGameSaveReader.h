#ifndef vanhelsing_engine_io_StorageGameSaveReader_
#define vanhelsing_engine_io_StorageGameSaveReader_

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/log.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace inventory { class Item; } } }

namespace vanhelsing { namespace engine { namespace io {

class StreamHelperReader;

class StorageGameSaveReader : public GameSaveContainerReader
{
public:
    StorageGameSaveReader(StorageGameSave& gameSave, std::istream& inStream);
    virtual ~StorageGameSaveReader();

private:
    void readArtifacts(StreamHelperReader& stream);
    inventory::Item* readItem(StreamHelperReader& stream);
    void readEnchantments(StreamHelperReader& stream, inventory::Item& item);
    void readUnknownMaybeEnchantments(StreamHelperReader& stream, inventory::Item& item);

    Log m_logger;
    StorageGameSave& m_gameSave;
};

}}} // namespace
#endif // guard
