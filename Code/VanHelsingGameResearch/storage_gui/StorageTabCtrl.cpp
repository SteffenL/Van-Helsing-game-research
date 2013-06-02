#include "stdafx.h"
#include "StorageTabs.h"
#include "resource.h"

StorageTabs::StorageTabs()
{
}

StorageTabs::~StorageTabs()
{
}

BOOL StorageTabs::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

LRESULT StorageTabs::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //DlgResize_Init(false, false, WS_CHILD);
    return 0;
}
