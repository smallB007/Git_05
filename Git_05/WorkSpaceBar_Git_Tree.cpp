// workspace2.cpp : implementation of the CWorkSpaceBar_Git_Tree class
//

#include "stdafx.h"
#include "Git_05.h"
#include "WorkSpaceBar_Git_Tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree

BEGIN_MESSAGE_MAP(CWorkSpaceBar_Git_Tree, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
#ifdef MFC_DIRECT_2D
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CWorkSpaceBar_Git_Tree::OnDrawDirect2D)
#else
	ON_WM_PAINT()
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree construction/destruction

CWorkSpaceBar_Git_Tree::CWorkSpaceBar_Git_Tree()
{
	// TODO: add one-time construction code here
	EnableD2DSupport();
}

CWorkSpaceBar_Git_Tree::~CWorkSpaceBar_Git_Tree()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree message handlers

int CWorkSpaceBar_Git_Tree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	//m_pRender = new Direct2DHandler(m_hWnd, D2D1::ColorF::Aqua);//make it shared
	m_pRender = std::make_unique<Direct2DHandler>(m_hWnd, D2D1::ColorF::Aqua);//make it shared
	//m_pRender->Initialize();
// 	CRect rectDummy;
// 	rectDummy.SetRectEmpty();
// 	
// 	// Create tree control:
// 	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
// 								TVS_LINESATROOT | TVS_HASBUTTONS;
// 	
// 	m_wndTree.m_bVisualManagerStyle = TRUE;
// 
// 	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
// 	{
// 		TRACE0("Failed to create workspace view\n");
// 		return -1;      // fail to create
// 	}
// 
// 	// Setup tree content:
// 	HTREEITEM hRoot = m_wndTree.InsertItem(_T("Root 1"));
// 	m_wndTree.InsertItem(_T("Item 1"), hRoot);
// 	m_wndTree.InsertItem(_T("Item 2"), hRoot);
// 	
// 	m_wndTree.Expand(hRoot, TVE_EXPAND);

	return 0;
}

void CWorkSpaceBar_Git_Tree::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	//GetRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod, 1.0f));
	// Tree control should cover a whole client area:
// 	m_wndTree.SetWindowPos(NULL, nBorderSize, nBorderSize, 
// 		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
// 		SWP_NOACTIVATE | SWP_NOZORDER);
}

#ifdef MFC_DIRECT_2D
afx_msg LRESULT CWorkSpaceBar_Git_Tree::OnDrawDirect2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod, 1.0f));
	return TRUE;
}
#else
void CWorkSpaceBar_Git_Tree::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;

	if (m_pRender)
		m_pRender->OnRender();
// 	m_wndTree.GetWindowRect(rectTree);
// 	ScreenToClient(rectTree);
// 
// 	rectTree.InflateRect(nBorderSize, nBorderSize);
// 
// 	dc.Draw3dRect(rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
}
#endif