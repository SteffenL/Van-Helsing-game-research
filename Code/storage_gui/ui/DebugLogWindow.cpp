#include "DebugLogWindow.h"
#include <stdexcept>
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>


class my_console_output_buffer : public std::streambuf
{
public:
    my_console_output_buffer(wxTextCtrl* textCtrl);
    virtual ~my_console_output_buffer();
protected:
    int sync();
    int overflow(int c);
private:

    int write(char const *p, int n);

    static const int buffer_size = 1024;
    char buffer_[buffer_size];
    
    wxTextCtrl* textCtrl;
    std::ostringstream line;
};

int my_console_output_buffer::write(char const *p, int n)
{
    char const *b = p;
    char const *e = p + n;
    if (n > buffer_size)
        return -1;

    line.write(buffer_, n);
    return n;
}

int my_console_output_buffer::overflow(int c)
{
    int n = pptr() - pbase();
    int r = 0;

    if (n > 0 && (r = write(pbase(), n)) < 0)
        return -1;
    if (r < n) {
        memmove(pbase(), pbase() + r, n - r);
    }
    setp(buffer_, buffer_ + buffer_size);
    pbump(n - r);
    if (c != EOF)
        sputc(c);
    return 0;
}

int my_console_output_buffer::sync()
{
    auto x = overflow(EOF);

    if (line.tellp() > 0) {
        textCtrl->AppendText(line.str());
        line.str("");
    }

    return x;
}

my_console_output_buffer::my_console_output_buffer(wxTextCtrl* textCtrl)
    : textCtrl(textCtrl)
{
}

my_console_output_buffer::~my_console_output_buffer()
{
}



DebugLogWindow* DebugLogWindow::m_instance;


DebugLogWindow::DebugLogWindow(wxWindow* parent, std::ostream* ostr)
    : DebugLogWindowBase(parent),
    m_ostream(ostr)
{
    m_streambuf = std::make_unique<my_console_output_buffer>(m_text);
    m_ostream->set_rdbuf(m_streambuf.get());

    // TODO: Show this when fully implemented
    m_sdbSizer1Save->Hide();
    Layout();
}

DebugLogWindow::~DebugLogWindow()
{
    if (m_ostream) {
        m_ostream->set_rdbuf(nullptr);
    }
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

void DebugLogWindow::onClose(wxCloseEvent& event)
{
    if (event.CanVeto()) {
        Hide();
        event.Veto();
        return;
    }

    event.Skip();
}
