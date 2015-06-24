#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelperReader.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveReader::StorageGameSaveReader(StorageGameSave& gameSave, std::istream& inStream)
    : GameSaveContainerReader(gameSave.ContainerInfo, inStream), m_gameSave(gameSave), m_logger(LogLevel::Trace)
{
    StreamHelperReader stream(getInStream());
    readAllStoredItems(stream);
}

void StorageGameSaveReader::readAllStoredItems(StreamHelperReader& stream)
{
    readArtifactList(stream, m_gameSave.GetArtifacts1());
    readArtifactList(stream, m_gameSave.GetArtifacts2());

    stream.Read(m_gameSave.Unknown.Artifacts.v1);
    stream.Read(m_gameSave.Unknown.Artifacts.v2);

    // TODO: When arg_0 is equal to 1, more reads follow; but I don't know where this value comes from
}

std::shared_ptr<inventory::Artifact> StorageGameSaveReader::readArtifact(StreamHelperReader& stream, inventory::Artifact::List& itemList)
{
    using inventory::Artifact;
    auto item = std::make_shared<Artifact>();

    stream.Read(item->Id);
    stream.Read(item->Attribute1);
    stream.Read(item->Attribute2);
    stream.Read(item->Quantity);

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
        item->Unknown.List1.push_back(Artifact::UnknownList1Item(v1, v2));
    }

    m_logger << Log::outdent;

    stream.Read(item->Quality);
    stream.Read(item->Rarity);
    m_logger << "Quality: " << item->Quality << std::endl;
    m_logger << "Rarity: " << item->Rarity << std::endl;

    readEnchantments(stream, item->GetEnchantmentsWritable());
    readEnchantments(stream, item->Unknown.MaybeEnchantments);

    stream.Read(item->IsIdentified);
    m_logger << "Is identified: " << item->IsIdentified << std::endl;
    stream.Read(item->Unknown.v1);
    m_logger << "Unknown: " << item->Unknown.v1 << std::endl;

    stream.Read(item->Unknown.v2);
    {
        // TODO: Move code in this block into a function
        // IDA hint: my_storage_ReadArtifact_UnknownFunc001

        auto count = stream.Read<int>();
        m_logger << "Unknown (" << count << "):" << std::endl;
        m_logger << Log::indent;
        for (int i = 0; i < count; ++i) {
            auto v1 = stream.Read<unsigned int>();
            auto v2 = stream.Read<int>();
            m_logger << v1 << ", " << v2 << std::endl;
            item->Unknown.List2.push_back(Artifact::UnknownList2Item(v1, v2));
        }

        m_logger << Log::outdent;
    }

    stream.Read(item->Unknown.v3);
    readUnknownStruct1(stream, item->Unknown.UnknownStruct1_1);
    readUnknownStruct1(stream, item->Unknown.UnknownStruct1_2);
    readUnknown1List(stream, item->Unknown.List3);
    readUnknown2List(stream, item->Unknown.List4);
    readUnknown1List(stream, item->Unknown.List5);
    readUnknownStruct1(stream, item->Unknown.UnknownStruct1_3);
    stream.Read(item->Unknown.v4);
    readUnknownStruct1(stream, item->Unknown.UnknownStruct1_4);
    stream.Read(item->Unknown.v5);
    stream.Read(item->Unknown.v6);
    stream.Read(item->Unknown.v7);
    stream.Read(item->Unknown.v8);

    itemList.Add(item);
    return item;
}

void StorageGameSaveReader::readEnchantments(StreamHelperReader& stream, inventory::Enchantment::List& enchantments)
{
    auto count = stream.Read<unsigned int>();
    m_logger << "Enchantments (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (unsigned int i = 0; i < count; ++i) {
        using inventory::Enchantment;
        auto enchantment = std::make_shared<Enchantment>();

        m_logger << "#" << i << ":" << std::endl;
        m_logger << Log::indent;

        stream.Read(enchantment->Id);
        stream.Read(enchantment->EffectValue);
        stream.Read(enchantment->EffectModifier);
        stream.Read(enchantment->Unknown.v4);

        auto& name = enchantment->GetName();
        m_logger << "ID: 0x" << std::hex << enchantment->Id << std::dec << " (" << (!name.empty() ? name : "unknown") << ")" << std::endl;
        m_logger << "Effect value: " << enchantment->EffectValue << std::endl;
        m_logger << "Effect modifier: " << enchantment->EffectModifier << std::endl;

        m_logger << "Unknown:" << std::endl;
        m_logger << Log::indent;
        m_logger << enchantment->Unknown.v4 << std::endl;

        if (m_containerInfo.Version >= 0x2b6) {
            stream.Read(enchantment->Unknown.v5);
            stream.Read(enchantment->Unknown.v6);
            stream.Read(enchantment->Unknown.v7);

            m_logger << enchantment->Unknown.v5 << ", " << enchantment->Unknown.v6 << ", " << enchantment->Unknown.v7 << std::endl;
        }

        enchantments.Add(enchantment);

        m_logger << Log::outdent << Log::outdent;
        m_logger << "" << std::endl;
    }

    m_logger << Log::outdent;
}

StorageGameSaveReader::~StorageGameSaveReader() {}

void StorageGameSaveReader::readUnknownStruct1(StreamHelperReader& stream, inventory::Artifact::UnknownStruct1& us1)
{
    stream.Read(us1.v1);
    // TODO: What is being being compared to 1?
    //if (something == 1) {
    // Read 2 bytes but I'm not sure if it's int16 or an array
    stream.Read(us1.v2);
    //}
}

void StorageGameSaveReader::readUnknown1List(StreamHelperReader& stream, std::vector<inventory::Artifact::UnknownList3Item>& list)
{
    auto count = stream.Read<unsigned int>();
    for (decltype(count) i = 0; i < count; ++i) {
        // TODO: Learn more about the case when count > 0
        throw std::runtime_error("This file must be investigated");

        inventory::Artifact::UnknownList3Item item;
        readUnknown1ListItem(stream, item);
        list.emplace_back(item);
    }
}

void StorageGameSaveReader::readUnknown1ListItem(StreamHelperReader& stream, inventory::Artifact::UnknownList3Item& item)
{
    stream.Read(item.v1);
    // TODO: What is being being compared to 1?
    //if (something == 1) {
    // Read 2 bytes but I'm not sure if it's int16 or an array
    stream.Read(item.v2);
    //}
}

void StorageGameSaveReader::readArtifactList(StreamHelperReader& stream, inventory::Artifact::List& list)
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
        auto item = readArtifact(stream, list);
        if (item) {
            item->BagNumber = bagNumber;
            item->SlotNumber = slotNumber;
        }

        m_logger << Log::outdent;
        m_logger << "" << std::endl;
    }

    m_logger << Log::outdent;
}

void StorageGameSaveReader::readUnknown2List(StreamHelperReader& stream, std::vector<inventory::Artifact::UnknownList4Item>& list)
{
    auto count = stream.Read<int>();
    for (decltype(count) i = 0; i < count; ++i) {
        // TODO: Investigate. Throw to make sure we know when to do it.
        throw std::runtime_error("This file must be investigated");

        inventory::Artifact::UnknownList4Item item;
        readUnknownStruct1(stream, item.v1);
        readUnknown1List(stream, item.v2);
        list.emplace_back(item);
    }
}

}}} // namespace
