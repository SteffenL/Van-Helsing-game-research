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
    m_itemProperty1.SubclassWindow(GetDlgItem(IDC_ITEM_ATTRIBUTE1_EDIT));
    m_itemProperty1.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    m_itemProperty2.SubclassWindow(GetDlgItem(IDC_ITEM_ATTRIBUTE2_EDIT));
    m_itemProperty2.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    m_itemQuantity.SubclassWindow(GetDlgItem(IDC_ITEM_QUANTITY_EDIT));
    m_itemQuantity.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    // Enchantments
    m_enchantmentEffectValue.SubclassWindow(GetDlgItem(IDC_ENCHANTMENT_EFFECT_VALUE_EDIT));
    m_enchantmentEffectValue.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));
    m_enchantmentEffectModifier.SubclassWindow(GetDlgItem(IDC_ENCHANTMENT_EFFECT_MODIFIER_EDIT));
    m_enchantmentEffectModifier.ApplyEvent->connect(boost::bind(&StorageItemsTabPage::editOnApply, this, _1));

    DoDataExchange(false);
    resize_class_t::DlgResize_Init(false, false, WS_CLIPCHILDREN);

    // Setup the rest of the UI

    // Items
    m_itemList.AddColumn(_T("Slot #"), 0);
    m_itemList.AddColumn(_T("Name"), 1);
    m_itemList.AddColumn(_T("Property 1"), 2);
    m_itemList.AddColumn(_T("Property 2"), 3);
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
    m_enchantmentList.AddColumn(_T("Value"), 1);
    m_enchantmentList.AddColumn(_T("Modifier"), 2);
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
    auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(firstSelection));
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
        // Modifier
        {
            ::GetDlgItemText(*this, IDC_ENCHANTMENT_EFFECT_MODIFIER_EDIT, text, _countof(text));
            std::wstringstream ss(text);
            ss >> item->Modifier;
        }
    }*/

    return 0;
}

void StorageItemsTabPage::updateItemListItem(int i)
{
#if 0
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    auto& textManager = GameData::Get().GetTextManager();
    auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));

    auto& name = nowide::widen(textManager.GetItemText(item->GetName()));
    CString slotNumber;
    slotNumber.Format(_T("%u"), item->SlotNumber);
    CString property1;
    property1.Format(_T("%d"), item->Property1);
    CString property2;
    property2.Format(_T("%d"), item->Property2);
    auto& rarity = nowide::widen(textManager.GetRarityText(item->Rarity));
    auto& quality = nowide::widen(textManager.GetQualityText(item->Quality));
    CString quantity;
    quantity.Format(_T("%d"), item->Quantity);
    CString identified(item->IsIdentified ? _T("Yes") : _T("No"));

    m_itemList.SetItemText(i, 0, static_cast<LPCTSTR>(slotNumber));
    m_itemList.SetItemText(i, 1, name.c_str());
    m_itemList.SetItemText(i, 2, static_cast<LPCTSTR>(property1));
    m_itemList.SetItemText(i, 3, static_cast<LPCTSTR>(property2));
    m_itemList.SetItemText(i, 4, rarity.c_str());
    m_itemList.SetItemText(i, 5, quality.c_str());
    m_itemList.SetItemText(i, 6, static_cast<LPCTSTR>(quantity));
    m_itemList.SetItemText(i, 7, static_cast<LPCTSTR>(identified));
#endif
}

void StorageItemsTabPage::updateModifyItemSection(
    const vanhelsing::engine::TextManager& textManager,
    const vanhelsing::engine::inventory::Artifact* item)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    // Property 1
    {
        std::wstringstream ss;
        ss << item->Property1;
        m_itemProperty1.SetValue(ss.str().c_str());
    }
    // Property 2
    {
        std::wstringstream ss;
        ss << item->Property2;
        m_itemProperty2.SetValue(ss.str().c_str());
    }
    // Rarity
    m_itemRarity.SetCurSel(Artifact::Rarity::IsValid(item->Rarity) ? item->Rarity : -1);
    // Quality
    m_itemQuality.SetCurSel(Artifact::Quality::IsValid(item->Quality) ? item->Quality : -1);
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
    const vanhelsing::engine::inventory::Artifact* item)
{
#if 0
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
#endif
}

void StorageItemsTabPage::editOnApply(CMyEdit::ApplyEventArg& e)
{
    auto obj = e.GetEventObject();
    // Items
    if (obj == &m_itemProperty1) {
        applyItemProperty1();
    }
    else if (obj == &m_itemProperty2) {
        applyItemProperty2();
    }
    else if (obj == &m_itemQuantity) {
        applyItemQuantity();
    }
    // Enchantments
    else if (obj == &m_enchantmentEffectValue) {
        applyEnchantmentEffectValue();
    }
    else if (obj == &m_enchantmentEffectModifier) {
        applyEnchantmentEffectModifier();
    }
}

