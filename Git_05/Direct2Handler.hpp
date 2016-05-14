#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <vector>

/*Responsible for drawing*/
class Direct2DHandler
{
private:
	std::vector<CString> diff_lines_;
public:
	void add_diff_line(CString diffLine);
private:
	HRESULT Initialize_(); // Initialize the rendering
public:
	Direct2DHandler(HWND hWnd, D2D1::ColorF backgroundColor = D2D1::ColorF::BlanchedAlmond); // ctor
	~Direct2DHandler(void); // dtor
	HRESULT OnRender(); // Called from OnPaint function
	void OnResize(UINT width, UINT height);

	void CreateRectangle(LPCRECT pRect, bool bFill);
	void CreateRoundedRectangle(LPCRECT pRect, int radiusx, int radiusY, bool bFill);
	void CreateEllipse(LPCRECT pRectBoundingBox);
	void DrawBitmap(CImage* image, LPCRECT pRect);
	void CreateSolidBrush(COLORREF rgbval);
	void CreateGradientBrush(COLORREF rgbval);

private:
	void draw_diff_header_();
	void draw_diff_content_(int numberOfLInes);
	void draw_column_(int numberOfLInes, int offset);
private:
	ID2D1PathGeometry* form_geometry_;
	void create_form_geometry_(CRect rect, int numberOfLines);
	void mark_deletions(CRect rect, int rowStart,int rowEnd);
	void mark_additions(CRect rect, int rowStart, int rowEnd);
	void mark_changes(CRect rect, int rowStart, int rowEnd, ID2D1SolidColorBrush* brush);
private:
	HRESULT CreateDeviceResources(); // Create resources for drawing
	void DiscardDeviceResources(); // Release resources for drawing
	HRESULT GetWICFactory(IWICImagingFactory** factory);
private:

	HWND m_hWnd;
	ID2D1Factory*			m_pDirect2dFactory;
	ID2D1HwndRenderTarget*	m_pRenderTarget;
	ID2D1SolidColorBrush*	m_pDarkGrayBrush;
	ID2D1SolidColorBrush*	m_pBlackBrush;
	ID2D1SolidColorBrush*	m_p_RED_Brush;
	ID2D1SolidColorBrush*	m_p_GREEN_Brush;
	D2D1::ColorF			background_color_;
	std::vector<ID2D1Geometry*> m_Geometries;

private:
//https://msdn.microsoft.com/en-us/library/windows/desktop/dd371554%28v=vs.85%29.aspx
	ID2D1SolidColorBrush* pIRedBrush{nullptr};
	IDWriteTextFormat* pITextFormat{nullptr};
	IDWriteFactory* pIDWriteFactory{nullptr};
private:
	DWRITE_LINE_SPACING_METHOD text_format_lineSpacingMethod_{ DWRITE_LINE_SPACING_METHOD_UNIFORM };
	FLOAT text_format_line_spacing_{20};
	FLOAT text_format_baseline_{ 20 };
	void setup_text_format_();
	int column_width_{ 40 };
};

