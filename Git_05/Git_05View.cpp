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
#ifdef MFC_DIRECT_2D
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CGit_05View::OnDrawDirect2D)
#endif
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CGit_05View construction/destruction

void CGit_05View::add_diff_line(CString diffLine)
{
	m_pRender->add_diff_line(diffLine);
}

CGit_05View::CGit_05View()
{
	// TODO: add construction code here
	//EnableD2DSupport();
}

CGit_05View::~CGit_05View()
{
}
CGit_05View* CGit_05View::GetView()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	CView * pView = pFrame->GetActiveView();

	if (!pView)
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if (!pView->IsKindOf(RUNTIME_CLASS(CGit_05View)))
		return NULL;

	return (CGit_05View *)pView;
}
BOOL CGit_05View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGit_05View drawing

void CGit_05View::OnDraw(CDC* pDC)
{
	CGit_05Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(2, 2, 2, 2);
	CBrush brush_highligth_; 
	brush_highligth_.CreateSysColorBrush(2);
	pDC->FrameRect(&rect, &brush_highligth_);
	//pDC->DrawText(L"Hello Git_05", rect,0);
	render_diffed_file_(pDC,pDoc->get_diffed_file());


	///////////////
	
	// TODO: add draw code for native data here
	//if (m_pRender)
	//{
	//	m_pRender->OnRender();
	//}
}

void CGit_05View::render_diffed_file_(CDC* pDC, const diffed_file_t& diffedFile)
{

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(5, 5);

	COLORREF color_red(RGB(255, 0, 0));
	COLORREF color_green(RGB(0, 255, 0));
	COLORREF color_black(RGB(0, 0, 0));

	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd183499%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
	CFont font;
	font.CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Tahoma"));

	for (const auto& hunk : diffedFile.hunk_lines)
	{
		for (const git_o5_diff_line_t& diffed_line : hunk.second)
		{
			COLORREF line_color;
			char origin = diffed_line.origin;
			switch (origin)
			{
			case '+':
				line_color = color_green;
				break;
			case '-':
				line_color = color_red;
				break;
			default:
				line_color = color_black;
				break;
			}
			CString sign(origin);
			CA2W w_str(diffed_line.content.c_str());
			CString line(sign + L"  " + w_str);
			//pDC->SelectObject(&myPen2);
			if (pDC)
			{
				pDC->SetTextColor(line_color);
				pDC->SelectObject(&font);
				auto height = pDC->DrawText(line, rect, DT_END_ELLIPSIS);
				rect.top += 20;
			}
		}
	}
}

#ifdef VIEW_MFC_DIRECT_2D
LRESULT CGit_05View::OnDrawDirect2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AntiqueWhite, 1.0f));
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

int CGit_05View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_pRender = std::make_unique<Direct2DHandler>(m_hWnd,D2D1::ColorF::LightGray);//make it shared
	//m_pRender->Initialize();
	


	return 0;
}


void CGit_05View::InvalidateRect_()
{
	CRect rect;
	GetWindowRect(rect);
	InvalidateRect(&rect);
}

void CGit_05View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
 	CRect rect;
 	GetWindowRect(rect);
	InvalidateRect(&rect);
 	//m_pRender->OnResize(rect.Width(), rect.Height());
}


// void CGit_05View::display_diffed_file(const diffed_file_t& diffedFile)
// {
//
//}