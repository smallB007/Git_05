#pragma once
#include <set>
class CListCtrl_DataModel;

class CListCtrl_Category_Groups : public CListCtrl
{
	COLORREF background_color_;// { RGB(200, 200, 200) };
	DWORD hover_time_{ 50 };

	CBrush brush_highligth_;
	CBrush brush_background_;
	LOGBRUSH logic_brush_;
	CListCtrl_DataModel* m_DataModel_;//read only
	
public:
	git_delta_t get_item_state(const CString& fileName);
	CString get_item_status(const CString& fileName);
	void set_data_model(CListCtrl_DataModel* dataModel)
	{
		m_DataModel_ = dataModel;
	}
	~CListCtrl_Category_Groups()
	{
		//int a{0};
	}
	CListCtrl_Category_Groups()
		:m_SortCol(-1)
		,m_Ascending(false)
	{
		brush_highligth_.CreateSysColorBrush(COLOR_ACTIVECAPTION);
		brush_highligth_.GetLogBrush(&logic_brush_);
		brush_background_.CreateSysColorBrush(COLOR_3DFACE);
		LOGBRUSH logic_brush;
		brush_background_.GetLogBrush(&logic_brush);
		background_color_ = logic_brush.lbColor;
	}

	LRESULT InsertGroupHeader(int nIndex, int nGroupID, const CString& strHeader, DWORD dwState = LVGS_NORMAL, DWORD dwAlign = LVGA_HEADER_LEFT);

	CString GetGroupHeader(int nGroupID);
	int GetRowGroupId(int nRow);
	BOOL SetRowGroupId(int nRow, int nGroupID);
	int GroupHitTest(const CPoint& point);

	BOOL GroupByColumn(int nCol);
	void DeleteEntireGroup(int nGroupId);
	BOOL HasGroupState(int nGroupID, DWORD dwState);
	BOOL SetGroupState(int nGroupID, DWORD dwState);
	BOOL IsGroupStateEnabled();

	std::set<CString> get_unchecked_items()const;
	bool is_group_checked(int nGroupId);
	void CheckEntireGroup(int nGroupId, bool bChecked = true);
	void UnCheckEntireGroup(int nGroupId);
	std::set<CString> get_checked_items();
	void set_item_checked(const CString& fileName);
	bool SortColumn(int columnIndex, bool ascending);
	void SetSortArrow(int colIndex, bool ascending);

	void CollapseAllGroups();
	void ExpandAllGroups();

	BOOL SetGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign = LVGA_FOOTER_CENTER);
	BOOL SetGroupTask(int nGroupID, const CString& task);
	BOOL SetGroupSubtitle(int nGroupID, const CString& subtitle);
	BOOL SetGroupTitleImage(int nGroupID, int nImage, const CString& topDesc, const CString& bottomDesc);

	BOOL SetTilesViewLinesCount(int nCount);
	BOOL SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount);
	int GetColumnCount();
	void AdjustColumnWidth();
protected:
	virtual void PreSubclassWindow();

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnGroupTaskClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	int m_SortCol;
	bool m_Ascending;
};





