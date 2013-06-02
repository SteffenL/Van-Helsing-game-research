#pragma once

#include "HaliteTabCtrl.hpp"
#include "StorageItemsTabPage.h"

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
        //CHAIN_MSG_MAP(CAutoSizeWindow<CMainDlg>)
        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);

private:
    CHalTabCtrl m_storageTabs;
    std::map<int, StorageItemsTabPage> m_storageItemsTabPage;
    CMenu m_mainMenu;
};
