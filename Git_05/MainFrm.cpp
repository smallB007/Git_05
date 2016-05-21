// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Git_05.h"
#include "GIT_Commit_Local.hpp"
#include "MainFrm.h"
#include "BackStagePageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	//ON_COMMAND(ID_VIEW_WORKSPACE_GIT_TREE2, OnViewWorkspace_Git_Tree2)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE_GIT_TREE2, OnUpdateViewWorkspace_Git_Tree2)
	ON_COMMAND(ID_VIEW_WORKSPACE3, OnViewWorkspace3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE3, OnUpdateViewWorkspace3)
	ON_COMMAND(ID_VIEW_WORKSPACE4, OnViewWorkspaceRepos)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE4, OnUpdateViewWorkspaceRepos)
	ON_COMMAND(IDC_STAGING_AREA, OnViewWorkspace_Staging_Area)
	ON_UPDATE_COMMAND_UI(IDC_STAGING_AREA, OnUpdateViewWorkspace_Staging_Area)
	ON_COMMAND(ID_VIEW_WORKSPACE42, OnViewWorkspaceCommits)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE42, OnUpdateViewWorkspaceCommits)
	ON_COMMAND(ID_VIEW_WORKSPACE41, OnViewWorkspace_Git_Tree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE41, OnUpdateViewWorkspace_Git_Tree)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewPropGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewPropGrid)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_REGISTERED_MESSAGE(BCGM_ON_BEFORE_RIBBON_BACKSTAGE_VIEW, OnBeforeShowRibbonBackstageView)
	ON_COMMAND(IDC_PROGRESS1055, OnProgressBar)
	ON_COMMAND(IDC_PALETTE_BTN_THEME, OnPaletteTheme)
	ON_COMMAND(ID_SOME_OFFICE_THEME, OnPaletteTheme)
	//ON_CBN_SELCHANGE(IDC_REPO_BRANCHES_COMBO, OnCbn_Git_Tree_Branches_SelchangeCombo)
	ON_COMMAND(IDC_REPO_BRANCHES_COMBO, OnCbn_Git_Tree_Branches_SelchangeCombo)
	ON_COMMAND(IDC_VIEW_REPO_DIR_COMBO, OnCbn_Git_View_Repo_SelchangeCombo)
	
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

// void CMainFrame::selectRepository()
// {
// 	m_wndWorkSpace_Repos_.select_repository_according_to_policy();
// }

void CMainFrame::select_active_repo()
{
	m_wndWorkSpace_Repos_.set_active_repo();
}

void CMainFrame::set_current_repo(const CString & repoName)
{
	m_wndWorkSpace_Repos_.set_current_repo(repoName);
	//m_wndWorkSpace_UntrackedFiles_.add_untracked_files_to_list_ctrl_(repoName);

	//GIT_Engine::list_files_in_working_dir(repo, pathName);
}

CString CMainFrame::get_current_branch()const
{
	CBCGPRibbonComboBox* branches_cmb = get_branches_cmb_();
	auto inx = branches_cmb->GetCurSel();
	CString branch = branches_cmb->GetItem(inx);
	return branch;
}

CString CMainFrame::get_current_repo()const
{
	CString current_repo = m_wndWorkSpace_Repos_.get_current_item();

	return current_repo;
}

CString CMainFrame::get_current_commit()const
{
	CString current_commit = m_wndWorkSpace_Commits_.get_current_item();

	return current_commit;
}

GIT_Commit_Local CMainFrame::get_commit(const CString & currentRepo, const CString & currentBranch, const CString & sha)
{
	return m_wndWorkSpace_Repos_.get_commit(currentRepo, currentBranch, sha);
}
//#include "Git_05View.h"
void CMainFrame::display_info_for_diffed_file(const CString& sha, const CString& fileName)
{
	CString current_repo = get_current_repo();
	CString current_branch =  get_current_branch();
	GIT_Commit_Local current_commit = get_commit(current_repo,current_branch,sha);
	diffed_file_t diffed_file = current_commit.get_diffed_file(fileName);
	//CGit_05View::GetView()->display_diffed_file_(diffed_file);
	static_cast<CGit_05App*>(AfxGetApp())->set_file_for_display(diffed_file);
}



