#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveReader::StorageGameSaveReader(GameSave& gameSave, std::istream& inStream) : GameSaveReader(gameSave, inStream)
{
    StreamHelper stream(getStream());
    readItems(stream);
}

void StorageGameSaveReader::readItems(StreamHelper& stream)
{
    for (int i = 0, count = stream.Read<int>(); i < count; ++i) {
        auto bagNumber = stream.Read<int>();
        auto slotNumber = stream.Read<int>();
        readItem(stream);
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
    for (int i = 0, count = stream.Read<int>(); i < count; ++i) {
        stream.Read<unsigned int>();
        stream.Read<int>();
    }

    item->Quality = stream.Read<Item::Quality::type>();
    item->Rarity = stream.Read<Item::Rarity::type>();
    readItem2(stream);
    readItem3(stream);
    item->IsIdentified = stream.Read<bool>();
    stream.Read<bool>();

    auto& manager = getGameSave().GetInventoryManager();
    manager.Add(item.get());
    // Manager owns it now
    item.release();
}

void StorageGameSaveReader::readItem2(StreamHelper& stream)
{
    for (int i = 0, count = stream.Read<unsigned int>(); i < count; ++i) {
        stream.Read<unsigned int>();
        stream.Read<int>();
        stream.Read<float>();
        stream.Read<unsigned int>();

        if (m_unknown1 >= 0x2b6) {
            stream.Read<int>();
            stream.Read<unsigned int>();
            stream.Read<int>();
        }
    }
}

void StorageGameSaveReader::readItem3(StreamHelper& stream)
{
    for (int i = 0, count = stream.Read<unsigned int>(); i < count; ++i) {
        stream.Read<unsigned int>();
        stream.Read<int>();
        stream.Read<float>();
        stream.Read<unsigned int>();

        if (m_unknown1 >= 0x2b6) {
            stream.Read<int>();
            stream.Read<unsigned int>();
            stream.Read<int>();
        }
    }
}

}}} // namespace
