#pragma once

#include "stdafx.h"
#include "HaliteTabPage.hpp"
#include <vanhelsing/engine/StorageGameSave.h>

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

    StorageItemsTabPage();
    void OnOpenGameSave(vanhelsing::engine::StorageGameSave* gameSave);
    void SetBagNumber(int bagNumber);

    BOOL PreTranslateMessage(MSG* pMsg)
    {
        return base_class_t::IsDialogMessage(pMsg);
    }

    BEGIN_MSG_MAP_EX(this_class_t)

        if (uMsg == WM_FORWARDMSG) {
            if (PreTranslateMessage((LPMSG)lParam)) return TRUE;
        }

        MSG_WM_INITDIALOG(onInitDialog)

        CHAIN_MSG_MAP(resize_class_t)
        CHAIN_MSG_MAP(base_class_t)

        NOTIFY_HANDLER_EX(IDC_ITEM_LIST, LVN_ITEMCHANGED, OnItemChanged)
        NOTIFY_HANDLER_EX(IDC_ENCHANTMENT_LIST, LVN_ITEMCHANGED, OnEnchantmentItemChanged)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    BOOL DoDataExchange(BOOL bSaveAndValidate = FALSE, UINT nCtlID = (UINT)-1);

    BEGIN_DLGRESIZE_MAP(this_class_t)
        DLGRESIZE_CONTROL(IDC_ITEM_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_HEADING, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_LIST, DLSZ_MOVE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    LRESULT onInitDialog(HWND, LPARAM);
    LRESULT OnClick(int, LPNMHDR pnmh, BOOL&);
    LRESULT OnItemChanged(LPNMHDR lpHdr);
    LRESULT OnEnchantmentItemChanged(LPNMHDR lpHdr);
    LRESULT OnRClick(int i, LPNMHDR pnmh, BOOL&);

private:
    void syncItemList();
    void syncEnchantmentList();

protected:
    CListViewCtrl m_itemList;
    std::vector<int> m_itemListSelectedItems;

    CListViewCtrl m_enchantmentList;
    std::vector<int> m_enchantmentListSelectedItems;

    vanhelsing::engine::StorageGameSave* m_gameSave;
    int m_bagNumber;
};
