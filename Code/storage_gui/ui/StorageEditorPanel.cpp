#include "StorageEditorPanel.h"
#include "viewmodels/ArtifactViewModel.h"
#include "viewmodels/EnchantmentViewModel.h"
#include "../services/ApplicationServices.h"
#include <vanhelsing/engine/GameData.h>

#include "res/inventory.png.h"

#include <boost/bind.hpp>
#include <wx/wx.h>
#include <map>
#include <set>


StorageEditorPanel::StorageEditorPanel(wxWindow* parent)
    : StorageEditorPanelBase(parent)
{
    initializeUi();
    subscribeToEvents();
}

StorageEditorPanel::~StorageEditorPanel()
{
}

void StorageEditorPanel::artifactBagOnToolClicked(wxCommandEvent& event)
{
    using namespace vanhelsing::engine::inventory;

    tryKillFocusFromPropertyGrid();

    auto toolbar = reinterpret_cast<wxToolBar*>(event.GetEventObject());
    auto tool = toolbar->FindById(event.GetId());
    auto& indexToBagPair = *reinterpret_cast<IndexToArtifactBagPair*>(tool->GetClientData());
    auto& bag = indexToBagPair.second;

    clearDependentOnArtifactBag();

    m_artifactViewModel = new ArtifactViewModel(bag);
    m_artifacts->AssociateModel(m_artifactViewModel.get());
}

void StorageEditorPanel::artifactBagOnUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(!!m_gameSave);
}

void StorageEditorPanel::artifactsOnDataViewCtrlSelectionChanging(wxDataViewEvent& event)
{
    tryKillFocusFromPropertyGrid();
}

void StorageEditorPanel::onGameSaveOpened(vanhelsing::services::OpenedStorageGameSaveFile& fileInfo)
{
    clearDependentOnGameSave();
    m_gameSave = fileInfo.GetGameSave();
    initializeGameSaveDependentUi();
    populateDependentOnGameSave();
}

void StorageEditorPanel::subscribeToEvents()
{
    m_artifacts->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGING, wxDataViewEventHandler(StorageEditorPanel::artifactsOnDataViewCtrlSelectionChanging), NULL, this);
    m_artifactEnchantments->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGING, wxDataViewEventHandler(StorageEditorPanel::artifactEnchantmentsOnDataViewCtrlSelectionChanging), NULL, this);

    auto& services = ApplicationServices::Get();
    services.StorageGameSave.OnOpened.connect(boost::bind(&StorageEditorPanel::onGameSaveOpened, this, _1));
}

void StorageEditorPanel::populateArtifacts()
{
    auto& bags = m_gameSave->GetArtifactBags1();
    if (!bags.empty()) {
        auto& firstIndexToBagPair = *bags.begin();
        auto& bag = firstIndexToBagPair.second;

        m_artifactViewModel = new ArtifactViewModel(bag);
        m_artifacts->AssociateModel(m_artifactViewModel.get());
    }
}

void StorageEditorPanel::populateEnchantments(vanhelsing::engine::inventory::Artifact& artifact)
{
    m_enchantmentViewModel = new EnchantmentViewModel(artifact.GetEnchantmentsWritable());
    m_artifactEnchantments->AssociateModel(m_enchantmentViewModel.get());
}

void StorageEditorPanel::populateBags()
{
    // Add tools
    const auto& toolImage = inventory_png_to_wx_bitmap();
    wxToolBarToolBase* firstTool = nullptr;
    auto& bags = m_gameSave->GetArtifactBags1();
    for (auto& indexToBagPair : bags) {
        auto tool = m_artifactBagToolBar->AddRadioTool(
            wxID_ANY,
            wxString::Format(wxT("%d"), indexToBagPair.first + 1),
            toolImage,
            wxNullBitmap,
            wxEmptyString,
            wxEmptyString,
            reinterpret_cast<wxObject*>(&indexToBagPair)
        );

        firstTool = firstTool ? firstTool : tool;

        // Hook up events
        Connect(tool->GetId(), wxEVT_TOOL, wxCommandEventHandler(StorageEditorPanel::artifactBagOnToolClicked));
        Connect(tool->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(StorageEditorPanel::artifactBagOnUpdateUI));
    }

    // Select the first tool
    m_artifactBagToolBar->ToggleTool(firstTool->GetId(), true);

    m_artifactBagToolBar->Realize();
    m_artifactBagToolBar->Show(!bags.empty());
    Layout();
}

void StorageEditorPanel::populateDependentOnGameSave()
{
    populateBags();
    populateArtifacts();
}

