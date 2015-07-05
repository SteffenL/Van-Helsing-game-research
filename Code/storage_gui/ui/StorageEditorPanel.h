#pragma once

#include "generated/main.h"
#include <vanhelsing/services/OpenedStorageGameSaveFile.h>
#include <vanhelsing/engine/inventory.h>

#include <memory>

class ArtifactViewModel;
class EnchantmentViewModel;

namespace vanhelsing { namespace engine { namespace inventory {
class Artifact;
class Enchantment;
}}}

class StorageEditorPanel : public StorageEditorPanelBase
{
public:
    StorageEditorPanel(wxWindow* parent);
    virtual ~StorageEditorPanel();

private:
    // Inherited event handlers
    virtual void artifactBagOnToolClicked(wxCommandEvent& event);
    virtual void artifactBagOnUpdateUI(wxUpdateUIEvent& event);
    virtual void artifactsOnDataViewCtrlSelectionChanged(wxDataViewEvent& event);
    virtual void artifactEnchantmentsOnDataViewCtrlSelectionChanged(wxDataViewEvent& event);
    virtual void propertyManagerOnPropertyGridChanged(wxPropertyGridEvent& event);

    // Event handlers
    void onGameSaveOpened(vanhelsing::services::OpenedStorageGameSaveFile& fileInfo);


    void subscribeToEvents();
    void populateArtifacts();
    void populateEnchantments(vanhelsing::engine::inventory::Artifact& artifact);
    void populateBags();
    void populateGameSaveDependentUi();
    void initializeUi();
    void showPropertiesForArtifact(vanhelsing::engine::inventory::Artifact& artifact);
    void showPropertiesForEnchantment(vanhelsing::engine::inventory::Enchantment& enchantment);
    void initializeGameDataDependentProperties();
    void initializeGameSaveDependentUi();
    void clearGameSaveDependentUi();
    void clearArtifacts();
    void clearEnchantments();
    void clearProperties();
    void showImageForArtifact(vanhelsing::engine::inventory::Artifact& artifact);

    wxObjectDataPtr<ArtifactViewModel> m_artifactViewModel;
    wxObjectDataPtr<EnchantmentViewModel> m_enchantmentViewModel;
    std::shared_ptr<vanhelsing::engine::StorageGameSave> m_gameSave;
};
