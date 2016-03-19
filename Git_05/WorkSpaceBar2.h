// workspace2.h : interface of the CWorkSpaceBar2 class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWorkSpaceBar2 : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar2();

// Attributes
protected:
	CBCGPTreeCtrl	m_wndTree;

// Operations
public:

// Overrides

// Implementation
public:
	virtual ~CWorkSpaceBar2();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
