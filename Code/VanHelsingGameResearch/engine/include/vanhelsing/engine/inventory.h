#ifndef __vanhelsing_engine_inventory__
#define __vanhelsing_engine_inventory__

#include <vector>
#include <array>
#include <memory>

namespace vanhelsing { namespace engine { namespace inventory {

struct EquipSlot
{
    enum type {
        Melee, Ranged, Helmet, Amulet, Armor, Gloves, Boots, Cape, Belt, Trophy,
        Ring1, // Right hand
        Ring2 // Left hand
    };
};

class Item
{
public:
    typedef unsigned int IdType;

    virtual std::string GetName() const = 0;

    IdType Id;
};

// Artifact attributes:
// Armor: Defense, Essence capacity

template<typename ItemType>
class ItemList
{
public:
    void Add(ItemType* item)
    {
        m_items.push_back(std::unique_ptr<ItemType>(item));
    }

protected:
    std::vector<std::unique_ptr<ItemType>> m_items;
};

class Enchantment : public Item
{
public:
    class List : public ItemList<Enchantment>
    {
    public:

    };

    virtual std::string GetName() const;

    float Multiplier;
    struct
    {
        int v2;
        float v3;
        unsigned int v4;
        int v5;
        unsigned int v6;
        int v7;
    } Unknown;
};

class Artifact : public Item
{
public:
    class List : public ItemList<Artifact>
    {
    public:
        void FindByBagNumber(int bagNumber, std::vector<Artifact*>& items);
    };

    struct Rarity { enum type : unsigned int { Normal, Magic, Rare, Epic, Set, Random }; };
    struct Quality { enum type : unsigned int { Normal, Cracked, Masterwork }; };

    virtual std::string GetName() const;

    unsigned int BagNumber;
    unsigned int SlotNumber;

    int Attribute1;
    int Attribute2;
    int Quantity;
    Artifact::Quality::type Quality;
    Artifact::Rarity::type Rarity;
    bool IsIdentified;
    struct
    {

    } Unknown;

    Enchantment::List& GetEnchantments();

protected:
    Enchantment::List m_enchantments;
};

}}} // namespace
#endif
