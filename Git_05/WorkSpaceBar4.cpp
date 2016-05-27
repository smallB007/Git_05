// workspace.cpp : implementation of the CWorkSpaceBar4 class
//

#include "stdafx.h"
#include "Git_05.h"
#include "MainFrm.h"
#include "WorkSpaceBar4.h"
#include "GIT_Commit_Local.hpp"
#include "Git_05_ListCtr.hpp"
#include "GIT_Engine.hpp"
#include <algorithm>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 0;
//#define IDC_LIST_CTRL 1
/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4

BEGIN_MESSAGE_MAP(CWorkSpaceBar4, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
#ifdef MFC_DIRECT_2D
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CWorkSpaceBar4::OnDrawDirect2D)
#else
	ON_WM_PAINT()
#endif
//	ON_WM_LBUTTONDOWN()

ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

// void CWorkSpaceBar4::OnLButtonDown(UINT flag, CPoint point)
// {
// 	int a{ 0 };
// }

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4 construction/destruction
// CGit_05App* CWorkSpaceBar4::get_main_app_()const
// {
// 	return static_cast<CGit_05App*>(AfxGetApp());
// };//copy from BackStagePageInfo.h

CWorkSpaceBar4::CWorkSpaceBar4() :working_dir_{L""}
{
	// TODO: add one-time construction code here
	//EnableD2DSupport();
}

CWorkSpaceBar4::~CWorkSpaceBar4()
{
	
}

void CWorkSpaceBar4::set_view_type(EVIEW_TYPE view_type)
{
	eview_type_ = view_type;
}

CString CWorkSpaceBar4::get_current_item()const
{
	CString current_item = m_wndListCtrl_->get_active_item();
	return current_item;
}

void CWorkSpaceBar4::set_active_repo()
{
	m_wndListCtrl_->set_active_repo();
}

CString CWorkSpaceBar4::get_current_branch()const
{
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	CString current_branch = pMainWnd->get_current_branch();
	return current_branch;
}


CString CWorkSpaceBar4::get_current_repo()const
{
	
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	CString current_repo = pMainWnd->get_current_repo();
	return current_repo;
}

CString CWorkSpaceBar4::get_current_commit()const
{
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	CString current_commit = pMainWnd->get_current_commit();
	return current_commit;
}

GIT_Commit_Local CWorkSpaceBar4::get_commit(const CString & repoName, const CString & branchName, const CString& commitId) 
{
	CT2CA pszConvertedAnsiString(commitId);
	// construct a std::string using the LPCSTR input
	std::string strStd_commit_id(pszConvertedAnsiString);
	auto branches_commit = repo_branches_[repoName];
	auto commits = branches_commit[branchName];
	
 	for (GIT_Commit_Local& commit : commits)
 	{
		if (commit.commit_id == strStd_commit_id)
		{
			return commit;
		}
 	}
	return GIT_Commit_Local();
}



void CWorkSpaceBar4::set_branches_for_repo(const CString & repoName)
{
	//CT2CA ctstring(repoName);
	//std::string repo(ctstring);
	auto branch_commits = repo_branches_[repoName];
	add_branches_to_combo_(branch_commits);
}

std::vector<CString> CWorkSpaceBar4::get_branches_for_repo(const CString & repoName)
{
	auto branch_commits = repo_branches_[repoName];
	std::vector<branch_name_t> branches;
	for (const auto& aPair : branch_commits)
	{
		branches.push_back(aPair.first);
	}
	return branches;
}

void CWorkSpaceBar4::set_commits_for_branch(const CString & repoName, const CString & branchName)
{
	//CT2CA ctstring(repoName);
	//std::string repo(ctstring);
	auto branch_commits = repo_branches_[repoName];
	
	//add_commit_to_list_ctrl_()
}

void CWorkSpaceBar4::set_commits(const std::vector<GIT_Commit_Local>& commits)
{//this is purely for adding items to workspace_commits, repo and branch were resolved before calling this mthd
	m_wndListCtrl_->DeleteAllItems();
	for (const auto & commit : commits)
	{
		add_commit_to_list_ctrl_(commit);
	}
	m_wndListCtrl_->set_active_commit();
}

