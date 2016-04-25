// workspace2.cpp : implementation of the CWorkSpaceBar_Git_Tree2 class
//

#include "stdafx.h"
#include "Git_05.h"
#include "WorkSpaceBar_Git_Tree2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree2

BEGIN_MESSAGE_MAP(CWorkSpaceBar_Git_Tree2, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree2 construction/destruction

CWorkSpaceBar_Git_Tree2::CWorkSpaceBar_Git_Tree2()
{
	// TODO: add one-time construction code here
}

CWorkSpaceBar_Git_Tree2::~CWorkSpaceBar_Git_Tree2()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree2 message handlers

int CWorkSpaceBar_Git_Tree2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);

	return renderer_ ? 0 : -1;
}

void CWorkSpaceBar_Git_Tree2::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	renderer_->Resize(cx, cy);
}

void CWorkSpaceBar_Git_Tree2::OnPaint() 
{
	renderer_->DrawClientArea();
}
