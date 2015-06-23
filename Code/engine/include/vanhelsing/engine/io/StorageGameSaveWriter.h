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
    void writeAllStoredItems(StreamHelperWriter& stream);
    void writeArtifact(StreamHelperWriter& stream, const inventory::Item* item);
    void writeEnchantments(StreamHelperWriter& stream, const inventory::Item& item);
    void writeUnknownMaybeEnchantments(StreamHelperWriter& stream, const inventory::Item& item);
    void writeArtifactList(StreamHelperWriter& stream, inventory::Item::List& list);
    void writeUnknownStruct1(StreamHelperWriter& stream, const inventory::Item::UnknownStruct1& us1);
    void writeUnknown1List(StreamHelperWriter& stream, const std::vector<inventory::Item::UnknownList3Item>& list);
    void writeUnknown1ListItem(StreamHelperWriter& stream, const inventory::Item::UnknownList3Item& item);
    void writeUnknown2List(StreamHelperWriter& stream, const std::vector<inventory::Item::UnknownList4Item>& list);
    Log m_logger;
    StorageGameSave& m_gameSave;
};

}}} // namespace
#endif // guard
