#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>
#include <algorithm>

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

void Enchantment::SetValueIndex(int index)
{
    ValueIndex = index;
}

void Enchantment::SetValueScale(float scale)
{
    ValueScale = scale;
}

void Enchantment::SetSafeValueIndex(int index)
{
    GameData::EnchantmentData data;
    if (!GameData::Get().GetDataFor(Id, data)) {
        // Unsafe since we don't have data
        SetValueIndex(index);
        return;
    }

    if (data.MinValue.empty()) {
        // Not sure if this is possible, but do it unsafely anyway
        SetValueIndex(index);
        return;
    }

    // Check bounds
    auto safeIndex = std::max(0, std::min(static_cast<int>(data.MinValue.size()) - 1, index));
    SetValueIndex(safeIndex);
}

void Enchantment::SetSafeValueScale(float scale)
{
    // Check bounds
    auto safeScale = std::max(0.0f, std::min(1.0f, scale));
    SetValueScale(safeScale);
}

bool Enchantment::ValueIndexIsSafe(int index) const
{
    GameData::EnchantmentData data;
    if (!GameData::Get().GetDataFor(Id, data)) {
        // Unsafe since we don't have data
        return false;
    }

    if (data.MinValue.empty()) {
        // Not sure if this is possible, but do it unsafely anyway
        return false;
    }

    // Check bounds
    auto safeIndex = std::max(0, std::min(static_cast<int>(data.MinValue.size()) - 1, index));
    return index == safeIndex;
}

bool Enchantment::ScaleIsSafe(float scale) const
{
    // Check bounds
    auto safeScale = std::max(0.0f, std::min(1.0f, scale));
    return scale == safeScale;
}

bool Artifact::Rarity::IsValid(type v)
{
    return ((v >= FIRST) && (v < LAST_PLUS_ONE));
}

bool Artifact::Quality::IsValid(type v)
{
    return ((v >= FIRST) && (v < LAST_PLUS_ONE));
}

}}}
