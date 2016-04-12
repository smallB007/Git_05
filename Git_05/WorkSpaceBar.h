// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWorkSpaceBar : public CBCGPDockingControlBar
{
private:
	HTREEITEM hRoot_;
public:
	CWorkSpaceBar();

// Attributes
protected:
	CBCGPTreeCtrl m_wndTree;

// Operations
public:
	
// Overrides

// Implementation
public:
	virtual ~CWorkSpaceBar();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	void fill_repositories_();
	CGit_05App* get_main_app_()const { return static_cast<CGit_05App*>(AfxGetApp()); };//copy from BackStagePageInfo.h
};

/////////////////////////////////////////////////////////////////////////////
