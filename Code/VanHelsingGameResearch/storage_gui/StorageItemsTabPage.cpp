#include "stdafx.h"
#include "StorageItemsTabPage.h"
#include <nowide/convert.hpp>
#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/GameData.h>
#include <sstream>
#include <boost/bind.hpp>

StorageItemsTabPage::StorageItemsTabPage()
    : m_itemListSkipChangeNotification(false),
    m_enchantmentListSkipChangeNotification(false) {}

LRESULT StorageItemsTabPage::onInitDialog(HWND, LPARAM)
{
    // List-view selection change notification
    WM_ITEM_LVSELCHANGE = ::RegisterWindowMessage(_T("WM_ITEM_LVSELCHANGE"));
    WM_ENCHANTMENT_LVSELCHANGE = ::RegisterWindowMessage(_T("WM_ENCHANTMENT_LVSELCHANGE"));

    // Subclass controls before DoDataExchange
    // Items
    m_itemAttribute1.SubclassWindow(GetDlgItem(IDC_ITEM_ATTRIBUTE1_EDIT));
    m_itemAttribute1.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    m_itemAttribute2.SubclassWindow(GetDlgItem(IDC_ITEM_ATTRIBUTE2_EDIT));
    m_itemAttribute2.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    m_itemQuantity.SubclassWindow(GetDlgItem(IDC_ITEM_QUANTITY_EDIT));
    m_itemQuantity.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    // Enchantments
    m_enchantmentMultiplier.SubclassWindow(GetDlgItem(IDC_ENCHANTMENT_MULTIPLIER_EDIT));
    m_enchantmentMultiplier.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));

    DoDataExchange(false);
    resize_class_t::DlgResize_Init(false, false, WS_CLIPCHILDREN);

    // Setup the rest of the UI

    // Items
    m_itemList.AddColumn(_T("Slot #"), 0);
    m_itemList.AddColumn(_T("Name"), 1);
    m_itemList.AddColumn(_T("Attribute 1"), 2);
    m_itemList.AddColumn(_T("Attribute 2"), 3);
    m_itemList.AddColumn(_T("Rarity"), 4);
    m_itemList.AddColumn(_T("Quality"), 5);
    m_itemList.AddColumn(_T("Quantity"), 6);
    m_itemList.AddColumn(_T("Identified?"), 7);
    m_itemList.SetExtendedListViewStyle(m_itemList.GetExtendedListViewStyle() | (LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER));
    ::SetWindowTheme(m_itemList.m_hWnd, L"Explorer", NULL);

    using vanhelsing::engine::GameData;
    auto& gameData = GameData::Get();
    auto& textManager = gameData.GetTextManager();

    // Modify item
    for (auto rarity : gameData.GetRarityDataList()) {
        m_itemRarity.AddString(nowide::widen(textManager.GetRarityText(rarity)).c_str());
    }

    for (auto quality : gameData.GetQualityDataList()) {
        m_itemQuality.AddString(nowide::widen(textManager.GetQualityText(quality)).c_str());
    }

    // Enchantments
    m_enchantmentList.AddColumn(_T("Name"), 0);
    m_enchantmentList.AddColumn(_T("Multiplier"), 1);
    m_enchantmentList.SetExtendedListViewStyle(m_enchantmentList.GetExtendedListViewStyle() | (LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER));
    ::SetWindowTheme(m_enchantmentList.m_hWnd, L"Explorer", NULL);

    // Modify enchantment

    return 0;
}

void StorageItemsTabPage::OnOpenGameSave(vanhelsing::engine::StorageGameSave* gameSave)
{
    m_gameSave = gameSave;
    updateItemSection();
}

LRESULT StorageItemsTabPage::OnClick(int, LPNMHDR pnmh, BOOL&)
{
    return 0;
}

LRESULT StorageItemsTabPage::OnItemChanged(LPNMHDR lpHdr)
{
    auto pia = reinterpret_cast<LPNMITEMACTIVATE>(lpHdr);

    if (m_itemListSkipChangeNotification) {
        return 0;
    }

    // Make sure selection changed
    if ((pia->uChanged != 0) && (pia->uChanged & LVIF_STATE)) {
        if ((pia->uOldState & LVIS_SELECTED) || (pia->uNewState & LVIS_SELECTED)) {
            // Selection has changed
            m_itemListSkipChangeNotification = true;
            ::PostMessage(*this, WM_ITEM_LVSELCHANGE, 0, 0);
            return 0;
        }
    }

    return 0;
}

LRESULT StorageItemsTabPage::OnEnchantmentChanged(LPNMHDR lpHdr)
{
    auto pia = reinterpret_cast<LPNMITEMACTIVATE>(lpHdr);

    if (m_enchantmentListSkipChangeNotification) {
        return 0;
    }

    // Make sure selection changed
    if ((pia->uChanged != 0) && (pia->uChanged & LVIF_STATE)) {
        if ((pia->uOldState & LVIS_SELECTED) || (pia->uNewState & LVIS_SELECTED)) {
            // Selection has changed
            m_enchantmentListSkipChangeNotification = true;
            ::PostMessage(*this, WM_ENCHANTMENT_LVSELCHANGE, 0, 0);
            return 0;
        }
    }

    return 0;
}

