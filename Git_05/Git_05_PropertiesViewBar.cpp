// PropertiesViewBar.cpp : implementation of the Git05_CBCGPPropBar class
//

#include "stdafx.h"
#include "Git_05.h"
#include "Git_05_PropertiesViewBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Git05_CBCGPPropBar class

Git05_CBCGPPropBar::Git05_CBCGPPropBar()
{
	m_BorderColor = CBCGPColor::Red;
	m_FillBrush.SetColors(CBCGPColor::LightSteelBlue, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_RADIAL_TOP_LEFT, 0.75);
	m_TextBrush.SetColor(CBCGPColor::SteelBlue);
}

Git05_CBCGPPropBar::~Git05_CBCGPPropBar()
{
}

BEGIN_MESSAGE_MAP(Git05_CBCGPPropBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_COMMAND_CLICKED, OnCommandClicked)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_MENU_ITEM_SELECTED, OnMenuItemSelected)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void Git05_CBCGPPropBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width (), rectClient.Height (), SWP_NOACTIVATE | SWP_NOZORDER);
}

int Git05_CBCGPPropBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndPropList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

	InitPropList ();
	AdjustLayout ();

	return 0;
}

void Git05_CBCGPPropBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout ();
}

#include "GIT_Commit_Local.hpp"
void Git05_CBCGPPropBar::set_commit_info(const GIT_Commit_Local& commitInfo)
{
	commiter_group_uptr_->RemoveAllSubItems();
	auto name = commitInfo.commit_author.name;
	CA2W ca2w(name);
	std::wstring c_name = ca2w;
	CBCGPProp* pProp = new CBCGPProp(_T("Name"), c_name.c_str(), _T("Commiter's name"));
	pProp->AllowEdit(FALSE);
	commiter_group_uptr_->AddSubItem(pProp);
	auto email = commitInfo.commit_author.email;
	CA2W ca2email(email);
	std::wstring c_email = ca2email;
	pProp = new CBCGPProp(_T("Name"), c_email.c_str(), _T("Commiter's email"));
	pProp->AllowEdit(FALSE);
	commiter_group_uptr_->AddSubItem(pProp);

	auto message = commitInfo.commit_message;
	CA2W ca2message(message.c_str());
	std::wstring c_message = ca2message;
	pProp = new CBCGPProp(_T("Message"), c_message.c_str(),	_T("Long description of a commit"));
	pProp->AllowEdit(FALSE);
	commiter_group_uptr_->AddSubItem(pProp);

	m_wndPropList.AdjustLayout();

}

