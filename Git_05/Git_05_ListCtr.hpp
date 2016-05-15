#pragma once
class Git_05_ListCtr : public CListCtrl
{
	DECLARE_DYNAMIC(Git_05_ListCtr)
	friend class CWorkSpaceBar4;
	//those typedefs taken from Workspacebar4
	typedef CString repo_name_t;
	typedef CString branch_name_t;

private:
	int lastItem_{0};
	COLORREF background_color_;// { RGB(200, 200, 200) };
	DWORD hover_time_{ 50 };

	CBrush brush_highligth_;
	CBrush brush_background_;
	LOGBRUSH logic_brush_;
	//CImageList m_cImageListNormal, m_cImageListSmall;//:AC: refactoring to move the process of creating list from Workspace to here
private:
	//CWorkSpaceBar4* parent_;
public:
	const CString file_with_repo_to_set_as_active_ = L"repo_to_set_as_active.txt";
	enum GIT_ENTITY_TYPE { REPO, COMMIT, UNSPECIFIED };
	GIT_ENTITY_TYPE git_entity_type_{ UNSPECIFIED };
	Git_05_ListCtr();
	~Git_05_ListCtr();
	virtual HFONT OnGetCellFont(int /*nRow*/, int /*nColum*/, DWORD /*dwData*/ = 0)
	{
		return NULL;
	}
	//void addParent(CWorkSpaceBar4* parent)
	//{
	//	parent_ = parent;
	//}
	void set_git_entity_type(GIT_ENTITY_TYPE gitEntityType)
	{
		git_entity_type_ = gitEntityType;
	}
	
	GIT_ENTITY_TYPE get_git_entity_type()
	{
		return git_entity_type_;
	}

	CString get_active_item()
	{
		return GetItemText(lastItem_, 0);
	}
	void write_repo_name_to_file_(const CString& repoName)const;
	void read_repo_name_from_file_();
	void set_active_repo();
	void set_active_commit();
// 	BOOL PreTranslateMessage(MSG* pMsg)
// 	{
// 		
// // 		if (pMsg->message == WM_LBUTTONUP)
// // 		{
// // 			CPoint pos(::GetMessagePos());
// // 			ScreenToClient(&pos);
// // 			OnLButtonUp(0,pos);
// // 		}
// // 		else if (
// // 			//pMsg->message == WM_MOUSEHOVER 
// //  				///*||*/ WM_MOUSEMOVE == pMsg->message 
// // // 				|| WM_MOUSELEAVE == pMsg->message
// // // 				|| WM_MOUSEACTIVATE == pMsg->message
// // // 				|| WM_MOUSEFIRST == pMsg->message
// // // 				|| WM_MOUSELAST == pMsg->message
// // // 				|| WM_MOUSEWHEEL == pMsg->message
// //  				WM_NCMOUSEHOVER == pMsg->message
// // // 				|| WM_NCMOUSELEAVE == pMsg->message
// // // 				|| WM_NCMOUSEMOVE == pMsg->message
// // 				)
// // 		{
// // 			CPoint pos(::GetMessagePos());
// // 			ScreenToClient(&pos);
// // 			//OnMouseHover(0, pos);
// // 		}
// 		return CListCtrl::PreTranslateMessage(pMsg);
// 	}
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Git_05_ListCtr)
protected:
	//virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	//afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnMouseHover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd*);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	void selectItem(const repo_name_t & repoName);
	void selectItem(const int inx);
	
	void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	//static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//BOOL InitList();
	//void InitColors();

	//COLORREF GetDefaultBkColor();
	//COLORREF GetMarkedColor();
public:
	afx_msg void OnDestroy();
};

