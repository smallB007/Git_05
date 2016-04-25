// DemoListView.h : interface of the CDemoListView class
//
// Author:   Ovidiu Cucu
// Homepage: http://codexpert.ro/blog/

#pragma once


class CDemoListView : public CListView
{
	DECLARE_DYNCREATE(CDemoListView)

	// Attributes
private:
	CImageList m_imageList;
	CImageList m_imageListSmall;

	// Construction
protected:
	CDemoListView(); // create from serialization only
public:
	virtual ~CDemoListView();

	// Operations
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); // called first time after construct

									// Implementation
private:
	//CDemoDoc* GetDocument() const;
	void _CreateImageLists();
	void _InitAndFillList();
	BOOL _SetTilesViewLinesCount(int nCount);
	BOOL _SetTilesViewTileFixedWidth(int nWidth);
	BOOL _SetItemTileLines(int iItem, UINT* parrColumns, UINT nCount);

	// Message mandlers 
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewLargeicon();
	afx_msg void OnViewSmallicon();
	afx_msg void OnViewList();
	afx_msg void OnViewDetails();
	afx_msg void OnViewTile();
	afx_msg void OnUpdateViewLargeicon(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewSmallicon(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewDetails(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewTile(CCmdUI *pCmdUI);
};



