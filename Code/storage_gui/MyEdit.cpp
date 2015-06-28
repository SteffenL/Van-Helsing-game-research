#include "stdafx.h"
#include "MyEdit.hpp"

void CMyEdit::discardChanges()
{
    if (GetModify()) {
        SetValue(m_oldValue);
        SetModify(FALSE);
    }
}

void CMyEdit::applyChanges()
{
    // If modified, request to apply changes
    if (GetModify()) {
        m_oldValue = GetValue();
        SetModify(FALSE);
        ApplyEventArg e;
        e.SetEventObject(this);
        (*ApplyEvent)(e);
    }
}

void CMyEdit::OnKillFocus(CWindow wndFocus)
{
    // Apply changes if needed
    if (GetModify()) {
        applyChanges();
    }
    else {
        discardChanges();
    }

    DefWindowProc();
}

void CMyEdit::OnSetFocus(CWindow wndOld)
{
    // If non-modified, save the value in case we need to discard the changes
    if (!GetModify()) {
        m_oldValue = GetValue();
    }

    DefWindowProc();
}

void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar) {
    case VK_RETURN:
        applyChanges();
        return;

    case VK_ESCAPE:
        discardChanges();
        return;
    }

    DefWindowProc();
}

void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Avoid beep sound
    if ((nChar == 13) /* return */ || (nChar == 27 /* escape */)) {
        return;
    }

    DefWindowProc();
}

UINT CMyEdit::OnGetDlgCode(LPMSG lpMsg)
{
    // This let's us receive notifications for the key codes we need
    auto lr = DefWindowProc();
    if (lpMsg) {
        if ((lpMsg->wParam == VK_RETURN) || (lpMsg->wParam == VK_ESCAPE)) {
            lr |= DLGC_WANTMESSAGE;
            return lr;
        }
    }
    else {
        return lr;
    }

    return DefWindowProc();
}

void CMyEdit::SetValue(const CString& text)
{
    ::SetWindowText(*this, static_cast<LPCTSTR>(text));
}

CString CMyEdit::GetValue() const
{
    auto length = ::GetWindowTextLength(*this);
    CTempBuffer<TCHAR> text(length + 1);
    ::GetWindowText(*this, static_cast<LPTSTR>(text), length + 1);
    return static_cast<CString>(text);
}

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg)
{
    return FALSE;
}

CMyEdit& CMyEdit::operator=(HWND hWnd)
{
    m_hWnd = hWnd;
    return *this;
}

CMyEdit::CMyEdit(HWND hWnd) : ApplyEvent(new boost::signal<void (CMyEdit::ApplyEventArg&)>) {}
