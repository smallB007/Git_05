// Git_05View.cpp : implementation of the CGit_05View class
//

#include "stdafx.h"
#include "Git_05.h"

#include "Git_05Doc.h"
#include "Git_05View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGit_05View

IMPLEMENT_DYNCREATE(CGit_05View, CView)

BEGIN_MESSAGE_MAP(CGit_05View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
#ifdef DIRECT_2D
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CGit_05View::OnDrawDirect2D)
#endif
END_MESSAGE_MAP()

// CGit_05View construction/destruction

CGit_05View::CGit_05View()
{
	// TODO: add construction code here
	EnableD2DSupport();
}

CGit_05View::~CGit_05View()
{
}

BOOL CGit_05View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGit_05View drawing

void CGit_05View::OnDraw(CDC* /*pDC*/)
{
	CGit_05Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

#ifdef DIRECT_2D
LRESULT CGit_05View::OnDrawDirect2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine, 1.0f));
	return LRESULT();
}
#endif
// CGit_05View printing

void CGit_05View::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CGit_05View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGit_05View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGit_05View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGit_05View diagnostics

#ifdef _DEBUG
void CGit_05View::AssertValid() const
{
	CView::AssertValid();
}

void CGit_05View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGit_05Doc* CGit_05View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGit_05Doc)));
	return (CGit_05Doc*)m_pDocument;
}
#endif //_DEBUG


// CGit_05View message handlers

LRESULT CGit_05View::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

LRESULT CGit_05View::OnChangeVisualManager(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return 0;
}
