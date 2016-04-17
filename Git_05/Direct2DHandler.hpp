#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <vector>


class Direct2DHandler
{
public:
	Direct2DHandler(HWND hWnd); // ctor
	~Direct2DHandler(void); // dtor
	HRESULT Initialize(); // Initialize the rendering
	HRESULT OnRender(); // Called from OnPaint function
	void OnResize(UINT width, UINT height);

	void CreateRectangle(LPCRECT pRect, bool bFill);
	void CreateRoundedRectangle(LPCRECT pRect, int radiusx, int radiusY, bool bFill);
	void CreateEllipse(LPCRECT pRectBoundingBox);

	void CreateSolidBrush(COLORREF rgbval);
	void CreateGradientBrush(COLORREF rgbval);

private:
	HRESULT CreateDeviceResources(); // Create resources for drawing
	void DiscardDeviceResources(); // Release resouces for drawing

private:

	HWND m_hWnd;
	ID2D1Factory*			m_pDirect2dFactory;
	ID2D1HwndRenderTarget*	m_pRenderTarget;
	ID2D1SolidColorBrush*	m_pLightSlateGrayBrush;
	std::vector<ID2D1Geometry*> m_Geometries;
};