std::vector<GIT_Commit_Local> CWorkSpaceBar4::get_commits_for_branch(const CString & repoName, const CString & branchName)
{
	std::vector<GIT_Commit_Local> commits_for_branch;

	//CT2CA c_repo_name(repoName);
	//std::string repo(c_repo_name);
	auto branch_commits = repo_branches_[repoName];

	//CT2CA c_branch_name(branchName);
	//std::string branch(c_branch_name);

	commits_for_branch = branch_commits[branchName];
	return commits_for_branch;
}







void CWorkSpaceBar4::add_branches_to_combo_(const std::map<branch_name_t, std::vector<GIT_Commit_Local>>& branch_commits)
{
	std::vector<CString> branches;
	for (const auto& aPair: branch_commits)
	{
		branches.push_back(aPair.first);
	}
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	pMainWnd->setup_git_branches_combo_(branches);

}


void CWorkSpaceBar4::set_current_repo(const repo_name_t& repoName)
{
	m_wndListCtrl_->selectItem(repoName);
	auto branches_with_commits = repo_branches_[repoName];

	add_branches_to_combo_(branches_with_commits);
	//if (m_wndListCtrl_->git_entity_type_ == Git_05_ListCtr::GIT_ENTITY_TYPE::REPO)
	//{
		Working_Dir working_dir_content = GIT_Engine::list_files_in_working_dir(repoName);
		CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
		pMainWnd->setup_workdir_content(std::move(working_dir_content));
		CString current_working_dir_view_type = pMainWnd->get_current_working_dir_view_type();
		pMainWnd->fill_view_for_item(current_working_dir_view_type);
	//}
}

void CWorkSpaceBar4::setup_workdir_content(Working_Dir&& working_dir_content)
{
	working_dir_ = working_dir_content;
}
#include "CString_Utils.h"
// static CString normalize_file_name_(const CString& fileName)
// {//removes full path and leaves only file name (if the file is in the working dir) or filename and path to file from working dir to that file folder 
// 	CT2CA ct_file_name_path(fileName);
// 	std::string file_name_path(ct_file_name_path);
// 	// construct a std::string using the LPCSTR input
// 	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
// 	CString cs_current_repo_path = pMainWnd->get_current_repo();
// 	CT2CA ct_current_repo_path(cs_current_repo_path);
// 	std::string current_repo_path(ct_current_repo_path);
// 
// 	auto mis = std::mismatch(cbegin(file_name_path), cend(file_name_path), cbegin(current_repo_path), cend(current_repo_path));
// 	std::string normalized_name((mis.first),cend(file_name_path));
// 	CA2W w_str(normalized_name.c_str());
// 	CString result = w_str;
// 	return result;
// }

void CWorkSpaceBar4::fill_view_for_item(const CString& view_type)
{
	auto sorted_files = working_dir_.get_sorted_files();
	int inx{ 0 };
	m_wndListCtrl_->DeleteAllItems();
	for (const auto& _git_status : sorted_files)
	{
		for (const auto& file_name : _git_status.second)
		{
			m_wndListCtrl_->InsertItem(inx,normalize_file_name_(file_name));
			++inx;
		}
	}
	LVCOLUMN lvCol;
	::ZeroMemory((void *)&lvCol, sizeof(LVCOLUMN));
	lvCol.mask = LVCF_TEXT;
	m_wndListCtrl_->GetColumn(0, &lvCol);
	CString str;
	str.Format(L"%d", inx);
	str += " Files";
	lvCol.pszText = str.GetBuffer();
	m_wndListCtrl_->SetColumn(0, &lvCol);
	m_wndListCtrl_->AdjustColumnWidth();
}


