#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/log.h>
#include <vanhelsing/engine/CfgParser.h>
#include <vanhelsing/engine/inventory.h>

#include <boost/filesystem.hpp>
#include <nowide/fstream.hpp>
#include <iomanip>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

#include <array>

namespace bmi = boost::multi_index;

namespace vanhelsing { namespace engine {

class GameData::Impl
{
    friend class GameData;

public:
    typedef GameData::ArtifactData ArtifactData;
    struct ArtifactDataContainer
    {
        typedef bmi::multi_index_container<
            ArtifactData,
            bmi::indexed_by<
                bmi::ordered_unique<bmi::member<ArtifactData, inventory::Artifact::IdType, &ArtifactData::Id>>,
                bmi::ordered_unique<bmi::member<ArtifactData, std::string, &ArtifactData::Name>>
            >
        > type;
    };

    typedef GameData::EnchantmentData EnchantmentData;
    struct EnchantmentDataContainer
    {
        typedef bmi::multi_index_container<
            EnchantmentData,
            bmi::indexed_by<
                bmi::ordered_unique<bmi::member<EnchantmentData, inventory::Enchantment::IdType, &EnchantmentData::Id>>,
                bmi::ordered_unique<bmi::member<EnchantmentData, std::string, &EnchantmentData::Name>>
            >
        > type;
    };

    bool GetItemData(inventory::Artifact::IdType id, ArtifactData& data);
    bool GetItemData(inventory::Enchantment::IdType id, EnchantmentData& data);

private:
    ArtifactDataContainer::type m_artifactData;
    EnchantmentDataContainer::type m_enchantmentData;
};

bool GameData::Impl::GetItemData(inventory::Artifact::IdType id, ArtifactData& data)
{
    auto& view = m_artifactData.get<0>();
    auto it = view.find(id);
    if (it == m_artifactData.end()) {
        return false;
    }

    data = *it;
    return true;
}

bool GameData::Impl::GetItemData(inventory::Enchantment::IdType id, EnchantmentData& data)
{
    auto& view = m_enchantmentData.get<0>();
    auto it = view.find(id);
    if (it == m_enchantmentData.end()) {
        return false;
    }

    data = *it;
    return true;
}

std::unique_ptr<GameData> GameData::m_instance = std::unique_ptr<GameData>(new GameData);

void GameData::Load(const std::string& gameDir)
{
    m_gameDir = gameDir;
    if (!boost::filesystem::exists(m_gameDir)) {
        using namespace vanhelsing::engine;
        Log(LogLevel::Error) << "Game directory doesn't exist: " << m_gameDir << std::endl;
        return;
    }

    try {
        loadArtifacts();
        loadEnchantments();
    }
    catch (std::runtime_error& ex) {
        Log(LogLevel::Error) << "Exception while loading game data: " << ex.what() << std::endl;
        return;
    }
}

void GameData::loadArtifacts()
{
    namespace fs = boost::filesystem;
    fs::path filePath(m_gameDir);
    filePath /= "Cfg/Artifact/artifacts.cfg";

    if (!fs::exists(filePath)) {
        Log(LogLevel::Error) << "Game data file doesn't exist: " << filePath.string() << std::endl;
        return;
    }

    nowide::ifstream file(filePath.string().c_str());
    if (!file.is_open()) {
        Log(LogLevel::Error) << "Couldn't open file: " << filePath.string() << std::endl;
        return;
    }

    char buf[8 * 1024];
    file.rdbuf()->pubsetbuf(buf, sizeof(buf));
    CfgParser parser(file);
    try {
        parser.Parse();
    }
    catch (std::runtime_error&) {
        Log(LogLevel::Error) << "Failed to parse file: " << filePath.string() << std::endl;
        return;
    }

    auto& logger = Log(LogLevel::Trace);
    Log(LogLevel::Trace) << "Loading artifacts..." << std::endl;
    for (auto& group : parser.GetGroups()) {
        using namespace inventory;
        ArtifactData data;
        data.Name = group.at("Name");
        data.Id = GetItemIdFromName(data.Name);
        data.Icon = group.at("Icon");
        m_impl->m_artifactData.insert(data);
        //logger << "0x" << std::setfill('0') << std::setw(8) << std::hex << id << std::dec << "  " << name << std::endl;

    }
}

void GameData::loadEnchantments()
{
    namespace fs = boost::filesystem;
    fs::path filePath(m_gameDir);
    filePath /= "Cfg/Artifact/enchantments.cfg";

    if (!fs::exists(filePath)) {
        Log(LogLevel::Error) << "Game data file doesn't exist: " << filePath.string() << std::endl;
        return;
    }

    nowide::ifstream file(filePath.string().c_str());
    if (!file.is_open()) {
        Log(LogLevel::Error) << "Couldn't open file: " << filePath.string() << std::endl;
        return;
    }

    char buf[8 * 1024];
    file.rdbuf()->pubsetbuf(buf, sizeof(buf));
    CfgParser parser(file);
    try {
        parser.Parse();
    }
    catch (std::runtime_error&) {
        Log(LogLevel::Error) << "Failed to parse file: " << filePath.string() << std::endl;
        return;
    }

    auto& logger = Log(LogLevel::Trace);
    Log(LogLevel::Trace) << "Loading enchantments..." << std::endl;
    for (auto& group : parser.GetGroups()) {
        using namespace inventory;
        EnchantmentData data;
        data.Name = group.at("Name");
        data.Id = GetItemIdFromName(data.Name);
        m_impl->m_enchantmentData.insert(data);
        //logger << "0x" << std::setfill('0') << std::setw(8) << std::hex << id << std::dec << "  " << name << std::endl;
    }
}

GameData::GameData() : m_impl(new Impl) {}

GameData& GameData::Get()
{
    if (!m_instance.get()) {
        m_instance.reset(new GameData);
    }

    return *m_instance;
}

bool GameData::GetItemData(inventory::Artifact::IdType id, ArtifactData& data) const
{
    return m_impl->GetItemData(id, data);
}

bool GameData::GetItemData(inventory::Enchantment::IdType id, EnchantmentData& data) const
{
    return m_impl->GetItemData(id, data);
}

inventory::Item::IdType GameData::GetItemIdFromName(const std::string& name) const
{
    inventory::Item::IdType id = 0;
    for (auto ch : name) {
        id *= 0x01003f;
        id += ch;
    }

    return id;
}

std::string GameData::GetArtifactNameFromId(inventory::Artifact::IdType id) const
{
    ArtifactData data;
    if (!GetItemData(id, data)) {
        return std::string();
    }

    return data.Name;
}

std::string GameData::GetEnchantmentNameFromId(inventory::Enchantment::IdType id) const
{
    EnchantmentData data;
    if (!GetItemData(id, data)) {
        return std::string();
    }

    return data.Name;
}

const char* GameData::GetRarityText(inventory::Artifact::Rarity::type rarity) const
{
    static std::array<const char*, 6> text = {
        "Normal", "Magic", "Rare", "Epic", "Set", "Random"
    };

    try {
        return text.at(rarity);
    }
    catch (std::out_of_range&) {
        return "(invalid)";
    }
}

const char* GameData::GetQualityText(inventory::Artifact::Quality::type quality) const
{
    static std::array<const char*, 3> text = {
        "Normal", "Cracked", "Masterwork"
    };

    try {
        return text.at(quality);
    }
    catch (std::out_of_range&) {
        return "(invalid)";
    }
}

}} // namespace
