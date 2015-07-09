#include "ObjectInspector.h"
#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>

#include <wx/wx.h>


ObjectInspector::ObjectInspector(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : ObjectInspectorBase(parent, id, pos, size, style)
{
}

ObjectInspector::~ObjectInspector()
{
}

void ObjectInspector::Initialize(wxDataViewCtrl* artifacts, wxDataViewCtrl* enchantments)
{
    m_artifacts = artifacts;
    m_artifactEnchantments = enchantments;
}

void ObjectInspector::ShowPropertiesForArtifact(vanhelsing::engine::inventory::Artifact& artifact)
{
    m_artifactRarityProperty->SetChoiceSelection(artifact.Rarity);
    m_artifactQualityProperty->SetChoiceSelection(artifact.Quality);
    m_artifactQuantityProperty->SetValue(artifact.Quantity);
    m_artifactIdentifiedProperty->SetValue(artifact.IsIdentified);
    m_artifactProperty1Property->SetValue(artifact.Property1);
    m_artifactProperty2Property->SetValue(artifact.Property2);
    m_propertyManager->SelectPage(m_propertiesArtifactPage);
}

void ObjectInspector::ShowPropertiesForEnchantment(vanhelsing::engine::inventory::Enchantment& enchantment)
{
    m_enchantmentValueIndexProperty->SetValue(enchantment.ValueIndex);
    m_enchantmentValueScaleProperty->SetValue(enchantment.ValueScale);
    m_propertyManager->SelectPage(m_propertiesEnchantmentPage);
}

void ObjectInspector::InitializeGameDataDependentProperties()
{
    using vanhelsing::engine::GameData;
    auto& gameData = GameData::Get();
    auto& textManager = gameData.GetTextManager();

    {
        wxPGChoices choices;
        for (auto rarity : gameData.GetRarityDataList()) {
            choices.Add(wxString::FromUTF8(textManager.GetRarityText(rarity).c_str()));
        }

        m_artifactRarityProperty->SetChoices(choices);
    }

    {
        wxPGChoices choices;
        for (auto quality : gameData.GetQualityDataList()) {
            choices.Add(wxString::FromUTF8(textManager.GetQualityText(quality).c_str()));
        }

        m_artifactQualityProperty->SetChoices(choices);
    }
}

bool ObjectInspector::promptUseUnsafeValue()
{
    auto answer = wxMessageBox(_(
        "This value is considered unsafe.\n\n"
        "Values that are unsafe may have adverse effects on general stability, cause glitches, or actually have no function at all. Sometimes, a glitch will even be useful for something.\n\n"
        "Please proceed at your own risk; otherwise, the value will be constrained."),
        _("Unsafe value"),
        wxOK | wxCANCEL | wxICON_WARNING
        );
    return answer == wxOK;
}

void ObjectInspector::ClearDisplay()
{
    // Select blank property page
    m_propertyManager->SelectPage(m_propertiesEmptyPage);
}

void ObjectInspector::propertyManagerOnPropertyGridChanged(wxPropertyGridEvent& event)
{
    using namespace vanhelsing::engine::inventory;

    const auto property = event.GetProperty();
    const auto& value = event.GetPropertyValue();

    // TODO: Figure out the correct way to identify the page or whatever so that we can organize the code a bit.
    // This does work but I don't know if it's the one true way.
    if (property->GetParentState() == m_propertiesArtifactPage->GetStatePtr()) {
        wxDataViewItemArray selections;
        m_artifacts->GetSelections(selections);
        auto model = m_artifacts->GetModel();

        for (auto& selection : selections) {
            auto& artifact = *reinterpret_cast<Artifact*>(selection.GetID());

            if (property == m_artifactProperty1Property) {
                artifact.Property1 = value.GetAny().As<decltype(artifact.Property1)>();
            }
            else if (property == m_artifactProperty2Property) {
                artifact.Property2 = value.GetAny().As<decltype(artifact.Property2)>();
            }
            else if (property == m_artifactQuantityProperty) {
                artifact.Quantity = value.GetAny().As<decltype(artifact.Quantity)>();
            }
            else if (property == m_artifactIdentifiedProperty) {
                artifact.IsIdentified = value.GetAny().As<decltype(artifact.IsIdentified)>();
            }
            else if (property == m_artifactRarityProperty) {
                artifact.Rarity = static_cast<decltype(artifact.Rarity)>(value.GetLong());
            }
            else if (property == m_artifactQualityProperty) {
                artifact.Quality = static_cast<decltype(artifact.Quality)>(value.GetLong());
            }
            else {
                throw std::logic_error("Property is not mapped");
            }
        }

        // TODO: Update only changed items
        model->ItemsChanged(selections);
    }
    else if (property->GetParentState() == m_propertiesEnchantmentPage->GetStatePtr()) {
        wxDataViewItemArray selections;
        m_artifactEnchantments->GetSelections(selections);
        auto model = m_artifactEnchantments->GetModel();

        for (auto& selection : selections) {
            auto& enchantment = *reinterpret_cast<Enchantment*>(selection.GetID());

            if (property == m_enchantmentValueIndexProperty) {
                auto v = value.GetAny().As<decltype(enchantment.ValueIndex)>();
                if (!enchantment.ValueIndexIsSafe(v) && promptUseUnsafeValue()) {
                    enchantment.SetValueIndex(v);
                }
                else {
                    enchantment.SetSafeValueIndex(v);
                }

                property->SetValue(enchantment.ValueIndex);
            }
            else if (property == m_enchantmentValueScaleProperty) {
                auto v = value.GetAny().As<decltype(enchantment.ValueScale)>();
                if (!enchantment.ScaleIsSafe(v) && promptUseUnsafeValue()) {
                    enchantment.SetValueScale(v);
                }
                else {
                    enchantment.SetSafeValueScale(v);
                }

                property->SetValue(enchantment.ValueScale);
            }
            else {
                throw std::logic_error("Property is not mapped");
            }
        }

        // TODO: Update only changed items
        model->ItemsChanged(selections);
    }
    else {
        throw std::logic_error("Property page is not mapped");
    }
}

void ObjectInspector::ApplyPendingChanges()
{
    for (auto focusedWindow = wxWindow::FindFocus(); focusedWindow; focusedWindow = focusedWindow->GetParent()) {
        if (auto propertyGrid = wxDynamicCast(focusedWindow, wxPropertyGrid)) {
            wxFocusEvent focusEvent(wxEVT_KILL_FOCUS);
            propertyGrid->GetEventHandler()->ProcessEvent(focusEvent);
            break;
        }
    }
}
