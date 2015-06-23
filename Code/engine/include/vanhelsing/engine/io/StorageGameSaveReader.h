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
    void readAllStoredItems(StreamHelperReader& stream);
    inventory::Item* readArtifact(StreamHelperReader& stream, inventory::Item::List& itemList);
    void readEnchantments(StreamHelperReader& stream, inventory::Item& item);
    void readUnknownMaybeEnchantments(StreamHelperReader& stream, inventory::Item& item);
    void readUnknownStruct1(StreamHelperReader& stream, inventory::Item::UnknownStruct1& us1);
    void readUnknown1List(StreamHelperReader& stream, std::vector<inventory::Item::UnknownList3Item>& list);
    void readUnknown1ListItem(StreamHelperReader& stream, inventory::Item::UnknownList3Item& item);
    void readArtifactList(StreamHelperReader& stream, inventory::Item::List& list);
    void readUnknown2List(StreamHelperReader& stream, std::vector<inventory::Item::UnknownList4Item>& list);
    Log m_logger;
    StorageGameSave& m_gameSave;
};

}}} // namespace
#endif // guard
