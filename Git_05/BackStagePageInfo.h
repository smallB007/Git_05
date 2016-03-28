#pragma once
#include "afxwin.h"

// BackStagePageInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackStagePageInfo dialog

class CBackStagePageInfo : public CBCGPDialog
{
	DECLARE_DYNCREATE(CBackStagePageInfo)

// Construction
public:
	CBackStagePageInfo(CWnd* pParent = NULL);   // standard constructor
	~CBackStagePageInfo();
// Dialog Data
	enum { IDD = IDD_FORM_INFO };
	CBCGPButton	m_btnPreview;

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:


	// Generated message map functions
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
	afx_msg LRESULT OnDrawDirect2D(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	void OnDraw(CDC* pDC);
	void PreparePreviewBitmap();

	CString	m_strDocName;
	CString	m_strPath;
private:
	CD2DTextFormat* m_pTextFormat;
	CD2DSolidColorBrush* m_pBlackBrush;
	CD2DLinearGradientBrush* m_pLinearGradientBrush;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CEdit user_email_edit_;
	CEdit user_name_edit_;
private:
	/**/
	// (? : [a - z0 - 9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/= ? ^_`{ |}~- ] + )* | "(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@( ? : (? : [a - z0 - 9](? : [a - z0 - 9 - ] * [a - z0 - 9]) ? \.) + [a - z0 - 9](? : [a - z0 - 9 - ] * [a - z0 - 9]) ? | \[(? : (? : 25[0 - 5] | 2[0 - 4][0 - 9] | [01] ? [0 - 9][0 - 9] ? )\.) { 3 }(? : 25[0 - 5] | 2[0 - 4][0 - 9] | [01] ? [0 - 9][0 - 9] ? | [a - z0 - 9 - ] * [a - z0 - 9] : (? : [\x01 - \x08\x0b\x0c\x0e - \x1f\x21 - \x5a\x53 - \x7f] | \\[\x01 - \x09\x0b\x0c\x0e - \x7f]) + )\])
	/**/
	void validate_email_(CEdit* ceditCtrl);
	bool isEmailAdressValid_{ true };
public:
	afx_msg void OnEnChangeUsernameEdit();
	afx_msg void OnEnChangeUseremailEdit();
	UINT_PTR timer_ID_;
	void OnTimer(UINT nIDEvent);
	afx_msg void OnEnKillfocusUseremailEdit();
	afx_msg void OnBnClickedSaveconfig();
	afx_msg void OnBnClickedLogin();
	/*LOG IN*/
private:
	void get_login_credentials_();
private:
	CEdit user_name_login_;
	CEdit password_log_in_;
};
