// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Git_05.h"

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
	ON_COMMAND(ID_VIEW_WORKSPACE4, OnViewWorkspace4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE4, OnUpdateViewWorkspace4)
	ON_COMMAND(ID_VIEW_WORKSPACE41, OnViewWorkspace41)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE41, OnUpdateViewWorkspace41)
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
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}




void CMainFrame::setup_git_branches_combo_()
{
	CBCGPRibbonBar* pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	auto branches_p = static_cast<CBCGPRibbonComboBox*>(pRibbon->FindByID(IDC_REPO_BRANCHES_COMBO));
	ASSERT_VALID(branches_p);
	branches_p->AddItem(L"Item 2");
	branches_p->AddItem(L"Some really long text");
	branches_p->AddItem(L"Item 1");
	branches_p->SelectItem(0);
	if (0)
	{//width of combobox can be calculated ;)
		CClientDC dc(this);
		CFont * pOldFont = dc.SelectObject(this->GetFont());
		CSize len = dc.GetTextExtent(branches_p->GetItem(0));
	}
	branches_p->SetWidth(100);
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
	const int visible = 1;
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

	if (!m_wndWorkSpace4.Create(_T("View 4"), this, CRect(0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE4,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 4\n");
		return -1;      // fail to create
	}
	m_wndWorkSpace4.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);


		m_wndWorkSpace41.set_view_type(CWorkSpaceBar4::EVIEW_TYPE::GIT_TREE);

		if (!m_wndWorkSpace41.Create(_T("View 41"), this, CRect(0, 0, 200, 200),
			TRUE, ID_VIEW_WORKSPACE41,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Workspace bar 4\n");
			return -1;      // fail to create
		}

	m_wndWorkSpace41.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);

	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);

	if (!m_wndPropGrid.Create (_T("Properties"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_VIEW_PROPERTIES,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties Bar\n");
		return FALSE;		// fail to create
	}

	m_wndPropGrid.SetIcon (imagesWorkspace.ExtractIcon (3), FALSE);

	//wnd_workspace_git_tree_2.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace4.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace41.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPropGrid.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndWorkSpace);
	DockControlBar(&m_wndWorkSpace3);
	DockControlBar(&m_wndWorkSpace4);
	DockControlBar(&m_wndWorkSpace41);
	//m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);
	DockControlBar(&m_wndOutput);
	DockControlBar(&m_wndPropGrid);
	//DockControlBar(&wnd_workspace_git_tree_2);

	
	//branches_p->InsertItem(0, L"Item 0");
	//branches_p->SetText(L"Item 0");
	
	setup_git_branches_combo_();

	setup_ribbon_background_();
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

void CMainFrame::OnViewWorkspace4() 
{
	ShowControlBar (&m_wndWorkSpace4,
					!(m_wndWorkSpace4.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace4(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace4.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}

void CMainFrame::OnViewWorkspace41()
{
	ShowControlBar(&m_wndWorkSpace41,
		!(m_wndWorkSpace41.IsVisible()),
		FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewWorkspace41(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndWorkSpace41.IsVisible());
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
	ShowControlBar (&m_wndPropGrid,
					!(m_wndPropGrid.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewPropGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndPropGrid.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}
 // PROPERTYBAR
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

void CMainFrame::OnCbn_Git_Tree_Branches_SelchangeCombo()
{

}