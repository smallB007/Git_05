// outputbar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "Git_05.h"
#include "outputbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here
}

COutputBar::~COutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!m_wndTabs.Create (CBCGPTabWnd::STYLE_3D, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create list windows:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	m_wndList1.m_bVisualManagerStyle = TRUE;
	m_wndList2.m_bVisualManagerStyle = TRUE;
	m_wndList3.m_bVisualManagerStyle = TRUE;

	if (!m_wndList1.Create (dwViewStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndList2.Create (dwViewStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndList3.Create (dwViewStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}

	// Attach list windows to tab:
	m_wndTabs.AddTab (&m_wndList1, _T("Output 1"), -1);
	m_wndTabs.AddTab (&m_wndList2, _T("Output 2"), -1);
	m_wndTabs.AddTab (&m_wndList3, _T("Output 3"), -1);

	return 0;
}

void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	m_wndTabs.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
