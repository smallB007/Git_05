// workspace2.cpp : implementation of the CWorkSpaceBar2 class
//

#include "stdafx.h"
#include "Git_05.h"
#include "WorkSpaceBar2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2

BEGIN_MESSAGE_MAP(CWorkSpaceBar2, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 construction/destruction

CWorkSpaceBar2::CWorkSpaceBar2()
{
	// TODO: add one-time construction code here
}

CWorkSpaceBar2::~CWorkSpaceBar2()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 message handlers

int CWorkSpaceBar2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	// Create tree control:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	m_wndTree.m_bVisualManagerStyle = TRUE;

	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup tree content:
	HTREEITEM hRoot = m_wndTree.InsertItem(_T("Root 1"));
	m_wndTree.InsertItem(_T("Item 1"), hRoot);
	m_wndTree.InsertItem(_T("Item 2"), hRoot);
	
	m_wndTree.Expand(hRoot, TVE_EXPAND);

	return 0;
}

void CWorkSpaceBar2::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tree control should cover a whole client area:
	m_wndTree.SetWindowPos(NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(nBorderSize, nBorderSize);

	dc.Draw3dRect(rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
}
