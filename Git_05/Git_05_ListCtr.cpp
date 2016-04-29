#include "stdafx.h"
#include "Git_05_ListCtr.hpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(Git_05_ListCtr, CListCtrl)

BEGIN_MESSAGE_MAP(Git_05_ListCtr, CListCtrl)
	//{{AFX_MSG_MAP(CBCGPListCtrl)
	//ON_WM_CREATE()
	//ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//ON_WM_ERASEBKGND()
	//ON_WM_SYSCOLORCHANGE()
	//ON_WM_SIZE()
	//ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_PRINT, OnPrint)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//ON_MESSAGE(WM_STYLECHANGED, OnStyleChanged)
	//ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
END_MESSAGE_MAP()


void Git_05_ListCtr::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW & nm = *(LPNMLISTVIEW)pNMHDR;  // (1)
	int item = nm.iItem; // idx of item that changed
	bool wasSelected = (nm.uOldState & LVIS_SELECTED) != 0;
	bool  isSelected = (nm.uNewState & LVIS_SELECTED) != 0;
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	RECT prc;
	ListView_GetItemRect(m_hWnd, item, &prc, LVIR_BOUNDS);
	CRect rect;
	GetClientRect(rect);
	prc.right = rect.right;
	
// static 	CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
//  	CBrush brush_back_ground;
//  	brush_back_ground.CreateSysColorBrush(2);
//  	pDC->FillRect(&prc, &brush_back_ground);

	InvalidateRect(&prc);
	// (2)
}
//int Git_05_ListCtr::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//
//	//renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);
//	return 0;
//}

Git_05_ListCtr::Git_05_ListCtr() 
{
}


Git_05_ListCtr::~Git_05_ListCtr()
{
}

// COLORREF Git_05_ListCtr::OnGetCellTextColor(int /*nRow*/, int /*nColum*/)
// {
// 	return GetTextColor();
// }
// 
// COLORREF Git_05_ListCtr::OnGetCellBkColor(int /*nRow*/, int /*nColum*/)
// {
// 	return GetDefaultBkColor();
// }
// 
// COLORREF Git_05_ListCtr::GetDefaultBkColor()
// {
// 	return GetBkColor();
// }

// COLORREF Git_05_ListCtr::GetMarkedColor()
// {
// 	return  m_clrSortedColumn;
// }

void Git_05_ListCtr::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// Take the default processing unless we 
	// set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		//*pResult = CDRF_NOTIFYITEMDRAW;
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_POSTPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		break;
	case CDDS_ITEMPREPAINT:
	{
		RECT prc;
		ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
		if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
		{
			pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
			
			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CRect rect;
			GetClientRect(&rect);
			prc.right = rect.right;
			//pDC->DrawEdge(&prc, EDGE_BUMP, BF_RECT);

			COLORREF red;
			COLORREF green;
			COLORREF blue;
			red = RGB(255, 0, 0);
			green = RGB(0, 255, 0);
			blue = RGB(0, 0, 255);
			CBrush brush_back_ground;
			brush_back_ground.CreateSysColorBrush(2);

			LOGBRUSH br;
			brush_back_ground.GetLogBrush(&br);
			pDC->FillRect(&prc, &brush_back_ground);
			//pDC->FillRect(&prc, &brush_back_ground);//:AC: must be called twice otherwise the first selection will be only column width
			
			COLORREF crText = br.lbColor;
			pLVCD->clrTextBk = crText;
			pLVCD->clrFace = crText;
			//pLVCD->clrText = green;
			
			
		}
		else
		{
 			COLORREF background_color{ RGB(200, 200, 200) };
// 			//////////
// 			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// 			CRect rect;
// 			GetClientRect(&rect);
// 			prc.right = rect.right;
// 
// 			
// 			rect.bottom = prc.bottom;
// 			rect.top = prc.top;
// 			rect.left = prc.left;
// 			rect.right = prc.right;
// 
// 			CPoint pos(::GetMessagePos());
// 			ScreenToClient(&pos);
// 			if (rect.PtInRect(pos))
// 			{
// 				pDC->DrawEdge(&prc, EDGE_BUMP, BF_RECT);
// 			}
// 			else
// 			{
// 				
// 				InvalidateRect(&prc);
// 			}
			
			pLVCD->clrTextBk = background_color;
			pLVCD->clrFace = background_color;

// 			COLORREF red;
// 			COLORREF green;
// 			COLORREF blue;
// 			red = RGB(255, 0, 0);
// 			green = RGB(0, 255, 0);
// 			blue = RGB(0, 0, 255);
// 			CBrush brush_back_ground;
// 			brush_back_ground.CreateSysColorBrush(2);

// 			LOGBRUSH br;
// 			brush_back_ground.GetLogBrush(&br);
// 
// 			pDC->FillRect(&prc, &brush_back_ground);
// 			COLORREF crText = br.lbColor;
			//pLVCD->clrTextBk = crText;
			//pLVCD->clrFace = crText;
			//pLVCD->clrText = green;

			///////////
		}
			*pResult = CDRF_DODEFAULT;
	}
		break;
	case CDDS_ITEMPOSTPAINT:
	{
// 		RECT prc;
// 		ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
// 		COLORREF background_color{ RGB(0,0, 255) };
// 		pLVCD->clrTextBk = background_color;
// 		pLVCD->clrFace = background_color;
		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
	
}

void Git_05_ListCtr::OnLButtonUp(UINT flag, CPoint point)
{
	CListCtrl::OnLButtonUp(flag, point);
// 	CPaintDC dc(this);
// 	CRect rect;
// 	GetClientRect(rect);
// 	COLORREF crText;
// 
// 	crText = RGB(0, 255, 0);
// 	dc.FillSolidRect(rect, crText);
}

void Git_05_ListCtr::OnMouseHover(UINT flag, CPoint point)
{
	//CListCtrl::OnLButtonUp(flag, point);
}
