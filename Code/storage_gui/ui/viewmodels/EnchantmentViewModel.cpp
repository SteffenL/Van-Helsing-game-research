#include "EnchantmentViewModel.h"
#include "../../services/ApplicationServices.h"
#include <vanhelsing/engine/GameData.h>

#include <wx/wx.h>
#include <stdexcept>
#include <cassert>


EnchantmentViewModel::EnchantmentViewModel(vanhelsing::engine::inventory::EnchantmentCollection& enchantments)
    : m_enchantments(enchantments)
{
}

EnchantmentViewModel::~EnchantmentViewModel()
{
}

unsigned int EnchantmentViewModel::GetColumnCount() const
{
    return static_cast<decltype(GetColumnCount())>(ColumnId::LAST_PLUS_ONE);
}

wxString EnchantmentViewModel::GetColumnType(unsigned int col) const
{
    switch (static_cast<ColumnId>(col)) {
    case ColumnId::Description:
        return wxT("string");

    default:
        throw std::logic_error("Unexpected column");
    }
}

void EnchantmentViewModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    const auto& enchantment = *reinterpret_cast<Enchantment*>(item.GetID());
    const auto& gameData = GameData::Get();
    const auto& textManager = gameData.GetTextManager();

    switch (static_cast<ColumnId>(col)) {
    case ColumnId::Description:
        {
            GameData::EnchantmentData enchantmentData;
            if (gameData.GetDataFor(enchantment.Id, enchantmentData)) {
                const auto& propertyText = textManager.GetSkillPropertyText(enchantmentData.Property);
                std::string formattedDescription;

                int value = 0;
                if (enchantmentData.CalculateValue(value, enchantment)) {
                    formattedDescription = propertyText.GetFormattedText(value, enchantmentData.Property, enchantmentData.TargetType, enchantmentData.PriorityGroup);
                }
                else {
                    formattedDescription = propertyText.GetFormattedText("?", enchantmentData.Property, enchantmentData.TargetType, enchantmentData.PriorityGroup);
                }

                variant = wxString::FromUTF8(formattedDescription.data());
            }
            else {
                variant = wxString::FromUTF8(enchantment.GetName().data());
            }

            break;
        }

    default:
        throw std::logic_error("Unexpected column");
    }
}

bool EnchantmentViewModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col)
{
    throw std::exception("The method or operation is not implemented.");
}

wxDataViewItem EnchantmentViewModel::GetParent(const wxDataViewItem &item) const
{
    throw std::exception("The method or operation is not implemented.");
}

bool EnchantmentViewModel::IsContainer(const wxDataViewItem &item) const
{
    // Only the root item
    return !item;
}

unsigned int EnchantmentViewModel::GetChildren(const wxDataViewItem &item, wxDataViewItemArray &children) const
{
    // Only the root item can have children
    if (item || m_enchantments.empty()) {
        return 0;
    }

    decltype(GetChildren(item, children)) count = 0;
    for (auto& enchantment : m_enchantments) {
        children.Add(wxDataViewItem(enchantment.get()));
        ++count;
    }

    return count;
}
