// workspace2.cpp : implementation of the CWorkSpaceBar_Git_Tree class
//

#include "stdafx.h"
#include "Git_05.h"
#include "WorkSpaceBar_Git_Tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree

BEGIN_MESSAGE_MAP(CWorkSpaceBar_Git_Tree, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree construction/destruction

CWorkSpaceBar_Git_Tree::CWorkSpaceBar_Git_Tree()
{
	// TODO: add one-time construction code here
}

CWorkSpaceBar_Git_Tree::~CWorkSpaceBar_Git_Tree()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar_Git_Tree message handlers

int CWorkSpaceBar_Git_Tree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	HRESULT hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources();
	}

	return static_cast<int>(hr);
	//m_pRender = new Direct2DHandler(m_hWnd, D2D1::ColorF::Aqua);//make it shared
	//m_pRender = std::make_unique<Direct2DHandler>(m_hWnd, D2D1::ColorF::Black);//make it shared
// 	CRect rect;
// 	GetClientRect(rect);
// 	rect.DeflateRect(130, 130, 130, 130);
// 	m_pRender->CreateEllipse(rect);
// 	//m_pRender->Initialize();
// // 	CRect rectDummy;
// // 	rectDummy.SetRectEmpty();
// // 	
// // 	// Create tree control:
// // 	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
// // 								TVS_LINESATROOT | TVS_HASBUTTONS;
// // 	
// // 	m_wndTree.m_bVisualManagerStyle = TRUE;
// // 
// // 	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
// // 	{
// // 		TRACE0("Failed to create workspace view\n");
// // 		return -1;      // fail to create
// // 	}
// // 
// // 	// Setup tree content:
// // 	HTREEITEM hRoot = m_wndTree.InsertItem(_T("Root 1"));
// // 	m_wndTree.InsertItem(_T("Item 1"), hRoot);
// // 	m_wndTree.InsertItem(_T("Item 2"), hRoot);
// // 	
// // 	m_wndTree.Expand(hRoot, TVE_EXPAND);

	return 0;
}

void CWorkSpaceBar_Git_Tree::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
 	CRect rect;
 	GetClientRect(rect);
	D2D1_SIZE_U size = { rect.Width(),rect.Height() };
	m_renderTarget->Resize(size);

// 	m_pRender->OnResize(rect.Width(), rect.Height());
	// Tree control should cover a whole client area:
// 	m_wndTree.SetWindowPos(NULL, nBorderSize, nBorderSize, 
// 		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
// 		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar_Git_Tree::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawClientArea();
	//InvalidateRect(rect);
// 	CRect rect;
// 	GetClientRect(rect);

// 	CImage image;
// 	image.Load(_T("C:\\Users\\Artie Fuffkin\\Documents\\visual studio 2015\\Projects\\Git_05\\Git_05\\avatar.png")); // just change extension to load jpg
	
// 	if (m_pRender)
// 	{
// 		m_pRender->OnRender();
// 		m_pRender->DrawBitmap(&image,rect);
// 	}
}
//
// This method discards resources which are bound to a particular Direct2D render target
//
HRESULT CWorkSpaceBar_Git_Tree::DiscardDeviceResources()
{
	//HRESULT hr = S_OK;

// 	for (auto iter = m_carouselItems.begin(); iter != m_carouselItems.end(); iter++)
// 	{
// 		ComPtr<IThumbnail> thumbnail;
// 
// 		if (SUCCEEDED(hr))
// 		{
// 			hr = (*iter).QueryInterface(&thumbnail);
// 		}
// 
// 		thumbnail->DiscardResources();
// 	}
// 
// 	for (auto iter = m_carouselHistoryItems.begin(); iter != m_carouselHistoryItems.end(); iter++)
// 	{
// 		ComPtr<IThumbnail> thumbnail;
// 
// 		if (SUCCEEDED(hr))
// 		{
// 			hr = (iter)->Thumbnail.QueryInterface(&thumbnail);
// 		}
// 
// 		if (SUCCEEDED(hr))
// 		{
// 			thumbnail->DiscardResources();
// 		}
// 	}

	m_arrowBitmap = nullptr;
	m_fontBrush = nullptr;
	m_selectionBrush = nullptr;
// 	m_backgroundLinearGradientBrush = nullptr;
// 	m_radialGradientBrush = nullptr;
	m_renderTarget = nullptr;

	return S_OK;
}

