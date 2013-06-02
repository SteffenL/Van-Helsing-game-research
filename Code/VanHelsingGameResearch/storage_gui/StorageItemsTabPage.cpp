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

    resize_class_t::DlgResize_Init(false, true, WS_CLIPCHILDREN);	
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

    std::vector<Artifact*> items;
    m_gameSave->GetItems().FindByBagNumber(m_bagNumber, items);

    auto& textManager = GameData::Get().GetTextManager();

    m_itemList.DeleteAllItems();
    int i = 0;
    for (auto item : items) {
        auto& name = nowide::widen(textManager.GetArtifactText(item->GetName()));
        CString slotNumber;
        slotNumber.Format(_T("%d"), item->SlotNumber);
        CString attribute1;
        attribute1.Format(_T("%d"), item->Attribute1);
        CString attribute2;
        attribute2.Format(_T("%d"), item->Attribute2);
        auto& rarity = nowide::widen(textManager.GetRarityText(item->Rarity));
        auto& quality = nowide::widen(textManager.GetQualityText(item->Quality));
        CString identified(item->IsIdentified ? _T("Yes") : _T("No"));

        m_itemList.AddItem(i, 0, slotNumber);
        m_itemList.SetItemText(i, 1, name.c_str());
        m_itemList.SetItemText(i, 2, attribute1);
        m_itemList.SetItemText(i, 3, attribute2);
        m_itemList.SetItemText(i, 4, rarity.c_str());
        m_itemList.SetItemText(i, 5, quality.c_str());
        m_itemList.SetItemText(i, 6, identified);
        ++i;
    }
}

void StorageItemsTabPage::SetBagNumber(int bagNumber) { m_bagNumber = bagNumber; }
