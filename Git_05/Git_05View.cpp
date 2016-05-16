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
	
	
	pDC->FillRect(&rect, &background_brush_);

	rect.DeflateRect(2, 2, 2, 2);
	
	pDC->FrameRect(&rect, &brush_frame_);
	//pDC->DrawText(L"Hello Git_05", rect,0);
	render_diffed_file_(pDC,pDoc->get_diffed_file());


	///////////////
	
	// TODO: add draw code for native data here
	//if (m_pRender)
	//{
	//	m_pRender->OnRender();
	//}
}
#include <algorithm>
void CGit_05View::clean_header_(CString& header)
{
	
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(header);
	// construct a std::string using the LPCSTR input
	std::string std_header(pszConvertedAnsiString);
	std::reverse(begin(std_header),end(std_header));

	auto it = std::find(begin(std_header), end(std_header), '@');
	std_header.erase(begin(std_header), it);
	std::reverse(begin(std_header), end(std_header));

	CA2W w_str(std_header.c_str());
	header = w_str;
}


void CGit_05View::render_diffed_file_(CDC* pDC, const diffed_file_t& diffedFile)
{
	////
	/*CClientDC dc(this);
	HFONT hfontOld = SetCurrFont(&dc);
	*/
	/////
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(5, 5);
	

	CRect background_rect = rect;
	background_rect.top += font_height;
	background_rect.bottom = background_rect.top;
	
	CRect column_1_rect = background_rect;
	CRect column_2_rect = background_rect;
	column_1_rect.right = column_1_rect.left + column_width;
	column_2_rect.left = column_1_rect.right;
	column_2_rect.right = column_2_rect.left + column_width;
	background_rect.left = column_2_rect.right;
	CRect column_1_top_rect = column_1_rect;
	CRect column_2_top_rect = column_2_rect;
	pDC->SetBkMode(TRANSPARENT);

	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd183499%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
	
	
	pDC->SelectObject(&font);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	CRect header_rect = rect;
	header_rect.bottom = font_height + tm.tmDescent;
	COLORREF background_color;
	CString header;

	CString sign;
	size_t last_line_no{ 0 };//to prevent false lines like //No newline at the end of file being displayed Header.h git_test_2.sln
	for (const auto& hunk : diffedFile.hunk_lines)
	{
		header = hunk.first.header;
		clean_header_(header);
		pDC->FillRect(&header_rect, &header_brush_);
		pDC->DrawText(header, header_rect, DT_CENTER);
		rect.top += font_height;

		for (const git_o5_diff_line_t& diffed_line : hunk.second)
		{
			
			char origin = diffed_line.origin;
			sign = origin;
			switch (origin)
			{
			case '+':
				background_color = color_green;
				break;
			case '-':
				background_color = color_red;
				break;
			default:
				background_color = background_color_;
				sign = "";
 				break;
			}
			
			if (diffed_line.new_lineno != last_line_no)
			{
				last_line_no = diffed_line.new_lineno;

				CString old_line_no;
				//if (-1 != diffed_line.old_lineno)
				//{
					old_line_no.Format(_T("%d"), diffed_line.old_lineno);
				//}

				CString new_line_no;
				new_line_no.Format(_T("%d"), diffed_line.new_lineno);

				CA2W w_str(diffed_line.content.c_str());
				CString line(/*old_line_no + L"  " + new_line_no + L"  " + */L" " + sign + L"  " + w_str);
				//pDC->SelectObject(&myPen2);
				if (pDC)
				{
					//pDC->SetTextColor(background_color);
					//pDC->SetBkColor(background_color);a
					CBrush brush;
					brush.CreateSolidBrush(background_color);
					background_rect.bottom += font_height + tm.tmDescent;
					column_1_rect.bottom = background_rect.bottom;
					column_2_rect.bottom = background_rect.bottom;
					pDC->FillRect(&column_1_rect, &brush);
					//pDC->FrameRect(&column_1_rect, &header_brush_);
					pDC->FillRect(&column_2_rect, &brush);
					//pDC->FrameRect(&column_2_rect, &header_brush_);
					pDC->FillRect(&background_rect, &brush);

					pDC->SelectObject(&font);
					pDC->DrawText(line, background_rect, DT_END_ELLIPSIS);
					pDC->SelectObject(&column_font);
					pDC->DrawText(old_line_no, column_1_rect, DT_CENTER );
					pDC->DrawText(new_line_no, column_2_rect, DT_CENTER );
					//rect.top += font_height;
					background_rect.top = background_rect.bottom;
					column_1_rect.top = background_rect.top;
					column_2_rect.top = background_rect.top;
				}
			}
		}
	}
	if (diffedFile.hunk_lines.size())
	{//to prevent drawing line in case of any file not been selected yet
		column_1_rect.top = column_1_top_rect.top;
		column_2_rect.top = column_2_top_rect.top;
		background_rect.top = column_2_top_rect.top;
		pDC->FrameRect(&column_1_rect, &header_brush_);
		pDC->FrameRect(&column_2_rect, &header_brush_);
		pDC->FrameRect(&background_rect, &header_brush_);
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
	background_brush_.CreateSysColorBrush(COLOR_3DFACE);
	brush_frame_.CreateSysColorBrush(COLOR_ACTIVECAPTION);
	
	LOGBRUSH logic_brush;
	background_brush_.GetLogBrush(&logic_brush);
	background_color_ = logic_brush.lbColor;

	header_brush_.CreateSysColorBrush(2);
	col_1_brush_.CreateSysColorBrush(5);
	col_2_brush_.CreateSysColorBrush(10);
	//COLORREF header_color_;
	volatile int column_font_height{ font_height };
	
	font.CreateFont(font_height, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, PROOF_QUALITY, 0, _T("Courier New"));
	column_font.CreateFont(column_font_height, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, PROOF_QUALITY, 0, _T("Courier New"));
	return 0;
}


void CGit_05View::InvalidateRect_()
{
	CRect rect;
	GetClientRect(rect);
	InvalidateRect(&rect);
}

void CGit_05View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
 	CRect rect;
	GetClientRect(rect);
	InvalidateRect(&rect);
 	//m_pRender->OnResize(rect.Width(), rect.Height());
}


// void CGit_05View::display_diffed_file(const diffed_file_t& diffedFile)
// {
//
//}