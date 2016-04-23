// Git_05View.h : interface of the CGit_05View class
//


#pragma once
#include "Direct2Handler.hpp"
#include <memory>

class CGit_05View : public CView
{
	//std::unique_ptr<Direct2DHandler> m_pRender;
protected: // create from serialization only
	CGit_05View();
	DECLARE_DYNCREATE(CGit_05View)

// Attributes
public:
	CGit_05Doc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef MFC_DIRECT_2D
	afx_msg LRESULT OnDrawDirect2D(WPARAM wParam, LPARAM lParam);
#endif // MFC_DIRECT_2D

virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGit_05View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeVisualManager(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Git_05View.cpp
inline CGit_05Doc* CGit_05View::GetDocument() const
   { return reinterpret_cast<CGit_05Doc*>(m_pDocument); }
#endif

