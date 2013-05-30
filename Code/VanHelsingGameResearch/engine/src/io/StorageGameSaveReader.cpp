#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StreamHelper.h>
#include <nowide/iostream.hpp>
#include <iomanip>

namespace vanhelsing { namespace engine { namespace io {

StorageGameSaveReader::StorageGameSaveReader(GameSave& gameSave, std::istream& inStream)
    : GameSaveReader(gameSave, inStream), m_logger(LogLevel::Trace)
{
    StreamHelper stream(getStream());
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
        readArtifact(stream);
        m_logger << Log::outdent;
        m_logger << "" << std::endl;
    }

    if (stream.Read<int>() > 0) {
        // TODO: Need to test with more files because there are more reads here
        throw std::runtime_error("This file must be investigated");
    }

    m_logger << Log::outdent;
}

void StorageGameSaveReader::readArtifact(StreamHelper& stream)
{
    using inventory::Artifact;
    std::unique_ptr<Artifact> artifact(new Artifact);

    artifact->Id = stream.Read<Artifact::IdType>();
    artifact->Stat1 = stream.Read<int>();
    artifact->Stat2 = stream.Read<int>();
    artifact->Quantity = stream.Read<int>();

    m_logger << "ID: 0x" << std::hex << artifact->Id << std::dec << std::endl;
    m_logger << "Stat 1: " << artifact->Stat1 << std::endl;
    m_logger << "Stat 2: " << artifact->Stat2 << std::endl;
    m_logger << "Quantity: " << artifact->Quantity << std::endl;

    auto& name = artifact->GetName();
    m_logger << "Name: " << (!name.empty() ? name : "(unknown)") << std::endl;
    
    auto count = stream.Read<int>();
    m_logger << "Unknown (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (int i = 0; i < count; ++i) {
        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        m_logger << v1 << ", " << v2 << std::endl;
    }

    m_logger << Log::outdent;

    artifact->Quality = stream.Read<Artifact::Quality::type>();
    artifact->Rarity = stream.Read<Artifact::Rarity::type>();
    m_logger << "Quality: " << artifact->Quality << std::endl;
    m_logger << "Rarity: " << artifact->Rarity << std::endl;

    readEnchantments(stream);
    readUnknownMaybeEnchantments(stream);

    artifact->IsIdentified = stream.Read<bool>();
    m_logger << "Is identified: " << artifact->IsIdentified << std::endl;
    auto v1 = stream.Read<bool>();
    m_logger << "Unknown: " << v1 << std::endl;

    auto& manager = getGameSave().GetInventoryManager();
    manager.Add(artifact.get());
    // Manager owns it now
    artifact.release();
}

void StorageGameSaveReader::readEnchantments(StreamHelper& stream)
{
    auto count = stream.Read<unsigned int>();
    m_logger << "Enchantments (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (unsigned int i = 0; i < count; ++i) {
        m_logger << "#" << i << ":" << std::endl;
        m_logger << Log::indent;

        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        auto v3 = stream.Read<float>();
        auto v4 = stream.Read<unsigned int>();

        m_logger << "0x" << std::hex << v1 << std::dec << ", " << v2 << ", " << v3 << ", " << v4 << std::endl;

        if (m_unknown1 >= 0x2b6) {
            auto v5 = stream.Read<int>();
            auto v6 = stream.Read<unsigned int>();
            auto v7 = stream.Read<int>();

            m_logger << v5 << ", " << v6 << ", " << v7 << std::endl;
        }

        m_logger << Log::outdent;
        m_logger << "" << std::endl;
    }

    m_logger << Log::outdent;
}

void StorageGameSaveReader::readUnknownMaybeEnchantments(StreamHelper& stream)
{
    auto count = stream.Read<unsigned int>();
    m_logger << "Unknown (" << count << "):" << std::endl;
    m_logger << Log::indent;
    for (unsigned int i = 0; i < count; ++i) {
        m_logger << "#" << i << ":" << std::endl;
        m_logger << Log::indent;

        auto v1 = stream.Read<unsigned int>();
        auto v2 = stream.Read<int>();
        auto v3 = stream.Read<float>();
        auto v4 = stream.Read<unsigned int>();

        m_logger << "0x" << std::hex << v1 << std::dec << std::endl;
        m_logger << v2 << std::endl;
        m_logger << v3 << std::endl;
        m_logger << v4 << std::endl;
        m_logger << "" << std::endl;

        if (m_unknown1 >= 0x2b6) {
            auto v5 = stream.Read<int>();
            auto v6 = stream.Read<unsigned int>();
            auto v7 = stream.Read<int>();

            m_logger << v5 << std::endl;
            m_logger << v6 << std::endl;
            m_logger << v7 << std::endl;
        }

        m_logger << Log::outdent;
        m_logger << "" << std::endl;
    }

    m_logger << Log::outdent;
}

StorageGameSaveReader::~StorageGameSaveReader() {}

}}} // namespace