void Git05_CBCGPPropBar::InitPropList ()
{
	// Add commands:
	CStringList lstCommands;
	lstCommands.AddTail (_T("Command 1..."));
	lstCommands.AddTail (_T("Command 2..."));

	m_wndPropList.SetCommands (lstCommands);

	// Add custom menu items:
	CStringList lstCustomMenuItem;
	lstCustomMenuItem.AddTail (_T("Custom menu item 1"));
	lstCustomMenuItem.AddTail (_T("Custom menu item 2"));
	lstCustomMenuItem.AddTail (_T("Custom menu item 3"));

	m_wndPropList.SetCustomMenuItems(lstCustomMenuItem);

	// Setup general look:
	m_wndPropList.EnableToolBar();
	m_wndPropList.EnableSearchBox();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.EnableContextMenu();

	// Add properties:
	commiter_group_uptr_ = std::make_unique<CBCGPProp> (_T("Commiter"));
	m_wndPropList.AddProperty(commiter_group_uptr_.get());
	//CBCGPProp* pProp = new CBCGPProp(_T("Name"), _T("artie_fuffkin"),
	//				_T("Commiter's name"));
	//pProp->AllowEdit(FALSE);
	//
	//pCommiterGroup->AddSubItem(pProp);

	////pProp = new CBCGPProp (_T("Email"), _T("email@emaill"),
		//_T("Commiter's email"));
	//pProp->AddOption (_T("None"));
	//pProp->AddOption (_T("Thin"));
	//pProp->AddOption (_T("Resizable"));
	//pProp->AddOption (_T("Dialog Frame"));
	//pProp->AllowEdit (FALSE);

	//pCommiterGroup->AddSubItem (pProp);
	
	//pProp = new CBCGPProp(_T("Message"), (_variant_t)_T("Initial commit"),
	//	_T("Long description of a commit"));
	//pProp->AllowEdit(FALSE);
	//pCommiterGroup->AddSubItem(pProp);
	//
	//COleDateTime date = COleDateTime::GetCurrentTime();
	//pProp = new CBCGPProp(_T("Date"),(_variant_t)date,
	//		_T("Commit's date"));
	//pProp->AllowEdit(FALSE);
	//pCommiterGroup->AddSubItem(pProp);
	//
	//m_wndPropList.AddProperty (pCommiterGroup);


	//////Commit's stats////
	CBCGPProp* pStatsGroup = new CBCGPProp(_T("Statistics"));
	CBCGPProp* pModificationsGroup = new CBCGPProp(_T("Modifications"), 0, TRUE);
	pModificationsGroup->AllowEdit(FALSE);
	std::vector<CBCGPProp*> vecModifications;
	CBCGPProp* pStatsModificationProp;
	std::vector<CBCGPProp*> vec;
	for (int i{0}; i < 14; ++i)
	{
 		pStatsModificationProp = new CBCGPProp(_T("File"), (_variant_t)i,
			_T("Modified file"));
		pStatsModificationProp->AllowEdit(FALSE);
		vec.push_back(pStatsModificationProp);
		pModificationsGroup->AddSubItem(pStatsModificationProp);

		//vecModifications.push_back(pStatsModificationProp);
	}
	//m_wndPropList.insertGroup(pModificationsGroup,vec);
	//pModificationsGroup->add_real_list(list);
	//pModificationsGroup->SetData(14);
	//for (const auto prop : vecModifications)
	//{
	//	pModificationsGroup->AddSubItem(pStatsModificationProp);
	//}


	pStatsGroup->AddSubItem(pModificationsGroup);

// 	CBCGPProp* pStatsProp = new CBCGPProp(_T("Additions"), _T("201"),
// 		_T("Added files in this commit"));
// 	pStatsProp->AllowEdit(FALSE);
// 
// 	pStatsGroup->AddSubItem(pStatsProp);
// 	pStatsProp = new CBCGPProp(_T("Deletions"), _T("7"),
// 		_T("Deleted files in this commit"));
// 	pStatsProp->AllowEdit(FALSE);
// 	
// 	pStatsGroup->AddSubItem(pStatsProp);
// 	
	m_wndPropList.AddProperty(pStatsGroup);
	///////SHA///////////
	CBCGPProp* pSHAGroup = new CBCGPProp(_T("SHA"));
	CBCGPProp* pSHAProp = new CBCGPProp(_T("Short version"), _T("d58654a"),
		_T("Short version of SHA"));
	pSHAProp->AllowEdit(FALSE);

	pSHAGroup->AddSubItem(pSHAProp);
	pSHAProp = new CBCGPProp(_T("Long version"), _T("65a6s5df65654asd6f6465a6s5df65654asd6f64"),
		_T("Long version of SHA"));
	pSHAProp->AllowEdit(FALSE);

	pSHAGroup->AddSubItem(pSHAProp);
	m_wndPropList.AddProperty(pSHAGroup);
// 	CBCGPProp* pSize = new CBCGPProp (_T("Window Size"), 0, TRUE);
// 
// 	pProp = new CBCGPProp (_T("Height"), (_variant_t) 250l,
// 							_T("Specifies the dialog's height"));
// 	pProp->EnableSpinControl (TRUE, 0, 1000);
// 	pSize->AddSubItem (pProp);
// 
// 	pProp = new CBCGPProp (	_T("Width"), (_variant_t) 150l,
// 							_T("Specifies the dialog's width"));
// 	pProp->EnableSpinControl (TRUE, 0, 1000);
// 	pSize->AddSubItem (pProp);
// 
// 	m_wndPropList.AddProperty (pSize);
// 
// 	CBCGPProp* pGroup2 = new CBCGPProp (_T("Font"));
// 
// 	LOGFONT lf;
// 	CFont* font = CFont::FromHandle ((HFONT) GetStockObject (DEFAULT_GUI_FONT));
// 	font->GetLogFont (&lf);
// 
// 	lstrcpy (lf.lfFaceName, _T("Arial"));
// 
// 	pGroup2->AddSubItem (new CBCGPFontProp (_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the dialog")));
// 	pGroup2->AddSubItem (new CBCGPProp (_T("Use System Font"), (_variant_t) true, _T("Specifies that the dialog uses MS Shell Dlg font")));
// 
// 	m_wndPropList.AddProperty (pGroup2);
// 
// 	CBCGPProp* pGroup3 = new CBCGPProp (_T("Brushes"));
// 
// 	CBCGPColorProp* pColorProp = new CBCGPColorProp (_T("Border Color"), m_BorderColor, NULL, _T("Specifies the default dialog border color"));
// 	pColorProp->EnableOtherButton (_T("Other..."));
// 	pColorProp->EnableAutomaticButton (_T("Default"), globalData.clrBarShadow);
// 	pGroup3->AddSubItem (pColorProp);
// 
// 	pGroup3->AddSubItem (new CBCGPBrushProp (_T("Background"), m_FillBrush, NULL,
// 		_T("Specifies the default background")));
// 
// 	pGroup3->AddSubItem (new CBCGPBrushProp (_T("Foreground"), m_TextBrush, NULL,
// 		_T("Specifies the default foreground")));
// 
// 	m_wndPropList.AddProperty (pGroup3);
// 
// 	CBCGPProp* pGroup4 = new CBCGPProp (_T("Misc"));
// 	pProp = new CBCGPProp (_T("(Name)"), _T("IDD_ABOUT_BOX (dialog)"));
// 	pProp->Enable (FALSE);
// 	pGroup4->AddSubItem (pProp);
// 
// 	static TCHAR BASED_CODE szFilter[] = _T("Icon Files (*.ico)|*.ico|All Files (*.*)|*.*||");
// 	pGroup4->AddSubItem (new CBCGPFileProp (_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the dialog icon")));
// 
// 	pGroup4->AddSubItem (new CBCGPFileProp (_T("Folder"), _T("c:\\")));
// 
// // 	COleDateTime date = COleDateTime::GetCurrentTime ();
// // 	pGroup4->AddSubItem (new CBCGPDateTimeProp (_T("Date"), date,
// // 		_T("Set a date"), 0, CBCGPDateTimeCtrl::DTM_DATE));
// 
// 	pGroup4->AddSubItem (new CBCGPDateTimeProp (_T("Time"), date,
// 		_T("Set a time"), 0, CBCGPDateTimeCtrl::DTM_TIME));
// 
// 	m_wndPropList.AddProperty (pGroup4);
// 
// 	CBCGPProp* pGroup5 = new CBCGPProp (_T("Hierarchy"));
// 
// 	CBCGPProp* pGroup51 = new CBCGPProp (_T("First sub-level"));
// 	pGroup5->AddSubItem (pGroup51);
// 
// 	CBCGPProp* pGroup511 = new CBCGPProp (_T("Second sub-level"));
// 	pGroup51->AddSubItem (pGroup511);
// 
// 	pGroup511->AddSubItem (new CBCGPProp (_T("Item 1"), (_variant_t) _T("Value 1"),
// 		_T("This is a description")));
// 	pGroup511->AddSubItem (new CBCGPProp (_T("Item 2"), (_variant_t) _T("Value 2"),
// 		_T("This is a description")));
// 	pGroup511->AddSubItem (new CBCGPProp (_T("Item 3"), (_variant_t) _T("Value 3"),
// 		_T("This is a description")));
// 
// 	pGroup5->Expand (FALSE);
// 	m_wndPropList.AddProperty (pGroup5);
}

void Git05_CBCGPPropBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus ();
}

LRESULT Git05_CBCGPPropBar::OnCommandClicked(WPARAM, LPARAM lp)
{
//	int nCommandIndex = (int)lp;

	// TODO: Add your command handler code here
	
	return 0;
}

LRESULT Git05_CBCGPPropBar::OnMenuItemSelected(WPARAM /*wp*/, LPARAM /*lp*/)
{
//	int nMenuIndex = (int)wp;

//	CBCGPProp* pProp = (CBCGPProp*)lp;
//	ASSERT_VALID(pProp);

	// TODO: Add your menu item processing code here

	return 0;
}