void CMainFrame::set_info_for_commit(const CString & commit_id)
{
	CString current_repo = m_wndWorkSpace_Repos_.get_current_repo();
	CString current_branch = get_current_branch();//we could find commit by just having repo name but this will speed up our search
	GIT_Commit_Local commit_info = m_wndWorkSpace_Repos_.get_commit(current_repo, current_branch, commit_id);
	m_wndCommitPropertiesGrid.set_commit_info(commit_info);
}
 
void CMainFrame::deselect_file_from_commit_detail_window()
{
	m_wndCommitPropertiesGrid.deselect_file_from_commit_detail_window();
}

void CMainFrame::reset_view()const
{
	static_cast<CGit_05App*>(AfxGetApp())->reset_view();
}

void CMainFrame::set_branches_for_repo(const CString & repoName)
{
	m_wndWorkSpace_Repos_.set_branches_for_repo(repoName);
}

CBCGPRibbonComboBox* CMainFrame::get_branches_cmb_()const
{
	CBCGPRibbonBar* pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	auto branches_p = static_cast<CBCGPRibbonComboBox*>(pRibbon->FindByID(IDC_REPO_BRANCHES_COMBO));
	ASSERT_VALID(branches_p);
	return branches_p;
}

CBCGPRibbonComboBox* CMainFrame::get_repo_view_cmb_()const
{
	CBCGPRibbonBar* pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	auto repo_view_p = static_cast<CBCGPRibbonComboBox*>(pRibbon->FindByID(IDC_VIEW_REPO_DIR_COMBO));
	ASSERT_VALID(repo_view_p);
	return repo_view_p;
}


void CMainFrame::setup_git_branches_combo_(const std::vector<CString>& branches)
{
	//CBCGPRibbonBar* pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	//ASSERT_VALID(pRibbon);
	//auto branches_p = static_cast<CBCGPRibbonComboBox*>(pRibbon->FindByID(IDC_REPO_BRANCHES_COMBO));
	//ASSERT_VALID(branches_p);
	auto branches_p = get_branches_cmb_();
	branches_p->RemoveAllItems();
	for (const auto & branch : branches)
	{
		branches_p->AddItem(branch);
	}
	branches_p->SelectItem(0);
	OnCbn_Git_Tree_Branches_SelchangeCombo();
	if (0)
	{//width of combobox can be calculated ;)
		CClientDC dc(this);
		CFont * pOldFont = dc.SelectObject(this->GetFont());
		CSize len = dc.GetTextExtent(branches_p->GetItem(0));
	}
	//branches_p->SetWidth(100);
}

