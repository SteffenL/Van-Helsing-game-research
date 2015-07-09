#include "VisualAppearanceView.h"

#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/inventory.h>

#include <wx/wx.h>
#include <wx/mstream.h>


VisualAppearanceView::VisualAppearanceView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : VisualAppearanceViewBase(parent, id, pos, size, style)
{
}

VisualAppearanceView::~VisualAppearanceView()
{
}

void VisualAppearanceView::ShowImageForArtifact(vanhelsing::engine::inventory::Artifact& artifact)
{
    using namespace vanhelsing::engine;

    auto& gameData = GameData::Get();

    GameData::ItemData artifactData;
    if (!gameData.GetDataFor(artifact.Id, artifactData)) {
        // TODO: Log error
        return;
    }

    std::vector<char> artifactIconData;
    if (!gameData.GetArtifactIcon(artifactData.Icon, artifactIconData)) {
        // TODO: Log error
        return;
    }

    wxMemoryInputStream memIStream(artifactIconData.data(), artifactIconData.size());
    wxImage image(memIStream, wxBITMAP_TYPE_TGA);
    wxBitmap bmp(image);

    m_visualAppearanceImage->SetBitmap(bmp);
    Layout();
}

void VisualAppearanceView::ClearDisplay()
{
    m_visualAppearanceImage->SetBitmap(wxNullBitmap);
    Layout();
}
