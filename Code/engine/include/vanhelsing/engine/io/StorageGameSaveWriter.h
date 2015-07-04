#pragma once

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/GameSaveContainerWriter.h>
#include <common/Log.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace inventory { class Artifact; } } }

namespace vanhelsing { namespace engine { namespace io {

class StreamHelperWriter;

class StorageGameSaveWriter : public GameSaveContainerWriter
{
public:
    StorageGameSaveWriter(StorageGameSave& gameSave, std::ostream& outStream);
    virtual ~StorageGameSaveWriter();

private:
    void writeAllStoredItems(StreamHelperWriter& stream);
    void writeArtifact(StreamHelperWriter& stream, const inventory::Artifact& item);
    void writeEnchantments(StreamHelperWriter& stream, const inventory::EnchantmentCollection& enchantments);
    void writeInfusedArtifacts(StreamHelperWriter& stream, const inventory::ArtifactCollection& items);
    void writeArtifactBags(StreamHelperWriter& stream, inventory::ArtifactBagCollection& bags);
    void writeUnknownStruct1(StreamHelperWriter& stream, const inventory::Artifact::UnknownStruct1& us1);
    void writeUnknown1List(StreamHelperWriter& stream, const std::vector<inventory::Artifact::UnknownList3Item>& list);
    void writeUnknown1ListItem(StreamHelperWriter& stream, const inventory::Artifact::UnknownList3Item& item);
    void writeUnknown2List(StreamHelperWriter& stream, const std::vector<inventory::Artifact::UnknownList4Item>& list);

    common::Log m_logger;
    StorageGameSave& m_gameSave;
};

}}}
