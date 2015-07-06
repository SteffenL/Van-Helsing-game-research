#pragma once

#include <vanhelsing/engine/inventory.h>

#include <wx/dataview.h>
#include <functional>
#include <memory>
#include <vector>


class ApplicationServices;


class EnchantmentViewModel : public wxDataViewModel
{
    enum class ColumnId
    {
        Description,
        LAST_PLUS_ONE
    };

public:
    EnchantmentViewModel(vanhelsing::engine::inventory::EnchantmentCollection& enchantments);
    virtual ~EnchantmentViewModel();

private:
    // Overrides
    virtual unsigned int GetColumnCount() const;
    virtual wxString GetColumnType(unsigned int col) const;
    virtual void GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const;
    virtual bool SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col);
    virtual wxDataViewItem GetParent(const wxDataViewItem &item) const;
    virtual bool IsContainer(const wxDataViewItem &item) const;
    virtual unsigned int GetChildren(const wxDataViewItem &item, wxDataViewItemArray &children) const;

    // Event handlers


    vanhelsing::engine::inventory::EnchantmentCollection& m_enchantments;
};