static const CString get_repo_name(CString c_repo_path)
{
	std::string repo_name;
	CT2CA ct_repo_path(c_repo_path);
	std::string repo_path(ct_repo_path);

	char path_separator{ '\\' };
	if (repo_path.find('/') != std::string::npos)
	{
		path_separator = '/';
	}
	std::string git_suffix(".git");
	git_suffix.insert(cbegin(git_suffix), path_separator);

	if (repo_path.substr(repo_path.size() - git_suffix.size()) == git_suffix)
	{
		auto dot_git_removed = repo_path.substr(0, repo_path.size() - git_suffix.size());
		size_t position = dot_git_removed.find_last_of(path_separator);
		if (position != std::string::npos)
		{
			repo_name = dot_git_removed.substr(position);
			repo_name.erase(std::remove(begin(repo_name), end(repo_name), path_separator), cend(repo_name));
		}
		// 		auto rbeg = crbegin(dot_git_removed);
		// 		auto rend = crend(dot_git_removed);
		// 
		// 		while (rbeg != rend)
		// 		{
		// 			repo_name += *rbeg;
		// 			++rbeg;
		// 			if (*rbeg == path_separator)
		// 			{
		// 				break;
		// 			}
		// 		}
	}
	CA2W w_str(repo_name.c_str());
	return w_str;
}

void CWorkSpaceBar4::add_repo(std::map<repo_name_t, std::map<branch_name_t, std::vector<GIT_Commit_Local>>>&& repo_branches)
{
	//branch_commits_ = branchCommits;
	auto repo_name = (*cbegin(repo_branches)).first;
	auto branches_with_commits = (*cbegin(repo_branches)).second;
	repo_branches_[repo_name] = branches_with_commits;
	auto repo_name_short = get_repo_name(repo_name);
	add_repo_to_list_ctrl_(repo_name, repo_name_short);
	set_current_repo(repo_name);
}



/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4 message handlers

// void CWorkSpaceBar4::DoDataExchange(CDataExchange* pDX)
// {
// 	CBCGPDockingControlBar::DoDataExchange(pDX);
// 	DDX_Control(pDX, IDC_LIST_CTRL, m_wndListCtrl_);
// 	//DDX_Control(pDX, IDC_USERNAME_EDIT, user_name_edit_);
// 	//DDX_Control(pDX, IDC_USEREMAIL_EDIT, user_email_edit_);
// 	//DDX_Control(pDX, IDC_USERNAMELOGIN_EDIT, user_name_login_);
// 	//DDX_Control(pDX, IDC_USEREMAILLOGIN_EDIT, password_log_in_);
// 	//DDX_Control(pDX, IDC_ACCOUNT_BUTTON, avatar_btn_);
// }

int CWorkSpaceBar4::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//// Create tree control:
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES |
	//	TVS_LINESATROOT | TVS_HASBUTTONS;

	//m_wndTree.m_bVisualManagerStyle = TRUE;

	//if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
	//{
	//	TRACE0("Failed to create workspace view\n");
	//	return -1;      // fail to create
	//}
	switch (eview_type_)
	{
		//Tree control should cover a whole client area:
	case EVIEW_TYPE::LIST_CTRL:
	{
		//renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);//just so rectArea around list item can be drawn
		m_wndListCtrl_ = std::make_unique<Git_05_ListCtr>();
		return create_list_ctrl_();
	}
	break;
// 	case EVIEW_TYPE::COMMITS:
// 	{
// 		//renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);//just so rectArea around list item can be drawn
// 		m_wndListCtrl_ = std::make_unique<Git_05_ListCtr>();
// 		return create_list_ctrl_();
// 	}
// 	break;
	case EVIEW_TYPE::DX_RENDERER:
	{
		renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);
		return renderer_ ? 0 : -1;
	}
	break;
	}
	
	// Setup tree content:
	/*HTREEITEM *///hRoot_ = m_wndTree.InsertItem(_T("Repositories"));
	// 	m_wndTree.InsertItem(_T("Item 1"), hRoot_);
	// 	m_wndTree.InsertItem(_T("Item 2"), hRoot_);
	// 	
	// 	m_wndTree.Expand(hRoot_, TVE_EXPAND);
	//fill_repositories_();
	

	//return renderer_ ? 0 : -1;
}

void CWorkSpaceBar4::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	switch (eview_type_)
	{
		//Tree control should cover a whole client area:
		case EVIEW_TYPE::LIST_CTRL:
		{
			m_wndListCtrl_->SetWindowPos(NULL, nBorderSize, nBorderSize,
				cx - 2 * nBorderSize, cy - 2 * nBorderSize,
				SWP_NOACTIVATE | SWP_NOZORDER);

		}
		break;
		case EVIEW_TYPE::DX_RENDERER:
		{
			renderer_->Resize(cx, cy);
		}
		break;
	}
}



