#pragma once

#include "generated/main.h"
#include <wx/textctrl.h>
#include <iostream>
#include <streambuf>
#include <memory>


class DebugLogWindow : public DebugLogWindowBase
{
public:
    DebugLogWindow(wxWindow* parent, std::ostream* ostr);
    virtual ~DebugLogWindow();

    static DebugLogWindow& Get();
    static DebugLogWindow& Create(wxWindow* parent, std::ostream* ostr);

protected:
    virtual void onSaveButtonClick(wxCommandEvent& event);
    virtual void onClose(wxCloseEvent& event);

private:
    // This is a raw pointer because wxWidgets manages the window's lifetime
    static DebugLogWindow* m_instance;
    std::unique_ptr<std::streambuf> m_streambuf;
    std::ostream* m_ostream;
};
