#pragma once
#include "ComPtr.hpp"
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff934857.aspx?f=255&MSPPError=-2147217396

class GIT_Commit_Local;
class Direct2DRenderer
{
	HWND hWnd_;
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<IDWriteFactory> m_dWriteFactory;
	ComPtr<IDWriteTextFormat> m_textFormat;
	ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
	ComPtr<ID2D1Bitmap> m_arrowBitmap;
	ComPtr<ID2D1Bitmap> head_bitmap_;
	ComPtr<ID2D1Bitmap> tail_bitmap_;
	ComPtr<ID2D1Bitmap> commit_bitmap_;
	ComPtr<ID2D1SolidColorBrush> m_fontBrush;
	ComPtr<ID2D1SolidColorBrush> m_selectionBrush;
	HRESULT hr_{ S_OK };
protected:
	//Taken from HILO app
	//{{HILO
	// Direct2D methods
	HRESULT CreateDeviceResources();
	HRESULT CreateDeviceIndependentResources();
	HRESULT DiscardDeviceResources();

public:
	// Rendering methods
	void Resize(int cx, int cy);
	HRESULT DrawClientArea(const std::vector<GIT_Commit_Local>*const commits);
	void drawRectangle(LPRECT rect);
	//void DrawHistoryItems();
	//void DrawOrbitItems();
	//void DrawSelectionBox(D2D1_ROUNDED_RECT rect);

	//HRESULT ResetOrbitValues();
	//}}HILO
	Direct2DRenderer(HWND hWnd);
	~Direct2DRenderer();
private:
	void draw_tail_();
	void draw_commits_(const std::vector<GIT_Commit_Local>*const commits);
	void draw_head_();
};

