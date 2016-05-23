#include "stdafx.h"
#include "Git_05_ListCtr.hpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//https://msdn.microsoft.com/en-us/library/ms364048%28v=vs.80%29.aspx

//http://stackoverflow.com/questions/13071965/how-to-force-a-clistctrl-to-alway-have-one-item-selected
//http://www.codeproject.com/Articles/79/Neat-Stuff-to-Do-in-List-Controls-Using-Custom-Dra

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
	ON_NOTIFY_REFLECT(NM_HOVER, OnMouseHover)
	
	//ON_MESSAGE(WM_STYLECHANGED, OnStyleChanged)
	//ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	//ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnClick)//same as above, the only purpose of it is to keep item on list selected in case click will occur outside of list items
	ON_NOTIFY_REFLECT(NM_RCLICK, OnClick)//same as above, the only purpose of it is to keep item on list selected in case click will occur outside of list items
	ON_NOTIFY_REFLECT(NM_RDBLCLK, OnClick)//same as above, the only purpose of it is to keep item on list selected in case click will occur outside of list items
	ON_WM_DESTROY()
END_MESSAGE_MAP()




LRESULT Git_05_ListCtr::OnMouseLeave(WPARAM, LPARAM)
{
	/*This surprisingly prevents focus from current selection to be lost, yuppie ;)*/
	//CRect rect;
	//	GetClientRect(rect);
	//	InvalidateRect(&rect);
	for (int i{ 0 }, end = GetItemCount(); i < end; ++i)
	{
		SetItemState(i, 0, LVIS_SELECTED);
		//SetHotItem(lastItem_);
		//if (i == lastItem_)
		//{
		//}
		//else
		//{
		//	SetItemState(i, ~LVIS_SELECTED, ~LVIS_SELECTED);
		//
		//}
	}
	return TRUE;
}
#include "MainFrm.h"
#include "WorkSpaceBar4.h"
void Git_05_ListCtr::selectItem(const int inx)
{
	LVITEMW pitem;
	ZeroMemory(&pitem, sizeof(pitem));
	pitem.mask = LVIF_IMAGE;
	pitem.iItem = inx;
	pitem.iSubItem = 0;
	pitem.iImage = 1;
	SetItem(&pitem);
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
	CString commit_id = GetItemText(inx, 3);
	pMainWnd->set_info_for_commit(commit_id);
}
void Git_05_ListCtr::selectItem(const repo_name_t& itemName)
{
	//CString c_repo_name(itemName.c_str());

	for (int i{ 0 }, end = GetItemCount(); i < end; ++i)
	{
		auto c_txt = GetItemText(i,4);//4 is a hidden column in repo with fullpath as a repo name 0 is the short name
		if (c_txt == itemName)
		{
			lastItem_ = i;

			LVITEMW pitem;
			ZeroMemory(&pitem, sizeof(pitem));
			pitem.mask = LVIF_IMAGE;
			pitem.iItem = i;
			pitem.iSubItem = 0;
			pitem.iImage = 1;
			SetItem(&pitem);
		}
	}
}


void Git_05_ListCtr::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CRect rect;
	GetClientRect(rect);
	CPoint pos(::GetMessagePos());
	ScreenToClient(&pos);
	RECT prc;
	ListView_GetItemRect(m_hWnd, 0, &prc, LVIR_BOUNDS);//take item on 0 index just to get its height
	auto item_number = pos.y / prc.bottom;
	lastItem_ = item_number;
	if (item_number > GetItemCount())
	{
		lastItem_ = GetItemCount() - 1;
		item_number = lastItem_;
	}

	SetItemState(item_number, LVIS_SELECTED, LVIS_SELECTED);
	auto git_entity_name = GetItemText(item_number, 0);
	
	CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());

	pMainWnd->reset_view();
	
	if (git_entity_type_ == GIT_ENTITY_TYPE::REPO)
	{
		//1. Populate combo box with branch names
		pMainWnd->set_branches_for_repo(git_entity_name);
		
	}
	else if (git_entity_type_ == GIT_ENTITY_TYPE::COMMIT)
	{
		
		auto commit_id = GetItemText(item_number, 3);//3 because it is a third column set in CWorkSpaceBar4::set_type_list_ctrl_commits()
		//auto repo_name = parent_->get_current_repo();
		//auto branch_name = parent_->get_current_branch();
		//pMainWnd->set_current_repo(commit_id);
		pMainWnd->set_info_for_commit(commit_id);
	}
	else if (git_entity_type_ == GIT_ENTITY_TYPE::WORKING_DIR)
	{
		//list untracked files
	}

	for (int i{ 0 }, end = GetItemCount(); i < end; ++i)
	{
		LVITEMW pitem;
		ZeroMemory(&pitem, sizeof(pitem));
		pitem.mask = LVIF_IMAGE;
		pitem.iItem = i;
		pitem.iSubItem = 0;

		if (i == item_number)
		{
			pitem.iImage = 1;
			//lastItem_ = i;
		}
		else
		{
			pitem.iImage = 2;
		}
		SetItem(&pitem);
	}
	return;
}

