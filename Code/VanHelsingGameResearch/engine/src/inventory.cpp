#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>

namespace vanhelsing { namespace engine { namespace inventory {

std::string Item::GetName() const
{
    return GameData::Get().GetItemNameFromId(Id);
}

const Enchantment::List& Item::GetEnchantments() const
{
    return m_enchantments;
}

Enchantment::List& Item::GetEnchantmentsWritable()
{
    return m_enchantments;
}

std::string Enchantment::GetName() const
{
    return GameData::Get().GetEnchantmentNameFromId(Id);
}


void Item::List::FindByBagNumber(int bagNumber, std::vector<Item*>& items)
{
    for (auto& item : m_items) {
        if (item->BagNumber == bagNumber) {
            items.push_back(item.get());
        }
    }
}

}}} // namespace
