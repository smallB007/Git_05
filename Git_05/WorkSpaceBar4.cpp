// workspace.cpp : implementation of the CWorkSpaceBar4 class
//

#include "stdafx.h"
#include "Git_05.h"
#include "MainFrm.h"
#include "WorkSpaceBar4.h"
#include "GIT_Commit_Local.hpp"
#include "Git_05_ListCtr.hpp"
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

END_MESSAGE_MAP()

// void CWorkSpaceBar4::OnLButtonDown(UINT flag, CPoint point)
// {
// 	int a{ 0 };
// }

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4 construction/destruction
CGit_05App* CWorkSpaceBar4::get_main_app_()const
{
	return static_cast<CGit_05App*>(AfxGetApp());
};//copy from BackStagePageInfo.h

CWorkSpaceBar4::CWorkSpaceBar4()
{
	// TODO: add one-time construction code here
	//EnableD2DSupport();
}

void CWorkSpaceBar4::set_view_type(EVIEW_TYPE view_type)
{
	eview_type_ = view_type;
}

void CWorkSpaceBar4::set_branches_for_repo(const CString & repoName)
{
	CT2CA ctstring(repoName);
	std::string repo(ctstring);
	auto branch_commits = repo_branches_[repo];
	add_branches_to_combo_(branch_commits);
}

void CWorkSpaceBar4::set_commits_for_branch(const branch_name_t & branchName)
{
}

void CWorkSpaceBar4::write_repo_name_to_file_(const CString& repoName)
{
	std::ofstream f_out(file_with_repo_to_set_as_active_);
	if (f_out)
	{
		CT2CA ctstring(repoName);
		std::string repo(ctstring);
		f_out << repo;
	}
}

CWorkSpaceBar4::repo_name_t CWorkSpaceBar4::read_repo_name_from_file_()
{
	std::ifstream f_in(file_with_repo_to_set_as_active_);
	repo_name_t active_repo;
	if (f_in)
	{
		f_in >> active_repo;
	}

	return active_repo;
}

void CWorkSpaceBar4::select_repository_according_to_policy()
{//for the moment last selected will be the one we will select at the start of our application
	repo_name_t active_repo = read_repo_name_from_file_();
	if (active_repo.size())
	{
		select_repo_(active_repo);
	}
}

void CWorkSpaceBar4::add_branches_to_combo_(const std::map<branch_name_t, std::vector<GIT_Commit_Local>>& branch_commits)
{
	std::vector<CString> branches;
	for (const auto& aPair: branch_commits)
	{
		CA2W ca2w(aPair.first.c_str());
		std::wstring wide_str = ca2w;
		branches.push_back(wide_str.c_str());
	}
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	pMainWnd->setup_git_branches_combo_(branches);

}

void CWorkSpaceBar4::select_repo_(const repo_name_t& repoName)
{
	m_wndListCtrl_Repos->selectItem(repoName);
	auto branches_with_commits = repo_branches_[repoName];

	add_branches_to_combo_(branches_with_commits);
}

void CWorkSpaceBar4::git_tree(std::map<repo_name_t, std::map<branch_name_t, std::vector<GIT_Commit_Local>>>&& repo_branches)
{
	//branch_commits_ = branchCommits;
	auto repo_name = (*cbegin(repo_branches)).first;
	auto branches_with_commits = (*cbegin(repo_branches)).second;
	repo_branches_[repo_name] = branches_with_commits;
	
	add_repo_to_list_ctrl_(repo_name);
	
}