void Git_05_ListCtr::OnKillFocus(CWnd*)
{
	/*This surprisingly prevents focus from current selection to be lost, yuppie ;)*/
	//CRect rect;
	//	GetClientRect(rect);
	//	InvalidateRect(&rect);
	//SetItemState(lastItem_, LVIS_SELECTED, LVIS_SELECTED);
}

void Git_05_ListCtr::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW & nm = *(LPNMLISTVIEW)pNMHDR;  // (1)
		//lastItem_ = nm.iItem;//store the item so in case click outside of list items we can select it again
		int item = nm.iItem; // idx of item that changed
		//bool wasSelected = (nm.uOldState & LVIS_SELECTED) != 0;
		//bool  isSelected = (nm.uNewState & LVIS_SELECTED) != 0;
		RECT prc;
		ListView_GetItemRect(m_hWnd, item, &prc, LVIR_BOUNDS);
		CRect rect;
		GetClientRect(rect);
		prc.right = rect.right;
		InvalidateRect(&prc);
		if (git_entity_type_ == WORKING_DIR)
		{
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			*pResult = 0;

			if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
				return;    // No change


						   // Old check box state
			BOOL bPrevState = (BOOL)(((pNMListView->uOldState &
				LVIS_STATEIMAGEMASK) >> 12) - 1);
			if (bPrevState < 0)    // On startup there's no previous state 
				bPrevState = 0; // so assign as false (unchecked)

								// New check box state
			BOOL bChecked =
				(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1);
			if (bChecked < 0) // On non-checkbox notifications assume false
				bChecked = 0;

			if (bPrevState == bChecked) // No change in check box
				return;
		}
}

int Git_05_ListCtr::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_TRACKSELECT | LVS_EX_BORDERSELECT);
	 //GetSysColor(COLOR_3DFACE);
	SetBkColor(background_color_);
	SetHoverTime(hover_time_);

	SendMessage(WM_CHANGEUISTATE, MAKEWPARAM(UIS_INITIALIZE, 0), 0);//to remove dotted focus rectangle

	return 0;
}

Git_05_ListCtr::Git_05_ListCtr() 
{
	brush_highligth_.CreateSysColorBrush(COLOR_ACTIVECAPTION);
	brush_highligth_.GetLogBrush(&logic_brush_);
	brush_background_.CreateSysColorBrush(COLOR_3DFACE);
	LOGBRUSH logic_brush;
	brush_background_.GetLogBrush(&logic_brush);
	background_color_ = logic_brush.lbColor;
}


