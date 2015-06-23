#include <vanhelsing/engine/io/StorageGameSaveWriter.h>
#include <vanhelsing/engine/io/StreamHelperWriter.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveWriter::StorageGameSaveWriter(StorageGameSave& gameSave, std::ostream& outStream)
    : GameSaveContainerWriter(gameSave.ContainerInfo, outStream), m_gameSave(gameSave), m_logger(LogLevel::Trace)
{
    StreamHelperWriter stream(getOutStream());
    writeAllStoredItems(stream);
}

void StorageGameSaveWriter::writeAllStoredItems(StreamHelperWriter& stream)
{
    writeArtifactList(stream, m_gameSave.GetArtifacts1());
    writeArtifactList(stream, m_gameSave.GetArtifacts2());

    stream.Write<int>(m_gameSave.Unknown.Artifacts.v1);
    stream.Write<int>(m_gameSave.Unknown.Artifacts.v2);

    // TODO: When arg_0 is equal to 1, more writes follow; but I don't know where this value comes from. Applies to only reads or also writes?
}

void StorageGameSaveWriter::writeArtifact(StreamHelperWriter& stream, const inventory::Item* item)
{
    using inventory::Item;

    stream.Write<Item::IdType>(item->Id);
    stream.Write<int>(item->Attribute1);
    stream.Write<int>(item->Attribute2);
    stream.Write<int>(item->Quantity);

    stream.Write<int>(item->Unknown.List1.size());
    for (auto& i : item->Unknown.List1) {
        stream.Write<unsigned int>(i.v1);
        stream.Write<int>(i.v2);
    }

    stream.Write<Item::Quality::type>(item->Quality);
    stream.Write<Item::Rarity::type>(item->Rarity);

    writeEnchantments(stream, *item);
    writeUnknownMaybeEnchantments(stream, *item);

    stream.Write<bool>(item->IsIdentified);
    stream.Write<bool>(item->Unknown.v1);

    stream.Write<unsigned int>(item->Unknown.v2);
    {
        // TODO: Move code in this block into a function
        // IDA hint: my_storage_WriteArtifact_UnknownFunc001

        stream.Write<int>(item->Unknown.List2.size());
        for (const auto& item : item->Unknown.List2) {
            stream.Write<unsigned int>(item.v1);
            stream.Write<int>(item.v2);
        }
    }

    stream.Write<bool>(item->Unknown.v3);
    writeUnknownStruct1(stream, item->Unknown.UnknownStruct1_1);
    writeUnknownStruct1(stream, item->Unknown.UnknownStruct1_2);
    writeUnknown1List(stream, item->Unknown.List3);
    writeUnknown2List(stream, item->Unknown.List4);
    writeUnknown1List(stream, item->Unknown.List5);
    writeUnknownStruct1(stream, item->Unknown.UnknownStruct1_3);
    stream.Write<int>(item->Unknown.v4);
    writeUnknownStruct1(stream, item->Unknown.UnknownStruct1_4);
    stream.Write<int>(item->Unknown.v5);
    stream.Write<int>(item->Unknown.v6);
    stream.Write<float>(item->Unknown.v7);
    stream.Write<float>(item->Unknown.v8);
}

void StorageGameSaveWriter::writeEnchantments(StreamHelperWriter& stream, const inventory::Item& item)
{
    auto& enchantments = item.GetEnchantments().GetItems();

    stream.Write<unsigned int>(enchantments.size());
    for (auto enchantment : enchantments) {
        using inventory::Enchantment;

        stream.Write<Enchantment::IdType>(enchantment->Id);
        stream.Write<int>(enchantment->Unknown.v2);
        stream.Write<float>(enchantment->Multiplier);
        stream.Write<unsigned int>(enchantment->Unknown.v4);

        if (m_containerInfo.Version >= 0x2b6) {
            stream.Write<int>(enchantment->Unknown.v5);
            stream.Write<unsigned int>(enchantment->Unknown.v6);
            stream.Write<int>(enchantment->Unknown.v7);
        }
    }
}

void StorageGameSaveWriter::writeUnknownMaybeEnchantments(StreamHelperWriter& stream, const inventory::Item& item)
{
    auto& enchantments = item.Unknown.MaybeEnchantments.GetItems();

    stream.Write<unsigned int>(enchantments.size());
    for (auto enchantment : enchantments) {
        using inventory::Enchantment;

        stream.Write<Enchantment::IdType>(enchantment->Id);
        stream.Write<int>(enchantment->Unknown.v2);
        stream.Write<float>(enchantment->Multiplier);
        stream.Write<unsigned int>(enchantment->Unknown.v4);

        if (m_containerInfo.Version >= 0x2b6) {
            stream.Write<int>(enchantment->Unknown.v5);
            stream.Write<unsigned int>(enchantment->Unknown.v6);
            stream.Write<int>(enchantment->Unknown.v7);
        }
    }
}

void StorageGameSaveWriter::writeArtifactList(StreamHelperWriter& stream, inventory::Item::List& list)
{
    auto& items = list.GetItems();
    stream.Write<int>(items.size());
    for (auto item : items) {
        stream.Write<int>(item->BagNumber);
        stream.Write<int>(item->SlotNumber);
        writeArtifact(stream, item.get());
    }
}

StorageGameSaveWriter::~StorageGameSaveWriter() {}

void StorageGameSaveWriter::writeUnknownStruct1(StreamHelperWriter& stream, const inventory::Item::UnknownStruct1& us1)
{
    stream.Write<unsigned int>(us1.v1);
    // TODO: What is being being compared to 1? Does this apply to writes or only reads?
    //if (something == 1) {
    // Read 2 bytes but I'm not sure if it's int16 or an array
    stream.Write<short>(us1.v2);
    //}
}

void StorageGameSaveWriter::writeUnknown1List(StreamHelperWriter& stream, const std::vector<inventory::Item::UnknownList3Item>& list)
{
    stream.Write<unsigned int>(list.size());
    for (const auto& item : list) {
        writeUnknown1ListItem(stream, item);
    }
}

void StorageGameSaveWriter::writeUnknown1ListItem(StreamHelperWriter& stream, const inventory::Item::UnknownList3Item& item)
{
    stream.Write<unsigned int>(item.v1);
    // TODO: What is being being compared to 1? Does this apply to writes or only reads?
    //if (something == 1) {
    // Read 2 bytes but I'm not sure if it's int16 or an array
    stream.Write<short>(item.v2);
    //}
}

void StorageGameSaveWriter::writeUnknown2List(StreamHelperWriter& stream, const std::vector<inventory::Item::UnknownList4Item>& list)
{
    stream.Write<int>(list.size());
    for (const auto& item : list) {
        writeUnknownStruct1(stream, item.v1);
        writeUnknown1List(stream, item.v2);
    }
}

}}} // namespace
