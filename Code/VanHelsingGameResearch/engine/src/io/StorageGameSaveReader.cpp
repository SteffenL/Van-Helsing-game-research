#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveReader::StorageGameSaveReader(StorageGameSave& gameSave, std::istream& inStream)
    : GameSaveContainerReader(gameSave.ContainerInfo, inStream), m_gameSave(gameSave), m_logger(LogLevel::Trace)
{
    StreamHelper stream(getInStream());
    readArtifacts(stream);
}

void StorageGameSaveReader::readArtifacts(StreamHelper& stream)
{
    auto count = stream.Read<int>();
    m_logger << "Artifacts (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (int i = 0; i < count; ++i) {
        m_logger << "#" << i << ":" << std::endl;
        m_logger << Log::indent;
        auto bagNumber = stream.Read<int>();
        auto slotNumber = stream.Read<int>();
        m_logger << "Bag #: " << bagNumber << std::endl;
        m_logger << "Slot #: " << slotNumber << std::endl;
        m_logger << "" << std::endl;
        auto item = readItem(stream);
        if (item) {
            item->BagNumber = bagNumber;
            item->SlotNumber = slotNumber;
        }

        m_logger << Log::outdent;
        m_logger << "" << std::endl;
    }

    if (stream.Read<int>() > 0) {
        // TODO: Need to test with more files because there are more reads here
        throw std::runtime_error("This file must be investigated");
    }

    m_logger << Log::outdent;
}

inventory::Item* StorageGameSaveReader::readItem(StreamHelper& stream)
{
    using inventory::Item;
    std::unique_ptr<Item> item(new Item);

    item->Id = stream.Read<Item::IdType>();
    item->Attribute1 = stream.Read<int>();
    item->Attribute2 = stream.Read<int>();
    item->Quantity = stream.Read<int>();

    auto& name = item->GetName();
    m_logger << "ID: 0x" << std::hex << item->Id << std::dec << " (" << (!name.empty() ? name : "unknown") << ")" << std::endl;
    m_logger << "Attribute 1: " << item->Attribute1 << std::endl;
    m_logger << "Attribute 2: " << item->Attribute2 << std::endl;
    m_logger << "Quantity: " << item->Quantity << std::endl;
    
    auto count = stream.Read<int>();
    m_logger << "Unknown (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (int i = 0; i < count; ++i) {
        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        m_logger << v1 << ", " << v2 << std::endl;
        item->Unknown.List1.push_back(Item::UnknownList1Item(v1, v2));
    }

    m_logger << Log::outdent;

    item->Quality = stream.Read<Item::Quality::type>();
    item->Rarity = stream.Read<Item::Rarity::type>();
    m_logger << "Quality: " << item->Quality << std::endl;
    m_logger << "Rarity: " << item->Rarity << std::endl;

    readEnchantments(stream, *item);
    readUnknownMaybeEnchantments(stream, *item);

    item->IsIdentified = stream.Read<bool>();
    m_logger << "Is identified: " << item->IsIdentified << std::endl;
    item->Unknown.v1 = stream.Read<bool>();
    m_logger << "Unknown: " << item->Unknown.v1 << std::endl;

    auto& manager = m_gameSave.GetItems();
    auto itemPtr = item.get();
    manager.Add(itemPtr);
    // Manager owns it now
    item.release();
    return itemPtr;
}

void StorageGameSaveReader::readEnchantments(StreamHelper& stream, inventory::Item& item)
{
    auto& enchantments = item.GetEnchantmentsWritable();

    auto count = stream.Read<unsigned int>();
    m_logger << "Enchantments (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (unsigned int i = 0; i < count; ++i) {
        using inventory::Enchantment;
        std::unique_ptr<Enchantment> enchantment(new Enchantment);

        m_logger << "#" << i << ":" << std::endl;
        m_logger << Log::indent;

        enchantment->Id = stream.Read<Enchantment::IdType>();
        enchantment->Unknown.v2 = stream.Read<int>();
        enchantment->Multiplier = stream.Read<float>();
        enchantment->Unknown.v4 = stream.Read<unsigned int>();

        auto& name = enchantment->GetName();
        m_logger << "ID: 0x" << std::hex << enchantment->Id << std::dec << " (" << (!name.empty() ? name : "unknown") << ")" << std::endl;
        m_logger << "Unknown:" << std::endl;
        m_logger << Log::indent;
        m_logger << enchantment->Unknown.v2 << ", " << enchantment->Multiplier << ", " << enchantment->Unknown.v4 << std::endl;

        if (m_containerInfo.Version >= 0x2b6) {
            enchantment->Unknown.v5 = stream.Read<int>();
            enchantment->Unknown.v6 = stream.Read<unsigned int>();
            enchantment->Unknown.v7 = stream.Read<int>();

            m_logger << enchantment->Unknown.v5 << ", " << enchantment->Unknown.v6 << ", " << enchantment->Unknown.v7 << std::endl;
        }

        enchantments.Add(enchantment.get());
        enchantment.release();

        m_logger << Log::outdent << Log::outdent;
        m_logger << "" << std::endl;
    }

    m_logger << Log::outdent;
}

void StorageGameSaveReader::readUnknownMaybeEnchantments(StreamHelper& stream, inventory::Item& item)
{
    auto& enchantments = item.Unknown.MaybeEnchantments;

    auto count = stream.Read<unsigned int>();
    m_logger << "Unknown, maybe enchantments (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (unsigned int i = 0; i < count; ++i) {
        using inventory::Enchantment;
        std::unique_ptr<Enchantment> enchantment(new Enchantment);

        m_logger << "#" << i << ":" << std::endl;
        m_logger << Log::indent;

        enchantment->Id = stream.Read<Enchantment::IdType>();
        enchantment->Unknown.v2 = stream.Read<int>();
        enchantment->Multiplier = stream.Read<float>();
        enchantment->Unknown.v4 = stream.Read<unsigned int>();

        auto& name = enchantment->GetName();
        m_logger << "ID: 0x" << std::hex << enchantment->Id << std::dec << " (" << (!name.empty() ? name : "unknown") << ")" << std::endl;
        m_logger << "Unknown:" << std::endl;
        m_logger << Log::indent;
        m_logger << enchantment->Unknown.v2 << ", " << enchantment->Multiplier << ", " << enchantment->Unknown.v4 << std::endl;

        if (m_containerInfo.Version >= 0x2b6) {
            enchantment->Unknown.v5 = stream.Read<int>();
            enchantment->Unknown.v6 = stream.Read<unsigned int>();
            enchantment->Unknown.v7 = stream.Read<int>();

            m_logger << enchantment->Unknown.v5 << ", " << enchantment->Unknown.v6 << ", " << enchantment->Unknown.v7 << std::endl;
        }

        enchantments.Add(enchantment.get());
        enchantment.release();

        m_logger << Log::outdent << Log::outdent;
        m_logger << "" << std::endl;
    }

    m_logger << Log::outdent;
}

StorageGameSaveReader::~StorageGameSaveReader() {}

}}} // namespace
