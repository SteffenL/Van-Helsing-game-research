#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>

namespace vanhelsing { namespace engine { namespace inventory {

std::string Artifact::GetName() const
{
    return GameData::Get().GetArtifactNameFromId(Id);
}

Enchantment::List& Artifact::GetEnchantments()
{
    return m_enchantments;
}

std::string Enchantment::GetName() const
{
    return GameData::Get().GetEnchantmentNameFromId(Id);
}


void Artifact::List::FindByBagNumber(int bagNumber, std::vector<Artifact*>& items)
{
    for (auto& item : m_items) {
        if (item->BagNumber == bagNumber) {
            items.push_back(item.get());
        }
    }
}

}}} // namespace
