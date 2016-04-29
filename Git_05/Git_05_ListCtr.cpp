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
	ON_WM_CREATE()
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
	
END_MESSAGE_MAP()

int Git_05_ListCtr::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;


	//renderer_ = std::make_unique<Direct2DRenderer>(m_hWnd);
	return 0;
}

Git_05_ListCtr::Git_05_ListCtr() 
{
}


Git_05_ListCtr::~Git_05_ListCtr()
{
}

COLORREF Git_05_ListCtr::OnGetCellTextColor(int /*nRow*/, int /*nColum*/)
{
	return GetTextColor();
}

COLORREF Git_05_ListCtr::OnGetCellBkColor(int /*nRow*/, int /*nColum*/)
{
	return GetDefaultBkColor();
}

COLORREF Git_05_ListCtr::GetDefaultBkColor()
{
	return GetBkColor();
}

COLORREF Git_05_ListCtr::GetMarkedColor()
{
	return  m_clrSortedColumn;
}

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
		*pResult = CDRF_DODEFAULT;
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
			pDC->DrawEdge(&prc, EDGE_BUMP, BF_RECT);



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

			COLORREF crText = br.lbColor;;
			pLVCD->clrTextBk = crText;
			pLVCD->clrFace = crText;

			pDC->FillRect(&prc, &brush_back_ground);
			pDC->Draw3dRect(&prc, red, red);

		}
		else
		{
			COLORREF background_color{ RGB(200, 200, 200) };
			pLVCD->clrTextBk = background_color;
			pLVCD->clrFace = background_color;
// 			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// 			CRect rect;
// 			GetClientRect(&rect);
// 			prc.right = rect.right;
// 			pDC->DrawEdge(&prc, EDGE_BUMP, BF_RECT);
// 
// 
// 
// 			COLORREF red;
// 			COLORREF green;
// 			COLORREF blue;
// 			red = RGB(255, 0, 0);
// 			green = RGB(0, 255, 0);
// 			blue = RGB(0, 0, 255);
// 			CBrush brush_back_ground;
// 			brush_back_ground.CreateSysColorBrush(2);
// 
// 			LOGBRUSH br;
// 			brush_back_ground.GetLogBrush(&br);
// 
// 			COLORREF crText = br.lbColor;;
// 			pDC->FillRect(&prc, &brush_back_ground);
// 			pDC->Draw3dRect(&prc, red, red);
// 
// 			pLVCD->clrTextBk = crText;
// 			pLVCD->clrFace = crText;

		}
			*pResult = CDRF_DODEFAULT;
	}
		break;
	//case CDDS_ITEMPREPAINT:
// 		if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
// 		{
// 			//pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
// // 			RECT prc;
// // 			ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
// // 
// // 			
// // 			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// // 			CRect rect;
// // 			GetClientRect(&rect);
// // 			prc.right = rect.right;
// // 
// // 			COLORREF crText;
// // 			crText = RGB(0, 0, 150);
// // 			pLVCD->clrTextBk = crText;
// // 			pLVCD->clrFace = crText;
// // 			pDC->FillSolidRect(&prc, crText);
// // 			
// // 			crText = RGB(255, 0, 0);
// // 			pLVCD->clrText = crText;
// // 
//  			*pResult = CDRF_DODEFAULT;
// 		}
// 		else
// 		{
// 			RECT prc;
// 			ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
// 			
// 			
// // 			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// // 			CRect rect;
// // 			GetClientRect(&rect);
// // 			prc.right = rect.right;
// // 			pDC->DrawEdge(&prc, EDGE_BUMP, BF_RECT);
// // 
// // 			COLORREF red;
// // 			COLORREF green;
// // 			COLORREF blue;
// // 			red = RGB(255, 0, 0);
// // 			green = RGB(0, 255, 0);
// // 			blue = RGB(0, 0, 255);
// // 			CBrush brush_back_ground;
// // 			brush_back_ground.CreateSysColorBrush(2);
// // 			pDC->FillRect(&prc, &brush_back_ground);
// // 			pDC->Draw3dRect(&prc, red, red);
// 			
// 			//crText = RGB(0, 255, 0);
// 			//pLVCD->clrTextBk = crText;
// 			//pLVCD->clrFace = crText;
// 
// 			*pResult = CDRF_DODEFAULT;
// 		}
		//break;
