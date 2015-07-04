#pragma once

#include "generated/main.h"

#include <memory>

class MainFrame : public MainFrameBase
{
    static wxEventType FirstIdleEvent;

public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

private:
    DECLARE_EVENT_TABLE()

    // Inherited event handlers
    virtual void openOnMenuSelection(wxCommandEvent& event);
    virtual void saveOnMenuSelection(wxCommandEvent& event);
    virtual void saveOnUpdateUI(wxUpdateUIEvent& event);
    virtual void exitOnMenuSelection(wxCommandEvent& event);
    virtual void aboutOnMenuSelection(wxCommandEvent& event);
    virtual void onIdle(wxIdleEvent& event);

    // Event handlers
    void onFirstIdle(wxCommandEvent& event);

    void loadGameData();


    bool m_gotFirstIdleEvent;
};
