#include "ArtifactViewModel.h"
#include "../../services/ApplicationServices.h"
#include <vanhelsing/engine/GameData.h>

#include <wx/wx.h>
#include <stdexcept>
#include <cassert>


ArtifactViewModel::ArtifactViewModel(vanhelsing::engine::inventory::ArtifactBag& artifactBag)
    : m_artifactBag(artifactBag)
{
}

ArtifactViewModel::~ArtifactViewModel()
{
}

unsigned int ArtifactViewModel::GetColumnCount() const
{
    return static_cast<decltype(GetColumnCount())>(ColumnId::LAST_PLUS_ONE);
}

wxString ArtifactViewModel::GetColumnType(unsigned int col) const
{
    switch (static_cast<ColumnId>(col)) {
    case ColumnId::Name:
        return wxT("string");
    case ColumnId::Property1:
    case ColumnId::Property2:
    case ColumnId::Quantity:
        return wxT("long");
    case ColumnId::Quality:
    case ColumnId::Rarity:
        return wxT("string"); // TODO: list?
    case ColumnId::Identified:
        return wxT("bool");

    default:
        throw std::logic_error("Unexpected column");
    }
}

void ArtifactViewModel::GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    const auto& artifact = reinterpret_cast<Artifact*>(item.GetID());

    switch (static_cast<ColumnId>(col)) {
    case ColumnId::Name:
        {
            const auto& textManager = GameData::Get().GetTextManager();
            const auto& friendlyName = textManager.GetItemText(artifact->GetName());
            variant = wxString::FromUTF8(friendlyName.data());
            break;
        }

    case ColumnId::Property1:
        variant = static_cast<long>(artifact->Property1);
        break;

    case ColumnId::Property2:
        variant = static_cast<long>(artifact->Property2);
        break;

    case ColumnId::Quantity:
        variant = static_cast<long>(artifact->Quantity);
        break;

    case ColumnId::Quality:
        {
            const auto& textManager = GameData::Get().GetTextManager();
            const auto& friendlyName = textManager.GetQualityText(artifact->Quality);
            variant = wxString::FromUTF8(friendlyName.data());
            break;
        }

    case ColumnId::Rarity:
        {
            const auto& textManager = GameData::Get().GetTextManager();
            const auto& friendlyName = textManager.GetRarityText(artifact->Rarity);
            variant = wxString::FromUTF8(friendlyName.data());
            break;
        }

    case ColumnId::Identified:
        variant = artifact->IsIdentified;
        break;

    default:
        throw std::logic_error("Unexpected column");
    }
}

bool ArtifactViewModel::SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col)
{
    throw std::exception("The method or operation is not implemented.");
}

wxDataViewItem ArtifactViewModel::GetParent(const wxDataViewItem &item) const
{
    // TODO: Not sure why and if we need to handle this correctly
    return wxDataViewItem();
}

bool ArtifactViewModel::IsContainer(const wxDataViewItem &item) const
{
    if (!item) {
        // Root has children
        return true;
    }

    // Artifacts may have children
    const auto& artifact = *reinterpret_cast<Artifact*>(item.GetID());
    return !artifact.GetInfusedArtifacts().empty();
}

unsigned int ArtifactViewModel::GetChildren(const wxDataViewItem &item, wxDataViewItemArray &children) const
{
    // Root item
    if (!item) {
        for (auto& slot : m_artifactBag) {
            auto artifactPtr = slot.second.get();
            children.Add(wxDataViewItem(artifactPtr));
        }

        return m_artifactBag.size();
    }

    const auto& artifact = *reinterpret_cast<Artifact*>(item.GetID());
    if (!artifact.GetInfusedArtifacts().empty()) {
        const auto& infusedArtifacts = artifact.GetInfusedArtifacts();
        for (auto& infusedArtifact : infusedArtifacts) {
            auto artifactPtr = infusedArtifact.get();
            children.Add(wxDataViewItem(artifactPtr));
        }

        return infusedArtifacts.size();
    }

    assert(!!"Should not happen");
    return 0;
}

bool ArtifactViewModel::HasContainerColumns(const wxDataViewItem& WXUNUSED(item)) const
{
    return true;
}
