#include "StorageEditorPanel.h"
#include "viewmodels/ArtifactViewModel.h"
#include "viewmodels/EnchantmentViewModel.h"
#include "ObjectInspector.h"
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

    m_objectInspector->ApplyPendingChanges();

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
    m_objectInspector->ApplyPendingChanges();
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

    m_objectInspector->Initialize(m_artifacts, m_artifactEnchantments);
}

void StorageEditorPanel::initializeGameSaveDependentUi()
{
    m_objectInspector->InitializeGameDataDependentProperties();
}

void StorageEditorPanel::clearDependentOnGameSave()
{
    m_artifactBagToolBar->ClearTools();
    clearDependentOnArtifactBag();
}

void StorageEditorPanel::clearDependentOnEnchantment()
{
    m_objectInspector->ClearDisplay();
}

void StorageEditorPanel::populateDependentOnArtifact(vanhelsing::engine::inventory::Artifact& artifact)
{
    populateEnchantments(artifact);
    m_objectInspector->ShowPropertiesForArtifact(artifact);
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
    m_objectInspector->ApplyPendingChanges();
}

void StorageEditorPanel::artifactEnchantmentsOnDataViewCtrlSelectionChanged(wxDataViewEvent& event)
{
    using namespace vanhelsing::engine::inventory;
    clearDependentOnEnchantment();

    if (!event.GetItem()) {
        return;
    }

    auto& enchantment = *reinterpret_cast<Enchantment*>(event.GetItem().GetID());
    m_objectInspector->ShowPropertiesForEnchantment(enchantment);
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
