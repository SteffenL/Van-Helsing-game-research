#pragma once

#include <vanhelsing/engine/inventory.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace vanhelsing { namespace engine {

namespace io { namespace n2pk { class N2pkFile; } }

class TextManager
{
    typedef std::map<std::string, std::string> TextMap;

    struct SkillProperty
    {
        SkillProperty() {}

        std::string GetFormattedText(const std::string& value, const std::string& propertyName, const std::string& targetType, const std::string& propertyGroup) const;
        std::string GetFormattedText(int value, const std::string& propertyName, const std::string& targetType, const std::string& propertyGroup) const;
        std::string GetPropertyString(const std::string& propertyName, const std::string& targetType, const std::string& propertyGroup) const;
        std::map<std::string, std::string> PropertyStrings;
    };

    typedef std::map<std::string, SkillProperty> SkillPropertiesTextMap;

public:
    bool Load(const std::string& filePath);
    std::string GetRarityText(inventory::Artifact::Rarity::type rarity) const;
    std::string GetQualityText(inventory::Artifact::Quality::type quality) const;
    std::string GetItemText(const std::string& name) const;
    std::string GetSetNameText(const std::string& name) const;
    /*const std::string GetTypeText(type) const;
    const std::string GetSubTypesText(subType) const;*/
    const SkillProperty& GetSkillPropertyText(const std::string& name) const;

private:
    bool loadArtifactText(const io::n2pk::N2pkFile& package);
    bool loadSkillText(const io::n2pk::N2pkFile& package);

private:
    SkillPropertiesTextMap m_skillProperties;
    TextMap m_rarity;
    TextMap m_quality;
    TextMap m_items;
    TextMap m_enchantments;
    TextMap m_setName;
    TextMap m_types;
    TextMap m_subTypes;
};

class GameData
{
public:
    class ItemData
    {
    public:
        inventory::Artifact::IdType Id;
        std::string Name;
        std::string Icon;
    };

    class EnchantmentData
    {
    public:
        inventory::Enchantment::IdType Id;
        std::string Name;
        std::string Property;
        std::vector<int> MinValue;
        std::vector<int> MaxValue;
        std::string TargetType;
        std::string PriorityGroup;

        bool CalculateValue(int& value, const inventory::Enchantment& enchantment) const;
    };

    GameData();
    static GameData& Get();
    void Load(const std::string& gameDir);
    // Inventory items
    bool GetDataFor(inventory::Artifact::IdType id, ItemData& data) const;
    bool GetDataFor(inventory::Enchantment::IdType id, EnchantmentData& data) const;
    inventory::Artifact::IdType GetArtifactIdFromName(const std::string& name) const;
    const TextManager& GetTextManager() const;
    std::string GetItemNameFromId(inventory::Artifact::IdType id) const;
    std::string GetEnchantmentNameFromId(inventory::Artifact::IdType id) const;
    const std::vector<inventory::Artifact::Quality::type> GetQualityDataList() const;
    const std::vector<inventory::Artifact::Rarity::type> GetRarityDataList() const;
    bool GetArtifactIcon(const std::string& name, std::vector<char>& imageData) const;

private:
    void loadTexts();
    void loadArtifacts();
    void loadEnchantments();
    void loadArtifactIcons();

private:
    static std::unique_ptr<GameData> m_instance;
    std::string m_gameDir;
    class Impl;
    std::unique_ptr<Impl> m_impl;
    TextManager m_texts;
    std::unique_ptr<io::n2pk::N2pkFile> m_artifactIconsPackage;
};

}}
