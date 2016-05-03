// DemoListView.cpp : implementation of the CDemoListView class
//
// Author:   Ovidiu Cucu
// Homepage: http://codexpert.ro/blog/
#if 0


#include "stdafx.h"
#include "resource.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
//#ifndef SHARED_HANDLERS
//#include "Demo.h"
//#endif

//#include "DemoDoc.h"
#include "DemoListView.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemoListView

IMPLEMENT_DYNCREATE(CDemoListView, CListView)

BEGIN_MESSAGE_MAP(CDemoListView, CListView)
	ON_COMMAND(ID_VIEW_LARGEICON, &CDemoListView::OnViewLargeicon)
	ON_COMMAND(ID_VIEW_SMALLICON, &CDemoListView::OnViewSmallicon)
	ON_COMMAND(ID_VIEW_LIST, &CDemoListView::OnViewList)
	ON_COMMAND(ID_VIEW_DETAILS, &CDemoListView::OnViewDetails)
	ON_COMMAND(ID_VIEW_TILE, &CDemoListView::OnViewTile)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LARGEICON, &CDemoListView::OnUpdateViewLargeicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLICON, &CDemoListView::OnUpdateViewSmallicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, &CDemoListView::OnUpdateViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, &CDemoListView::OnUpdateViewDetails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TILE, &CDemoListView::OnUpdateViewTile)
END_MESSAGE_MAP()

// CDemoView construction/destruction

CDemoListView::CDemoListView()
{
	// TODO: add construction code here

}

CDemoListView::~CDemoListView()
{
}

BOOL CDemoListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CDemoListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);

	_CreateImageLists();
	_InitAndFillList();

	listCtrl.SetView(LV_VIEW_TILE);

}

void CDemoListView::_CreateImageLists()
{
	ASSERT(NULL == m_imageList.m_hImageList);      // init only once
	ASSERT(NULL == m_imageListSmall.m_hImageList); // init only once

	CWinApp* pApp = AfxGetApp();
	VERIFY(m_imageList.Create(128, 128, ILC_COLOR32, 0, 0));
	VERIFY(m_imageListSmall.Create(48, 48, ILC_COLOR32, 0, 0));

	m_imageList.Add(pApp->LoadIcon(IDI_CAT));
	m_imageList.Add(pApp->LoadIcon(IDI_DOG));
	m_imageList.Add(pApp->LoadIcon(IDI_ELEPHANT));

	m_imageListSmall.Add(pApp->LoadIcon(IDI_CAT_SMALL));
	m_imageListSmall.Add(pApp->LoadIcon(IDI_DOG_SMALL));
	m_imageListSmall.Add(pApp->LoadIcon(IDI_ELEPHANT_SMALL));
}

void CDemoListView::_InitAndFillList()
{
	CListCtrl& listCtrl = GetListCtrl();

	listCtrl.SetImageList(&m_imageList, LVSIL_NORMAL);
	listCtrl.SetImageList(&m_imageListSmall, LVSIL_SMALL);
	listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);

	listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 100, 0);
	listCtrl.InsertColumn(1, _T("Age"), LVCFMT_RIGHT, 100, 1);
	listCtrl.InsertColumn(2, _T("Owner"), LVCFMT_LEFT, 150, 2);
	listCtrl.InsertColumn(3, _T("City, Country"), LVCFMT_LEFT, 200, 3);

	listCtrl.InsertItem(0, _T("Martafoi"), 0);
	listCtrl.InsertItem(1, _T("Zdreanta"), 1);
	listCtrl.InsertItem(2, _T("Jumbo"), 2);

	listCtrl.SetItemText(0, 1, _T("8 months"));
	listCtrl.SetItemText(1, 1, _T("7 years"));
	listCtrl.SetItemText(2, 1, _T("35 years"));

	listCtrl.SetItemText(0, 2, _T("John Doe"));
	listCtrl.SetItemText(1, 2, _T("Brigitte Bardot"));
	listCtrl.SetItemText(2, 2, _T("Hannibal Barcas"));

	listCtrl.SetItemText(0, 3, _T("New York, USA"));
	listCtrl.SetItemText(1, 3, _T("Paris, France"));
	listCtrl.SetItemText(2, 3, _T("Barcelona, Spain"));

	VERIFY(_SetTilesViewLinesCount(3));
	VERIFY(_SetTilesViewTileFixedWidth(250));

	UINT arrColumns[3] = { 1, 2, 3 };
	VERIFY(_SetItemTileLines(0, arrColumns, 3));
	VERIFY(_SetItemTileLines(1, arrColumns, 3));
	VERIFY(_SetItemTileLines(2, arrColumns, 3));
}

