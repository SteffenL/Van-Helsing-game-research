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
        stream.Read<int>();
        stream.Read<int>();
        readItem(stream);
    }

    if (stream.Read<int>() > 0) {
        // TODO: Need to test with more files because there are more reads here
        throw std::runtime_error("This file must be investigated");
    }
}

void StorageGameSaveReader::readItem(StreamHelper& stream)
{
    auto itemId = stream.Read<unsigned int>();
    stream.Read<int>();
    stream.Read<int>();
    stream.Read<int>();
    for (int i = 0, count = stream.Read<int>(); i < count; ++i) {
        stream.Read<unsigned int>();
        stream.Read<int>();
    }

    stream.Read<unsigned int>();
    stream.Read<unsigned int>();
    readItem2(stream);
    readItem3(stream);
    stream.Read<bool>();
    stream.Read<bool>();

    auto item = new inventory::Item();

    auto& manager = getGameSave().GetInventoryManager();
    manager.Add(item);
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
