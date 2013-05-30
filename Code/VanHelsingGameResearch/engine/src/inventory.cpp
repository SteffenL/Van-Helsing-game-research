#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>

namespace vanhelsing { namespace engine { namespace inventory {

void Manager::Add(Artifact* item)
{
    m_artifacts.push_back(std::unique_ptr<Artifact>(item));
}

void Manager::Add(Enchantment* item)
{
    m_enchantments.push_back(std::unique_ptr<Enchantment>(item));
}

std::string Artifact::GetName() const
{
    return GameData::Get().GetArtifactNameFromId(Id);
}

std::string Enchantment::GetName() const
{
    return GameData::Get().GetEnchantmentNameFromId(Id);
}

}}} // namespace