Git_05_ListCtr::~Git_05_ListCtr()
{
	
	//auto app = static_cast<CGit_05App*>(AfxGetApp());
	//CString current_repo = GetItemText(lastItem_, 0);
	//app->write_repo_name_to_file_(current_repo);
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
void Git_05_ListCtr::OnMouseHover(NMHDR* pNMHDR, LRESULT* pResult)
{
	//OnClick(pNMHDR, pResult);
	//OnItemChanged(pNMHDR, pResult);
// 	LPNMLISTVIEW nm = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
// 	if (nm->iItem < GetItemCount())
// 	{
// 		SetItemState(nm->iItem, LVIS_SELECTED, LVIS_SELECTED);
// 		
// 	}
// 	else
// 	{
// 		SetItemState(lastItem_, LVIS_SELECTED, LVIS_SELECTED);
// 	}
// 	return;
// 	CRect rect;
// 	GetClientRect(&rect);
// 	RECT prc;
// 	ListView_GetItemRect(m_hWnd, lastItem_, &prc, LVIR_BOUNDS);
// 	prc.right = rect.right;
// 
// 	CBrush brush_highligth;
// 	brush_highligth.CreateSysColorBrush(2);
// 
// 	LOGBRUSH logic_brush_;
// 	brush_highligth.GetLogBrush(&logic_brush_);
// 	auto pDC = GetDC();
// 
// 	pDC->FillRect(&prc, &brush_highligth);

}

void Git_05_ListCtr::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// Take the default processing unless we 
	// set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	
		RECT prc;
		ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
		CRect rect;
		GetClientRect(&rect);
		prc.right = rect.right;

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
		{

		}
			break;
		case CDDS_ITEMPREPAINT:
		{
			//RECT prc;
			//ListView_GetItemRect(m_hWnd, pLVCD->nmcd.dwItemSpec, &prc, LVIR_BOUNDS);
			if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
			{
				pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
				pLVCD->nmcd.lItemlParam = 1;//set it here so it can communicate with postpaint

				CDC*  pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
				CRect rect;
				GetClientRect(&rect);
				prc.right = rect.right;
				
				//CBrush brush_highligth;
				//brush_highligth.CreateSysColorBrush(2);

				LOGBRUSH logic_brush_;
				brush_highligth_.GetLogBrush(&logic_brush_);
				pDC->FillRect(&prc, &brush_highligth_);
				
				COLORREF crText = logic_brush_.lbColor;
				pLVCD->clrTextBk = crText;
				pLVCD->clrFace = crText;
				pLVCD->iIconEffect = 0;//no effect
			}
			else
			{
				pLVCD->clrTextBk = background_color_;
				pLVCD->clrFace = background_color_;
			}
			*pResult = CDRF_NOTIFYPOSTPAINT;
		}
		break;
		case CDDS_ITEMPOSTPAINT:
		{


			//LVITEMW pitem;
			//ZeroMemory(&pitem, sizeof(pitem));
			//pitem.mask = LVIF_IMAGE;
			//GetItem(&pitem);
			
			//if (pitem.state == 1)
			//{
			
				//if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
					if (pLVCD->nmcd.lItemlParam == 1)
				{
				CDC dc;
				CBrush br;
				br.CreateSysColorBrush(COLOR_HIGHLIGHT);
				dc.Attach(pLVCD->nmcd.hdc);
				dc.FrameRect(&prc, &br);
				//dc.DrawEdge(&prc, 1, 1);
				dc.Detach();
			}
			//}
			*pResult = CDRF_DODEFAULT;
		}
		break;
		}
	
	
}

// void Git_05_ListCtr::OnLButtonUp(UINT flag, CPoint point)
// {
// 	CListCtrl::OnLButtonUp(flag, point);
// // 	CPaintDC dc(this);
// // 	CRect rect;
// // 	GetClientRect(rect);
// // 	COLORREF crText;
// // 
// // 	crText = RGB(0, 255, 0);
// // 	dc.FillSolidRect(rect, crText);
// }



void Git_05_ListCtr::OnDestroy()
{
	if (GIT_ENTITY_TYPE::REPO == git_entity_type_)
	{
		CString repo_name = GetItemText(lastItem_, 4);//4 is the hidden column with full repo name, 0 is short version of that name
		write_repo_name_to_file_(repo_name);
	}
	CListCtrl::OnDestroy();
	// TODO: Add your message handler code here
}

void Git_05_ListCtr::write_repo_name_to_file_(const CString& repoName)const
{
	if (!repoName.IsEmpty())
	{
		std::ofstream f_out(file_with_repo_to_set_as_active_);
		if (f_out)
		{
			CT2CA ctstring(repoName);
			std::string repo(ctstring);
			f_out << repo;
		}
	}
}

void Git_05_ListCtr::read_repo_name_from_file_()
{
	std::ifstream f_in(file_with_repo_to_set_as_active_);
	std::string active_repo;
	if (f_in)
	{
		f_in >> active_repo;
	}
	if (active_repo.size())
	{
		if (GIT_ENTITY_TYPE::REPO == git_entity_type_)
		{

			CMainFrame *pMainWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
			CA2W w_str(active_repo.c_str());
			CString current_repo(w_str);
			pMainWnd->set_current_repo(current_repo);
		}
	}
}

void Git_05_ListCtr::set_active_repo()
{
	read_repo_name_from_file_();
}

void Git_05_ListCtr::set_active_commit()
{
	selectItem(0);
}

void Git_05_ListCtr::AdjustColumnWidth()
{
	SetRedraw(FALSE);
	int nColumnCount = GetColumnCount();

	for (int i = 0; i < nColumnCount; i++)
	{
		SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = GetColumnWidth(i);
		SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = GetColumnWidth(i);
		SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
	}
	SetRedraw(TRUE);
}
int Git_05_ListCtr::GetColumnCount()
{
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	return (pHeaderCtrl->GetItemCount());
}