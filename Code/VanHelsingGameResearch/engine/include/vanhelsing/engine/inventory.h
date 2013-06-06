#ifndef vanhelsing_engine_inventory_
#define vanhelsing_engine_inventory_

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

    const std::vector<std::shared_ptr<ItemType>>& GetItems() const
    {
        return m_items;
    }

    std::vector<std::shared_ptr<ItemType>>& GetItemsWritable()
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

    struct Rarity
    {
        enum type : unsigned int { FIRST, Normal = FIRST, Magic, Rare, Epic, Set, Random, LAST_PLUS_ONE };
        static bool IsValid(type v);
    };
    
    struct Quality
    {
        enum type : unsigned int { FIRST, Normal = FIRST, Cracked, Masterwork, LAST_PLUS_ONE };
        static bool IsValid(type v);
    };

    struct UnknownList1Item
    {
        unsigned int v1;
        int v2;

        UnknownList1Item(unsigned int v1_, int v2_) : v1(v1_), v2(v2_) {}
    };

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
        std::vector<UnknownList1Item> List1;
        bool v1;
        Enchantment::List MaybeEnchantments;
    } Unknown;

    const Enchantment::List& GetEnchantments() const;
    Enchantment::List& GetEnchantmentsWritable();

protected:
    Enchantment::List m_enchantments;
};

}}} // namespace
#endif
