#pragma once

#include <wx/wx.h>
#include <wx/debugrpt.h>
#include <wx/cmdline.h>
#include <memory>

class DebugLogWindow;


class App : public wxApp
{
public:
    struct CmdLineArgs
    {
        bool EnableDebug;
        bool GameDirSpecified;
        wxString GameDir;
    };

    const CmdLineArgs& GetCmdLineArgs() const;
    static App* Get();

private:
    virtual bool OnInit();
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    virtual void OnInitCmdLine(wxCmdLineParser& parser);

    virtual void OnUnhandledException();
    virtual bool OnExceptionInMainLoop();

    bool generateCrashReport(wxDebugReport::Context context) const;

private:
    struct CmdLineArgs;
    std::unique_ptr<CmdLineArgs> m_cmdArgs;
};
