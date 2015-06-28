#pragma once

#include <memory>
#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>

class CMyEdit :
    public CWindowImpl<CMyEdit, CEdit>
{
public:
    struct ApplyEventArg
    {
        void SetEventObject(void* obj) { m_eventObject = obj; }
        void* GetEventObject() { return m_eventObject; }

    private:
        void* m_eventObject;
    };

    boost::shared_ptr<boost::signal<void (ApplyEventArg&)>> ApplyEvent;

    CMyEdit(HWND hWnd = NULL);
    CMyEdit& operator=(HWND hWnd);
    BOOL PreTranslateMessage(MSG* pMsg);
    CString GetValue() const;
    void SetValue(const CString& text);

    BEGIN_MSG_MAP_EX(CMyEdit)
        MSG_WM_GETDLGCODE(OnGetDlgCode)
        MSG_WM_CHAR(OnChar)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_SETFOCUS(OnSetFocus)
        MSG_WM_KILLFOCUS(OnKillFocus)
    END_MSG_MAP()

    UINT OnGetDlgCode(LPMSG lpMsg);
    void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void OnSetFocus(CWindow wndOld);
    void OnKillFocus(CWindow wndFocus);

private:
    void applyChanges();
    void discardChanges();

    CString m_oldValue;
};
