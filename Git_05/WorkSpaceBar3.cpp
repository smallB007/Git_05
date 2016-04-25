// workspace.cpp : implementation of the CWorkSpaceBar3 class
//

#include "stdafx.h"
#include "Git_05.h"
#include "WorkSpaceBar3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 0;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar3

BEGIN_MESSAGE_MAP(CWorkSpaceBar3, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
#ifdef MFC_DIRECT_2D
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CWorkSpaceBar3::OnDrawDirect2D)
#else
	ON_WM_PAINT()
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar3 construction/destruction

CWorkSpaceBar3::CWorkSpaceBar3()
{
	// TODO: add one-time construction code here
	//EnableD2DSupport();
}

CWorkSpaceBar3::~CWorkSpaceBar3()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar3 message handlers

int CWorkSpaceBar3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tree control:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES |
		TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndTree.m_bVisualManagerStyle = TRUE;

	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup tree content:
	/*HTREEITEM */hRoot_ = m_wndTree.InsertItem(_T("Repositories"));
	// 	m_wndTree.InsertItem(_T("Item 1"), hRoot_);
	// 	m_wndTree.InsertItem(_T("Item 2"), hRoot_);
	// 	
	// 	m_wndTree.Expand(hRoot_, TVE_EXPAND);
	fill_repositories_();
	return 0;
}

void CWorkSpaceBar3::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tree control should cover a whole client area:
	m_wndTree.SetWindowPos(NULL, nBorderSize, nBorderSize,
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}



#ifdef MFC_DIRECT_2D
LRESULT CWorkSpaceBar3::OnDrawDirect2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;

	CRect rectTree;
	m_wndTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);
	rectTree.InflateRect(nBorderSize, nBorderSize);
	return LRESULT();

}
#else
void CWorkSpaceBar3::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(nBorderSize, nBorderSize);

	dc.Draw3dRect(rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
}
#endif // MFC_DIRECT_2D

void CWorkSpaceBar3::fill_repositories_()
{
	CGit_05App* main_app_p = get_main_app_();
	const HTTPS_GIT_Client* https_git_client_p = main_app_p->get_https_git_client_p();
	std::set<Git_Repository, Less<Git_Repository>> repos = https_git_client_p->current_user_repositories();

	for (auto repo : repos)
	{
		auto repo_name = repo.get_attribute(_T("name"));
		m_wndTree.InsertItem(repo_name.c_str(), hRoot_);
	}

	m_wndTree.Expand(hRoot_, TVE_EXPAND);
}