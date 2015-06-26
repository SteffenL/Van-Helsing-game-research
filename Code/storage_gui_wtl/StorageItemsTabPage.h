#pragma once

#include "stdafx.h"
#include "HaliteTabPage.hpp"
#include <vanhelsing/engine/StorageGameSave.h>

namespace vanhelsing { namespace engine { class TextManager; namespace inventory { class Artifact; } } }

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

    BEGIN_DDX_MAP(CMainDlg)
        // Items
        DDX_CONTROL_HANDLE(IDC_ITEM_LIST, m_itemList)
        DDX_CONTROL_HANDLE(IDC_ITEM_ATTRIBUTE1_EDIT, m_itemProperty1)
        DDX_CONTROL_HANDLE(IDC_ITEM_ATTRIBUTE2_EDIT, m_itemProperty2)
        DDX_CONTROL_HANDLE(IDC_ITEM_RARITY_COMBO, m_itemRarity)
        DDX_CONTROL_HANDLE(IDC_ITEM_QUALITY_COMBO, m_itemQuality)
        DDX_CONTROL_HANDLE(IDC_ITEM_IDENTIFIED_CHECK, m_itemIsIdentified)
        DDX_CONTROL_HANDLE(IDC_ITEM_QUANTITY_EDIT, m_itemQuantity)
        // Enchantments
        DDX_CONTROL_HANDLE(IDC_ENCHANTMENT_LIST, m_enchantmentList)
        DDX_CONTROL_HANDLE(IDC_ENCHANTMENT_EFFECT_VALUE_EDIT, m_enchantmentEffectValue)
        DDX_CONTROL_HANDLE(IDC_ENCHANTMENT_EFFECT_MODIFIER_EDIT, m_enchantmentEffectModifier)
    END_DDX_MAP()

    BEGIN_MSG_MAP_EX(this_class_t)
        if (uMsg == WM_FORWARDMSG) {
            if (PreTranslateMessage((LPMSG)lParam)) return TRUE;
        }

        MSG_WM_INITDIALOG(onInitDialog)

        CHAIN_MSG_MAP(resize_class_t)
        CHAIN_MSG_MAP(base_class_t)

        // Items
        NOTIFY_HANDLER_EX(IDC_ITEM_LIST, LVN_ITEMCHANGED, OnItemChanged)
        MESSAGE_HANDLER_EX(WM_ITEM_LVSELCHANGE, OnItemSelectionChange)
        COMMAND_HANDLER_EX(IDC_ITEM_RARITY_COMBO, CBN_SELCHANGE, comboOnSelectionChange)
        COMMAND_HANDLER_EX(IDC_ITEM_QUALITY_COMBO, CBN_SELCHANGE, comboOnSelectionChange)
        COMMAND_HANDLER_EX(IDC_ITEM_IDENTIFIED_CHECK, BN_CLICKED, buttonOnClicked)
        // Enchantments
        NOTIFY_HANDLER_EX(IDC_ENCHANTMENT_LIST, LVN_ITEMCHANGED, OnEnchantmentChanged)
        MESSAGE_HANDLER_EX(WM_ENCHANTMENT_LVSELCHANGE, OnEnchantmentSelectionChange)

        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(this_class_t)
        // Items
        DLGRESIZE_CONTROL(IDC_ITEM_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_MODIFY_ITEM_GROUP, DLSZ_SIZE_X | DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_ATTRIBUTE1_TEXT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_ATTRIBUTE1_EDIT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_ATTRIBUTE2_TEXT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_ATTRIBUTE2_EDIT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_RARITY_TEXT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_RARITY_COMBO, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_QUALITY_TEXT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_QUALITY_COMBO, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_IDENTIFIED_CHECK, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_QUANTITY_TEXT, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ITEM_QUANTITY_EDIT, DLSZ_MOVE_Y)
        // Enchantments
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_HEADING, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_LIST, DLSZ_MOVE_X | DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_MODIFY_ENCHANTMENT_GROUP, DLSZ_MOVE_X | DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_EFFECT_VALUE_TEXT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_EFFECT_VALUE_EDIT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_EFFECT_MODIFIER_TEXT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_ENCHANTMENT_EFFECT_MODIFIER_EDIT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()

    LRESULT onInitDialog(HWND, LPARAM);
    LRESULT OnClick(int, LPNMHDR pnmh, BOOL&);
    LRESULT OnItemChanged(LPNMHDR lpHdr);
    LRESULT OnItemSelectionChange(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnEnchantmentChanged(LPNMHDR lpHdr);
    LRESULT OnEnchantmentSelectionChange(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnRClick(int i, LPNMHDR pnmh, BOOL&);
    LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void comboOnSelectionChange(UINT uNotifyCode, int nID, CWindow wndCtl);
    void buttonOnClicked(UINT uNotifyCode, int nID, CWindow wndCtl);
    
    void editOnApply(CMyEdit::ApplyEventArg& e);

    void FullUpdate();

private:
    // Items
    void syncItemList();
    void updateItemListItem(int i);
    void updateItemSection();
    void updateModifyItemSection(
        const vanhelsing::engine::TextManager& textManager,
        const vanhelsing::engine::inventory::Artifact* item);
    void applyItemProperty1();
    void applyItemProperty2();
    void applyItemRarity();
    void applyItemQuality();
    void applyItemIsIdentified();
    void applyItemQuantity();
    // Enchantments
    void syncEnchantmentList();
    void updateEnchantmentListItem(int i);
    void updateEnchantmentSection(
        const vanhelsing::engine::TextManager& textManager,
        const vanhelsing::engine::inventory::Artifact* item);
    void updateModifyEnchantmentSection(
        const vanhelsing::engine::TextManager& textManager,
        const vanhelsing::engine::inventory::Enchantment* item);
    void applyEnchantmentEffectModifier();
    void applyEnchantmentEffectValue();
protected:
    int WM_ITEM_LVSELCHANGE;
    int WM_ENCHANTMENT_LVSELCHANGE;
    bool m_itemListSkipChangeNotification;
    bool m_enchantmentListSkipChangeNotification;
    
    vanhelsing::engine::StorageGameSave* m_gameSave;
    int m_bagNumber;

    // Items
    CListViewCtrl m_itemList;
    std::vector<int> m_itemListSelectedItems;
    CMyEdit m_itemProperty1;
    CUpDownCtrl m_itemProperty1SpinCtrl;
    CMyEdit m_itemProperty2;
    CUpDownCtrl m_itemProperty2SpinCtrl;
    CComboBox m_itemRarity;
    CComboBox m_itemQuality;
    CButton m_itemIsIdentified;
    CMyEdit m_itemQuantity;

    // Enchantments
    CListViewCtrl m_enchantmentList;
    std::vector<int> m_enchantmentListSelectedItems;
    CMyEdit m_enchantmentEffectValue;
    CMyEdit m_enchantmentEffectModifier;
};
