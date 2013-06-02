#include "stdafx.h"
#include "StorageItemsTabPage.h"

LRESULT StorageItemsTabPage::onInitDialog(HWND, LPARAM)
{
    resize_class_t::DlgResize_Init(false, true, WS_CLIPCHILDREN);	
    DoDataExchange(false);

    return 0;
}

BOOL StorageItemsTabPage::DoDataExchange(BOOL bSaveAndValidate, UINT nCtlID)
{
    return TRUE;
}