HRESULT CWorkSpaceBar_Git_Tree::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	// 	if (nullptr == m_renderTarget)
	// 	{
	// // 		ComPtr<IWindow> window;
	// // 		hr = GetWindow(&window);
	// 
	// 		if (SUCCEEDED(hr))
	// 		{
	// // 			CRect rect;
	// // 			/*hr = window->*/GetClientRect(&rect);
	// 			
	// 			if (SUCCEEDED(hr))
	// 			{
	// // 				HWND hWnd;
	// // 				hr = window->GetWindowHandle(&hWnd);
	// 				
	// 				if (SUCCEEDED(hr))
	// 				{
						// Create a Direct2D render target
	// 					hr = m_d2dFactory->CreateHwndRenderTarget(
	// 						RenderTargetProperties(),
	// 						HwndRenderTargetProperties(m_hWnd, SizeU(rect.right, rect.bottom)),
	// 						&m_renderTarget);

	CRect rc;
	GetClientRect(rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	// Create a Direct2D render target.
	hr = m_d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hWnd, size),
		&m_renderTarget
	);

	// 				}
	// 			}
	// 		}

	if (SUCCEEDED(hr))
	{
		// Load back button image
		hr = Direct2DUtility::LoadBitmapFromResource(
			m_renderTarget,
			L"ARROWIMAGE",
			L"PNG",
			0,
			0,
			&m_arrowBitmap);
	}

	// 		if (SUCCEEDED(hr))
	// 		{
	// 			// Load default thumbnail image...
	// 			hr = Direct2DUtility::LoadBitmapFromResource(
	// 				m_renderTarget,
	// 				L"DefaultFolderImage",
	// 				L"PNG",
	// 				0,
	// 				0,
	// 				&m_defaultFolderBitmap);
	// 		}
	// 
	// 		if (SUCCEEDED(hr))
	// 		{
	// 			// Load annotator button image
	// 			hr = Direct2DUtility::LoadBitmapFromResource(
	// 				m_renderTarget,
	// 				L"AnnotatorButtonImage",
	// 				L"PNG",
	// 				0,
	// 				0,
	// 				&m_annotatorButtonImage);
	// 		}
	// 
	// 		if (SUCCEEDED(hr))
	// 		{
	// 			// Load share button image
	// 			hr = Direct2DUtility::LoadBitmapFromResource(
	// 				m_renderTarget,
	// 				L"SharingButtonImage",
	// 				L"PNG",
	// 				0,
	// 				0,
	// 				&m_sharingButtonImage);
	// 		}

	// 		if (SUCCEEDED(hr))
	// 		{
	// 			// Create gradient brush for background
	// 			ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
	// 			D2D1_GRADIENT_STOP gradientStops[3];
	// 
	// 			if (SUCCEEDED(hr))
	// 			{
	// 				gradientStops[0].color = ColorF(BackgroundColor);
	// 				gradientStops[0].position = 0.0f;
	// 				gradientStops[1].color = ColorF(ColorF::White);
	// 				gradientStops[1].position = 0.25f;
	// 			}
	// 
	// 			if (SUCCEEDED(hr))
	// 			{
	// 
	// 				hr = m_renderTarget->CreateGradientStopCollection(
	// 					gradientStops,
	// 					2,
	// 					D2D1_GAMMA_2_2,
	// 					D2D1_EXTEND_MODE_CLAMP,
	// 					&gradientStopCollection);
	// 			};
	// 
	// 			if (SUCCEEDED(hr))
	// 			{
	// 				hr = m_renderTarget->CreateLinearGradientBrush(
	// 					LinearGradientBrushProperties(
	// 						Point2F(m_renderTarget->GetSize().width, 0),
	// 						Point2F(m_renderTarget->GetSize().width, m_renderTarget->GetSize().height)),
	// 					gradientStopCollection,
	// 					&m_backgroundLinearGradientBrush);
	// 			}
	// 
	// 			gradientStopCollection = nullptr;
	// 
	// 			// Create gradient brush for drawing orbital rings
	// 			gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
	// 			gradientStops[0].position = 0.7f;
	// 			gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::LightBlue, 1.0f);
	// 			gradientStops[1].position = 0.95f;
	// 			gradientStops[2].color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
	// 			gradientStops[2].position = 1.0f;
	// 
	// 			hr = m_renderTarget->CreateGradientStopCollection(
	// 				gradientStops,
	// 				3,
	// 				D2D1_GAMMA_2_2,
	// 				D2D1_EXTEND_MODE_CLAMP,
	// 				&gradientStopCollection);
	// 
	// 			if (SUCCEEDED(hr))
	// 			{
	// 				hr = m_renderTarget->CreateRadialGradientBrush(
	// 					D2D1::RadialGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, 0), 200, 200),
	// 					gradientStopCollection,
	// 					&m_radialGradientBrush);
	// 			}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_fontBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_selectionBrush);
	}

	// 			if (SUCCEEDED(hr))
	// 			{
	// 				// Set rendering parameters
	// 				m_renderingParameters.renderTarget = m_renderTarget;
	// 				m_renderingParameters.solidBrush = m_fontBrush;
	// 				m_renderingParameters.textFormat = m_textFormat;
	// 			}

	// 			if (SUCCEEDED(hr))
	// 			{
	// 				hr = m_AsyncLoaderHelper->StartBackgroundLoading();
	// 			}
	return hr;
}

