// workspacebar.h : interface of the CWorkSpaceBar4 class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DemoListView.hpp"
#include "Direct2DRenderer.hpp"
#include <memory>
class CWorkSpaceBar4 : public CBCGPDockingControlBar
{
public:
	enum EVIEW_TYPE{REPOS,GIT_TREE};
private:
	std::unique_ptr<Direct2DRenderer> renderer_;
	HTREEITEM hRoot_;
	CImageList m_cImageListNormal, m_cImageListSmall;
	EVIEW_TYPE eview_type_{ REPOS };
	int CWorkSpaceBar4::create_list_ctrl_();
public:
	CWorkSpaceBar4();
	void set_view_type(EVIEW_TYPE view_type);
	// Attributes
protected:
	//CBCGPTreeCtrl m_wndTree;
	//CListCtrl& listCtrl;
	CListCtrl m_wndListCtrl;
	//CDemoListView m_listView;
	// Operations
public:

	// Overrides

	// Implementation
public:
	virtual ~CWorkSpaceBar4();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

#ifdef MFC_DIRECT_2D
	afx_msg LRESULT OnDrawDirect2D(WPARAM wParam, LPARAM lParam);
#else
	afx_msg void OnPaint();
#endif
	DECLARE_MESSAGE_MAP()
private:
	void fill_repositories_();
	CGit_05App* get_main_app_()const { return static_cast<CGit_05App*>(AfxGetApp()); };//copy from BackStagePageInfo.h
};

/////////////////////////////////////////////////////////////////////////////