LRESULT StorageItemsTabPage::OnItemSelectionChange(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    m_itemListSkipChangeNotification = false;
    syncItemList();
    return 0;
}

LRESULT StorageItemsTabPage::OnEnchantmentSelectionChange(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    m_enchantmentListSkipChangeNotification = false;
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
    if (m_itemList.GetSelectionMark() == -1) {
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

    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;
    auto firstSelection = m_itemListSelectedItems[0];
    auto& textManager = GameData::Get().GetTextManager();
    auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(firstSelection));
    updateModifyItemSection(textManager, item);
    updateEnchantmentSection(textManager, item);
}

void StorageItemsTabPage::syncEnchantmentList()
{
    m_enchantmentListSelectedItems.clear();
    if (m_enchantmentList.GetSelectionMark() == -1) {
        return;
    }

    // Find selected items
    for (int i = 0, count = m_enchantmentList.GetItemCount(); i < count; ++i) {
        if (m_enchantmentList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) {
            m_enchantmentListSelectedItems.push_back(i);
        }
    }

    // Not a single selection?
    if (m_enchantmentListSelectedItems.size() != 1) {
        return;
    }

    // Single selection

    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;
    auto firstSelection = m_enchantmentListSelectedItems[0];
    auto& textManager = GameData::Get().GetTextManager();
    auto item = reinterpret_cast<Enchantment*>(m_enchantmentList.GetItemData(firstSelection));
    updateModifyEnchantmentSection(textManager, item);
}

LRESULT StorageItemsTabPage::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    using namespace vanhelsing::engine::inventory;


    /*
    
    for (auto i : m_enchantmentListSelectedItems) {
        auto item = reinterpret_cast<vanhelsing::engine::inventory::Enchantment*>(m_enchantmentList.GetItemData(i));

        TCHAR text[100];
        // Multiplier
        {
            ::GetDlgItemText(*this, IDC_ENCHANTMENT_MULTIPLIER_EDIT, text, _countof(text));
            std::wstringstream ss(text);
            ss >> item->Multiplier;
        }
    }*/

    return 0;
}

void StorageItemsTabPage::updateItemListItem(int i)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    auto& textManager = GameData::Get().GetTextManager();
    auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));

    auto& name = nowide::widen(textManager.GetItemText(item->GetName()));
    CString slotNumber;
    slotNumber.Format(_T("%u"), item->SlotNumber);
    CString attribute1;
    attribute1.Format(_T("%d"), item->Attribute1);
    CString attribute2;
    attribute2.Format(_T("%d"), item->Attribute2);
    auto& rarity = nowide::widen(textManager.GetRarityText(item->Rarity));
    auto& quality = nowide::widen(textManager.GetQualityText(item->Quality));
    CString quantity;
    quantity.Format(_T("%d"), item->Quantity);
    CString identified(item->IsIdentified ? _T("Yes") : _T("No"));

    m_itemList.SetItemText(i, 0, static_cast<LPCTSTR>(slotNumber));
    m_itemList.SetItemText(i, 1, name.c_str());
    m_itemList.SetItemText(i, 2, static_cast<LPCTSTR>(attribute1));
    m_itemList.SetItemText(i, 3, static_cast<LPCTSTR>(attribute2));
    m_itemList.SetItemText(i, 4, rarity.c_str());
    m_itemList.SetItemText(i, 5, quality.c_str());
    m_itemList.SetItemText(i, 6, static_cast<LPCTSTR>(quantity));
    m_itemList.SetItemText(i, 7, static_cast<LPCTSTR>(identified));
}

void StorageItemsTabPage::updateModifyItemSection(
    const vanhelsing::engine::TextManager& textManager,
    const vanhelsing::engine::inventory::Item* item)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    // Attribute 1
    {
        std::wstringstream ss;
        ss << item->Attribute1;
        m_itemAttribute1.SetValue(ss.str().c_str());
    }
    // Attribute 2
    {
        std::wstringstream ss;
        ss << item->Attribute2;
        m_itemAttribute2.SetValue(ss.str().c_str());
    }
    // Rarity
    m_itemRarity.SetCurSel(Item::Rarity::IsValid(item->Rarity) ? item->Rarity : -1);
    // Quality
    m_itemQuality.SetCurSel(Item::Quality::IsValid(item->Quality) ? item->Quality : -1);
    // Identified
    m_itemIsIdentified.SetCheck(item->IsIdentified ? BST_CHECKED : BST_UNCHECKED);
    // Quantity
    {
        std::wstringstream ss;
        ss << item->Quantity;
        m_itemQuantity.SetValue(ss.str().c_str());
    }
}

