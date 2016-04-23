// workspace2.h : interface of the CWorkSpaceBar_Git_Tree class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
//#include "Direct2Handler.hpp"
#include <memory>
class CWorkSpaceBar_Git_Tree : public CBCGPDockingControlBar
{
	//std::unique_ptr<Direct2DHandler> m_pRender;
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<IDWriteFactory> m_dWriteFactory;
	ComPtr<IDWriteTextFormat> m_textFormat;
	ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
	ComPtr<ID2D1Bitmap> m_arrowBitmap;
	ComPtr<ID2D1SolidColorBrush> m_fontBrush;
	ComPtr<ID2D1SolidColorBrush> m_selectionBrush;
	HRESULT hr_{ S_OK };
public:
	CWorkSpaceBar_Git_Tree();

// Attributes
protected:
	//CBCGPTreeCtrl	m_wndTree;

// Operations
public:
	//Taken from HILO app
	//{{HILO
	// Direct2D methods
	HRESULT CreateDeviceResources();
	HRESULT CreateDeviceIndependentResources();
	HRESULT DiscardDeviceResources();

	// Rendering methods
	HRESULT DrawClientArea();
	//void DrawHistoryItems();
	//void DrawOrbitItems();
	//void DrawSelectionBox(D2D1_ROUNDED_RECT rect);

	//HRESULT ResetOrbitValues();
	//}}HILO

// Overrides

// Implementation
public:
	virtual ~CWorkSpaceBar_Git_Tree();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
