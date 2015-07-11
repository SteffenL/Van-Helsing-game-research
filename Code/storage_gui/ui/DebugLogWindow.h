#pragma once

#include "generated/main.h"
#include <wx/textctrl.h>
#include <iostream>


class DebugLogWindow : public DebugLogWindowBase
{
public:
    DebugLogWindow(wxWindow* parent, std::ostream* ostr);
    virtual ~DebugLogWindow();

    static DebugLogWindow& Get();
    static DebugLogWindow& Create(wxWindow* parent, std::ostream* ostr);

protected:
    void onSaveButtonClick(wxCommandEvent& event);

private:
    // This is a raw pointer because wxWidgets manages the window's lifetime
    static DebugLogWindow* m_instance;
    wxStreamToTextRedirector m_stdOutRedirector;
};
