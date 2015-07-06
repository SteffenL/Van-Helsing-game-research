#include <vanhelsing/engine/GameData.h>
#include <common/Log.h>
#include <vanhelsing/engine/CfgParser.h>
#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/io/n2pk/N2pkFile.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>

#include <pugixml.hpp>
#include <nowide/fstream.hpp>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <array>
#include <functional>
#include <map>

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
                bmi::ordered_unique<bmi::member<ItemData, inventory::Artifact::IdType, &ItemData::Id>>,
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

    bool GetItemData(inventory::Artifact::IdType id, ItemData& data);
    bool GetItemData(inventory::Enchantment::IdType id, EnchantmentData& data);

private:
    ItemDataContainer::type m_itemData;
    EnchantmentDataContainer::type m_enchantmentData;
};

bool GameData::Impl::GetItemData(inventory::Artifact::IdType id, ItemData& data)
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

std::unique_ptr<GameData> GameData::m_instance = std::make_unique<GameData>();

void GameData::Load(const std::string& gameDir)
{
    m_gameDir = gameDir;
    if (!boost::filesystem::exists(m_gameDir)) {
        using namespace common;
        Log(LogLevel::Error) << "Game directory doesn't exist: " << m_gameDir << std::endl;
        return;
    }

    try {
        loadTexts();
        loadArtifacts();
        loadEnchantments();
        loadArtifactIcons();
    }
    catch (const VanHelsingEngineError& ex) {
        using namespace common;
        Log(LogLevel::Error) << "Exception while loading game data: " << ex.what() << std::endl;
        return;
    }
}