void CMainFrame::setup_ribbon_background_()
{
	CBCGPToolBarImages background;
	background.Load(IDR_RIBBON_BACKGROUND1);
	CBCGPWinApp::BCGP_VISUAL_THEME theme{ theApp.GetVisualTheme() };
	
 		if (theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2013_DARK_GRAY)
 		 	{
		 		// Make background image darker for Office 2013 Dark theme:
		 		background.AddaptColors(RGB(255, 255, 255), RGB(230, 230, 230));

		 	}

	m_wndRibbonBar.SetBackgroundImage(background);
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (!CreateRibbonBar ())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//const int visible = 1;
	// TODO: add your status bar panes here:
	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE1, _T("Pane 1"), TRUE), _T("Pane 1"));
	CBCGPRibbonStatusBarPane* pane = new CBCGPRibbonStatusBarPane(ID_STATUSBAR_PANE2, _T("Pane 2"), TRUE);
	
	m_wndStatusBar.AddExtendedElement (pane, _T("Pane 2"));
	

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.Load (IDB_WORKSPACE);
	imagesWorkspace.SmoothResize(globalData.GetRibbonImageScale());

	//	if (!wnd_workspace_git_tree_2.Create(_T("Git Tree 2"), this, CRect(0, 0, 200, 200),
	//		TRUE, ID_VIEW_WORKSPACE_GIT_TREE2,
	//		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	//	{
	//		TRACE0("Failed to create Workspace bar\n");
	//		return -1;      // fail to create
	//	}

	//wnd_workspace_git_tree_2.SetIcon(imagesWorkspace.ExtractIcon(0), FALSE);

	//if (!m_wndWorkSpace.Create (_T("View 1"), this, CRect (0, 0, 200, 200),
	//	TRUE, ID_VIEW_WORKSPACE,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Workspace bar\n");
	//	return -1;      // fail to create
	//}
	//
	//m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);
	//
	//if (!m_wndWorkSpace2.Create (_T("View 2"), this, CRect (0, 0, 200, 200),
	//	TRUE, ID_VIEW_WORKSPACE2,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Workspace bar 2\n");
	//	return -1;      // fail to create
	//}

	//m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);

	if (!m_wndWorkSpace3.Create (_T("View 3"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE3,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 3\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace3.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);
////////////////////
	m_wndWorkSpace_Repos_.set_view_type(CWorkSpaceBar4::EVIEW_TYPE::LIST_CTRL);
	if (!m_wndWorkSpace_Repos_.Create(_T("Repositories"), this, CRect(0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE4,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 4\n");
		return -1;      // fail to create
	}
	m_wndWorkSpace_Repos_.set_type_list_ctrl_repos();
	m_wndWorkSpace_Repos_.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);

///////////////////////
	m_wndWorkSpace_Commits_.set_view_type(CWorkSpaceBar4::EVIEW_TYPE::LIST_CTRL);
	if (!m_wndWorkSpace_Commits_.Create(_T("Commits"), this, CRect(0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE42,//For some bizarre reason ID must be ID_VIEW_WORKSPACE AND A NUMBER, NOTHING ELSE WILL DO!
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 4\n");
		return -1;      // fail to create
	}
	CString selected_item = setup_repo_view_cmb_();//AC we need to call it here so we get the chance to set the correct name in WorkspaceView
	m_wndWorkSpace_Commits_.set_type_list_ctrl_commits();
	m_wndWorkSpace_Commits_.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);
//////////////////////
	m_wndWorkSpace_UntrackedFiles_.set_view_type(CWorkSpaceBar4::EVIEW_TYPE::LIST_CTRL);
	if (!m_wndWorkSpace_UntrackedFiles_.Create(selected_item, this, CRect(0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE43,//For some bizarre reason ID must be ID_VIEW_WORKSPACE AND A NUMBER, NOTHING ELSE WILL DO!
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 4\n");
		return -1;      // fail to create
	}
	m_wndWorkSpace_UntrackedFiles_.set_type_list_ctrl_untracked_files();
	m_wndWorkSpace_UntrackedFiles_.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);
	//
		m_wndWorkSpace_Git_Tree_.set_view_type(CWorkSpaceBar4::EVIEW_TYPE::DX_RENDERER);

		if (!m_wndWorkSpace_Git_Tree_.Create(_T("View 41"), this, CRect(0, 0, 200, 200),
			TRUE, ID_VIEW_WORKSPACE41,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Workspace bar 4\n");
			return -1;      // fail to create
		}

		m_wndWorkSpace_Git_Tree_.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);
//////////////////////
	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);

	if (!m_wndCommitPropertiesGrid.Create (_T("Commit Details"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_VIEW_PROPERTIES,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties Bar\n");
		return FALSE;		// fail to create
	}

	m_wndCommitPropertiesGrid.SetIcon (imagesWorkspace.ExtractIcon (3), FALSE);

	//wnd_workspace_git_tree_2.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace_UntrackedFiles_.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace_Repos_.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace_Commits_.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace_Git_Tree_.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCommitPropertiesGrid.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndWorkSpace);
	DockControlBar(&m_wndWorkSpace3);
	DockControlBar(&m_wndWorkSpace_UntrackedFiles_);
	DockControlBar(&m_wndWorkSpace_Repos_);
	DockControlBar(&m_wndWorkSpace_Git_Tree_);
	DockControlBar(&m_wndWorkSpace_Commits_);
	//m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);
	DockControlBar(&m_wndOutput);
	DockControlBar(&m_wndCommitPropertiesGrid);
	//DockControlBar(&wnd_workspace_git_tree_2);

	setup_ribbon_background_();
	
	CBCGPRibbonBar* pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	auto branches_p = static_cast<CBCGPRibbonComboBox*>(pRibbon->FindByID(IDC_REPO_BRANCHES_COMBO));
	ASSERT_VALID(branches_p);
	branches_p->EnableAutoComplete(TRUE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




BOOL CMainFrame::CreateRibbonBar ()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}
	//m_wndRibbonBar.LoadFromVSRibbon(IDR_RIBBON1);
	if (!m_wndRibbonBar.LoadFromXML (_T("IDR_BCGP_RIBBON_XML")))
	{
		return FALSE;
	}

	CBCGPRibbonCategory* pBackstageCategory = m_wndRibbonBar.GetBackstageCategory ();

	if (pBackstageCategory != NULL)
	{
		CBCGPRibbonBackstageViewPanel* pBackstagePanel = DYNAMIC_DOWNCAST (CBCGPRibbonBackstageViewPanel, pBackstageCategory->GetPanel (0));
		pBackstagePanel->SetPageTransitionEffect(CBCGPPageTransitionManager::BCGPPageTransitionSlide);//doesn't work?
		
		if (pBackstagePanel != NULL)
		{
			pBackstagePanel->AttachViewToItem (4, new CBCGPRibbonBackstageViewItemForm(IDD_FORM_INFO, RUNTIME_CLASS(CBackStagePageInfo)), FALSE);
			pBackstagePanel->AttachRecentViewToItem(5, BCGP_SHOW_FOLDERS_LIST | BCGP_SHOW_PINS, FALSE);
			pBackstagePanel->AttachPrintPreviewToItem(6, FALSE);
		}
	}
	
 // USE_RIBBON_DESIGNER
	return TRUE;
}

LRESULT CMainFrame::OnRibbonCustomize (WPARAM wp, LPARAM /*lp*/)
{
	ShowOptions (wp == 0 ? 1 : 0);
	return 1;
}

void CMainFrame::OnToolsOptions()
{
	ShowOptions (0);
}

void CMainFrame::ShowOptions (int nPage)
{
	// Create custom categories:
	
	// "Popular" items:
	
	CList<UINT, UINT> lstPopular;
	
	lstPopular.AddTail (ID_FILE_NEW);
	lstPopular.AddTail (ID_FILE_OPEN);
	lstPopular.AddTail (ID_FILE_SAVE);
	lstPopular.AddTail (ID_FILE_PRINT_PREVIEW);
	lstPopular.AddTail (ID_FILE_PRINT_DIRECT);
	lstPopular.AddTail (ID_EDIT_UNDO);
	
	// Hidden commands:
	CList<UINT,UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList (lstHidden, TRUE);
	
	// All commands:
	CList<UINT,UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList (lstAll);

	// Create "Customize QAT" page:
	CBCGPRibbonCustomizeQATPage pageCustomizeQAT(&m_wndRibbonBar);
	
	pageCustomizeQAT.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeQAT.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeQAT.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create "Customize Ribbon" page:
	CBCGPRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);
	
	pageCustomizeRibbon.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create property sheet:
	CBCGPPropertySheet propSheet (_T("Options"), this, nPage);
	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.EnableLayout();
	
	propSheet.SetLook(CBCGPPropertySheet::PropSheetLook_List, 150 /* List width */);
	
	propSheet.AddPage (&pageCustomizeRibbon);
	
	propSheet.AddPage (&pageCustomizeQAT);

	// TODO: add your option pages here:
	// COptionsPage1 pageOptions1;
	// propSheet.AddPage (&pageOptions1);
	//
	// COptionsPage1 pageOptions2;
	// propSheet.AddPage (&pageOptions2);
	
	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}

void CMainFrame::OnViewWorkspace_Git_Tree2()
{
	/*ShowControlBar(&wnd_workspace_git_tree_2,
		!(wnd_workspace_git_tree_2.IsVisible()),
		FALSE, TRUE);
	RecalcLayout();*/
}

void CMainFrame::OnUpdateViewWorkspace_Git_Tree2(CCmdUI* pCmdUI)
{
	/*pCmdUI->SetCheck(wnd_workspace_git_tree_2.IsVisible());
	pCmdUI->Enable(!GetDockManager()->IsPrintPreviewValid());*/
}

void CMainFrame::OnViewWorkspace3() 
{
	ShowControlBar (&m_wndWorkSpace3,
					!(m_wndWorkSpace3.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace3(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace3.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}

void CMainFrame::OnViewWorkspaceRepos() 
{
	ShowControlBar (&m_wndWorkSpace_Repos_,
					!(m_wndWorkSpace_Repos_.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspaceRepos(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace_Repos_.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}

void CMainFrame::OnViewWorkspaceCommits()
{
	ShowControlBar(&m_wndWorkSpace_Commits_,
		!(m_wndWorkSpace_Commits_.IsVisible()),
		FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewWorkspaceCommits(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndWorkSpace_Commits_.IsVisible());
	pCmdUI->Enable(!GetDockManager()->IsPrintPreviewValid());
}

void CMainFrame::OnViewWorkspace_Git_Tree()
{
	ShowControlBar(&m_wndWorkSpace_Git_Tree_,
		!(m_wndWorkSpace_Git_Tree_.IsVisible()),
		FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewWorkspace_Git_Tree(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndWorkSpace_Git_Tree_.IsVisible());
	pCmdUI->Enable(!GetDockManager()->IsPrintPreviewValid());
}

 // WORKSPACEBAR
void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}
 // OUTPUTBAR

void CMainFrame::OnViewPropGrid() 
{
	ShowControlBar (&m_wndCommitPropertiesGrid,
					!(m_wndCommitPropertiesGrid.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewPropGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndCommitPropertiesGrid.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}
 // PROPERTYBAR

void CMainFrame::OnViewWorkspace_Staging_Area()
{
	ShowControlBar(&m_wndWorkSpace_UntrackedFiles_,
		!(m_wndWorkSpace_UntrackedFiles_.IsVisible()),
		FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewWorkspace_Staging_Area(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndWorkSpace_UntrackedFiles_.IsVisible());
	pCmdUI->Enable(!GetDockManager()->IsPrintPreviewValid());
}
 // UI_TYPE_RIBBON

LRESULT CMainFrame::OnBeforeShowRibbonBackstageView(WPARAM, LPARAM)
{
	theApp.CreateScreenshot(m_bmpAppPreview);
	return 0;
}

void CMainFrame::OnProgressBar()
{

}

void CMainFrame::OnPaletteTheme()
{

}

// void CMainFrame::DoDataExchange(CDataExchange * pDX)
// {
	//CBCGPFrameWnd::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_REPO_BRANCHES_COMBO, git_tree_branches_);
	
//}
#include "GIT_Commit_Local.hpp"
void CMainFrame::OnCbn_Git_Tree_Branches_SelchangeCombo()
{
	auto branches_cmb_p = get_branches_cmb_();
	auto ix = branches_cmb_p->GetCurSel();
	LPCTSTR branch_name = branches_cmb_p->GetItem(ix);
	CString repo_name = m_wndWorkSpace_Repos_.get_current_repo();
	std::vector<GIT_Commit_Local> commits =	m_wndWorkSpace_Repos_.get_commits_for_branch(repo_name, branch_name);
	m_wndWorkSpace_Commits_.set_commits(commits);
}

void CMainFrame::set_info_for_working_dir_(const CString& view_type)
{
	m_wndWorkSpace_UntrackedFiles_.SetWindowTextW(view_type);
}

void CMainFrame::OnCbn_Git_View_Repo_SelchangeCombo()
{
	auto repo_view_cmb = get_repo_view_cmb_();
	auto ix = repo_view_cmb->GetCurSel();
	
	auto item = repo_view_cmb->GetItem(ix);
	set_info_for_working_dir_(item);
}

CString CMainFrame::setup_repo_view_cmb_()
{
	auto repo_view_cmb_p = get_repo_view_cmb_();
	repo_view_cmb_p->AddItem(L"Working Dir");
	repo_view_cmb_p->AddItem(L"Modified Items");
	repo_view_cmb_p->AddItem(L"Added Items");
	repo_view_cmb_p->AddItem(L"Deleted Items");
	repo_view_cmb_p->AddItem(L"Untracked Items");
	repo_view_cmb_p->AddItem(L"Ignored Items");
	repo_view_cmb_p->SelectItem(0);//chance here to actually select last selection (store it in a file Artie;)
	int ix = repo_view_cmb_p->GetCurSel();
	return repo_view_cmb_p->GetItem(ix);
}