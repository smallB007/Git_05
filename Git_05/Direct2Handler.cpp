#include "StdAfx.h"
#include "Direct2Handler.hpp"


template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
} 

Direct2DHandler::Direct2DHandler(HWND hWnd, D2D1::ColorF backgroundColor) :
	m_hWnd(hWnd),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pDarkGrayBrush(NULL),
	background_color_(backgroundColor)
{
	CoInitialize(NULL);
	Initialize_();

}



Direct2DHandler::~Direct2DHandler(void)
{
	for (std::vector<ID2D1Geometry*>::iterator it = m_Geometries.begin();
		it != m_Geometries.end(); ++it)
	{
		(*it)->Release();

	}

	m_Geometries.clear();

	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDarkGrayBrush);
	CoUninitialize();
}

HRESULT Direct2DHandler::Initialize_()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT Direct2DHandler::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr))
		{
			m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_FORCE_DWORD);
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::DarkGray),
				&m_pDarkGrayBrush
			);
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pBlackBrush
			);
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red,0.5),
				&m_p_RED_Brush
			);
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Green,0.5),
				&m_p_GREEN_Brush
			);
			
			
			
			
		}
	}

	///////////////////
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pIDWriteFactory));
	}

	if (SUCCEEDED(hr))
	{
		hr = pIDWriteFactory->CreateTextFormat(
			L"Arial",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			10.0f * 96.0f / 72.0f,
			L"en-US",
			&pITextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&pIRedBrush
		);
	}
	//////////////

	return hr;
}

void Direct2DHandler::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDarkGrayBrush);
}
void Direct2DHandler::add_diff_line(CString diffLine)
{
	diff_lines_.push_back(diffLine);
}
HRESULT Direct2DHandler::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	
	if (SUCCEEDED(hr))
	{

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(background_color_);//D2D1::ColorF(D2D1::ColorF::Aquamarine, 1.0f)
		// Iterate and draw all primitives
		
		//
		{
			CRect rc;
			GetClientRect(m_hWnd, &rc);
			rc.DeflateRect(2.0, 2.0, 2.0, 2.0);
			mark_deletions(rc, 2, 5);
			mark_additions(rc, 6, 8);
			int i{ 0 };
			for (; i < diff_lines_.size(); ++i)
			{

				//pITextFormat->GetLineSpacing(&lineSpacingMethod, &lineSpacing, &baseline);
				CRect rc;
				GetClientRect(m_hWnd, &rc);
				m_pRenderTarget->DrawText(
					diff_lines_[i],
					diff_lines_[i].GetLength(),
					pITextFormat,
					D2D1::RectF(55, i * text_format_line_spacing_ + text_format_line_spacing_, rc.Width(), rc.Height()),//+20 because lines are drawn under the header
					m_pBlackBrush
				);
			}
			
			create_form_geometry_(rc, i + 1);//+1 additional line for a header
			//draw_diff_header_();
			//draw_column_(i + 1, 0);
			//draw_column_(i + 1, column_width_);
			//draw_diff_content_(i + 1);//+1 because lines are drawn under the header which is one line high
		}
		//

		for (std::vector<ID2D1Geometry*>::iterator it = m_Geometries.begin();
			it != m_Geometries.end(); ++it)
		{
			m_pRenderTarget->DrawGeometry(*it, m_pBlackBrush,1.0);
			//m_pRenderTarget->FillGeometry(*it, m_pDarkGrayBrush);
		}
		

		hr = m_pRenderTarget->EndDraw();

	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

void Direct2DHandler::draw_column_(int numberOfLInes, int offset)
{
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	D2D1_RECT_F dx_rec;
	dx_rec.top = 0;
	dx_rec.left = offset;
	dx_rec.right = column_width_ + offset;
	dx_rec.bottom = numberOfLInes * text_format_line_spacing_;
	m_pRenderTarget->DrawRectangle(dx_rec, m_pDarkGrayBrush);
}

void Direct2DHandler::draw_diff_header_()
{
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	D2D1_RECT_F dx_rec_header;
	dx_rec_header.top = 0;
	dx_rec_header.left = 0;
	dx_rec_header.right = rc.Width();
	dx_rec_header.bottom = text_format_line_spacing_;
	m_pRenderTarget->DrawRectangle(dx_rec_header, m_pDarkGrayBrush);

}

void Direct2DHandler::draw_diff_content_(int numberOfLInes)
{
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	D2D1_RECT_F dx_rec;
	dx_rec.top = text_format_line_spacing_;
	dx_rec.left = 0;
	dx_rec.right = rc.Width();
	dx_rec.bottom = numberOfLInes * text_format_line_spacing_;
	m_pRenderTarget->DrawRectangle(dx_rec, m_pDarkGrayBrush);
}

void Direct2DHandler::setup_text_format_()
{
	pITextFormat->SetLineSpacing(text_format_lineSpacingMethod_, text_format_line_spacing_, text_format_baseline_);
}

void Direct2DHandler::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void Direct2DHandler::CreateRectangle(LPCRECT pRect, bool bFill)
{
	D2D1_RECT_F rectangle = D2D1::Rect(pRect->left, pRect->top, pRect->right, pRect->bottom);

	ID2D1RectangleGeometry* pRectangle;
	m_pDirect2dFactory->CreateRectangleGeometry(rectangle, &pRectangle);
	m_Geometries.push_back(pRectangle);
}



void Direct2DHandler::DrawBitmap(CImage* image, LPCRECT pRect)
{
	D2D1_RECT_F rectangle = D2D1::Rect(pRect->left, pRect->top, pRect->right, pRect->bottom);
	D2D1_SIZE_U size;
	size.height = 500;// rectangle.top - rectangle.bottom;
	size.width = 500; //rectangle.right - rectangle.left;
	ID2D1Bitmap* bitmap;
	// Create a pixel format and initial its format
	// and alphaMode fields.
	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	//ResourceLoader.LoadResourceBitmap(,64, 64, bitmap);
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd756686%28v=vs.85%29.aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd756685%28v=vs.85%29.aspx
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ee719660%28v=vs.85%29.aspx
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms648042%28v=vs.85%29.aspx
	//WIC Factory
//	https://msdn.microsoft.com/en-us/library/windows/desktop/ff973956.aspx
	//Hilo app
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ff708696.aspx
// 	D2D1_BITMAP_PROPERTIES bitmap_props{ pixelFormat,480,480 };
// 	m_pRenderTarget->CreateBitmap(size, bitmap_props, &bitmap);
// 	HBITMAP h_bitmap = image->Detach();
// 	
// 	m_pRenderTarget->DrawBitmap(bitmap, rectangle);
	int a = { 0 };
}

void Direct2DHandler::CreateRoundedRectangle(LPCRECT pRect, int radiusx, int radiusY, bool bFill)
{
	D2D1_ROUNDED_RECT rectangle = D2D1::RoundedRect(D2D1::Rect(pRect->left, pRect->top, pRect->right, pRect->bottom), radiusx, radiusY);
	ID2D1RoundedRectangleGeometry* pRRectangle;
	m_pDirect2dFactory->CreateRoundedRectangleGeometry(rectangle, &pRRectangle);
	m_Geometries.push_back(pRRectangle);
}

void Direct2DHandler::CreateEllipse(LPCRECT pRectBoundingBox)
{
	int halfX = (pRectBoundingBox->right - pRectBoundingBox->left) / 2;
	int halfY = (pRectBoundingBox->bottom - pRectBoundingBox->top) / 2;
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(pRectBoundingBox->left + halfX, pRectBoundingBox->top + halfY), halfX, halfY);

	ID2D1EllipseGeometry* pEllipse;
	m_pDirect2dFactory->CreateEllipseGeometry(ellipse, &pEllipse);
	
	m_Geometries.push_back(pEllipse);
}

