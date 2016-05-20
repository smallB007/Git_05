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
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_GET_MENU_ITEM_STATE, OnPropertyGetMenuItemState)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTYLIST_PROPERTY_SELECTED, OnPropertySelected)

	//ON_WM_LBUTTONUP()
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

void Git05_CBCGPPropBar::assign_diffed_files_(const GIT_Commit_Local& commitInfo)
{

	CBCGPProp* pStatsModificationProp;
	CBCGPProp* pStatsAdditionProp;
	CBCGPProp* pStatsDeletionProp;

	for (const auto& file : commitInfo.diffed_files)
	{
		//
		std::string afile = file.diff_delta.new_file.path;
		CA2W w_str_path(afile.c_str());
		CString file_path = w_str_path;

		switch (file.diff_delta.status)
		{
		case GIT_DELTA_MODIFIED:
		{
			pStatsModificationProp = new CBCGPProp(file_path, (_variant_t)0,
				_T("Modified file"));
			pStatsModificationProp->AllowEdit(FALSE);
			pModificationsGroup->AddSubItem(pStatsModificationProp);
		}
		break;
		
		case GIT_DELTA_ADDED:
		{
			pStatsAdditionProp = new CBCGPProp(file_path, (_variant_t)0,
						_T("Added file"));
					pStatsAdditionProp->AllowEdit(FALSE);
					pAdditionsGroup->AddSubItem(pStatsAdditionProp);
		}
		break;

		case GIT_DELTA_DELETED:
		{
			pStatsDeletionProp = new CBCGPProp(file_path, (_variant_t)0,
						_T("Deleted file"));
					pStatsDeletionProp->AllowEdit(FALSE);
					pDeletionsGroup->AddSubItem(pStatsDeletionProp);
		}
		break;

		}
		
	}

}

