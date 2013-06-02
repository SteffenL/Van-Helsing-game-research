#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/log.h>
#include <vanhelsing/engine/CfgParser.h>
#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/io/n2pk/N2pkFile.h>

#include <pugixml.hpp>
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
    typedef GameData::ItemData ItemData;
    struct ItemDataContainer
    {
        typedef bmi::multi_index_container<
            ItemData,
            bmi::indexed_by<
                bmi::ordered_unique<bmi::member<ItemData, inventory::Item::IdType, &ItemData::Id>>,
                bmi::ordered_unique<bmi::member<ItemData, std::string, &ItemData::Name>>
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

    bool GetItemData(inventory::Item::IdType id, ItemData& data);
    bool GetItemData(inventory::Enchantment::IdType id, EnchantmentData& data);

private:
    ItemDataContainer::type m_itemData;
    EnchantmentDataContainer::type m_enchantmentData;
};

bool GameData::Impl::GetItemData(inventory::Item::IdType id, ItemData& data)
{
    auto& view = m_itemData.get<0>();
    auto it = view.find(id);
    if (it == m_itemData.end()) {
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
        loadTexts();
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
        ItemData data;
        data.Name = group.at("Name");
        data.Id = GetArtifactIdFromName(data.Name);
        data.Icon = group.at("Icon");
        m_impl->m_itemData.insert(data);
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
        data.Id = GetArtifactIdFromName(data.Name);
        //data.
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

bool GameData::GetArtifactData(inventory::Item::IdType id, ItemData& data) const
{
    return m_impl->GetItemData(id, data);
}

bool GameData::GetArtifactData(inventory::Enchantment::IdType id, EnchantmentData& data) const
{
    return m_impl->GetItemData(id, data);
}

inventory::Artifact::IdType GameData::GetArtifactIdFromName(const std::string& name) const
{
    inventory::Artifact::IdType id = 0;
    for (auto ch : name) {
        id *= 0x01003f;
        id += ch;
    }

    return id;
}

std::string GameData::GetItemNameFromId(inventory::Item::IdType id) const
{
    ItemData data;
    if (!GetArtifactData(id, data)) {
        return std::string();
    }

    return data.Name;
}

std::string GameData::GetEnchantmentNameFromId(inventory::Enchantment::IdType id) const
{
    EnchantmentData data;
    if (!GetArtifactData(id, data)) {
        return std::string();
    }

    return data.Name;
}

const TextManager& GameData::GetTextManager() const
{
    return m_texts;
}

void GameData::loadTexts()
{
    namespace fs = boost::filesystem;
    fs::path filePath(m_gameDir);
    filePath /= "Strings/Files.N2PK";

    if (!fs::exists(filePath)) {
        Log(LogLevel::Error) << "In-game text file doesn't exist: " << filePath.string() << std::endl;
        return;
    }

    m_texts.Load(filePath.string());
}


bool TextManager::Load(const std::string& filePath)
{
    io::n2pk::N2pkFile package(filePath);

    loadArtifactText(package);
    loadSkillText(package);

    return true;
}

const std::string TextManager::GetRarityText(inventory::Item::Rarity::type rarity) const
{
    const char* textName = nullptr;
    switch (rarity)
    {
    case vanhelsing::engine::inventory::Item::Rarity::Normal:
        textName = "Normal";
        break;

    case vanhelsing::engine::inventory::Item::Rarity::Magic:
        textName = "Magic";
        break;

    case vanhelsing::engine::inventory::Item::Rarity::Rare:
        textName = "Rare";
        break;

    case vanhelsing::engine::inventory::Item::Rarity::Epic:
        textName = "Epic";
        break;

    case vanhelsing::engine::inventory::Item::Rarity::Set:
        textName = "Set";
        break;

    case vanhelsing::engine::inventory::Item::Rarity::Random:
        return "Random?";
        break;

    default:
        return "(invalid)";
    }

    try {
        return m_rarity.at(textName);
    }
    catch (std::out_of_range&) {
        return "(invalid)";
    }
}

const std::string TextManager::GetQualityText(inventory::Item::Quality::type quality) const
{
    const char* textName = nullptr;
    switch (quality)
    {
    case vanhelsing::engine::inventory::Item::Quality::Normal:
        return "";

    case vanhelsing::engine::inventory::Item::Quality::Cracked:
        textName = "Cracked";
        break;

    case vanhelsing::engine::inventory::Item::Quality::Masterwork:
        textName = "Masterwork";
        break;

    default:
        return "(invalid)";
    }

    try {
        return m_quality.at(textName);
    }
    catch (std::out_of_range&) {
        return "(invalid)";
    }
}

const std::string TextManager::GetItemText(const std::string& name) const
{
    try {
        return m_items.at(name);
    }
    catch (std::out_of_range&) {
        std::string text(name);
        text += " (no name)";
        return text;
    }
}

const std::string TextManager::GetSetNameText(const std::string& name) const
{
    try {
        return m_setName.at(name);
    }
    catch (std::out_of_range&) {
        std::string text(name);
        text += " (no name)";
        return text;
    }
}

bool TextManager::loadArtifactText(const io::n2pk::N2pkFile& package)
{
    auto file = package.GetFile("lang_artifacts.xml");
    if (!file) {
        return false;
    }

    auto& fileEntry = package.GetFileEntry("lang_artifacts.xml");

    pugi::xml_document doc;
    std::vector<char> buf;
    // Disable warnings about conversion and possible loss of data
#pragma warning(push)
#pragma warning(disable: 4244)
    buf.resize(fileEntry.GetSize());
    file->read(&buf[0], fileEntry.GetSize());
    pugi::xml_parse_result result = doc.load_buffer_inplace(&buf[0], fileEntry.GetSize());
#pragma warning(pop)

    if (!result) {
        throw std::runtime_error("Error while parsing language XML file.");
    }

    // Items
    {
        auto items = doc.select_nodes("/Root/Artifacts/Items/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.child("Name").first_child().child_value();
            m_items[name] = text;
        }
    }

    // Enchantments
    {
        auto items = doc.select_nodes("/Root/Artifacts/Enchantment/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.child("desc").first_child().child_value();
            m_enchantments[name] = text;
        }
    }

    // Quality
    {
        auto items = doc.select_nodes("/Root/Artifacts/Quality/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.first_child().child_value();
            m_quality[name] = text;
        }
    }

    // Rarity
    {
        auto items = doc.select_nodes("/Root/Artifacts/Rarity/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.first_child().child_value();
            m_rarity[name] = text;
        }
    }

    // Set name
    {
        auto items = doc.select_nodes("/Root/Artifacts/SetName/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.first_child().child_value();
            m_setName[name] = text;
        }
    }

    // Types
    {
        auto items = doc.select_nodes("/Root/Artifacts/Types/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.first_child().child_value();
            m_types[name] = text;
        }
    }

    // SubTypes
    {
        auto items = doc.select_nodes("/Root/Artifacts/SubTypes/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.first_child().child_value();
            m_subTypes[name] = text;
        }
    }

    return true;
}

bool TextManager::loadSkillText(const io::n2pk::N2pkFile& package)
{
    auto file = package.GetFile("lang_skills.xml");
    if (!file) {
        return false;
    }

    auto& fileEntry = package.GetFileEntry("lang_skills.xml");

    pugi::xml_document doc;
    std::vector<char> buf;
    // Disable warnings about conversion and possible loss of data
#pragma warning(push)
#pragma warning(disable: 4244)
    buf.resize(fileEntry.GetSize());
    file->read(&buf[0], fileEntry.GetSize());
    pugi::xml_parse_result result = doc.load_buffer_inplace(&buf[0], fileEntry.GetSize());
#pragma warning(pop)

    if (!result) {
        throw std::runtime_error("Error while parsing language XML file.");
    }

    // Properties
    {
        auto items = doc.select_nodes("/Root/Property/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::string text = node.child("desc").first_child().child_value();
            m_skillProperties[name] = text;
        }
    }

    return true;
}

}} // namespace