CWorkSpaceBar4::~CWorkSpaceBar4()
{
// 	if (m_wndListCtrl_Repos)
// 	{
// 		auto i = m_wndListCtrl_Repos->GetNextItem(-1, LVNI_SELECTED);
// 		auto repo_name = m_wndListCtrl_Repos->GetItemText(i, 0);
// 		write_repo_name_to_file_(repo_name);
// 	}
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4 message handlers

// void CWorkSpaceBar4::DoDataExchange(CDataExchange* pDX)
// {
// 	CBCGPDockingControlBar::DoDataExchange(pDX);
// 	DDX_Control(pDX, IDC_LIST_CTRL, m_wndListCtrl_Repos);
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
	case EVIEW_TYPE::REPOS:
	{
		//renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);//just so rectArea around list item can be drawn
		m_wndListCtrl_Repos = std::make_unique<Git_05_ListCtr>();
		return create_list_ctrl_repos();
	}
	break;
	case EVIEW_TYPE::COMMITS:
	{
		//renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);//just so rectArea around list item can be drawn
		m_wndListCtrl_Repos = std::make_unique<Git_05_ListCtr>();
		return create_list_ctrl_commits();
	}
	break;
	case EVIEW_TYPE::GIT_TREE:
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
		case EVIEW_TYPE::REPOS:
		{
			m_wndListCtrl_Repos->SetWindowPos(NULL, nBorderSize, nBorderSize,
				cx - 2 * nBorderSize, cy - 2 * nBorderSize,
				SWP_NOACTIVATE | SWP_NOZORDER);

		}
		break;
		case EVIEW_TYPE::GIT_TREE:
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
	case EVIEW_TYPE::REPOS:
	{
// 		CRect rectTree;
// 		m_wndListCtrl_Repos->GetWindowRect(rectTree);
// 		ScreenToClient(rectTree);
// 
// 		rectTree.InflateRect(nBorderSize, nBorderSize);
// 
// 		dc.Draw3dRect(rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
// 		CRect rect;
// 		m_wndListCtrl_Repos->GetItemRect(2, &rect, 0);
// 		renderer_->drawRectangle(rect);
	}
	break;
	case EVIEW_TYPE::GIT_TREE:
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
	CGit_05App* main_app_p = get_main_app_();
	const HTTPS_GIT_Client* https_git_client_p = main_app_p->get_https_git_client_p();
	std::set<Git_Repository, Less<Git_Repository>> repos = https_git_client_p->current_user_repositories();

	/*for (auto repo : repos)
	{
		auto repo_name = repo.get_attribute(_T("name"));
		m_wndTree.InsertItem(repo_name.c_str(), hRoot_);
	}

	m_wndTree.Expand(hRoot_, TVE_EXPAND);*/
}

int CWorkSpaceBar4::create_list_ctrl_commits()
{
	// Create list control:
	const DWORD dwViewStyle = WS_BORDER | WS_CHILD | WS_VISIBLE;// | TVS_HASLINES |
																//TVS_LINESATROOT | TVS_HASBUTTONS;

																//m_wndListCtrl_Repos->m_bVisualManagerStyle = TRUE;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndListCtrl_Repos->Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	m_wndListCtrl_Repos->addParent(this);
	CWinApp* pApp = AfxGetApp();
	VERIFY(m_cImageListNormal.Create(64, 64, ILC_COLOR32, 0, 0));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_GREEN));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_RED));
	m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_BW));
	m_wndListCtrl_Repos->SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	// 		
	m_wndListCtrl_Repos->InsertColumn(0, _T("Author"), LVCFMT_CENTER, -1, 0);
	m_wndListCtrl_Repos->InsertColumn(1, _T("Date"), LVCFMT_RIGHT, -1, 1);
	m_wndListCtrl_Repos->InsertColumn(2, _T("Short info"), LVCFMT_CENTER, -1, 2);
	return m_wndListCtrl_Repos->SetView(LV_VIEW_TILE);// LV_VIEW_TILE
}
int CWorkSpaceBar4::create_list_ctrl_repos()
{
	// Create list control:
	const DWORD dwViewStyle = WS_BORDER | WS_CHILD | WS_VISIBLE;// | TVS_HASLINES |
													//TVS_LINESATROOT | TVS_HASBUTTONS;

													//m_wndListCtrl_Repos->m_bVisualManagerStyle = TRUE;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndListCtrl_Repos->Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}
	
	m_wndListCtrl_Repos->addParent(this);
		CWinApp* pApp = AfxGetApp();
		VERIFY(m_cImageListNormal.Create(64, 64, ILC_COLOR32, 0, 0));
		m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_GREEN));
		m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_RED));
		m_cImageListNormal.Add(pApp->LoadIcon(IDI_GIT_BW));
		m_wndListCtrl_Repos->SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
// 		
		m_wndListCtrl_Repos->InsertColumn(0, _T("Repo name"), LVCFMT_CENTER, -1, 0);
		m_wndListCtrl_Repos->InsertColumn(1, _T("Age"), LVCFMT_RIGHT, -1, 1);
		m_wndListCtrl_Repos->InsertColumn(2, _T("Owner"), LVCFMT_CENTER, -1, 2);