#ifdef MFC_DIRECT_2D
LRESULT CWorkSpaceBar4::OnDrawDirect2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;

	CRect rectTree;
	m_wndTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);
	rectTree.InflateRect(nBorderSize, nBorderSize);
	return LRESULT();

}
#else
void CWorkSpaceBar4::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	switch (eview_type_)
	{
		//Tree control should cover a whole client area:
	case EVIEW_TYPE::LIST_CTRL:
	{
// 		CRect rectTree;
// 		m_wndListCtrl_->GetWindowRect(rectTree);
// 		ScreenToClient(rectTree);
// 
// 		rectTree.InflateRect(nBorderSize, nBorderSize);
// 
// 		dc.Draw3dRect(rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
// 		CRect rect;
// 		m_wndListCtrl_->GetItemRect(2, &rect, 0);
// 		renderer_->drawRectangle(rect);
	}
	break;
	case EVIEW_TYPE::DX_RENDERER:
	{
		const std::vector<GIT_Commit_Local>* commits{nullptr};
		//if (branch_commits_.size())
		//{
		//	auto beg = cbegin(branch_commits_);
		//	commits = &(beg->second);//for now only first branch from map, and perhaps I should move it from OnPaint
		//}
		//renderer_->DrawClientArea(commits);
		
	}
	break;
	}

	
	
}
#endif // MFC_DIRECT_2D

void CWorkSpaceBar4::fill_repositories_()
{
	CGit_05App* main_app_p = static_cast<CGit_05App*>(AfxGetApp());
	const HTTPS_GIT_Client* https_git_client_p = main_app_p->get_https_git_client_p();
	std::set<Git_Repository, Less<Git_Repository>> repos = https_git_client_p->current_user_repositories();

	/*for (auto repo : repos)
	{
		auto repo_name = repo.get_attribute(_T("name"));
		m_wndTree.InsertItem(repo_name.c_str(), hRoot_);
	}

	m_wndTree.Expand(hRoot_, TVE_EXPAND);*/
}

int CWorkSpaceBar4::set_type_list_ctrl_working_directory()
{
	m_wndListCtrl_->ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON | LVS_SORTASCENDING | LVS_REPORT);
	m_wndListCtrl_->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_TRACKSELECT | LVS_EX_BORDERSELECT | LVS_EX_CHECKBOXES);

	CWinApp* pApp = AfxGetApp();
	VERIFY(m_cImageListShell.Create(20, 20, ILC_COLOR32, 0, 0));
	//typedef vector<CString> file_extensions;
	//file_extensions extentions = GIT_Engine::list_files_in_working_dir();
	//m_cImageListShell.Add(pApp->LoadIcon(IDI_GIT_GREEN));
	//m_cImageListShell.Add(pApp->LoadIcon(IDI_GIT_RED));
	//m_cImageListShell.Add(pApp->LoadIcon(IDI_GIT_BW));
	m_wndListCtrl_->SetImageList(&m_cImageListShell, LVSIL_NORMAL);
	// 		
	m_wndListCtrl_->InsertColumn(0, _T("Files"), LVCFMT_CENTER, -1, 0);
	//
	// Here's where we can add the checkbox to the column header
	// First, we need to snag the header control and give it the
	// HDS_CHECKBOXES style since the list view doesn't do this for us
	HWND header = ListView_GetHeader(*m_wndListCtrl_.get());
	DWORD dwHeaderStyle = ::GetWindowLong(header, GWL_STYLE);
	//#define HDS_CHECKBOXES 0x0040
	dwHeaderStyle |= HDS_CHECKBOXES;
	::SetWindowLong(header, GWL_STYLE, dwHeaderStyle);

	// Store the ID of the header control so we can handle its notification by ID
	auto m_HeaderId = ::GetDlgCtrlID(header);
	//
	//
