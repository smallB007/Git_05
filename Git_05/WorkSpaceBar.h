// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWorkSpaceBar : public CBCGPDockingControlBar
{
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
};

/////////////////////////////////////////////////////////////////////////////
