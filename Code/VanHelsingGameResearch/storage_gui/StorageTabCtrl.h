#pragma once

#include "resource.h"
#include <atlctrlx.h>

class StorageTabs
    : /*public CTabCtrl,
    public CWindowImpl<StorageTabs, CTabCtrl>,*/
    public CDialogImpl<StorageTabs>
    //public CDialogResize<StorageTabs>
{
public:
    enum { IID = IDC_STORAGE_TABS };
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_DDX_MAP(CMainDlg)
    END_DDX_MAP()

    BEGIN_MSG_MAP(StorageTabs)
        CHAIN_MSG_MAP(CDialogImpl<StorageTabs>)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        //CHAIN_MSG_MAP(CDialogResize<StorageTabs>)
    END_MSG_MAP()

    /*BEGIN_DLGRESIZE_MAP(StorageTabs)
        DLGRESIZE_CONTROL(IDC_STORAGE_ITEMS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()*/

public:
    StorageTabs();
    virtual ~StorageTabs();
/*

    using CWindowImpl<StorageTabs, CTabCtrl>::m_hWnd;
    using CDialogImpl<StorageTabs>::operator=;*/

protected:
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