// 	LVCOLUMN pColumn = { 0 };
// 	pColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
// #define HDF_CHECKBOX            0x0040
// 	m_wndListCtrl_->GetColumn(0, &pColumn);
// 	pColumn.fmt |= HDF_CHECKBOX;
// 	pColumn.pszText = L"";
// 	pColumn.cx = 25;
// 	pColumn.iSubItem = 1;
// 
// 	m_wndListCtrl_->InsertColumn(0, &pColumn);
	//
	//m_wndListCtrl_->InsertColumn(1, _T("Date"), LVCFMT_RIGHT, -1, 1);
	//m_wndListCtrl_->InsertColumn(2, _T("Short info"), LVCFMT_CENTER, -1, 2);
	//m_wndListCtrl_->InsertColumn(3, _T("INVISIBLE_SHA"), LVCFMT_CENTER, -1, 2);//this column will not be visible
	m_wndListCtrl_->set_git_entity_type(Git_05_ListCtr::GIT_ENTITY_TYPE::WORKING_DIR);
	
	//
// 	LV_COLUMN lvColumn;
// 
// 	memset(&lvColumn, 0, sizeof(lvColumn));
// 	lvColumn.mask = LVCF_FMT;
// 
// 	m_wndListCtrl_->GetColumn(0, &lvColumn);
// 
// 	//if ((m_gListColumn[0].fmt & LVCFMT_RIGHT) == LVCFMT_RIGHT)
// 	//	lvColumn.fmt |= LVCFMT_RIGHT;
// 	//else if (m_gListColumn[0].fmt & LVCFMT_CENTER) == LVCFMT_CENTER)
// 	lvColumn.fmt |= LVCFMT_CENTER;
// 
// 	m_wndListCtrl_->GetColumn(0, &lvColumn);

	
	return m_wndListCtrl_->SetView(LV_VIEW_DETAILS /*| LV_VIEW_TILE*/);// LV_VIEW_TILE

}

