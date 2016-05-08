// PropertiesViewBar.h : interface of the Git05_CBCGPPropBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "BCGPPropList.h"
#include <memory>
class GIT_Commit_Local;
class Git05_CBCGPPropBar : public CBCGPDockingControlBar  
{
private:
	std::unique_ptr<CBCGPProp> commiter_group_uptr_;
// Construction
public:
	Git05_CBCGPPropBar();

	void AdjustLayout ();

// Attributes
public:
	void SetVSDotNetLook (BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook (bSet);
		m_wndPropList.SetGroupNameFullWidth (bSet);
	}
	void set_commit_info(const GIT_Commit_Local& commitInfo);
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
	virtual ~Git05_CBCGPPropBar();

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