m_wndListCtrl_Repos->InsertColumn(3, _T("City, Country"), LVCFMT_LEFT, -1, 3);
// 	
// m_wndListCtrl_Repos->InsertItem(0, _T("Martafoi"), 0);
// m_wndListCtrl_Repos->SetItemText(0, 1, _T("8 months"));
// m_wndListCtrl_Repos->SetItemText(0, 2, _T("John Doe"));
// m_wndListCtrl_Repos->SetItemText(0, 3, _T("New York, USA"));
// 
// 	m_wndListCtrl_Repos->InsertItem(1, _T("Zdreanta"), 1);
// 	m_wndListCtrl_Repos->SetItemText(1, 1, _T("7 years"));
// 	m_wndListCtrl_Repos->SetItemText(1, 2, _T("Brigitte Bardot"));
// 	m_wndListCtrl_Repos->SetItemText(1, 3, _T("Paris, France"));
// 
// 	m_wndListCtrl_Repos->InsertItem(2, _T("Jumbo"), 2);
// 	m_wndListCtrl_Repos->SetItemText(2, 1, _T("35 years"));
// 	m_wndListCtrl_Repos->SetItemText(2, 2, _T("Hannibal Barcas"));
// 	m_wndListCtrl_Repos->SetItemText(2, 3, _T("Barcelona, Spain"));
// 
// 
// 	/*m_wndListCtrl_Repos->InsertItem(3, _T("Jumbo"), 2);*/
// 	//m_wndListCtrl_Repos->SetItemText(3, 1, _T("35 years"));
// 	//m_wndListCtrl_Repos->SetItemText(3, 2, _T("Hannibal Barcas"));
// 	//m_wndListCtrl_Repos->SetItemText(3, 3, _T("Barcelona, Spain"));
// 	VERIFY(_SetTilesViewLinesCount(3));
// 
// 	UINT arrColumns[3] = { 1, 2, 3 };
// 	VERIFY(_SetItemTileLines(0, arrColumns, 3));
// 	VERIFY(_SetItemTileLines(1, arrColumns, 3));
// 	VERIFY(_SetItemTileLines(2, arrColumns, 3));
	//m_wndListCtrl_Repos->DeleteAllItems();
	return m_wndListCtrl_Repos->SetView(LV_VIEW_TILE);// LV_VIEW_TILE
}


int CWorkSpaceBar4::add_repo_to_list_ctrl_(repo_name_t repoName)
{
	CA2W ca2w(repoName.c_str());
	std::wstring wide_str = ca2w;
	auto itemNo = m_wndListCtrl_Repos->GetItemCount();
	m_wndListCtrl_Repos->InsertItem(itemNo, wide_str.c_str(), 2);
	m_wndListCtrl_Repos->SetItemText(itemNo, 1, _T("Unpublished commits: 1"));
	m_wndListCtrl_Repos->SetItemText(itemNo, 2, _T("Changes: 20"));
	m_wndListCtrl_Repos->SetItemText(itemNo, 3, _T("Ahead of Master: 17"));
	VERIFY(_SetTilesViewLinesCount(3));
	UINT arrColumns[3] = { 1, 2, 3 };
	for (auto item{0}, end {m_wndListCtrl_Repos->GetItemCount()}; item < end; ++item)
	VERIFY(_SetItemTileLines(item, arrColumns, 3));

	return 0;
}

BOOL CWorkSpaceBar4::_SetTilesViewLinesCount(int nCount)
{
	LVTILEVIEWINFO lvtvwi = { 0 };
	lvtvwi.cbSize = sizeof(LVTILEVIEWINFO);
	lvtvwi.dwMask = LVTVIM_COLUMNS;
	lvtvwi.cLines = nCount;

	return m_wndListCtrl_Repos->SetTileViewInfo(&lvtvwi);
}
BOOL CWorkSpaceBar4::_SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount)
{
	LVTILEINFO lvti = { 0 };
	lvti.cbSize = sizeof(LVTILEINFO);
	lvti.cColumns = nCount;
	lvti.iItem = nItem;
	lvti.puColumns = parrColumns;

	return m_wndListCtrl_Repos->SetTileInfo(&lvti);
}