BOOL CDemoListView::_SetTilesViewLinesCount(int nCount)
{
	CListCtrl& listCtrl = GetListCtrl();

	LVTILEVIEWINFO lvtvwi = { 0 };
	lvtvwi.cbSize = sizeof(LVTILEVIEWINFO);
	lvtvwi.dwMask = LVTVIM_COLUMNS;
	lvtvwi.cLines = nCount;

	return listCtrl.SetTileViewInfo(&lvtvwi);
}

BOOL CDemoListView::_SetTilesViewTileFixedWidth(int nWidth)
{
	CListCtrl& listCtrl = GetListCtrl();

	LVTILEVIEWINFO lvtvwi = { 0 };
	lvtvwi.cbSize = sizeof(LVTILEVIEWINFO);
	lvtvwi.dwMask = LVTVIM_TILESIZE;

	lvtvwi.dwFlags = LVTVIF_FIXEDWIDTH;
	lvtvwi.sizeTile.cx = nWidth;
	lvtvwi.sizeTile.cy = 0;

	return listCtrl.SetTileViewInfo(&lvtvwi);
}

BOOL CDemoListView::_SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount)
{
	CListCtrl& listCtrl = GetListCtrl();

	LVTILEINFO lvti = { 0 };
	lvti.cbSize = sizeof(LVTILEINFO);
	lvti.cColumns = nCount;
	lvti.iItem = nItem;
	lvti.puColumns = parrColumns;

	return listCtrl.SetTileInfo(&lvti);
}


// CDemoListView message handlers

void CDemoListView::OnViewLargeicon()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetView(LV_VIEW_ICON);
}

void CDemoListView::OnViewSmallicon()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetView(LV_VIEW_SMALLICON);
}

void CDemoListView::OnViewList()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetView(LV_VIEW_LIST);
}

void CDemoListView::OnViewDetails()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetView(LV_VIEW_DETAILS);
}

void CDemoListView::OnViewTile()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetView(LV_VIEW_TILE);
}

void CDemoListView::OnUpdateViewLargeicon(CCmdUI *pCmdUI)
{
	CListCtrl& listCtrl = GetListCtrl();
	const DWORD dwView = listCtrl.GetView();
	pCmdUI->SetCheck(LV_VIEW_ICON == dwView);
}

void CDemoListView::OnUpdateViewSmallicon(CCmdUI *pCmdUI)
{
	CListCtrl& listCtrl = GetListCtrl();
	const DWORD dwView = listCtrl.GetView();
	pCmdUI->SetCheck(LV_VIEW_SMALLICON == dwView);
}

void CDemoListView::OnUpdateViewList(CCmdUI *pCmdUI)
{
	CListCtrl& listCtrl = GetListCtrl();
	const DWORD dwView = listCtrl.GetView();
	pCmdUI->SetCheck(LV_VIEW_LIST == dwView);
}

void CDemoListView::OnUpdateViewDetails(CCmdUI *pCmdUI)
{
	CListCtrl& listCtrl = GetListCtrl();
	const DWORD dwView = listCtrl.GetView();
	pCmdUI->SetCheck(LV_VIEW_DETAILS == dwView);
}

void CDemoListView::OnUpdateViewTile(CCmdUI *pCmdUI)
{
	CListCtrl& listCtrl = GetListCtrl();
	const DWORD dwView = listCtrl.GetView();
	pCmdUI->SetCheck(LV_VIEW_TILE == dwView);
}
#endif