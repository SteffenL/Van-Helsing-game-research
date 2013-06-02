// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    /*CPropertySheet sheet;
    CPropertyPage<IDD_PROPPAGE_SMALL> propPage;

    //sheet.m_psh.dwFlags = sheet.m_psh.dwFlags & ~PSH_NOAPPLYNOW;

    sheet.AddPage ( propPage );
    sheet.AddPage ( propPage );
    sheet.AddPage ( propPage );
    sheet.Create(*this);*/

    DlgResize_Init(false, false, WS_CLIPCHILDREN);
    DoDataExchange(false);

    m_storageTabs.SubclassWindow(GetDlgItem(IDC_STORAGE_TABS));
    m_storageItemsTabPage[0].Create(m_storageTabs);
    m_storageItemsTabPage[1].Create(m_storageTabs);
    m_storageItemsTabPage[2].Create(m_storageTabs);
    m_storageTabs.AddPage(m_storageItemsTabPage[0], L"Page 1");
    m_storageTabs.AddPage(m_storageItemsTabPage[1], L"Page 2");
    m_storageTabs.AddPage(m_storageItemsTabPage[2], L"Page 3");
    m_storageTabs.SetCurrentPage(0);


    //m_storageTabs.SubclassWindow(GetDlgItem(IDC_STORAGE_TABS));
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

    m_mainMenu.Attach(::LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN_MENU)));
    SetMenu(m_mainMenu);

    //m_storageTabs.Attach(GetDlgItem(IDC_STORAGE_TABS));

    /*RECT rect;
    GetClientRect(&rect);
    rect.left += 5;
    rect.top += 5;
    rect.right -= 5;
    rect.bottom -= 5;
    m_storageTabs.Create(m_hWnd, rect);
    m_storageTabs.AddPage(GetDlgItem(IDC_STORAGE_TABS), _T("Page 1"));
    m_storageTabs.AddPage(GetDlgItem(IDC_STORAGE_TABS), _T("Page 2"));
    m_storageTabs.AddPage(GetDlgItem(IDC_STORAGE_TABS), _T("Page 3"));*/

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnFileExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
