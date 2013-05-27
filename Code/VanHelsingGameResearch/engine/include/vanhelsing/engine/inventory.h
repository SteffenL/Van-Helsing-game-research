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
    struct Rarity { enum type : unsigned int { Normal, Magic, Rare, Epic, Set, Random }; };
    struct Quality { enum type : unsigned int { Normal, Cracked, Masterwork }; };

public:
    unsigned int BagNumber;
    unsigned int SlotNumber;

    unsigned int Id;
    int Attribute1;
    int Attribute2;
    int Quantity;
    Item::Quality::type Quality;
    Item::Rarity::type Rarity;
    bool IsIdentified;
};


// Item attributes:
// Armor: Defense, Essence capacity

class Manager
{
public:
    void Add(Item* item);

private:
    std::vector<std::unique_ptr<Item>> m_items;
};

}}} // namespace
#endif
