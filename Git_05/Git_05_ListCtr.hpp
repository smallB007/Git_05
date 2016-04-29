#pragma once
#include "Direct2DRenderer.hpp"
#include <memory>
class Git_05_ListCtr : public CListCtrl
{
	DECLARE_DYNAMIC(Git_05_ListCtr)
private:
// 	COLORREF		m_clrSortedColumn{ RGB(0,150,25) };
// 	HFONT			m_hOldFont;

	//std::unique_ptr<Direct2DRenderer> renderer_;
	
public:
	Git_05_ListCtr();
	~Git_05_ListCtr();
	// Support for individual cells text/background colors:
	//virtual COLORREF OnGetCellTextColor(int nRow, int nColum);
	//virtual COLORREF OnGetCellBkColor(int nRow, int nColum);
	//int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual HFONT OnGetCellFont(int /*nRow*/, int /*nColum*/, DWORD /*dwData*/ = 0)
	{
		return NULL;
	}
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		
		if (pMsg->message == WM_LBUTTONUP)
		{
			CPoint pos(::GetMessagePos());
			ScreenToClient(&pos);
			OnLButtonUp(0,pos);
		}
		else if (
			pMsg->message == WM_MOUSEHOVER 
 				///*||*/ WM_MOUSEMOVE == pMsg->message 
// 				|| WM_MOUSELEAVE == pMsg->message
// 				|| WM_MOUSEACTIVATE == pMsg->message
// 				|| WM_MOUSEFIRST == pMsg->message
// 				|| WM_MOUSELAST == pMsg->message
// 				|| WM_MOUSEWHEEL == pMsg->message
 				///*|| */WM_NCMOUSEHOVER == pMsg->message
// 				|| WM_NCMOUSELEAVE == pMsg->message
// 				|| WM_NCMOUSEMOVE == pMsg->message
				)
		{
			CPoint pos(::GetMessagePos());
			ScreenToClient(&pos);
			OnMouseHover(0, pos);
		}
		return CListCtrl::PreTranslateMessage(pMsg);
	}
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Git_05_ListCtr)
protected:
	//virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnSysColorChange();
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnNcPaint();
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg LRESULT OnStyleChanged(WPARAM wp, LPARAM lp);
	//afx_msg LRESULT OnBCGSetControlVMMode(WPARAM, LPARAM);
	//afx_msg LRESULT OnPrint(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnMouseHover(UINT, CPoint);
	void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	//static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//BOOL InitList();
	//void InitColors();

	//COLORREF GetDefaultBkColor();
	//COLORREF GetMarkedColor();
};

