#ifndef __vanhelsing_engine_inventory__
#define __vanhelsing_engine_inventory__

#include <vector>
#include <array>
#include <memory>

namespace vanhelsing { namespace engine { namespace inventory {

class Item
{
public:
    struct Rarity { enum type { Normal, Magic, Rare, Epic, Set, Random }; };
    struct Quality { enum type { Normal, Cracked, Masterwork }; };

public:

protected:
    unsigned int m_bagNumber;
    unsigned int m_slotNumber;
    unsigned int m_itemId;
    bool m_isIdentified;

    char m_v1;
    char m_v2[4];
};

class ArmorItem
{
protected:
    unsigned int m_defencePoints;
    unsigned int m_essenceCapacity;
    std::vector<std::array<char, 8>> m_v1;
    Item::Quality::type m_quality;
    Item::Rarity::type m_rarity;
    char m_v2[8];
};

class Manager
{
public:
    void Add(Item* item);

private:
    std::vector<std::unique_ptr<Item>> m_items;
};

}}} // namespace
#endif
