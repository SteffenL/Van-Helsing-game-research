#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>

namespace vanhelsing { namespace engine { namespace inventory {

std::string Artifact::GetName() const
{
    return GameData::Get().GetItemNameFromId(Id);
}

const EnchantmentCollection& Artifact::GetEnchantments() const
{
    return m_enchantments;
}

EnchantmentCollection& Artifact::GetEnchantmentsWritable()
{
    return m_enchantments;
}

const ArtifactCollection& Artifact::GetInfusedArtifacts() const
{
    return m_infusedArtifacts;
}

ArtifactCollection& Artifact::GetInfusedArtifactsWritable()
{
    return m_infusedArtifacts;
}

std::string Enchantment::GetName() const
{
    return GameData::Get().GetEnchantmentNameFromId(Id);
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
