#ifndef __vanhelsing_engine_GameData__
#define __vanhelsing_engine_GameData__

#include <vanhelsing/engine/inventory.h>
#include <string>
#include <map>
#include <memory>

namespace vanhelsing { namespace engine {

class GameData
{
public:
    class ArtifactData
    {
    public:
        inventory::Artifact::Item::IdType Id;
        std::string Name;
        std::string Icon;
    };

    class EnchantmentData
    {
    public:
        inventory::Enchantment::Item::IdType Id;
        std::string Name;
    };

    GameData();
    static GameData& Get();
    void Load(const std::string& gameDir);
    // Inventory items
    bool GetItemData(inventory::Artifact::IdType id, ArtifactData& data) const;
    bool GetItemData(inventory::Enchantment::IdType id, EnchantmentData& data) const;
    inventory::Item::IdType GetItemIdFromName(const std::string& name) const;
    std::string GetArtifactNameFromId(inventory::Item::IdType id) const;
    std::string GetEnchantmentNameFromId(inventory::Item::IdType id) const;
    const char* GetRarityText(inventory::Artifact::Rarity::type rarity) const;
    const char* GetQualityText(inventory::Artifact::Quality::type quality) const;

private:
    void loadArtifacts();
    void loadEnchantments();

private:
    static std::unique_ptr<GameData> m_instance;
    std::string m_gameDir;
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace
#endif // guard
