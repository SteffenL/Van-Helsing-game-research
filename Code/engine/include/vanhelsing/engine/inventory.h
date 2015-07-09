#pragma once

#include <vector>
#include <array>
#include <memory>
#include <set>
#include <map>

namespace vanhelsing { namespace engine { namespace inventory {

struct EquipSlot
{
    enum type {
        Melee, Ranged, Helmet, Amulet, Armor, Gloves, Boots, Cape, Belt, Trophy,
        Ring1, // Right hand
        Ring2 // Left hand
    };
};

class AbstractInventoryItem
{
public:
    using IdType = unsigned int;

    virtual std::string GetName() const = 0;

    IdType Id;
};

template<typename ItemType, typename ContainerType = std::vector<ItemType>>
class AbstractInventoryItemList : public ContainerType
{
public:
    AbstractInventoryItemList() {}
    virtual ~AbstractInventoryItemList() {}
};


using BagIndexType = unsigned int;
using BagSlotIndexType = unsigned int;

// A bag contains slots for items
template<typename ItemType>
using AbstractInventoryItemBag = std::map<BagSlotIndexType, ItemType>;


// Artifact properties:
// Armor: Defense, Essence capacity

class Enchantment;

using EnchantmentPtr = std::unique_ptr<Enchantment>;
using EnchantmentCollection = std::vector<EnchantmentPtr>;
using EnchantmentIndexType = EnchantmentCollection::size_type;

class Enchantment : public AbstractInventoryItem
{
public:
    virtual std::string GetName() const;
    void SetValueIndex(int index);
    void SetValueScale(float scale);
    void SetSafeValueIndex(int index);
    void SetSafeValueScale(float scale);
    bool ValueIndexIsSafe(int index) const;
    bool ScaleIsSafe(float scale) const;

    int ValueIndex;
    float ValueScale;

    struct
    {
        unsigned int v4;
        int v5;
        unsigned int v6;
        int v7;
    } Unknown;
};


class Artifact;

using ArtifactPtr = std::unique_ptr<Artifact>;
using ArtifactCollection = std::vector<ArtifactPtr>;
using ArtifactBag = AbstractInventoryItemBag<ArtifactPtr>;
using ArtifactBagSlot = ArtifactBag::value_type;

class Artifact : public AbstractInventoryItem
{
public:
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

    struct UnknownStruct1
    {
        unsigned int v1;
        short v2;

        UnknownStruct1() {}
        UnknownStruct1(unsigned int v1_, short v2_) : v1(v1_), v2(v2_) {}
    };

    struct UnknownList1Item
    {
        unsigned int v1;
        int v2;

        UnknownList1Item() {}
        UnknownList1Item(unsigned int v1_, int v2_) : v1(v1_), v2(v2_) {}
    };

    struct UnknownList2Item
    {
        unsigned int v1;
        int v2;

        UnknownList2Item() {}
        UnknownList2Item(unsigned int v1_, int v2_) : v1(v1_), v2(v2_) {}
    };

    struct UnknownList3Item
    {
        unsigned int v1;
        short v2;

        UnknownList3Item() {}
        UnknownList3Item(unsigned int v1_, short v2_) : v1(v1_), v2(v2_) {}
    };

    struct UnknownList4Item
    {
        UnknownStruct1 v1;
        std::vector<inventory::Artifact::UnknownList3Item> v2;

        UnknownList4Item() {}
    };

    using UnknownList5Item = UnknownList3Item;

    virtual std::string GetName() const;

    int Property1;
    int Property2;
    int Quantity;
    Artifact::Quality::type Quality;
    Artifact::Rarity::type Rarity;
    bool IsIdentified;

    struct
    {
        std::vector<UnknownList1Item> List1;
        bool v1;
        std::vector<UnknownList2Item> List2;
        unsigned int v2;
        bool v3;
        UnknownStruct1 UnknownStruct1_1;
        UnknownStruct1 UnknownStruct1_2;
        UnknownStruct1 UnknownStruct1_3;
        UnknownStruct1 UnknownStruct1_4;
        std::vector<UnknownList3Item> List3;
        std::vector<UnknownList4Item> List4;
        std::vector<UnknownList5Item> List5;
        int v4;
        int v5;
        int v6;
        float v7;
        float v8;
    } Unknown;

    const EnchantmentCollection& GetEnchantments() const;
    EnchantmentCollection& GetEnchantmentsWritable();

    const ArtifactCollection& GetInfusedArtifacts() const;
    ArtifactCollection& GetInfusedArtifactsWritable();

protected:
    EnchantmentCollection m_enchantments;
    ArtifactCollection m_infusedArtifacts;
};

class ArtifactBagCollection : public std::map<BagIndexType, ArtifactBag>
{
public:
    ArtifactBagCollection() {}
    virtual ~ArtifactBagCollection() {}
};

using IndexToArtifactBagPair = ArtifactBagCollection::value_type;

}}}
