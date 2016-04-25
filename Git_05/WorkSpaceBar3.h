// workspacebar.h : interface of the CWorkSpaceBar3 class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWorkSpaceBar3 : public CBCGPDockingControlBar
{
private:
	HTREEITEM hRoot_;
public:
	CWorkSpaceBar3();

	// Attributes
protected:
	CBCGPTreeCtrl m_wndTree;

	// Operations
public:

	// Overrides

	// Implementation
public:
	virtual ~CWorkSpaceBar3();

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
