#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>

namespace vanhelsing { namespace engine { namespace inventory {

std::string Artifact::GetName() const
{
    return GameData::Get().GetItemNameFromId(Id);
}

const Enchantment::List& Artifact::GetEnchantments() const
{
    return m_enchantments;
}

Enchantment::List& Artifact::GetEnchantmentsWritable()
{
    return m_enchantments;
}

std::string Enchantment::GetName() const
{
    return GameData::Get().GetEnchantmentNameFromId(Id);
}

void Artifact::List::FindByBagNumber(int bagNumber, std::vector<std::shared_ptr<Artifact>>& items)
{
    for (auto& item : m_items) {
        if (item->BagNumber == bagNumber) {
            items.push_back(item);
        }
    }
}

bool Artifact::Rarity::IsValid(type v)
{
    return ((v >= FIRST) && (v < LAST_PLUS_ONE));
}

bool Artifact::Quality::IsValid(type v)
{
    return ((v >= FIRST) && (v < LAST_PLUS_ONE));
}

}}} // namespace