void StorageEditorPanel::initializeUi()
{
    m_artifactBagToolBar->Hide();

    // Artifact list columns
    {
        m_artifacts->AppendTextColumn(_("Name"), 0, wxDATAVIEW_CELL_INERT, 200, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendTextColumn(_("Property 1"), 1, wxDATAVIEW_CELL_INERT, 90, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendTextColumn(_("Property 2"), 2, wxDATAVIEW_CELL_INERT, 90, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendTextColumn(_("Rarity"), 3, wxDATAVIEW_CELL_INERT, 90, wxALIGN_NOT, wxDATAVIEW_COL_RESIZABLE)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendTextColumn(_("Quality"), 4, wxDATAVIEW_CELL_INERT, 90, wxALIGN_NOT, wxDATAVIEW_COL_RESIZABLE)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendTextColumn(_("Quantity"), 5, wxDATAVIEW_CELL_INERT, 90, wxALIGN_RIGHT, wxDATAVIEW_COL_RESIZABLE)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendToggleColumn(_("Identified"), 6, wxDATAVIEW_CELL_INERT, 80, wxALIGN_CENTER, 0)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
        m_artifacts->AppendTextColumn(wxEmptyString, -1, wxDATAVIEW_CELL_INERT, -1, wxALIGN_NOT, wxDATAVIEW_COL_HIDDEN);
    }

    // Enchantment list columns
    {
        m_artifactEnchantments->AppendTextColumn(wxEmptyString, -1, wxDATAVIEW_CELL_INERT, -1, wxALIGN_NOT, wxDATAVIEW_COL_HIDDEN);
        m_artifactEnchantments->AppendTextColumn(_("Description"), 0, wxDATAVIEW_CELL_INERT, -1, wxALIGN_LEFT, 0)
            ->GetRenderer()->EnableEllipsize(wxELLIPSIZE_END);
    }
}

void StorageEditorPanel::showPropertiesForArtifact(vanhelsing::engine::inventory::Artifact& artifact)
{
    m_artifactRarityProperty->SetChoiceSelection(artifact.Rarity);
    m_artifactQualityProperty->SetChoiceSelection(artifact.Quality);
    m_artifactQuantityProperty->SetValue(artifact.Quantity);
    m_artifactIdentifiedProperty->SetValue(artifact.IsIdentified);
    m_artifactProperty1Property->SetValue(artifact.Property1);
    m_artifactProperty2Property->SetValue(artifact.Property2);
    m_propertyManager->SelectPage(m_propertiesArtifactPage);
}

void StorageEditorPanel::showPropertiesForEnchantment(vanhelsing::engine::inventory::Enchantment& enchantment)
{
    m_enchantmentValueIndexProperty->SetValue(enchantment.ValueIndex);
    m_enchantmentValueScaleProperty->SetValue(enchantment.ValueScale);
    m_propertyManager->SelectPage(m_propertiesEnchantmentPage);
}

void StorageEditorPanel::initializeGameDataDependentProperties()
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

void StorageEditorPanel::initializeGameSaveDependentUi()
{
    initializeGameDataDependentProperties();
}

void StorageEditorPanel::clearDependentOnGameSave()
{
    m_artifactBagToolBar->ClearTools();
    clearDependentOnArtifactBag();
}

void StorageEditorPanel::clearDependentOnEnchantment()
{
    // Select blank property page
    m_propertyManager->SelectPage(m_propertiesEmptyPage);
}

void StorageEditorPanel::tryKillFocusFromPropertyGrid()
{
    for (auto focusedWindow = wxWindow::FindFocus(); focusedWindow; focusedWindow = focusedWindow->GetParent()) {
        if (auto propertyGrid = wxDynamicCast(focusedWindow, wxPropertyGrid)) {
            wxFocusEvent focusEvent(wxEVT_KILL_FOCUS);
            propertyGrid->GetEventHandler()->ProcessEvent(focusEvent);
            break;
        }
    }
}

bool StorageEditorPanel::promptUseUnsafeValue()
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

void StorageEditorPanel::populateDependentOnArtifact(vanhelsing::engine::inventory::Artifact& artifact)
{
    populateEnchantments(artifact);
    showPropertiesForArtifact(artifact);
    showImageForArtifact(artifact);
}

void StorageEditorPanel::showImageForArtifact(vanhelsing::engine::inventory::Artifact& artifact)
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
    m_visualAppearancePanel->Layout();
}

void StorageEditorPanel::clearDependentOnArtifactBag()
{
    if (auto model = m_artifacts->GetModel()) {
        m_artifacts->AssociateModel(nullptr);
        model->Cleared();
    }

    clearDependentOnArtifact();
}

void StorageEditorPanel::clearDependentOnArtifact()
{
    if (auto model = m_artifactEnchantments->GetModel()) {
        m_artifactEnchantments->AssociateModel(nullptr);
        model->Cleared();
    }

    clearDependentOnEnchantment();

    m_visualAppearanceImage->SetBitmap(wxNullBitmap);
    m_visualAppearancePanel->Layout();
}

void StorageEditorPanel::artifactEnchantmentsOnDataViewCtrlSelectionChanging(wxDataViewEvent& event)
{
    tryKillFocusFromPropertyGrid();
}

void StorageEditorPanel::artifactEnchantmentsOnDataViewCtrlSelectionChanged(wxDataViewEvent& event)
{
    using namespace vanhelsing::engine::inventory;
    clearDependentOnEnchantment();

    if (!event.GetItem()) {
        return;
    }

    auto& enchantment = *reinterpret_cast<Enchantment*>(event.GetItem().GetID());
    showPropertiesForEnchantment(enchantment);
}

void StorageEditorPanel::propertyManagerOnPropertyGridChanged(wxPropertyGridEvent& event)
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

void StorageEditorPanel::artifactsOnDataViewCtrlSelectionChanged(wxDataViewEvent& event)
{
    using namespace vanhelsing::engine::inventory;
    clearDependentOnArtifact();

    if (!event.GetItem()) {
        return;
    }

    auto& artifact = *reinterpret_cast<Artifact*>(event.GetItem().GetID());
    populateDependentOnArtifact(artifact);
}