void GameData::loadArtifacts()
{
    using namespace common;
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
    catch (const VanHelsingEngineError&) {
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
    using namespace common;
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
    catch (const VanHelsingEngineError&) {
        Log(LogLevel::Error) << "Failed to parse file: " << filePath.string() << std::endl;
        return;
    }

    auto& logger = Log(LogLevel::Trace);
    Log(LogLevel::Trace) << "Loading enchantments..." << std::endl;
    for (auto& group : parser.GetGroups()) {
        using namespace inventory;
        EnchantmentData data;

        auto nameIter = group.find("Name");
        if (nameIter != group.end()) {
            data.Name = nameIter->second;
            data.Id = GetArtifactIdFromName(data.Name);
        }

        auto propertyIter = group.find("Property");
        if (propertyIter != group.end()) {
            data.Property = propertyIter->second;
        }

        auto minValueIter = group.find("MinValue");
        if (minValueIter != group.end()) {
            std::vector<std::string> temp;
            boost::split(temp, minValueIter->second, boost::is_any_of(","));
            for (auto& s : temp) {
                boost::trim(s);
                data.MinValue.push_back(boost::lexical_cast<int>(s));
            }
        }

        auto maxValueIter = group.find("MaxValue");
        if (maxValueIter != group.end()) {
            std::vector<std::string> temp;
            boost::split(temp, maxValueIter->second, boost::is_any_of(","));
            for (auto& s : temp) {
                boost::trim(s);
                data.MaxValue.push_back(boost::lexical_cast<int>(s));
            }
        }

        auto targetTypeIter = group.find("TargetType");
        if (targetTypeIter != group.end()) {
            data.TargetType = targetTypeIter->second;
        }

        auto priorityGroupIter = group.find("PriorityGroup");
        if (priorityGroupIter != group.end()) {
            data.PriorityGroup = priorityGroupIter->second;
        }

        m_impl->m_enchantmentData.insert(data);
        //logger << "0x" << std::setfill('0') << std::setw(8) << std::hex << id << std::dec << "  " << name << std::endl;
    }
}

void GameData::loadArtifactIcons()
{
    using namespace common;
    namespace fs = boost::filesystem;
    fs::path filePath(m_gameDir);
    filePath /= "UI/Artifacts/Files.N2PK";

    if (!fs::exists(filePath)) {
        Log(LogLevel::Error) << "Game data file doesn't exist: " << filePath.string() << std::endl;
        return;
    }

    nowide::ifstream file(filePath.string().c_str());
    if (!file.is_open()) {
        Log(LogLevel::Error) << "Couldn't open file: " << filePath.string() << std::endl;
        return;
    }

    m_artifactIconsPackage = std::make_unique<io::n2pk::N2pkFile>(filePath.string());
}

GameData::GameData() : m_impl(std::make_unique<Impl>()) {}

GameData& GameData::Get()
{
    if (!m_instance) {
        m_instance = std::make_unique<GameData>();
    }

    return *m_instance;
}

bool GameData::GetDataFor(inventory::Artifact::IdType id, ItemData& data) const
{
    return m_impl->GetItemData(id, data);
}

bool GameData::GetDataFor(inventory::Enchantment::IdType id, EnchantmentData& data) const
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

std::string GameData::GetItemNameFromId(inventory::Artifact::IdType id) const
{
    ItemData data;
    if (!GetDataFor(id, data)) {
        return std::string();
    }

    return data.Name;
}

std::string GameData::GetEnchantmentNameFromId(inventory::Enchantment::IdType id) const
{
    EnchantmentData data;
    if (!GetDataFor(id, data)) {
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
    using namespace common;
    namespace fs = boost::filesystem;
    fs::path filePath(m_gameDir);
    filePath /= "Strings/Files.N2PK";

    if (!fs::exists(filePath)) {
        Log(LogLevel::Error) << "In-game text file doesn't exist: " << filePath.string() << std::endl;
        return;
    }

    m_texts.Load(filePath.string());
}

const std::vector<vanhelsing::engine::inventory::Artifact::Quality::type> GameData::GetQualityDataList() const
{
    using vanhelsing::engine::inventory::Artifact;
    std::vector<Artifact::Quality::type> list;
    list.push_back(Artifact::Quality::Normal);
    list.push_back(Artifact::Quality::Cracked);
    list.push_back(Artifact::Quality::Masterwork);
    return list;
}

const std::vector<vanhelsing::engine::inventory::Artifact::Rarity::type> GameData::GetRarityDataList() const
{
    using inventory::Artifact;
    std::vector<Artifact::Rarity::type> list;
    list.push_back(Artifact::Rarity::Normal);
    list.push_back(Artifact::Rarity::Magic);
    list.push_back(Artifact::Rarity::Rare);
    list.push_back(Artifact::Rarity::Epic);
    list.push_back(Artifact::Rarity::Set);
    list.push_back(Artifact::Rarity::Random);
    return list;
}

bool GameData::GetArtifactIcon(const std::string& name, std::vector<char>& imageData) const
{
    if (!m_artifactIconsPackage) {
        // TODO: Log error instead of throwing?
        //throw VanHelsingEngineError("Artifact icon package is not loaded: " + name);
        return false;
    }

    const std::string& fileName = name + "_idle.tga";
    auto file = m_artifactIconsPackage->GetFile(fileName);
    if (!file) {
        // TODO: Log error instead of throwing?
        //throw VanHelsingEngineError("Failed to get artifact icon: " + name);
        return false;
    }

    auto& fileEntry = m_artifactIconsPackage->GetFileEntry(fileName);
    imageData.resize(static_cast<std::vector<char>::size_type>(fileEntry.GetSize()));
    file->read(imageData.data(), fileEntry.GetSize());

    return true;
}

bool TextManager::Load(const std::string& filePath)
{
    io::n2pk::N2pkFile package(filePath);

    loadArtifactText(package);
    loadSkillText(package);

    return true;
}

std::string TextManager::GetRarityText(inventory::Artifact::Rarity::type rarity) const
{
    using inventory::Artifact;

    std::string textName;
    switch (rarity)
    {
    case Artifact::Rarity::Normal:
        textName = "Normal";
        break;

    case Artifact::Rarity::Magic:
        textName = "Magic";
        break;

    case Artifact::Rarity::Rare:
        textName = "Rare";
        break;

    case Artifact::Rarity::Epic:
        textName = "Epic";
        break;

    case Artifact::Rarity::Set:
        textName = "Set";
        break;

    case Artifact::Rarity::Random:
        return "Random?";
        break;

    default:
        {
            std::stringstream ss;
            ss << rarity << " (invalid)";
            return ss.str();
        }
    }

    std::transform(textName.begin(), textName.end(), textName.begin(), ::tolower);
    try {
        return m_rarity.at(textName);
    }
    catch (std::out_of_range&) {
        std::stringstream ss;
        ss << rarity << " (invalid)";
        return ss.str();
    }
}

std::string TextManager::GetQualityText(inventory::Artifact::Quality::type quality) const
{
    using inventory::Artifact;

    std::string textName;
    switch (quality)
    {
    case Artifact::Quality::Normal:
        return "";

    case Artifact::Quality::Cracked:
        textName = "Cracked";
        break;

    case Artifact::Quality::Masterwork:
        textName = "Masterwork";
        break;

    default:
        {
            std::stringstream ss;
            ss << quality << " (invalid)";
            return ss.str();
        }
    }

    std::transform(textName.begin(), textName.end(), textName.begin(), ::tolower);
    try {
        return m_quality.at(textName);
    }
    catch (std::out_of_range&) {
        std::stringstream ss;
        ss << quality << " (invalid)";
        return ss.str();
    }
}

std::string TextManager::GetItemText(const std::string& name) const
{
    std::string nameLc(name);
    std::transform(nameLc.begin(), nameLc.end(), nameLc.begin(), ::tolower);
    try {
        return m_items.at(nameLc);
    }
    catch (std::out_of_range&) {
        std::string text(name);
        text += " (no name)";
        return text;
    }
}

std::string TextManager::GetSetNameText(const std::string& name) const
{
    std::string nameLc(name);
    std::transform(nameLc.begin(), nameLc.end(), nameLc.begin(), ::tolower);
    try {
        return m_setName.at(nameLc);
    }
    catch (std::out_of_range&) {
        std::string text(name);
        text += " (no name)";
        return text;
    }
}

const TextManager::SkillProperty& TextManager::GetSkillPropertyText(const std::string& name) const
{
    try {
        return m_skillProperties.at(name);
    }
    catch (std::out_of_range&) {
        static SkillProperty emptySkillProperty;
        return emptySkillProperty;
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
        throw VanHelsingEngineError("Error while parsing language XML file.");
    }

    // Items
    {
        auto items = doc.select_nodes("/Root/Artifacts/Items/*");
        for (pugi::xpath_node_set::const_iterator it = items.begin(), end = items.end(); it != end; ++it)
        {
            auto& node = it->node();
            std::string name = node.name();
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
        throw VanHelsingEngineError("Error while parsing language XML file.");
    }

    // Properties
    {
        auto items = doc.select_nodes("/Root/Property/*");
        for (pugi::xpath_node_set::const_iterator propertyRootNodeIter = items.begin(), end = items.end(); propertyRootNodeIter != end; ++propertyRootNodeIter)
        {
            auto& propertyRootNode = propertyRootNodeIter->node();
            std::string propertyName = propertyRootNode.name();
            //std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            //std::string desc = node.child("desc").first_child().child_value();
            //std::string bonusPercent = node.child("bonus_percent").child("desc").first_child().child_value();

            auto f = [](std::map<std::string, std::string>& strings, const pugi::xml_node& rootNode) {
                std::function<void(const pugi::xml_node&, std::string)> innerFunc = [&](const pugi::xml_node& node, std::string pathStr) {
                    if (!node.first_child()) {
                        strings[pathStr] = node.value();
                        return;
                    }

                    pathStr += "/";
                    pathStr += node.name();

                    std::string p;
                    for (const auto& child : node.children()) {
                        innerFunc(child, pathStr);
                    }
                };

                std::string pathStr;
                return innerFunc(rootNode, pathStr);
            };

            auto& propertyText = m_skillProperties[propertyName];
            f(propertyText.PropertyStrings, propertyRootNode);
        }
    }

    return true;
}

std::string TextManager::SkillProperty::GetFormattedText(int value, const std::string& propertyName, const std::string& targetType, const std::string& propertyGroup) const
{
    std::string propertyString = GetPropertyString(propertyName, targetType, propertyGroup);
    std::string tempStr = propertyString;
    auto sign = (value < 0) ? "-" : (value > 0) ? "+" : "";

    if (boost::find_first(tempStr, "$VALUE_PER_LEVEL$")) {
        // We don't know the character's level
        boost::replace_first(tempStr, "$SIGN$", "");
        boost::replace_first(tempStr, "$VALUE$", "?");

        boost::replace_first(tempStr, "$SIGN$", sign);
        boost::replace_first(tempStr, "$VALUE_PER_LEVEL$", std::to_string(value));
    }
    else {
        boost::replace_first(tempStr, "$SIGN$", sign);
        boost::replace_first(tempStr, "$VALUE$", std::to_string(value));
    }

    return tempStr;
}

std::string TextManager::SkillProperty::GetFormattedText(const std::string& value, const std::string& propertyName, const std::string& targetType, const std::string& propertyGroup) const
{
    const auto& propertyString = GetPropertyString(propertyName, targetType, propertyGroup);
    std::string tempStr = propertyString;

    if (boost::find_first(tempStr, "$VALUE_PER_LEVEL$")) {
        // We don't know the character's level
        boost::replace_first(tempStr, "$SIGN$", "");
        boost::replace_first(tempStr, "$VALUE$", "?");

        boost::replace_first(tempStr, "$SIGN$", "");
        boost::replace_first(tempStr, "$VALUE_PER_LEVEL$", value);
    }
    else {
        boost::replace_first(tempStr, "$SIGN$", "");
        boost::replace_first(tempStr, "$VALUE$", value);
    }

    return tempStr;
}

std::string TextManager::SkillProperty::GetPropertyString(const std::string& propertyName, const std::string& targetType, const std::string& propertyGroup) const
{
    std::string stringPath = "/" + propertyName;

    if (!targetType.empty()) {
        stringPath += "/" + targetType;
    }

    if (!propertyGroup.empty()) {
        stringPath += "/" + propertyGroup;
    }

    stringPath += "/desc/eng";

    const auto& propertyString = PropertyStrings.at(stringPath);
    return propertyString;
}

bool GameData::EnchantmentData::CalculateValue(int& value, const inventory::Enchantment& enchantment) const
{
    try {
        auto minValue = MinValue.at(enchantment.ValueIndex);
        if (!MaxValue.empty()) {
            auto maxValue = MaxValue.at(enchantment.ValueIndex);
            // Round half down
            value = static_cast<int>(std::ceil(minValue + (maxValue - minValue) * enchantment.ValueScale - 0.5f));
            return true;
        }

        value = minValue;
        return true;
    }
    catch (std::out_of_range&) {
        return false;
    }
}

}}
