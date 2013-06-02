#pragma once

#include "stdafx.h"
#include "HaliteTabPage.hpp"

class StorageItemsTabPage :
    public CHalTabPageImpl<StorageItemsTabPage>,
    public WTL::CDialogResize<StorageItemsTabPage>,
    public WTLx::WinDataExchangeEx<StorageItemsTabPage>
{
protected:
    typedef StorageItemsTabPage this_class_t;
    typedef CHalTabPageImpl<StorageItemsTabPage> base_class_t;
    typedef WTL::CDialogResize<StorageItemsTabPage> resize_class_t;

public:
    enum { IDD = IDD_STORAGE_TAB_PAGE };

    StorageItemsTabPage() {}

    BOOL PreTranslateMessage(MSG* pMsg)
    {
        return base_class_t::IsDialogMessage(pMsg);
    }

    BEGIN_MSG_MAP_EX(this_class_t)
        MSG_WM_INITDIALOG(onInitDialog)
        MSG_WM_CLOSE(onClose)

        if (uMsg == WM_FORWARDMSG)
            if (PreTranslateMessage((LPMSG)lParam)) return TRUE;

        CHAIN_MSG_MAP(resize_class_t)
        CHAIN_MSG_MAP(base_class_t)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BOOL DoDataExchange(BOOL bSaveAndValidate = FALSE, UINT nCtlID = (UINT)-1);

    BEGIN_DLGRESIZE_MAP(this_class_t)
        DLGRESIZE_CONTROL(IDC_STORAGE_ITEMS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    LRESULT onInitDialog(HWND, LPARAM);

protected:
};