int CWorkSpaceBar4::set_type_list_ctrl_commits()
{
	CWinApp* pApp = AfxGetApp();
	VERIFY(m_cImageListNormal.Create(64, 64, ILC_COLOR32, 0, 0));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_GREEN));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_RED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_BW));
	m_wndListCtrl_->SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	// 		
	m_wndListCtrl_->InsertColumn(0, _T("Author"), LVCFMT_CENTER, -1, 0);
	m_wndListCtrl_->InsertColumn(1, _T("Date"), LVCFMT_RIGHT, -1, 1);
	m_wndListCtrl_->InsertColumn(2, _T("Short info"), LVCFMT_CENTER, -1, 2);
	m_wndListCtrl_->InsertColumn(3, _T("INVISIBLE_SHA"), LVCFMT_CENTER, -1, 2);//this column will not be visible
	m_wndListCtrl_->set_git_entity_type(Git_05_ListCtr::GIT_ENTITY_TYPE::COMMIT);
	return m_wndListCtrl_->SetView(LV_VIEW_TILE);// LV_VIEW_TILE
}
int CWorkSpaceBar4::set_type_list_ctrl_repos()
{
	CWinApp* pApp = AfxGetApp();
	VERIFY(m_cImageListNormal.Create(64, 64, ILC_COLOR32, 0, 0));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_GREEN));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_RED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_BW));
	m_wndListCtrl_->SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	// 		
	m_wndListCtrl_->InsertColumn(0, _T("Repo name"), LVCFMT_CENTER, -1, 0);
	m_wndListCtrl_->InsertColumn(1, _T("Age"), LVCFMT_RIGHT, -1, 1);
	m_wndListCtrl_->InsertColumn(2, _T("Owner"), LVCFMT_CENTER, -1, 2);
	m_wndListCtrl_->InsertColumn(3, _T("City, Country"), LVCFMT_LEFT, -1, 3);
	m_wndListCtrl_->InsertColumn(4, _T("Hidden full repo name"), LVCFMT_LEFT, -1, 3);
	m_wndListCtrl_->set_git_entity_type(Git_05_ListCtr::GIT_ENTITY_TYPE::REPO);
	return m_wndListCtrl_->SetView(LV_VIEW_TILE);// LV_VIEW_TILE
}
int CWorkSpaceBar4::create_list_ctrl_()
{
	// Create list control:
	const DWORD dwViewStyle = WS_BORDER | WS_CHILD | WS_VISIBLE;// | TVS_HASLINES |
													//TVS_LINESATROOT | TVS_HASBUTTONS;

													//m_wndListCtrl_->m_bVisualManagerStyle = TRUE;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndListCtrl_->Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}
	
	//m_wndListCtrl_->addParent(this);
// 		CWinApp* pApp = AfxGetApp();
// 		VERIFY(m_cImageListNormal.Create(64, 64, ILC_COLOR32, 0, 0));
// 		m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_GREEN));
// 		m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_RED));
// 		m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_BW));
// 		m_wndListCtrl_->SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
// // 		
// 		m_wndListCtrl_->InsertColumn(0, _T("Repo name"), LVCFMT_CENTER, -1, 0);
// 		m_wndListCtrl_->InsertColumn(1, _T("Age"), LVCFMT_RIGHT, -1, 1);
// 		m_wndListCtrl_->InsertColumn(2, _T("Owner"), LVCFMT_CENTER, -1, 2);
// m_wndListCtrl_->InsertColumn(3, _T("City, Country"), LVCFMT_LEFT, -1, 3);
// 	
// m_wndListCtrl_->InsertItem(0, _T("Martafoi"), 0);
// m_wndListCtrl_->SetItemText(0, 1, _T("8 months"));
// m_wndListCtrl_->SetItemText(0, 2, _T("John Doe"));
// m_wndListCtrl_->SetItemText(0, 3, _T("New York, USA"));
// 
// 	m_wndListCtrl_->InsertItem(1, _T("Zdreanta"), 1);
// 	m_wndListCtrl_->SetItemText(1, 1, _T("7 years"));
// 	m_wndListCtrl_->SetItemText(1, 2, _T("Brigitte Bardot"));
// 	m_wndListCtrl_->SetItemText(1, 3, _T("Paris, France"));
// 
// 	m_wndListCtrl_->InsertItem(2, _T("Jumbo"), 2);
// 	m_wndListCtrl_->SetItemText(2, 1, _T("35 years"));
// 	m_wndListCtrl_->SetItemText(2, 2, _T("Hannibal Barcas"));
// 	m_wndListCtrl_->SetItemText(2, 3, _T("Barcelona, Spain"));
// 
// 
// 	/*m_wndListCtrl_->InsertItem(3, _T("Jumbo"), 2);*/
// 	//m_wndListCtrl_->SetItemText(3, 1, _T("35 years"));
// 	//m_wndListCtrl_->SetItemText(3, 2, _T("Hannibal Barcas"));
// 	//m_wndListCtrl_->SetItemText(3, 3, _T("Barcelona, Spain"));
// 	VERIFY(_SetTilesViewLinesCount(3));
// 
// 	UINT arrColumns[3] = { 1, 2, 3 };
// 	VERIFY(_SetItemTileLines(0, arrColumns, 3));
// 	VERIFY(_SetItemTileLines(1, arrColumns, 3));
// 	VERIFY(_SetItemTileLines(2, arrColumns, 3));
	//m_wndListCtrl_->DeleteAllItems();
	return 0;
}

int CWorkSpaceBar4::add_commit_to_list_ctrl_(const GIT_Commit_Local& commit)
{
	
	auto name = commit.commit_author.name;
	auto email = commit.commit_author.email;
	auto when = commit.commit_author.when;
	auto msg = commit.commit_message;
	auto commit_id = commit.commit_id;
 	//const git_oid commit_id = commit.commit_id;
	
	CA2W ca2w(name);
	std::wstring c_name = ca2w;
	
	CA2W ca2msg(msg.c_str());
	std::wstring c_msg = ca2msg;

	CA2W ca2commit(commit_id.c_str());
	std::wstring c_commit_id = ca2commit;
	//std::string str_commit(commit_id.id, std::cend(commit_id.id));
	//CA2W ca2commit_id(str_commit.c_str());//correct this and select first commit
	int no_of_diffed_files = commit.diffed_files.size();
	CString changes;
	changes.Format(_T("Files affected: %d"), no_of_diffed_files);
	//std::wstring c_commit_id = ca2commit_id;
	auto itemNo = m_wndListCtrl_->GetItemCount();
	m_wndListCtrl_->InsertItem(itemNo, c_name.c_str(), 2);
	m_wndListCtrl_->SetItemText(itemNo, 1, c_msg.c_str());
	m_wndListCtrl_->SetItemText(itemNo, 2, changes);
	m_wndListCtrl_->SetItemText(itemNo, 3, c_commit_id.c_str());
	VERIFY(_SetTilesViewLinesCount(2));
	UINT arrColumns[2] = { 1, 2 };
	for (auto item{ 0 }, end{ m_wndListCtrl_->GetItemCount() }; item < end; ++item)
	{
		VERIFY(_SetItemTileLines(item, arrColumns, 2));
	}

	return 0;

}

