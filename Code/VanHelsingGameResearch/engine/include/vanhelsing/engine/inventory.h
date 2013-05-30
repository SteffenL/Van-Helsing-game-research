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
    struct Rarity { enum type : unsigned int { Normal, Magic, Rare, Epic, Set, Random }; };
    struct Quality { enum type : unsigned int { Normal, Cracked, Masterwork }; };
    typedef unsigned int IdType;

    static IdType GetIdFromName(const std::string& name);
    static std::string GetNameFromId(IdType id);
    std::string GetName() const;

public:
    unsigned int BagNumber;
    unsigned int SlotNumber;

    IdType Id;
    int Stat1;
    int Stat2;
    int Quantity;
    Artifact::Quality::type Quality;
    Artifact::Rarity::type Rarity;
    bool IsIdentified;
};


// Artifact stats:
// Armor: Defense, Essence capacity

class Manager
{
public:
    void Add(Artifact* artifact);

private:
    std::vector<std::unique_ptr<Artifact>> m_artifacts;
};

}}} // namespace
#endif