void StorageItemsTabPage::updateEnchantmentSection(
    const vanhelsing::engine::TextManager& textManager,
    const vanhelsing::engine::inventory::Item* item)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;
    auto& enchantments = item->GetEnchantments();
    m_enchantmentList.DeleteAllItems();
    int i = 0;
    for (auto enchantment : enchantments.GetItems()) {
        m_enchantmentList.AddItem(i, 0, nullptr);
        m_enchantmentList.SetItemData(i, reinterpret_cast<DWORD_PTR>(enchantment.get()));
        updateEnchantmentListItem(i);
        ++i;
    }
}

void StorageItemsTabPage::editOnApply(CMyEdit::ApplyEventArg& e)
{
    auto obj = e.GetEventObject();
    // Items
    if (obj == &m_itemAttribute1) {
        applyItemAttribute1();
    }
    else if (obj == &m_itemAttribute2) {
        applyItemAttribute2();
    }
    else if (obj == &m_itemQuantity) {
        applyItemQuantity();
    }
    // Enchantments
    else if (obj == &m_enchantmentMultiplier) {
        applyEnchantmentMultiplier();
    }
}

void StorageItemsTabPage::applyItemAttribute1()
{
    using vanhelsing::engine::inventory::Item;
    TCHAR text[100];
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ITEM_ATTRIBUTE1_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> item->Attribute1;
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemAttribute2()
{
    using vanhelsing::engine::inventory::Item;
    TCHAR text[100];
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ITEM_ATTRIBUTE2_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> item->Attribute2;
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemRarity()
{
    using vanhelsing::engine::inventory::Item;
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));
        item->Rarity = static_cast<Item::Rarity::type>(m_itemRarity.GetCurSel());
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemQuality()
{
    using vanhelsing::engine::inventory::Item;
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));
        item->Quality = static_cast<Item::Quality::type>(m_itemQuality.GetCurSel());
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemIsIdentified()
{
    using vanhelsing::engine::inventory::Item;
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));
        item->IsIdentified = (m_itemIsIdentified.GetCheck() == BST_CHECKED);
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemQuantity()
{
    using vanhelsing::engine::inventory::Item;
    TCHAR text[100];
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Item*>(m_itemList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ITEM_QUANTITY_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> item->Quantity;
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::comboOnSelectionChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    switch (nID) {
    case IDC_ITEM_RARITY_COMBO:
        applyItemRarity();
        break;
        
    case IDC_ITEM_QUALITY_COMBO:
        applyItemQuality();
        break;

    default:
        ATLASSERT(!"Shouldn't happen");
        break;
    }
}

void StorageItemsTabPage::buttonOnClicked(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    switch (nID) {
    case IDC_ITEM_IDENTIFIED_CHECK:
        applyItemIsIdentified();
        break;

    default:
        ATLASSERT(!"Shouldn't happen");
        throw std::logic_error("Shouldn't happen");
    }
}

void StorageItemsTabPage::updateEnchantmentListItem(int i)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    auto enchantment = reinterpret_cast<Enchantment*>(m_enchantmentList.GetItemData(i));

    auto& name = nowide::widen(enchantment->GetName());
    CString multiplier;
    multiplier.Format(_T("%.2f"), enchantment->Multiplier);

    m_enchantmentList.SetItemText(i, 0, name.c_str());
    m_enchantmentList.SetItemText(i, 1, static_cast<LPCTSTR>(multiplier));
}

void StorageItemsTabPage::applyEnchantmentMultiplier()
{
    using vanhelsing::engine::inventory::Enchantment;
    TCHAR text[100];
    for (auto& i : m_enchantmentListSelectedItems) {
        auto enchantment = reinterpret_cast<Enchantment*>(m_enchantmentList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ENCHANTMENT_MULTIPLIER_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> enchantment->Multiplier;
        updateEnchantmentListItem(i);
    }
}

void StorageItemsTabPage::updateModifyEnchantmentSection(
    const vanhelsing::engine::TextManager& textManager,
    const vanhelsing::engine::inventory::Enchantment* item)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    // Multiplier
    {
        std::wstringstream ss;
        ss << item->Multiplier;
        m_enchantmentMultiplier.SetValue(ss.str().c_str());
    }
}

void StorageItemsTabPage::updateItemSection()
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    std::vector<Item*> items;
    m_gameSave->GetItems().FindByBagNumber(m_bagNumber, items);
    auto& textManager = GameData::Get().GetTextManager();

    m_itemList.DeleteAllItems();
    int i = 0;
    for (auto item : items) {
        m_itemList.AddItem(i, 0, nullptr);
        m_itemList.SetItemData(i, reinterpret_cast<DWORD_PTR>(item));
        updateItemListItem(i);
        ++i;
    }
}
