#pragma once

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveContainerReader.h>
#include <common/Log.h>
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
    std::unique_ptr<inventory::Artifact> readArtifact(StreamHelperReader& stream);
    void readEnchantments(StreamHelperReader& stream, inventory::EnchantmentCollection& enchantments);
    void readInfusedArtifacts(StreamHelperReader& stream, inventory::ArtifactCollection& items);
    void readUnknownStruct1(StreamHelperReader& stream, inventory::Artifact::UnknownStruct1& us1);
    void readUnknown1List(StreamHelperReader& stream, std::vector<inventory::Artifact::UnknownList3Item>& list);
    void readUnknown1ListItem(StreamHelperReader& stream, inventory::Artifact::UnknownList3Item& item);
    void readArtifactBags(StreamHelperReader& stream, inventory::ArtifactBagCollection& bags);
    void readUnknown2List(StreamHelperReader& stream, std::vector<inventory::Artifact::UnknownList4Item>& list);
    common::Log m_logger;
    StorageGameSave& m_gameSave;
};

}}}
