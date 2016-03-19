// outputbar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:
	CBCGPTabWnd	m_wndTabs;
	
	CBCGPListCtrl	m_wndList1;
	CBCGPListCtrl	m_wndList2;
	CBCGPListCtrl	m_wndList3;

// Operations
public:

// Overrides

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
