#ifndef vanhelsing_engine_io_StorageGameSaveReader_
#define vanhelsing_engine_io_StorageGameSaveReader_

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <vanhelsing/engine/log.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace inventory { class Artifact; } } }

namespace vanhelsing { namespace engine { namespace io {

class StreamHelperReader;

class StorageGameSaveReader : public GameSaveContainerReader
{
public:
    StorageGameSaveReader(StorageGameSave& gameSave, std::istream& inStream);
    virtual ~StorageGameSaveReader();

private:
    void readAllStoredItems(StreamHelperReader& stream);
    std::shared_ptr<inventory::Artifact> readArtifact(StreamHelperReader& stream, inventory::Artifact::List& itemList);
    void readEnchantments(StreamHelperReader& stream, inventory::Enchantment::List& enchantments);
    void readUnknownStruct1(StreamHelperReader& stream, inventory::Artifact::UnknownStruct1& us1);
    void readUnknown1List(StreamHelperReader& stream, std::vector<inventory::Artifact::UnknownList3Item>& list);
    void readUnknown1ListItem(StreamHelperReader& stream, inventory::Artifact::UnknownList3Item& item);
    void readArtifactList(StreamHelperReader& stream, inventory::Artifact::List& list);
    void readUnknown2List(StreamHelperReader& stream, std::vector<inventory::Artifact::UnknownList4Item>& list);
    Log m_logger;
    StorageGameSave& m_gameSave;
};

}}} // namespace
#endif // guard