void Direct2DHandler::create_form_geometry_(CRect rect, int numberOfLines)
{
//https://msdn.microsoft.com/en-us/library/windows/desktop/dd756653%28v=vs.85%29.aspx#composite_geometries
	CRect main_form_rect = rect;
	main_form_rect.top += 0.5;
	main_form_rect.bottom = numberOfLines * text_format_line_spacing_;
	D2D1_RECT_F rectangle = D2D1::Rect(main_form_rect.left, main_form_rect.top, main_form_rect.right, main_form_rect.bottom);

	ID2D1RectangleGeometry* main_form_p;
	m_pDirect2dFactory->CreateRectangleGeometry(rectangle, &main_form_p);
	m_Geometries.push_back(main_form_p);
	CRect first_column_rect = rect;
	first_column_rect.right = column_width_;
	first_column_rect.bottom = main_form_rect.bottom;
	D2D1_RECT_F rectangle_first_column = D2D1::Rect(first_column_rect.left, first_column_rect.top, first_column_rect.right, first_column_rect.bottom);
	ID2D1RectangleGeometry* first_column_p;
	m_pDirect2dFactory->CreateRectangleGeometry(rectangle_first_column, &first_column_p);
	m_Geometries.push_back(first_column_p);
	
// 
	CRect second_column_rect = rect;
	second_column_rect.left = column_width_;
	second_column_rect.right = column_width_ + column_width_;
	second_column_rect.bottom = first_column_rect.bottom;
	D2D1_RECT_F rectangle_second_column = D2D1::Rect(second_column_rect.left, second_column_rect.top, second_column_rect.right, second_column_rect.bottom);
	ID2D1RectangleGeometry* second_column_p;
	m_pDirect2dFactory->CreateRectangleGeometry(rectangle_second_column, &second_column_p);
	m_Geometries.push_back(second_column_p);
	CRect header_rect = rect;
	header_rect.bottom = text_format_line_spacing_;
	D2D1_RECT_F rectangle_header = D2D1::Rect(header_rect.left, header_rect.top, header_rect.right, header_rect.bottom);
	ID2D1RectangleGeometry* header_p;
	m_pDirect2dFactory->CreateRectangleGeometry(rectangle_header, &header_p);
	m_pRenderTarget->FillGeometry(header_p, m_pDarkGrayBrush);
	m_Geometries.push_back(header_p);
}


void Direct2DHandler::mark_changes(CRect rect, int rowStart, int rowEnd, ID2D1SolidColorBrush* brush)
{
	CRect deletion_rect = rect;
	deletion_rect.top = rowStart * text_format_line_spacing_;
	deletion_rect.bottom = ((rowEnd - rowStart) * text_format_line_spacing_) + deletion_rect.top;
	D2D1_RECT_F rectangle = D2D1::Rect(deletion_rect.left, deletion_rect.top, deletion_rect.right, deletion_rect.bottom);

	ID2D1RectangleGeometry* deletion_p;
	m_pDirect2dFactory->CreateRectangleGeometry(rectangle, &deletion_p);
	m_pRenderTarget->FillGeometry(deletion_p, brush);
}

void Direct2DHandler::mark_deletions(CRect rect, int rowStart, int rowEnd)
{
	mark_changes(rect, rowStart, rowEnd, m_p_RED_Brush);
}

void Direct2DHandler::mark_additions(CRect rect, int rowStart, int rowEnd)
{
	mark_changes(rect, rowStart, rowEnd, m_p_GREEN_Brush);
}