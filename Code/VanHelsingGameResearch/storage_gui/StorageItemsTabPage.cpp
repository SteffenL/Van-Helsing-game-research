#include "stdafx.h"
#include "StorageItemsTabPage.h"
#include <nowide/convert.hpp>
#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>

StorageItemsTabPage::StorageItemsTabPage() {}

LRESULT StorageItemsTabPage::onInitDialog(HWND, LPARAM)
{
    m_itemList.Attach(GetDlgItem(IDC_ITEM_LIST));
    m_itemList.AddColumn(_T("Slot #"), 0);
    m_itemList.AddColumn(_T("Name"), 1);
    m_itemList.AddColumn(_T("Attribute 1"), 2);
    m_itemList.AddColumn(_T("Attribute 2"), 3);
    m_itemList.AddColumn(_T("Rarity"), 4);
    m_itemList.AddColumn(_T("Quality"), 5);
    m_itemList.AddColumn(_T("Identified?"), 6);
    m_itemList.SetExtendedListViewStyle(m_itemList.GetExtendedListViewStyle() | (LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER));
    ::SetWindowTheme(m_itemList.m_hWnd, L"Explorer", NULL);

    m_enchantmentList.Attach(GetDlgItem(IDC_ENCHANTMENT_LIST));
    m_enchantmentList.AddColumn(_T("Name"), 0);
    m_enchantmentList.AddColumn(_T("Multiplier"), 1);
    m_enchantmentList.SetExtendedListViewStyle(m_enchantmentList.GetExtendedListViewStyle() | (LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER));
    ::SetWindowTheme(m_enchantmentList.m_hWnd, L"Explorer", NULL);

    resize_class_t::DlgResize_Init(false, false, WS_CLIPCHILDREN);	
    DoDataExchange(false);

    return 0;
}

BOOL StorageItemsTabPage::DoDataExchange(BOOL bSaveAndValidate, UINT nCtlID)
{
    return TRUE;
}

void StorageItemsTabPage::OnOpenGameSave(vanhelsing::engine::StorageGameSave* gameSave)
{
    m_gameSave = gameSave;
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    std::vector<Item*> items;
    m_gameSave->GetItems().FindByBagNumber(m_bagNumber, items);

    auto& textManager = GameData::Get().GetTextManager();

    m_itemList.DeleteAllItems();
    int i = 0;
    for (auto item : items) {
        auto& name = nowide::widen(textManager.GetItemText(item->GetName()));
        CString slotNumber;
        slotNumber.Format(_T("%d"), item->SlotNumber);
        CString attribute1;
        attribute1.Format(_T("%d"), item->Attribute1);
        CString attribute2;
        attribute2.Format(_T("%d"), item->Attribute2);
        auto& rarity = nowide::widen(textManager.GetRarityText(item->Rarity));
        auto& quality = nowide::widen(textManager.GetQualityText(item->Quality));
        CString identified(item->IsIdentified ? _T("Yes") : _T("No"));

        m_itemList.AddItem(i, 0, static_cast<LPCTSTR>(slotNumber));
        m_itemList.SetItemData(i, reinterpret_cast<DWORD_PTR>(item));
        m_itemList.SetItemText(i, 1, name.c_str());
        m_itemList.SetItemText(i, 2, static_cast<LPCTSTR>(attribute1));
        m_itemList.SetItemText(i, 3, static_cast<LPCTSTR>(attribute2));
        m_itemList.SetItemText(i, 4, rarity.c_str());
        m_itemList.SetItemText(i, 5, quality.c_str());
        m_itemList.SetItemText(i, 6, static_cast<LPCTSTR>(identified));
        ++i;
    }
}

LRESULT StorageItemsTabPage::OnClick(int, LPNMHDR pnmh, BOOL&)
{
    return 0;
}

LRESULT StorageItemsTabPage::OnItemChanged(LPNMHDR lpHdr)
{
    syncItemList();
    return 0;
}

LRESULT StorageItemsTabPage::OnEnchantmentItemChanged(LPNMHDR lpHdr)
{
    syncEnchantmentList();
    return 0;
}

LRESULT StorageItemsTabPage::OnRClick(int i, LPNMHDR pnmh, BOOL&)
{
    LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pnmh;

    /*if (TBase::LISTVIEW_ID_MENU)
    {
        assert (menu_.IsMenu());
        CMenuHandle sMenu = menu_.GetSubMenu(0);
        assert (sMenu.IsMenu());

        POINT ptPoint;
        GetCursorPos(&ptPoint);
        sMenu.TrackPopupMenu(0, ptPoint.x, ptPoint.y, m_hWnd);
    }*/

    return 0;
}

void StorageItemsTabPage::SetBagNumber(int bagNumber) { m_bagNumber = bagNumber; }

void StorageItemsTabPage::syncItemList()
{
    m_itemListSelectedItems.clear();
    // No selection?
    int firstSelection = m_itemList.GetSelectionMark();
    if (firstSelection == -1) {
        return;
    }

    // Find selected items
    for (int i = 0, count = m_itemList.GetItemCount(); i < count; ++i) {
        if (m_itemList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
            m_itemListSelectedItems.push_back(i);
        }
    }

    // Not a single selection?
    if (m_itemListSelectedItems.size() != 1) {
        return;
    }

    // Single selection
    //updateEnchantmentSection();

    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    auto& textManager = GameData::Get().GetTextManager();
    auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(firstSelection));
    auto& enchantments = item->GetEnchantments();

    m_enchantmentList.DeleteAllItems();
    int i = 0;
    for (auto enchantment : enchantments.GetItems()) {
        auto& name = nowide::widen(enchantment->GetName());
        CString multiplier;
        multiplier.Format(_T("%.2f"), enchantment->Multiplier);

        m_enchantmentList.AddItem(i, 0, name.c_str());
        m_enchantmentList.SetItemData(i, reinterpret_cast<DWORD_PTR>(enchantment.get()));
        m_enchantmentList.SetItemText(i, 1, static_cast<LPCTSTR>(multiplier));
        ++i;
    }
}

void StorageItemsTabPage::syncEnchantmentList()
{
    m_enchantmentListSelectedItems.clear();
    int firstSelection = m_enchantmentList.GetSelectionMark();
    if (firstSelection == -1) {
        return;
    }

    for (int i = 0, count = m_enchantmentList.GetSelectedCount(); i < count; ++i) {
        if (m_enchantmentList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
            m_enchantmentListSelectedItems.push_back(i);
        }
    }
}
