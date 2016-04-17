#include "StdAfx.h"
#include "Direct2Handler.hpp"

#pragma comment( lib, "d2d1.lib" )


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
	m_pLightSlateGrayBrush(NULL),
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
	SafeRelease(&m_pLightSlateGrayBrush);
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
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Cyan),
				&m_pLightSlateGrayBrush
			);
		}
	}

	return hr;
}

void Direct2DHandler::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
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
		for (std::vector<ID2D1Geometry*>::iterator it = m_Geometries.begin();
			it != m_Geometries.end(); ++it)
		{
			m_pRenderTarget->DrawGeometry(*it, m_pLightSlateGrayBrush);
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