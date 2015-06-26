#pragma once

#include <wx/wx.h>
#include <wx/debugrpt.h>

class App : public wxApp
{
private:
    virtual bool OnInit();

    virtual void OnUnhandledException();
    virtual bool OnExceptionInMainLoop();

    bool generateCrashReport(wxDebugReport::Context context) const;

};
