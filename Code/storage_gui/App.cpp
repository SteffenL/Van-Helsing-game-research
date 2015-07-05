#include "App.h"
#include "ui/MainFrame.h"
#include "AppVersion.h"
#include "services/ApplicationServices.h"
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>


wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    SetVendorName(APP_VENDOR_NAME);
    SetAppName(APP_FRIENDLY_NAME);
    SetAppDisplayName(APP_FRIENDLY_NAME_I18N);

    if (!wxApp::OnInit()) {
        return false;
    }

    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxTGAHandler);

    auto frame = new MainFrame(nullptr);
    frame->Show();
    return true;
}

void App::OnUnhandledException()
{
#ifndef NDEBUG
    throw;
#endif

    generateCrashReport(wxDebugReport::Context_Current);
}

bool App::OnExceptionInMainLoop()
{
    using vanhelsing::engine::VanHelsingEngineError;

    try {
        throw;
    }
    catch (const VanHelsingEngineError& ex) {
        wxMessageBox(wxString::Format(_("An error has occurred.\n\n%1s"), ex.what()), _("Van Helsing engine error"), wxOK | wxICON_ERROR);
    }
    catch (const std::runtime_error& ex) {
        wxMessageBox(wxString::Format(_("An error has occurred.\n\n%1s"), ex.what()), _("Runtime error"), wxOK | wxICON_ERROR);
    }
    catch (...) {
#ifndef NDEBUG
        throw;
#endif
        generateCrashReport(wxDebugReport::Context_Current);
        return false;
    }

    return true;
}

bool App::generateCrashReport(wxDebugReport::Context context) const
{
    wxDebugReportCompress report;
    report.AddAll(context);
    if (!wxDebugReportPreviewStd().Show(report)) {
        return false;
    }

    if (!report.Process()) {
        return false;
    }

    const auto& msg = _("Thank you!\n\nI'll now attempt to open your default e-mail client and open the folder that contains the report.");
    if (wxMessageBox(msg, wxMessageBoxCaptionStr, wxOK_DEFAULT | wxCANCEL) != wxOK) {
        return false;
    }

    {
        // Open e-mail client if possible
        const auto& emailSubject = wxString::Format(wxT("Crash report for %1s %2s"), APP_FRIENDLY_NAME, APP_VERSION_STR);
        const auto& emailCmd = wxString::Format(wxT("mailto:%1s?subject=%2s"), APP_CRASH_REPORT_EMAIL, emailSubject);
        wxLaunchDefaultBrowser(emailCmd);
    }

    {
        // Open folder containing the file
#ifdef __WXMSW__
        wxExecute(wxString::Format(wxT("explorer.exe /select,\"%s\""), report.GetCompressedFileName()), wxEXEC_ASYNC);
#else
#error Not implemented
#endif
    }

    return false;
}