//
// Create resources that are not bound to a particular Direct2D render target
//
HRESULT CWorkSpaceBar_Git_Tree::CreateDeviceIndependentResources()
{
	HRESULT hr = Direct2DUtility::GetD2DFactory(&m_d2dFactory);

	if (SUCCEEDED(hr))
	{
		hr = Direct2DUtility::GetDWriteFactory(&m_dWriteFactory);
	}

	// Create text format for orbit and history icons
	if (SUCCEEDED(hr))
	{
		hr = m_dWriteFactory->CreateTextFormat(
			L"Arial",
			nullptr,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12,
			L"en-us",
			&m_textFormat);
	}

	// Create text layout for share button
// 	if (SUCCEEDED(hr))
// 	{
// 		wchar_t caption[20];
// 		::LoadString(HINST_THISCOMPONENT, IDS_SHARE_BUTTON_TEXT, caption, 20);
// 
// 		hr = m_dWriteFactory->CreateTextLayout(
// 			caption,
// 			static_cast<unsigned int>(wcslen(caption)),
// 			m_textFormat,
// 			ApplicationButtonSize + ApplicationButtonSelectionMargin * 2,
// 			ApplicationButtonSelectionMargin * 3,
// 			&m_textLayoutShare);
// 	}

// 	if (SUCCEEDED(hr))
// 	{
// 		// Setup text layout properties
// 		m_textLayoutShare->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
// 		m_textLayoutShare->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
// 		m_textLayoutShare->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
// 	}

	// Create text layout for annotate button
// 	if (SUCCEEDED(hr))
// 	{
// 		wchar_t caption[20];
// 		::LoadString(HINST_THISCOMPONENT, IDS_ANNOTATE_BUTTON_TEXT, caption, 20);
// 
// 		hr = m_dWriteFactory->CreateTextLayout(
// 			caption,
// 			static_cast<unsigned int>(wcslen(caption)),
// 			m_textFormat,
// 			ApplicationButtonSize + ApplicationButtonSelectionMargin * 2,
// 			ApplicationButtonSelectionMargin * 3,
// 			&m_textLayoutAnnotate);
// 	}

// 	if (SUCCEEDED(hr))
// 	{
// 		// Setup text layout properties
// 		m_textLayoutAnnotate->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
// 		m_textLayoutAnnotate->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
// 		m_textLayoutAnnotate->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
// 	}

	// Initialize animation controllers
// 	if (SUCCEEDED(hr))
// 	{
// 		hr = SharedObject<OrbitAnimation>::Create(&m_innerOrbitAnimation);
// 	}

// 	if (SUCCEEDED(hr))
// 	{
// 		hr = SharedObject<CarouselAnimation>::Create(&m_carouselAnimation);
// 	}

	return hr;
}

HRESULT CWorkSpaceBar_Git_Tree::DrawClientArea()
{
	if (D2DERR_RECREATE_TARGET == hr_)
	{
		CreateDeviceResources();
	}

	HRESULT hr = S_OK;
	CRect rect;
	GetClientRect(rect);
	D2D1_RECT_F dx_rect;

	dx_rect.bottom = rect.bottom;
	dx_rect.left = rect.left;
	dx_rect.right = rect.right;
	dx_rect.top = rect.top;

	{
		//Drawing part
		m_renderTarget->BeginDraw();

		m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));
		m_renderTarget->DrawBitmap(m_arrowBitmap, &dx_rect);

		hr = m_renderTarget->EndDraw();
	}
	
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr_ = D2DERR_RECREATE_TARGET;
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}
