#include <vanhelsing/engine/io/StorageGameSaveWriter.h>
#include <vanhelsing/engine/io/StreamHelperWriter.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveWriter::StorageGameSaveWriter(StorageGameSave& gameSave, std::ostream& outStream)
    : GameSaveContainerWriter(gameSave.ContainerInfo, outStream), m_gameSave(gameSave), m_logger(common::LogLevel::Trace)
{
    StreamHelperWriter stream(getOutStream());
    writeAllStoredItems(stream);
}

void StorageGameSaveWriter::writeAllStoredItems(StreamHelperWriter& stream)
{
    writeArtifactBags(stream, m_gameSave.GetArtifactBags1());
    writeArtifactBags(stream, m_gameSave.GetArtifactBags2());

    stream.Write(m_gameSave.Unknown.Artifacts.v1);
    stream.Write(m_gameSave.Unknown.Artifacts.v2);

    // TODO: When arg_0 is equal to 1, more writes follow; but I don't know where this value comes from. Applies to only reads or also writes?
}

void StorageGameSaveWriter::writeArtifact(StreamHelperWriter& stream, const inventory::Artifact& item)
{
    using inventory::Artifact;

    stream.Write(item.Id);
    stream.Write(item.Property1);
    stream.Write(item.Property2);
    stream.Write(item.Quantity);

    stream.Write<int>(item.Unknown.List1.size());
    for (auto& i : item.Unknown.List1) {
        stream.Write(i.v1);
        stream.Write(i.v2);
    }

    stream.Write(item.Quality);
    stream.Write(item.Rarity);

    writeEnchantments(stream, item.GetEnchantments());
    writeInfusedArtifacts(stream, item.GetInfusedArtifacts());

    stream.Write(item.IsIdentified);
    stream.Write(item.Unknown.v1);

    stream.Write(item.Unknown.v2);
    {
        // TODO: Move code in this block into a function
        // IDA hint: my_storage_WriteArtifact_UnknownFunc001

        stream.Write<int>(item.Unknown.List2.size());
        for (const auto& item : item.Unknown.List2) {
            stream.Write(item.v1);
            stream.Write(item.v2);
        }
    }

    stream.Write(item.Unknown.v3);
    writeUnknownStruct1(stream, item.Unknown.UnknownStruct1_1);
    writeUnknownStruct1(stream, item.Unknown.UnknownStruct1_2);
    writeUnknown1List(stream, item.Unknown.List3);
    writeUnknown2List(stream, item.Unknown.List4);
    writeUnknown1List(stream, item.Unknown.List5);
    writeUnknownStruct1(stream, item.Unknown.UnknownStruct1_3);
    stream.Write(item.Unknown.v4);
    writeUnknownStruct1(stream, item.Unknown.UnknownStruct1_4);
    stream.Write(item.Unknown.v5);
    stream.Write(item.Unknown.v6);
    stream.Write(item.Unknown.v7);
    stream.Write(item.Unknown.v8);
}

void StorageGameSaveWriter::writeEnchantments(StreamHelperWriter& stream, const inventory::EnchantmentCollection& enchantments)
{
    stream.Write<unsigned int>(enchantments.size());
    for (const auto& enchantment : enchantments) {
        using inventory::Enchantment;

        stream.Write(enchantment->Id);
        stream.Write(enchantment->ValueIndex);
        stream.Write(enchantment->ValueScale);
        stream.Write(enchantment->Unknown.v4);

        if (m_containerInfo.Version >= 0x2b6) {
            stream.Write(enchantment->Unknown.v5);
            stream.Write(enchantment->Unknown.v6);
            stream.Write(enchantment->Unknown.v7);
        }
    }
}

void StorageGameSaveWriter::writeInfusedArtifacts(StreamHelperWriter& stream, const inventory::ArtifactCollection& items)
{
    stream.Write<int>(items.size());
    for (const auto& item : items) {
        writeArtifact(stream, *item);
    }
}

void StorageGameSaveWriter::writeArtifactBags(StreamHelperWriter& stream, inventory::ArtifactBagCollection& bags)
{
    using namespace inventory;

    struct ArtifactBagCollectionFlatListItem
    {
        ArtifactBagCollectionFlatListItem(BagIndexType bagNumber, BagSlotIndexType slotNumber, const inventory::Artifact& item)
            : BagNumber(bagNumber), SlotNumber(slotNumber), Item(item)
        {}

        BagIndexType BagNumber;
        BagSlotIndexType SlotNumber;
        const inventory::Artifact& Item;
    };

    std::vector<ArtifactBagCollectionFlatListItem> items;

    for (const auto& bag : bags) {
        for (const auto& slot : bag.second) {
            ArtifactBagCollectionFlatListItem item(bag.first, slot.first, *slot.second);
            items.emplace_back(item);
        }
    }

    stream.Write<int>(items.size());
    for (const auto& item : items) {
        stream.Write(item.BagNumber);
        stream.Write(item.SlotNumber);
        writeArtifact(stream, item.Item);
    }
}

StorageGameSaveWriter::~StorageGameSaveWriter() {}

void StorageGameSaveWriter::writeUnknownStruct1(StreamHelperWriter& stream, const inventory::Artifact::UnknownStruct1& us1)
{
    stream.Write(us1.v1);
    // TODO: What is being being compared to 1? Does this apply to writes or only reads?
    //if (something == 1) {
    // Read 2 bytes but I'm not sure if it's int16 or an array
    stream.Write(us1.v2);
    //}
}

void StorageGameSaveWriter::writeUnknown1List(StreamHelperWriter& stream, const std::vector<inventory::Artifact::UnknownList3Item>& list)
{
    stream.Write<unsigned int>(list.size());
    for (const auto& item : list) {
        writeUnknown1ListItem(stream, item);
    }
}

void StorageGameSaveWriter::writeUnknown1ListItem(StreamHelperWriter& stream, const inventory::Artifact::UnknownList3Item& item)
{
    stream.Write(item.v1);
    // TODO: What is being being compared to 1? Does this apply to writes or only reads?
    //if (something == 1) {
    // Read 2 bytes but I'm not sure if it's int16 or an array
    stream.Write(item.v2);
    //}
}

void StorageGameSaveWriter::writeUnknown2List(StreamHelperWriter& stream, const std::vector<inventory::Artifact::UnknownList4Item>& list)
{
    stream.Write<int>(list.size());
    for (const auto& item : list) {
        writeUnknownStruct1(stream, item.v1);
        writeUnknown1List(stream, item.v2);
    }
}

}}}
