#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>
#include <vanhelsing/engine/log.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveReader::StorageGameSaveReader(GameSave& gameSave, std::istream& inStream) : GameSaveReader(gameSave, inStream)
{
    StreamHelper stream(getStream());
    readItems(stream);
}

void StorageGameSaveReader::readItems(StreamHelper& stream)
{
    auto count = stream.Read<int>();
    Log() << "Items (" << count << "):" << std::endl;
    Log::Indent();
    for (int i = 0; i < count; ++i) {
        Log() << "#" << i << ":" << std::endl;
        Log::Indent();
        auto bagNumber = stream.Read<int>();
        auto slotNumber = stream.Read<int>();
        Log() << "Bag #: " << bagNumber << std::endl;
        Log() << "Slot #: " << slotNumber << std::endl;
        Log() << "" << std::endl;
        readItem(stream);
        Log::Outdent();
        Log() << "" << std::endl;
    }

    if (stream.Read<int>() > 0) {
        // TODO: Need to test with more files because there are more reads here
        throw std::runtime_error("This file must be investigated");
    }

    Log::Outdent();
}

void StorageGameSaveReader::readItem(StreamHelper& stream)
{
    using inventory::Item;
    std::unique_ptr<Item> item(new Item);

    item->Id = stream.Read<unsigned int>();
    item->Attribute1 = stream.Read<int>();
    item->Attribute2 = stream.Read<int>();
    item->Quantity = stream.Read<int>();

    Log() << "ID: 0x" << std::hex << item->Id << std::endl;
    Log() << "Attribute1: " << item->Attribute1 << std::endl;
    Log() << "Attribute2: " << item->Attribute2 << std::endl;
    Log() << "Quantity: " << item->Quantity << std::endl;
    
    auto count = stream.Read<int>();
    Log() << "Unknown (" << count << ":" << std::endl;
    Log::Indent();
    for (int i = 0; i < count; ++i) {
        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        Log() << v1 << ", " << v2 << std::endl;
    }

    Log::Outdent();

    item->Quality = stream.Read<Item::Quality::type>();
    item->Rarity = stream.Read<Item::Rarity::type>();
    Log() << "Quality: " << item->Quality << std::endl;
    Log() << "Rarity: " << item->Rarity << std::endl;

    readItem2(stream);
    readItem3(stream);

    item->IsIdentified = stream.Read<bool>();
    Log() << "Is identified: " << item->IsIdentified << std::endl;
    auto v1 = stream.Read<bool>();
    Log() << "Unknown: " << v1 << std::endl;

    auto& manager = getGameSave().GetInventoryManager();
    manager.Add(item.get());
    // Manager owns it now
    item.release();
}

void StorageGameSaveReader::readItem2(StreamHelper& stream)
{
    auto count = stream.Read<unsigned int>();
    Log() << "Unknown (" << count << ":" << std::endl;
    Log::Indent();
    for (unsigned int i = 0; i < count; ++i) {
        Log() << "#" << i << ":" << std::endl;
        Log::Indent();

        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        auto v3 = stream.Read<float>();
        auto v4 = stream.Read<unsigned int>();

        Log() << "0x" << std::hex << v1 << ", " << v2 << ", " << v3 << ", " << v4 << std::endl;

        if (m_unknown1 >= 0x2b6) {
            auto v5 = stream.Read<int>();
            auto v6 = stream.Read<unsigned int>();
            auto v7 = stream.Read<int>();

            Log() << v5 << ", " << v6 << ", " << v7 << std::endl;
        }

        Log::Outdent();
        Log() << "" << std::endl;
    }

    Log::Outdent();
}

void StorageGameSaveReader::readItem3(StreamHelper& stream)
{
    auto count = stream.Read<unsigned int>();
    Log() << "Unknown (" << count << "):" << std::endl;
    Log::Indent();
    for (unsigned int i = 0; i < count; ++i) {
        Log() << "#" << i << ":" << std::endl;
        Log::Indent();

        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        auto v3 = stream.Read<float>();
        auto v4 = stream.Read<unsigned int>();

        Log() << "0x" << std::hex << v1 << std::endl;
        Log() << v2 << std::endl;
        Log() << v3 << std::endl;
        Log() << v4 << std::endl;
        Log() << "" << std::endl;

        if (m_unknown1 >= 0x2b6) {
            auto v5 = stream.Read<int>();
            auto v6 = stream.Read<unsigned int>();
            auto v7 = stream.Read<int>();

            Log() << v5 << std::endl;
            Log() << v6 << std::endl;
            Log() << v7 << std::endl;
        }

        Log::Outdent();
        Log() << "" << std::endl;
    }

    Log::Outdent();
}

StorageGameSaveReader::~StorageGameSaveReader() {}

}}} // namespace
