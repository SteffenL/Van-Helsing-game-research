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

class Artifact : public Item
{
public:
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
};

class Enchantment : public Item
{
public:
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

// Artifact attributes:
// Armor: Defense, Essence capacity

class Manager
{
public:
    void Add(Artifact* item);
    void Add(Enchantment* item);

private:
    std::vector<std::unique_ptr<Artifact>> m_artifacts;
    std::vector<std::unique_ptr<Enchantment>> m_enchantments;
};

}}} // namespace
#endif
