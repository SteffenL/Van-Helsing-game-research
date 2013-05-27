#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>
#include <nowide/iostream.hpp>
#include <iomanip>

int g_logIndent = 0;
#define LOG_INDENT() g_logIndent += 1
#define LOG_OUTDENT() g_logIndent -= 1
#define LOG(s) nowide::cout << std::setfill(' ') << std::setw(2 * g_logIndent) << "" << s << std::dec << std::endl

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveReader::StorageGameSaveReader(GameSave& gameSave, std::istream& inStream) : GameSaveReader(gameSave, inStream)
{
    StreamHelper stream(getStream());
    readItems(stream);
}

void StorageGameSaveReader::readItems(StreamHelper& stream)
{
    for (int i = 0, count = stream.Read<int>(); i < count; ++i) {
        LOG("Item " << i << ":");
        LOG_INDENT();
        auto bagNumber = stream.Read<int>();
        auto slotNumber = stream.Read<int>();
        LOG("Bag #: " << bagNumber);
        LOG("Slot #: " << slotNumber);
        LOG("");
        readItem(stream);
        LOG_OUTDENT();
        LOG("");
    }

    if (stream.Read<int>() > 0) {
        // TODO: Need to test with more files because there are more reads here
        throw std::runtime_error("This file must be investigated");
    }
}

void StorageGameSaveReader::readItem(StreamHelper& stream)
{
    using inventory::Item;
    std::unique_ptr<Item> item(new Item);

    item->Id = stream.Read<unsigned int>();
    item->Attribute1 = stream.Read<int>();
    item->Attribute2 = stream.Read<int>();
    item->Quantity = stream.Read<int>();

    LOG("ID: 0x" << std::hex << item->Id);
    LOG("Attribute1: " << item->Attribute1);
    LOG("Attribute2: " << item->Attribute2);
    LOG("Quantity: " << item->Quantity);

    LOG("Unknown:");
    LOG_INDENT();
    for (int i = 0, count = stream.Read<int>(); i < count; ++i) {
        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        LOG(v1 << ", " << v2);
    }

    LOG_OUTDENT();

    item->Quality = stream.Read<Item::Quality::type>();
    item->Rarity = stream.Read<Item::Rarity::type>();
    LOG("Quality: " << item->Quality);
    LOG("Rarity: " << item->Rarity);

    readItem2(stream);
    readItem3(stream);

    item->IsIdentified = stream.Read<bool>();
    LOG("Is identified: " << item->IsIdentified);
    auto v1 = stream.Read<bool>();
    LOG("Unknown: " << v1);

    auto& manager = getGameSave().GetInventoryManager();
    manager.Add(item.get());
    // Manager owns it now
    item.release();
}

void StorageGameSaveReader::readItem2(StreamHelper& stream)
{
    LOG("Unknown:");
    LOG_INDENT();
    for (int i = 0, count = stream.Read<unsigned int>(); i < count; ++i) {
        LOG("#" << i << ":");
        LOG_INDENT();

        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        auto v3 = stream.Read<float>();
        auto v4 = stream.Read<unsigned int>();

        LOG("0x" << std::hex << v1 << ", " << v2 << ", " << v3 << ", " << v4);

        if (m_unknown1 >= 0x2b6) {
            auto v5 = stream.Read<int>();
            auto v6 = stream.Read<unsigned int>();
            auto v7 = stream.Read<int>();

            LOG(v5 << ", " << v6 << ", " << v7);
        }

        LOG_OUTDENT();
        LOG("");
    }

    LOG_OUTDENT();
}

void StorageGameSaveReader::readItem3(StreamHelper& stream)
{
    LOG("Unknown:");
    LOG_INDENT();
    for (int i = 0, count = stream.Read<unsigned int>(); i < count; ++i) {
        LOG("#" << i << ":");
        LOG_INDENT();

        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        auto v3 = stream.Read<float>();
        auto v4 = stream.Read<unsigned int>();

        LOG("0x" << std::hex << v1);
        LOG(v2);
        LOG(v3);
        LOG(v4);
        LOG("");

        if (m_unknown1 >= 0x2b6) {
            auto v5 = stream.Read<int>();
            auto v6 = stream.Read<unsigned int>();
            auto v7 = stream.Read<int>();

            LOG(v5);
            LOG(v6);
            LOG(v7);
        }

        LOG_OUTDENT();
        LOG("");
    }

    LOG_OUTDENT();
}

}}} // namespace
