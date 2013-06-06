#pragma once

#include "HaliteTabCtrl.hpp"
#include "StorageItemsTabPage.h"
#include <memory>
#include <vanhelsing/engine/StorageGameSave.h>

//namespace vanhelsing { namespace engine { class StorageGameSave; } }

class CMainDlg
    : public CDialogImpl<CMainDlg>,
    public CUpdateUI<CMainDlg>,
    public CMessageFilter,
    public CIdleHandler,
    /*public CAutoSizeWindow<CMainDlg>,*/
    public CWinDataExchange<CMainDlg>,
    public CDialogResize<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

    CMainDlg();
    virtual ~CMainDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

    BEGIN_DDX_MAP(CMainDlg)
        //DDX_CONTROL_HANDLE(IDC_STORAGE_TABS, m_storageTabs)
    END_DDX_MAP()

    /*BEGIN_WINDOW_MAP(CMainDlg, 5, 5, 5, 5)
        WMB_HEAD(WMB_COL(_exp)),
        WMB_ROW(_exp, IDC_STORAGE_TABS),
        WMB_END()
    END_WINDOW_MAP()*/

    BEGIN_DLGRESIZE_MAP(CMainDlg)
        DLGRESIZE_CONTROL(IDC_STORAGE_TABS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    BEGIN_MSG_MAP(CMainDlg)
        if (uMsg == WM_FORWARDMSG) {
            if (PreTranslateMessage((LPMSG)lParam)) return TRUE;
        }

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER(ID_FILE_SAVE_AS, OnFileSaveAs)
        COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(IDCANCEL, OnFileExit)
        COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)

        //CHAIN_MSG_MAP(CAutoSizeWindow<CMainDlg>)
        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)

        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);

    void onStorageBagsSelChange();

private:
    void saveStorageGameSave(const std::string& filePath);

private:
    HACCEL m_hAccelerator;
    CHalTabCtrl m_storageTabs;
    StorageItemsTabPage m_storageItemsTabPage;
    CMenu m_mainMenu;
    struct
    {
        std::string StorageFilePath;
        std::unique_ptr<vanhelsing::engine::StorageGameSave> StorageGameSave;
    } m_openedFiles;
};
