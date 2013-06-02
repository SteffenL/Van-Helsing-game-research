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

class Artifact
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
        m_items.push_back(std::shared_ptr<ItemType>(item));
    }

    std::vector<std::shared_ptr<ItemType>>& GetItems()
    {
        return m_items;
    }

protected:
    std::vector<std::shared_ptr<ItemType>> m_items;
};

class Enchantment : public Artifact
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

class Item : public Artifact
{
public:
    class List : public ItemList<Item>
    {
    public:
        void FindByBagNumber(int bagNumber, std::vector<Item*>& items);
    };

    struct Rarity { enum type : unsigned int { Normal, Magic, Rare, Epic, Set, Random }; };
    struct Quality { enum type : unsigned int { Normal, Cracked, Masterwork }; };

    virtual std::string GetName() const;

    unsigned int BagNumber;
    unsigned int SlotNumber;

    int Attribute1;
    int Attribute2;
    int Quantity;
    Item::Quality::type Quality;
    Item::Rarity::type Rarity;
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
