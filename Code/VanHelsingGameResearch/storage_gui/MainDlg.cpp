// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/GamePaths.h>
#include <nowide/fstream.hpp>

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_hAccelerator) {
        if (::TranslateAccelerator(m_hWnd, m_hAccelerator, pMsg)) {
            return TRUE;
        }
    }

    return m_storageTabs.PreTranslateMessage(pMsg);
	//return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    using namespace vanhelsing::engine;
    DoDataExchange(false);
    DlgResize_Init(false, false, WS_CLIPCHILDREN);

    // Load game data before anything attempts to use it
    std::string gameDir;
    auto gameDir_c = std::getenv("VH_GAME_DIR");
    if (gameDir_c) {
        gameDir = gameDir_c;
    }
    else {
        gameDir = GamePaths::GetInstallPath();
    }

    if (!gameDir.empty()) {
        GameData::Get().Load(gameDir);
    }

    m_storageTabs.SubclassWindow(GetDlgItem(IDC_STORAGE_TABS));
    m_storageItemsTabPage[0].Create(m_storageTabs);
    m_storageItemsTabPage[1].Create(m_storageTabs);
    m_storageItemsTabPage[2].Create(m_storageTabs);
    m_storageItemsTabPage[0].SetBagNumber(0);
    m_storageItemsTabPage[1].SetBagNumber(1);
    m_storageItemsTabPage[2].SetBagNumber(2);
    m_storageTabs.AddPage(m_storageItemsTabPage[0], _T("Page 1"));
    m_storageTabs.AddPage(m_storageItemsTabPage[1], _T("Page 2"));
    m_storageTabs.AddPage(m_storageItemsTabPage[2], _T("Page 3"));
    m_storageTabs.SetCurrentPage(0);

	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

    m_hAccelerator = AtlLoadAccelerators(IDR_MAINFRAME);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

    m_mainMenu.Attach(::LoadMenu(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAIN_MENU)));
    SetMenu(m_mainMenu);

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

static UINT_PTR CALLBACK file(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CMainDlg::OnFileOpen(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   /* Kerr::FileOpenDialog dialog;
    if (FAILED(dialog.Load())) {
        return 0;
    }

    COMDLG_FILTERSPEC filter[] = { L"storage.sav", L"storage.sav" };
    dialog->SetFileTypes(1, filter);

    if (FAILED(dialog.DoModal())) {
        return 0;
    }

    CComPtr<IShellItem> selection;
    dialog->GetResult(&selection);*/

    CSSFileDialog dialog(TRUE, NULL, NULL,
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING,
        _T("storage.sav|storage.sav|"));

    if (dialog.DoModal(*this) != IDOK) {
        return 0;
    }

    auto& filePath(nowide::narrow(dialog.m_szFileName));
    nowide::ifstream file(filePath.c_str(), std::ios::binary);

    m_gameSave.reset(new vanhelsing::engine::StorageGameSave);
    vanhelsing::engine::io::StorageGameSaveReader reader(*m_gameSave, file);

    m_storageItemsTabPage[0].OnOpenGameSave(m_gameSave.get());
    m_storageItemsTabPage[1].OnOpenGameSave(m_gameSave.get());
    m_storageItemsTabPage[2].OnOpenGameSave(m_gameSave.get());

    return 0;
}

LRESULT CMainDlg::OnFileSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    return 0;
}

LRESULT CMainDlg::OnFileSaveAs(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
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

CMainDlg::CMainDlg() {}

CMainDlg::~CMainDlg() {}