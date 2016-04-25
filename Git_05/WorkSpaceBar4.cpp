// workspace.cpp : implementation of the CWorkSpaceBar4 class
//

#include "stdafx.h"
#include "Git_05.h"
#include "WorkSpaceBar4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 0;

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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4 construction/destruction

CWorkSpaceBar4::CWorkSpaceBar4()
{
	// TODO: add one-time construction code here
	//EnableD2DSupport();
}

void CWorkSpaceBar4::set_view_type(EVIEW_TYPE view_type)
{
	eview_type_ = view_type;
}

CWorkSpaceBar4::~CWorkSpaceBar4()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar4 message handlers

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
		return create_list_ctrl_();
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
			m_wndListCtrl.SetWindowPos(NULL, nBorderSize, nBorderSize,
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
		CRect rectTree;
		m_wndListCtrl.GetWindowRect(rectTree);
		ScreenToClient(rectTree);

		rectTree.InflateRect(nBorderSize, nBorderSize);

		dc.Draw3dRect(rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
	}
	break;
	case EVIEW_TYPE::GIT_TREE:
	{
		renderer_->DrawClientArea();
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

int CWorkSpaceBar4::create_list_ctrl_()
{
	// Create list control:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;// | TVS_HASLINES |
													//TVS_LINESATROOT | TVS_HASBUTTONS;

													//m_wndListCtrl.m_bVisualManagerStyle = TRUE;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndListCtrl.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	CRect rect;
	m_wndListCtrl.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_wndListCtrl.InsertColumn(0, _T("Item Name"), LVCFMT_LEFT, nColInterval * 3);
	m_wndListCtrl.InsertColumn(1, _T("Value"), LVCFMT_LEFT, nColInterval);
	m_wndListCtrl.InsertColumn(2, _T("Time"), LVCFMT_LEFT, rect.Width() - 4 * nColInterval);



	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(32, 32, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_cImageListNormal.Attach(hList);

	hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_cImageListSmall.Attach(hList);

	// Load the large icons
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_IMAGES_NORMAL);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	// Load the small icons
	cBmp.LoadBitmap(IDB_IMAGES_NORMAL);
	m_cImageListSmall.Add(&cBmp, RGB(255, 0, 255));

	m_wndListCtrl.SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	m_wndListCtrl.SetImageList(&m_cImageListSmall, LVSIL_SMALL);

	LVITEM lvi;
	CString strItem;
	for (int i = 0; i < 30; i++)
	{
		// Insert the first item
		lvi.mask = LVIF_IMAGE | LVIF_TEXT;
		strItem.Format(_T("Item %i"), i);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		lvi.iImage = i % 8;		// There are 8 images in the image list
		m_wndListCtrl.InsertItem(&lvi);
		// Set subitem 1
		strItem.Format(_T("%d"), 10 * i);
		lvi.iSubItem = 1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_wndListCtrl.SetItem(&lvi);
		// Set subitem 2
		strItem.Format(_T("%s"),
			COleDateTime::GetCurrentTime().Format(_T("Created: %I:%M:%S %p, %m/%d/%Y")));
		lvi.iSubItem = 2;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_wndListCtrl.SetItem(&lvi);
	}

	m_wndListCtrl.SetView(LV_VIEW_TILE);
}