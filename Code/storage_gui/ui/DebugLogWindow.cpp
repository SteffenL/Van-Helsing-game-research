#include "DebugLogWindow.h"
#include <stdexcept>


DebugLogWindow* DebugLogWindow::m_instance;


DebugLogWindow::DebugLogWindow(wxWindow* parent, std::ostream* ostr)
    : DebugLogWindowBase(parent),
    m_stdOutRedirector(m_text, ostr)
{
    // TODO: Show this when fully implemented
    m_sdbSizer1Save->Hide();
    Layout();
}

DebugLogWindow::~DebugLogWindow()
{
}

DebugLogWindow& DebugLogWindow::Get()
{
    if (!m_instance) {
        throw std::logic_error("Must create an instance first");
    }

    return *m_instance;
}

DebugLogWindow& DebugLogWindow::Create(wxWindow* parent, std::ostream* ostr)
{
    m_instance = new DebugLogWindow(parent, ostr);
    return *m_instance;
}

void DebugLogWindow::onSaveButtonClick(wxCommandEvent& event)
{
    // TODO: Implement onSaveButtonClick
}
