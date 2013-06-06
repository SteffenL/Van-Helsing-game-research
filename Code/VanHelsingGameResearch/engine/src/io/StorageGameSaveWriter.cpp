#include <vanhelsing/engine/io/StorageGameSaveWriter.h>
#include <vanhelsing/engine/io/StreamHelperWriter.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveWriter::StorageGameSaveWriter(StorageGameSave& gameSave, std::ostream& outStream)
    : GameSaveContainerWriter(gameSave.ContainerInfo, outStream), m_gameSave(gameSave), m_logger(LogLevel::Trace)
{
    StreamHelperWriter stream(getOutStream());
    writeArtifacts(stream);
}

void StorageGameSaveWriter::writeArtifacts(StreamHelperWriter& stream)
{
    auto& manager = m_gameSave.GetItems();
    auto& items = manager.GetItems();
    stream.Write<int>(items.size());
    for (auto item : items) {
        stream.Write<int>(item->BagNumber);
        stream.Write<int>(item->SlotNumber);
        writeItem(stream, item.get());
    }

    stream.Write<int>(m_gameSave.Unknown.Artifacts.v1);
}

void StorageGameSaveWriter::writeItem(StreamHelperWriter& stream, const inventory::Item* item)
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

StorageGameSaveWriter::~StorageGameSaveWriter() {}

}}} // namespace
