#include "App.h"
#include "ui/MainFrame.h"
#include "ui/DebugLogWindow.h"
#include "AppVersion.h"
#include "services/ApplicationServices.h"
#include <common/Log.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>
#include <common/my_console_output_buffer.h>

struct CmdLineArgName
{
    static const char* Debug;
    static const char* GameDir;
};

const char* CmdLineArgName::Debug = "debug";
const char* CmdLineArgName::GameDir = "game-dir";


wxIMPLEMENT_APP(App);


const App::CmdLineArgs& App::GetCmdLineArgs() const
{
    return *m_cmdArgs;
}

App* App::Get()
{
    return dynamic_cast<App*>(GetInstance());
}

bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    SetVendorName(APP_VENDOR_NAME);
    SetAppName(APP_FRIENDLY_NAME);
    SetAppDisplayName(APP_FRIENDLY_NAME_I18N);

    if (m_cmdArgs->EnableDebug) {
        using namespace common;
        Log::SetLogLevelFilter(LogLevel::Trace);
    }

    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxTGAHandler);

    auto frame = new MainFrame(nullptr, m_cmdArgs->EnableDebug);
    return true;
}

void App::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine(parser);
    parser.AddLongSwitch(CmdLineArgName::Debug, "enable verbose debug logging");
    parser.AddLongOption(CmdLineArgName::GameDir, "game directory", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser)) {
        return false;
    }

    m_cmdArgs = std::make_unique<CmdLineArgs>();
    m_cmdArgs->EnableDebug = parser.Found(CmdLineArgName::Debug);
    m_cmdArgs->GameDirSpecified = parser.Found(CmdLineArgName::GameDir, &m_cmdArgs->GameDir);
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
