#pragma once

#include "generated/main.h"


namespace vanhelsing { namespace engine { namespace inventory {
class Artifact;
class Enchantment;
}}}


class ObjectInspector : public ObjectInspectorBase
{
public:
    ObjectInspector(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
    virtual ~ObjectInspector();

    // Must be called from the parent window, because the generated UI code doesn't know how to construct it
    void Initialize(wxDataViewCtrl* artifacts, wxDataViewCtrl* enchantments);

    void ShowPropertiesForArtifact(vanhelsing::engine::inventory::Artifact& artifact);
    void ShowPropertiesForEnchantment(vanhelsing::engine::inventory::Enchantment& enchantment);
    void ApplyPendingChanges();
    void InitializeGameDataDependentProperties();
    void ClearDisplay();

protected:
    // Inherited event handlers
    void propertyManagerOnPropertyGridChanged(wxPropertyGridEvent& event);

private:
    bool promptUseUnsafeValue();


    // We don't own these
    wxDataViewCtrl* m_artifacts;
    wxDataViewCtrl* m_artifactEnchantments;
};