void StorageItemsTabPage::applyItemProperty1()
{
    using vanhelsing::engine::inventory::Artifact;
    TCHAR text[100];
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ITEM_ATTRIBUTE1_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> item->Property1;
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemProperty2()
{
    using vanhelsing::engine::inventory::Artifact;
    TCHAR text[100];
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ITEM_ATTRIBUTE2_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> item->Property2;
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemRarity()
{
    using vanhelsing::engine::inventory::Artifact;
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));
        item->Rarity = static_cast<Artifact::Rarity::type>(m_itemRarity.GetCurSel());
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemQuality()
{
    using vanhelsing::engine::inventory::Artifact;
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));
        item->Quality = static_cast<Artifact::Quality::type>(m_itemQuality.GetCurSel());
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemIsIdentified()
{
    using vanhelsing::engine::inventory::Artifact;
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));
        item->IsIdentified = (m_itemIsIdentified.GetCheck() == BST_CHECKED);
        updateItemListItem(i);
    }
}

void StorageItemsTabPage::applyItemQuantity()
{
    using vanhelsing::engine::inventory::Artifact;
    TCHAR text[100];
    for (auto& i : m_itemListSelectedItems) {
        auto item = reinterpret_cast<Artifact*>(m_itemList.GetItemData(i));
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

    CString effectValue;
    effectValue.Format(_T("%d"), enchantment->EffectValue);

    CString effectModifier;
    effectModifier.Format(_T("%.2f"), enchantment->EffectModifier);

    m_enchantmentList.SetItemText(i, 0, name.c_str());
    m_enchantmentList.SetItemText(i, 1, static_cast<LPCTSTR>(effectValue));
    m_enchantmentList.SetItemText(i, 2, static_cast<LPCTSTR>(effectModifier));
}

void StorageItemsTabPage::applyEnchantmentEffectModifier()
{
    using vanhelsing::engine::inventory::Enchantment;
    TCHAR text[100];
    for (auto& i : m_enchantmentListSelectedItems) {
        auto enchantment = reinterpret_cast<Enchantment*>(m_enchantmentList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ENCHANTMENT_EFFECT_MODIFIER_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> enchantment->EffectModifier;
        updateEnchantmentListItem(i);
    }
}

void StorageItemsTabPage::applyEnchantmentEffectValue()
{
    using vanhelsing::engine::inventory::Enchantment;
    TCHAR text[100];
    for (auto& i : m_enchantmentListSelectedItems) {
        auto enchantment = reinterpret_cast<Enchantment*>(m_enchantmentList.GetItemData(i));
        ::GetDlgItemText(*this, IDC_ENCHANTMENT_EFFECT_VALUE_EDIT, text, _countof(text));
        std::wstringstream ss(text);
        ss >> enchantment->EffectValue;
        updateEnchantmentListItem(i);
    }
}

void StorageItemsTabPage::updateModifyEnchantmentSection(
    const vanhelsing::engine::TextManager& textManager,
    const vanhelsing::engine::inventory::Enchantment* item)
{
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    // Effect value
    {
        std::wstringstream ss;
        ss << item->EffectValue;
        m_enchantmentEffectValue.SetValue(ss.str().c_str());
    }

    // Effect modifier
    {
        std::wstringstream ss;
        ss << item->EffectModifier;
        m_enchantmentEffectModifier.SetValue(ss.str().c_str());
    }
}

void StorageItemsTabPage::updateItemSection()
{
#if 0
    using namespace vanhelsing::engine;
    using namespace vanhelsing::engine::inventory;

    std::vector<std::shared_ptr<Artifact>> items;
    m_gameSave->GetArtifacts1().FindByBagNumber(m_bagNumber, items);
    auto& textManager = GameData::Get().GetTextManager();

    m_itemList.DeleteAllItems();
    int i = 0;
    for (auto item : items) {
        m_itemList.AddItem(i, 0, nullptr);
        m_itemList.SetItemData(i, reinterpret_cast<DWORD_PTR>(item.get()));
        updateItemListItem(i);
        ++i;
    }
#endif
}

void StorageItemsTabPage::FullUpdate()
{
    m_enchantmentList.DeleteAllItems();
    m_enchantmentListSelectedItems.clear();
    m_itemList.DeleteAllItems();
    m_itemListSelectedItems.clear();
    updateItemSection();
}
