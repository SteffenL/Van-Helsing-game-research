#pragma once

#include <vanhelsing/services/OpenedStorageGameSaveFile.h>
#include <vanhelsing/engine/inventory.h>

#include <wx/dataview.h>
#include <functional>
#include <memory>
#include <vector>


class ApplicationServices;


class ArtifactViewModel : public wxDataViewModel
{
    using BagSlotIndexType = vanhelsing::engine::inventory::BagSlotIndexType;
    using Artifact = vanhelsing::engine::inventory::Artifact;
    using ArtifactBag = vanhelsing::engine::inventory::ArtifactBag;
    using ArtifactBagSlot = vanhelsing::engine::inventory::ArtifactBagSlot;

    enum class ColumnId
    {
        Name, Property1, Property2, Rarity, Quality, Quantity, Identified,
        LAST_PLUS_ONE
    };

public:
    enum class ViewItemType
    {
        ArtifactInBagSlot, InfusedArtifact
    };

    ArtifactViewModel(vanhelsing::engine::inventory::ArtifactBag& artifactBag);
    virtual ~ArtifactViewModel();

private:
    // Overrides
    virtual unsigned int GetColumnCount() const;
    virtual wxString GetColumnType(unsigned int col) const;
    virtual void GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const;
    virtual bool SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col);
    virtual wxDataViewItem GetParent(const wxDataViewItem &item) const;
    virtual bool IsContainer(const wxDataViewItem &item) const;
    virtual unsigned int GetChildren(const wxDataViewItem &item, wxDataViewItemArray &children) const;
    virtual bool HasContainerColumns(const wxDataViewItem& WXUNUSED(item)) const;


    void getValue(wxVariant& variant, const ArtifactBagSlot& artifactSlot, unsigned int col) const;
    void getValue(wxVariant& variant, const Artifact& infusedArtifact, unsigned int col) const;

    vanhelsing::engine::inventory::ArtifactBag& m_artifactBag;
};