// typedef struct _SHFILEINFOA
// {
// 	HICON       hIcon;                      // out: icon
// 	int         iIcon;                      // out: icon index
// 	DWORD       dwAttributes;               // out: SFGAO_ flags
// 	CHAR        szDisplayName[MAX_PATH];    // out: display name (or path)
// 	CHAR        szTypeName[80];             // out: type name
// } SHFILEINFOA;

void CWorkSpaceBar4::load_system_icons_()
{
	//You should call this function from a background thread. Failure to do so could cause the UI to stop responding.
	//Keep in mind that you must free the obtained icon by passing hIcon to DestroyIcon() after you're done with it
	
	CString file_name(L"file.cpp");
	SHGetFileInfo(file_name, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO),
		SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON);
	m_cImageListShell.Add(shfi.hIcon);
}
int CWorkSpaceBar4::add_untracked_files_to_list_ctrl_(repo_name_t repoName)
{
	load_system_icons_();
	auto itemNo = m_wndListCtrl_->GetItemCount();
	m_wndListCtrl_->InsertItem(itemNo, repoName, 3);
	m_wndListCtrl_->SetItemText(itemNo, 1, _T("Unpublished commits: 1"));
	return 0;
}
int CWorkSpaceBar4::add_repo_to_list_ctrl_(repo_name_t repoName, repo_name_t repoNameShort)
{
	//CA2W ca2w(repoName.c_str());
	//std::wstring wide_str = ca2w;
	auto itemNo = m_wndListCtrl_->GetItemCount();
	m_wndListCtrl_->InsertItem(itemNo, repoNameShort, 2);
	m_wndListCtrl_->SetItemText(itemNo, 1, _T("Unpublished commits: 1"));
	m_wndListCtrl_->SetItemText(itemNo, 2, _T("Changes: 20"));
	m_wndListCtrl_->SetItemText(itemNo, 3, _T("Ahead of Master: 17"));
	m_wndListCtrl_->SetItemText(itemNo, 4, repoName);
	VERIFY(_SetTilesViewLinesCount(3));
	UINT arrColumns[3] = { 1, 2, 3 };
	for (auto item{ 0 }, end{ m_wndListCtrl_->GetItemCount() }; item < end; ++item)
	{
		VERIFY(_SetItemTileLines(item, arrColumns, 3));
	}

	return 0;
}

BOOL CWorkSpaceBar4::_SetTilesViewLinesCount(int nCount)
{
	LVTILEVIEWINFO lvtvwi = { 0 };
	lvtvwi.cbSize = sizeof(LVTILEVIEWINFO);
	lvtvwi.dwMask = LVTVIM_COLUMNS;
	lvtvwi.cLines = nCount;

	return m_wndListCtrl_->SetTileViewInfo(&lvtvwi);
}
BOOL CWorkSpaceBar4::_SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount)
{
	LVTILEINFO lvti = { 0 };
	lvti.cbSize = sizeof(LVTILEINFO);
	lvti.cColumns = nCount;
	lvti.iItem = nItem;
	lvti.puColumns = parrColumns;

	return m_wndListCtrl_->SetTileInfo(&lvti);
}

void CWorkSpaceBar4::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CBCGPDockingControlBar::OnClose();
	
}


void CWorkSpaceBar4::OnDestroy()
{
	CBCGPDockingControlBar::OnDestroy();

// 	if (m_wndListCtrl_->get_git_entity_type() == Git_05_ListCtr::GIT_ENTITY_TYPE::REPO)
// 	{
// 		CString repo_name = get_current_repo();
// 		CGit_05App* app_p = static_cast<CGit_05App*>(AfxGetApp());
// 		app_p->write_repo_name_to_file_(repo_name);
// 	}
}
