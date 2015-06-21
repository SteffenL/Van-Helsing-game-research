#ifndef vanhelsing_engine_io_StorageGameSaveWriter_
#define vanhelsing_engine_io_StorageGameSaveWriter_

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveContainerWriter.h>
#include <vanhelsing/engine/log.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace inventory { class Item; } } }

namespace vanhelsing { namespace engine { namespace io {

class StreamHelperWriter;

class StorageGameSaveWriter : public GameSaveContainerWriter
{
public:
    StorageGameSaveWriter(StorageGameSave& gameSave, std::ostream& outStream);
    virtual ~StorageGameSaveWriter();

private:
    void writeArtifacts(StreamHelperWriter& stream);
    void writeItem(StreamHelperWriter& stream, const inventory::Item* item);
    void writeEnchantments(StreamHelperWriter& stream, const inventory::Item& item);
    void writeUnknownMaybeEnchantments(StreamHelperWriter& stream, const inventory::Item& item);

    Log m_logger;
    StorageGameSave& m_gameSave;
};

}}} // namespace
#endif // guard