void Git05_CBCGPPropBar::set_commit_info(const GIT_Commit_Local& commitInfo)
{
	//m_wndPropList.RemoveAll();
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
	pProp = new CBCGPProp(_T("Email"), c_email.c_str(), _T("Commiter's email"));
	pProp->AllowEdit(FALSE);
	commiter_group_uptr_->AddSubItem(pProp);
	//
	auto commit_date = ctime(&(time_t)commitInfo.commit_author.when.time);
	CA2W ca2commit_date(commit_date);
	std::wstring c_commit_date = ca2commit_date;
	pProp = new CBCGPProp(_T("commit_date"), c_commit_date.c_str(), _T("The date this commit was made"));
	pProp->AllowEdit(FALSE);
	commiter_group_uptr_->AddSubItem(pProp);
	//

	auto message = commitInfo.commit_message;
	CA2W ca2message(message.c_str());
	std::wstring c_message = ca2message;
	pProp = new CBCGPProp(_T("Message"), c_message.c_str(),	_T("Long description of a commit"));
	pProp->AllowEdit(FALSE);
	commiter_group_uptr_->AddSubItem(pProp);

	//STATS//
	pModificationsGroup->RemoveAllSubItems();
	pAdditionsGroup->RemoveAllSubItems();
	pDeletionsGroup->RemoveAllSubItems();
	assign_diffed_files_(commitInfo);
	//CBCGPProp* pStatsModificationProp;
	//for (const auto& modified_file : commitInfo.files_modified)
	//{
	//	CA2W w_str_modified_path(modified_file.path);
	//	CString modified_file_path = w_str_modified_path;
	//		pStatsModificationProp = new CBCGPProp(modified_file_path, (_variant_t)0,
	//		_T("Modified file"));
	//	pStatsModificationProp->AllowEdit(FALSE);
	//	pModificationsGroup->AddSubItem(pStatsModificationProp);
	//
	//	
	//}
	//CBCGPProp* pStatsAdditionProp;
	//for (const auto& added_file : commitInfo.files_added)
	//{
	//	CA2W w_str_added_path(added_file.path);
	//	CString added_file_path = w_str_added_path;
	//	pStatsAdditionProp = new CBCGPProp(added_file_path, (_variant_t)0,
	//		_T("Added file"));
	//	pStatsAdditionProp->AllowEdit(FALSE);
	//	pAdditionsGroup->AddSubItem(pStatsAdditionProp);
	//}

	//CBCGPProp* pStatsDeletionProp;
	//for (const auto& deleted_file : commitInfo.files_deleted)
	//{
	//	CA2W w_str_deleted_path(deleted_file.path);
	//	CString deleted_file_path = w_str_deleted_path;
	//	pStatsDeletionProp = new CBCGPProp(deleted_file_path, (_variant_t)0,
	//		_T("Deleted file"));
	//	pStatsDeletionProp->AllowEdit(FALSE);
	//	pDeletionsGroup->AddSubItem(pStatsDeletionProp);
	//}

	pSHAGroup->RemoveAllSubItems();
	CBCGPProp* pSHAProp;
	
	CString w_str_sha_short(commitInfo.commit_id.substr(0, 7).c_str());
	CString w_str_sha_long(commitInfo.commit_id.c_str());


	pSHAProp = new CBCGPProp(_T("Short version"), w_str_sha_short.GetString(),
		_T("Short version of SHA"));
	pSHAProp->AllowEdit(FALSE);

	pSHAGroup->AddSubItem(pSHAProp);
	pSHAProp = new CBCGPProp(_T("Long version"), w_str_sha_long.GetString(),
		_T("Long version of SHA"));
	pSHAProp->AllowEdit(FALSE);

	pSHAGroup->AddSubItem(pSHAProp);
	pSHAGroup->Expand(FALSE);
	//pSHAProp->Expand(FALSE);
	////
	////////////SHA//////////////////////////////////////////////////////////////
	
		////	
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
	pStatsGroup = std::make_unique<CBCGPProp>(_T("Statistics"));
	pModificationsGroup = std::make_shared<CBCGPProp>(_T("Modifications"), 0, TRUE); 
	pModificationsGroup->AllowEdit(FALSE);
	pStatsGroup->AddSubItem(pModificationsGroup.get());
	pAdditionsGroup = std::make_shared<CBCGPProp>(_T("Additions"), 0, TRUE);
	pStatsGroup->AddSubItem(pAdditionsGroup.get());
	pDeletionsGroup = std::make_shared<CBCGPProp>(_T("Deletions"), 0, TRUE);
	pStatsGroup->AddSubItem(pDeletionsGroup.get());
	m_wndPropList.AddProperty(pStatsGroup.get());

	///////SHA///////////
	pSHAGroup = std::make_unique<CBCGPProp>(_T("SHA"));
	m_wndPropList.AddProperty(pSHAGroup.get());
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

LRESULT Git05_CBCGPPropBar::OnPropertyGetMenuItemState(WPARAM /*wp*/, LPARAM /*lp*/)
{
	//	int nMenuIndex = (int)wp;

	//	CBCGPProp* pProp = (CBCGPProp*)lp;
	//	ASSERT_VALID(pProp);

	// TODO: Add your menu item processing code here

	return 0;
}

#include "MainFrm.h"
LRESULT Git05_CBCGPPropBar::OnPropertySelected(WPARAM /*wp*/, LPARAM lp)
{
		CBCGPProp* pProp = (CBCGPProp*)lp;
		ASSERT_VALID(pProp);
		auto parent = pProp->GetParent();
		if (parent)
		{
			CBCGPProp* group = parent->GetParent();

			if (group)
			{
				CString group_name = group->GetName();

				if (group_name == L"Statistics")
				{
					
					CBCGPProp* shaGroup = m_wndPropList.GetProperty(2);
					CBCGPProp* shaProp = shaGroup->GetSubItem(1);
					CString sha = shaProp->GetValue();
					CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
					CString file = pProp->GetName();
					pMainWnd->display_info_for_diffed_file(sha, file);
				}
			}
		}
		return 0;
}


void Git05_CBCGPPropBar::deselect_file_from_commit_detail_window()
{
	//This was written in order to prevent crash when one file was selected in commit details option and user selected different commit
	//CBCGPProp* cur = m_wndPropList.GetCurSel()
	CBCGPProp* top = m_wndPropList.GetProperty(0);
	m_wndPropList.SetCurSel(top);
	
}

// void Git05_CBCGPPropBar::OnLButtonUp(UINT nFlags, CPoint point)
// {
// 	// TODO: Add your message handler code here and/or call default
// 
// 	CBCGPDockingControlBar::OnLButtonUp(nFlags, point);
// 	auto prop = m_wndPropList.GetCurSel();
// }