// 	case CDDS_PREPAINT:
// 		*pResult = CDRF_NOTIFYITEMDRAW;
// 		break;
// 	case CDDS_ITEMPREPAINT:
// 		COLORREF crText;
// 		
// 		crText = RGB(128, 128, 255);
// 
// 		// Store the color back in the NMLVCUSTOMDRAW struct.
// 		pLVCD->clrTextBk = crText;
// 		pLVCD->clrFace = crText;
// 
// 		if (pLVCD->nmcd.uItemState & CDIS_SELECTED) 
// 		{
// 			pLVCD->nmcd.uItemState &= CDIS_MARKED; // Remove the item selection state from being drawn
// // 			RECT prc;
// // 			ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
// 			
// 			int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
// // 			LVITEM rItem;
// // 			ZeroMemory(&rItem, sizeof(LVITEM));
// // 			rItem.mask = LVIF_STATE;
// // 			rItem.iItem = nItem;
// // 			rItem.stateMask = LVIS_SELECTED;
// // 			this->GetItem(&rItem);
// // 			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// // 			CRect rcIcon;
// // 
// // 			// Get the rect that holds the item's icon.
// // 			this->GetItemRect(nItem, &rcIcon, LVIR_BOUNDS);
// // 			crText = RGB(255, 0, 0);
// // 			pDC->FillSolidRect(rcIcon, crText);
// 		}
// 
// 		//pLVCD->uAlign = LVGA_HEADER_CENTER;
// 		// Tell Windows to paint the control itself.
// 		*pResult = CDRF_DODEFAULT;
// 		break;
// 	case CDDS_ITEMPOSTPAINT:
// 	{
// 		LVITEM rItem;
// 		int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
// 		
// 		RECT prc;
// 		ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
// 		CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// 		//pDC->DrawEdge(&prc, 1, 0);
// 		crText = RGB(0, 255, 0);
// 		pDC->FillSolidRect(&prc, crText);
// 		*pResult = CDRF_SKIPDEFAULT;
// 
// 		// Get the image index and state of this item.  Note that we need to
// 		// check the selected state manually.  The docs _say_ that the
// 		// item's state is in pLVCD->nmcd.uItemState, but during my testing
// 		// it was always equal to 0x0201, which doesn't make sense, since
// 		// the max CDIS_* constant in commctrl.h is 0x0100.
// 
// // 		ZeroMemory(&rItem, sizeof(LVITEM));
// // 		rItem.mask = LVIF_STATE;
// // 		rItem.iItem = nItem;
// // 		rItem.stateMask = LVIS_SELECTED;
// // 		this->GetItem(&rItem);
// // 
// // 		// If this item is selected, redraw the icon with its normal colors.
// // 		if (rItem.state & LVIS_SELECTED)
// // 		{
// // 			CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
// // 			CRect rcIcon;
// // 
// // 			// Get the rect that holds the item's icon.
// // 			this->GetItemRect(nItem, &rcIcon, LVIR_ICON);
// // 
// // 			// Draw the icon.
// // // 			m_imglist.Draw(pDC, rItem.iImage, rcIcon.TopLeft(),
// // // 				ILD_TRANSPARENT);
// // 			COLORREF crText;
// // 
// // 			crText = RGB(255, 0, 0);
// // 			//rcIcon.InflateRect(2, 2, 2, 2);
// // 			pDC->DrawEdge(rcIcon,2,0);
// // 
// // 			*pResult = CDRF_SKIPDEFAULT;
// // 		}
// 		break;
// 	}
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
