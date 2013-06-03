#ifndef vanhelsing_engine_GameData_
#define vanhelsing_engine_GameData_

#include <vanhelsing/engine/inventory.h>
#include <string>
#include <map>
#include <memory>

namespace vanhelsing { namespace engine {

namespace io { namespace n2pk { class N2pkFile; } }

class TextManager
{
    typedef std::map<std::string, std::string> TextMap;

public:
    bool Load(const std::string& filePath);
    const std::string GetRarityText(inventory::Item::Rarity::type rarity) const;
    const std::string GetQualityText(inventory::Item::Quality::type quality) const;
    const std::string GetItemText(const std::string& name) const;
    const std::string GetSetNameText(const std::string& name) const;
    /*const std::string GetTypeText(type) const;
    const std::string GetSubTypesText(subType) const;*/

private:
    bool loadArtifactText(const io::n2pk::N2pkFile& package);
    bool loadSkillText(const io::n2pk::N2pkFile& package);

private:
    TextMap m_skillProperties;
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
        inventory::Item::IdType Id;
        std::string Name;
        std::string Icon;
    };

    class EnchantmentData
    {
    public:
        inventory::Enchantment::IdType Id;
        std::string Name;
    };

    GameData();
    static GameData& Get();
    void Load(const std::string& gameDir);
    // Inventory items
    bool GetArtifactData(inventory::Item::IdType id, ItemData& data) const;
    bool GetArtifactData(inventory::Enchantment::IdType id, EnchantmentData& data) const;
    inventory::Artifact::IdType GetArtifactIdFromName(const std::string& name) const;
    const TextManager& GetTextManager() const;
    std::string GetItemNameFromId(inventory::Item::IdType id) const;
    std::string GetEnchantmentNameFromId(inventory::Artifact::IdType id) const;

private:
    void loadTexts();
    void loadArtifacts();
    void loadEnchantments();

private:
    static std::unique_ptr<GameData> m_instance;
    std::string m_gameDir;
    class Impl;
    std::unique_ptr<Impl> m_impl;
    TextManager m_texts;
};

}} // namespace
#endif // guard
