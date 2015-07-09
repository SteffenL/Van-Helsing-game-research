#pragma once

#include "generated/main.h"


namespace vanhelsing { namespace engine { namespace inventory {
class Artifact;
}}}


class VisualAppearanceView : public VisualAppearanceViewBase
{
public:
    VisualAppearanceView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
    virtual ~VisualAppearanceView();


    void ShowImageForArtifact(vanhelsing::engine::inventory::Artifact& artifact);
    void ClearDisplay();
};
