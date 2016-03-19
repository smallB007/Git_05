// PropertiesViewBar.h : interface of the CBCGPPropBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CBCGPPropBar : public CBCGPDockingControlBar  
{
// Construction
public:
	CBCGPPropBar();

	void AdjustLayout ();

// Attributes
public:
	void SetVSDotNetLook (BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook (bSet);
		m_wndPropList.SetGroupNameFullWidth (bSet);
	}

protected:
	CBCGPPropList	m_wndPropList;

	CBCGPColor		m_BorderColor;
	CBCGPBrush		m_FillBrush;
	CBCGPBrush		m_TextBrush;

// Attributes
protected:

// Overrides

// Implementation
public:
	virtual ~CBCGPPropBar();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnCommandClicked(WPARAM, LPARAM);
	afx_msg LRESULT OnMenuItemSelected(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	void InitPropList ();
};
