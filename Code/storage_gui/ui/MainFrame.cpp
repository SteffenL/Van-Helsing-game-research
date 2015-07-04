#include "MainFrame.h"
#include "StorageEditorPanel.h"
#include "../services/ApplicationServices.h"
#include "../AppVersion.h"
#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/GamePaths.h>

#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include <future>
#include <chrono>


wxEventType MainFrame::FirstIdleEvent = wxNewEventType();


BEGIN_EVENT_TABLE(MainFrame, MainFrameBase)
EVT_COMMAND(wxID_ANY, MainFrame::FirstIdleEvent, MainFrame::onFirstIdle)
END_EVENT_TABLE()


MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBase(parent),
    m_gotFirstIdleEvent(false)
{
    // Main window title
    SetTitle(APP_FRIENDLY_NAME_I18N);

    auto mainPanel = new StorageEditorPanel(this);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(mainPanel, wxSizerFlags().Expand().Proportion(1));
    SetSizer(sizer);

    loadGameData();
}

MainFrame::~MainFrame()
{
}

void MainFrame::openOnMenuSelection(wxCommandEvent& event)
{
    const auto& msg = _("Select storage file");
    const auto& filter = wxT("storage.sav|storage.sav");
    wxFileDialog dialog(this, msg, wxEmptyString, wxEmptyString, filter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    auto& services = ApplicationServices::Get();
    services.StorageGameSave.OpenFile(dialog.GetPath().ToUTF8().data());
}

void MainFrame::saveOnMenuSelection(wxCommandEvent& event)
{
    // Remove focus from children so that any pending changes are applied
    SetFocus();

    auto& services = ApplicationServices::Get();
    services.StorageGameSave.SaveFile();
}

void MainFrame::saveOnUpdateUI(wxUpdateUIEvent& event)
{
    auto& services = ApplicationServices::Get();
    event.Enable(services.StorageGameSave.HasOpenedFile());
}

void MainFrame::exitOnMenuSelection(wxCommandEvent& event)
{
    Close();
}

void MainFrame::aboutOnMenuSelection(wxCommandEvent& event)
{
    wxAboutDialogInfo info;
    info.SetVersion(APP_VERSION_STR);
    info.SetWebSite(APP_WEBSITE_URL, APP_WEBSITE_TEXT);
    info.SetDescription(APP_DESCRIPTION_I18N);

    wxAboutBox(info, this);
}

void MainFrame::loadGameData()
{
    // Loading screen heading (app name and version)
    const auto& heading = wxString::Format(_("%1s %2s"), APP_FRIENDLY_NAME_I18N, APP_VERSION_STR);
    const auto& statusMsg = _("Loading...");

    wxBusyCursor cursor;
    wxWindowDisabler wd;
    wxBusyInfo info(wxBusyInfoFlags().Parent(this).Title(heading).Text(statusMsg));

    auto& services = ApplicationServices::Get();
    auto task = std::async(std::launch::async, [&services]() {
        services.GameData.Load();
    });

    while (task.wait_for(std::chrono::milliseconds(30)) != std::future_status::ready) {
        wxYield();
    }
}

void MainFrame::onIdle(wxIdleEvent& event)
{
    if (!m_gotFirstIdleEvent) {
        m_gotFirstIdleEvent = true;
        wxPostEvent(this, wxCommandEvent(FirstIdleEvent));
    }
}

void MainFrame::onFirstIdle(wxCommandEvent& event)
{
    // TODO: Will we ever need this?
}
