// workspace2.h : interface of the CWorkSpaceBar_Git_Tree class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Direct2DRenderer.hpp"
#include <memory>

class CWorkSpaceBar_Git_Tree : public CBCGPDockingControlBar
{
	std::unique_ptr<Direct2DRenderer> renderer_;
public:
	CWorkSpaceBar_Git_Tree();

// Attributes
protected:
	//CBCGPTreeCtrl	m_wndTree;

// Operations
public:
// Overrides

// Implementation
public:
	virtual ~CWorkSpaceBar_Git_Tree();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
