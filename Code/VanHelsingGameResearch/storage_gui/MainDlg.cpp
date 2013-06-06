// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StorageGameSaveWriter.h>
#include <vanhelsing/engine/GameData.h>
#include <vanhelsing/engine/GamePaths.h>
#include <steffenl/common/backup.h>
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
    CSSFileDialog fileDialog(TRUE, NULL, NULL,
        OFN_FILEMUSTEXIST | OFN_ENABLESIZING,
        _T("storage.sav|storage.sav|*.sav|*.sav|"));

    if (fileDialog.DoModal(*this) != IDOK) {
        return 0;
    }

    auto& filePath(nowide::narrow(fileDialog.m_szFileName));
    nowide::ifstream file(filePath.c_str(), std::ios::binary);

    m_openedFiles.StorageGameSave.reset(new vanhelsing::engine::StorageGameSave);

    try {
        vanhelsing::engine::io::StorageGameSaveReader reader(*m_openedFiles.StorageGameSave, file);
    }
    catch (std::runtime_error& ex) {
        CTaskDialog dialog(*this);
        dialog.SetWindowTitle(L"Exception");
        dialog.SetMainInstructionText(L"Exception");
        auto& sw = nowide::widen(ex.what());
        dialog.SetContentText(sw.c_str());
        dialog.SetCommonButtons(TDCBF_OK_BUTTON);
        dialog.SetMainIcon(TD_ERROR_ICON);
        dialog.DoModal();
        return 0;
    }

    m_openedFiles.StorageFilePath = filePath;

    m_storageItemsTabPage[0].OnOpenGameSave(m_openedFiles.StorageGameSave.get());
    m_storageItemsTabPage[1].OnOpenGameSave(m_openedFiles.StorageGameSave.get());
    m_storageItemsTabPage[2].OnOpenGameSave(m_openedFiles.StorageGameSave.get());

    return 0;
}

LRESULT CMainDlg::OnFileSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (m_openedFiles.StorageFilePath.empty()) {
        return 0;
    }

    // Remove focus from children so that any pending changes are applied
    SetFocus();
    saveStorageGameSave(m_openedFiles.StorageFilePath);

    return 0;
}

LRESULT CMainDlg::OnFileSaveAs(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (m_openedFiles.StorageFilePath.empty()) {
        return 0;
    }

    // Remove focus from children so that any pending changes are applied
    SetFocus();
    // Ask user to choose path
    auto& StorageFilePathW = nowide::widen(m_openedFiles.StorageFilePath);
    CSSFileDialog dialog(FALSE, _T("sav"), StorageFilePathW.c_str(),
        OFN_ENABLESIZING,
        _T("storage.sav|storage.sav|*.sav|*.sav|"));

    if (dialog.DoModal(*this) != IDOK) {
        return 0;
    }

    auto& filePath(nowide::narrow(dialog.m_szFileName));
    saveStorageGameSave(filePath);

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

void CMainDlg::saveStorageGameSave(const std::string& filePath)
{
    //steffenl::common::Backup backup("AdvanSE_backup");

    nowide::ofstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        std::string ex("Couldn't open file: ");
        ex += filePath;
        throw std::runtime_error(ex);
    }

    vanhelsing::engine::io::StorageGameSaveWriter writer(*m_openedFiles.StorageGameSave, file);
}
