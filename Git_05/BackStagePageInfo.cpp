// BackStagePageInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Git_05.h"
#include "MainFrm.h"
#include "BackStagePageInfo.h"
#include "EmailValidator.hpp"
#include "CStringUtilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//Statics

/////////////////////////////////////////////////////////////////////////////
// CBackStagePageInfo dialog

IMPLEMENT_DYNCREATE(CBackStagePageInfo, CBCGPDialog)

BEGIN_MESSAGE_MAP(CBackStagePageInfo, CBCGPDialog)
	//ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CBackStagePageInfo::OnDrawDirect2D)
	ON_EN_CHANGE(IDC_USERNAME_EDIT, &CBackStagePageInfo::OnEnChangeUsernameEdit)
	ON_EN_CHANGE(IDC_USEREMAIL_EDIT, &CBackStagePageInfo::OnEnChangeUseremailEdit)
	ON_EN_KILLFOCUS(IDC_USEREMAIL_EDIT, &CBackStagePageInfo::OnEnKillfocusUseremailEdit)
	ON_BN_CLICKED(IDC_SAVECONFIG_BUTTON, &CBackStagePageInfo::OnBnClickedSaveconfig)
	ON_BN_CLICKED(IDC_LOGIN_BUTTON, &CBackStagePageInfo::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_ACCOUNT_BUTTON, &CBackStagePageInfo::OnBnClicked_AccountButton)
END_MESSAGE_MAP()

CBackStagePageInfo::CBackStagePageInfo(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CBackStagePageInfo::IDD, pParent)
{
	EnableLayout();
	// Enable D2D support for this window:
	EnableD2DSupport();
	/*those memory allocation need to be freed?*/
	m_pBlackBrush = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));

	m_pTextFormat = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 50);
	m_pTextFormat->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2D1_GRADIENT_STOP gradientStops[2];

	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
	gradientStops[0].position = 0.f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Indigo);
	gradientStops[1].position = 1.f;

	m_pLinearGradientBrush = new CD2DLinearGradientBrush(GetRenderTarget(),
															gradientStops, 
															ARRAYSIZE(gradientStops),
															D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, 0)));
	/**/
}

CBackStagePageInfo::~CBackStagePageInfo()
{
}

void CBackStagePageInfo::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW, m_btnPreview);
	DDX_Control(pDX, IDC_USERNAME_EDIT, user_name_edit_);
	DDX_Control(pDX, IDC_USEREMAIL_EDIT, user_email_edit_);
	DDX_Control(pDX, IDC_USERNAMELOGIN_EDIT, user_name_login_);
	DDX_Control(pDX, IDC_USEREMAILLOGIN_EDIT, password_log_in_);
	DDX_Control(pDX, IDC_ACCOUNT_BUTTON, avatar_btn_);
}




/////////////////////////////////////////////////////////////////////////////
// CBackStagePageInfo message handlers
  

BOOL CBackStagePageInfo::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();

	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	ASSERT_VALID(pLayout);
	//:AC: Every time you change layout in IDD_FORM_INFO you have to add appropriate changes here
	pLayout->AddAnchor(IDC_PATH_LABEL, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	
	pLayout->AddAnchor(IDC_SEPARATOR_1, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	//pLayout->AddAnchor(IDC_TEXT1, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	
	//pLayout->AddAnchor(IDC_SEPARATOR_2, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	//pLayout->AddAnchor(IDC_TEXT2, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	
	//pLayout->AddAnchor(IDC_SEPARATOR_3, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	//pLayout->AddAnchor(IDC_TEXT3, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
	
	pLayout->AddAnchor(IDC_SEPARATOR_4, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeVert);
	pLayout->AddAnchor(IDC_INFO_LABEL, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
	
// 	pLayout->AddAnchor(IDC_USERNAME_STATIC, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
// 	pLayout->AddAnchor(IDC_USEREMAIL_STATIC, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
// 	pLayout->AddAnchor(IDC_USERNAME_EDIT, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
// 	pLayout->AddAnchor(IDC_USEREMAIL_EDIT, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
	
	pLayout->AddAnchor(IDC_SEPARATOR_5, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
	pLayout->AddAnchor(IDC_PREVIEW, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);
	//pLayout->AddAnchor(IDC_TEXT4, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone);

	//*THIS PART OF CODE IS RESPONSIBLE FOR DRAWING PREVIEW BITMAP OF APP*//
// 	CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
// 	if (pFrame != NULL)
// 	{
// 		CDocument* pDoc = pFrame->GetActiveDocument();
// 		if (pDoc != NULL)
// 		{
// 			m_strDocName = pDoc->GetTitle();
// 			m_strPath = pDoc->GetPathName();
// 		}
// 	}
// 
// 	PreparePreviewBitmap();

	/*remove those two lines below*/
	user_name_login_.SetWindowText(_T("atch.cpp@gmail.com"));
	password_log_in_.SetWindowText(_T("@A445566tch@"));
	/**/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// void CBackStagePageInfo::OnPaint() 
// {
// 	//CPaintDC dc(this); // device context for painting
// 	//OnDrawDirect2D();
// 	//OnDraw(&dc);//:AC: replace this call into OnDrawDirect2D
// }

afx_msg LRESULT CBackStagePageInfo::OnDrawDirect2D(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	
	
	CRect rect;
 	GetClientRect(rect);
 	pRenderTarget->FillRectangle(rect, m_pLinearGradientBrush);
	
	GetDlgItem(IDC_PATH_LABEL)->GetWindowRect(rect);
	ScreenToClient(rect);
	
	pRenderTarget->DrawText(_T("ACCOUNTS"), rect, m_pBlackBrush);
	
	GetDlgItem(IDC_USERNAME_STATIC)->GetWindowRect(rect);
	ScreenToClient(rect);
	pRenderTarget->DrawText(_T("User name:"), rect, m_pBlackBrush);
	
	GetDlgItem(IDC_USEREMAIL_STATIC)->GetWindowRect(rect);
	ScreenToClient(rect);
	pRenderTarget->DrawText(_T("Email:"), rect, m_pBlackBrush);
	if (!isEmailAdressValid_)
	{
		GetDlgItem(IDC_USEREMAIL_ERROR_STATIC)->GetWindowRect(rect);
		ScreenToClient(rect);
		pRenderTarget->DrawText(_T("Email Adress Invalid"), rect, m_pBlackBrush);
	}

	return TRUE;
}

void CBackStagePageInfo::OnDraw(CDC* pDC)
{
	CRect rectPath;

	GetDlgItem(IDC_PATH_LABEL)->GetWindowRect(rectPath);
	ScreenToClient(rectPath);

	CRect rectInfo;

	GetDlgItem(IDC_INFO_LABEL)->GetWindowRect(rectInfo);
	ScreenToClient(rectInfo);

	CFont* pOldFont = pDC->SelectObject(&globalData.fontCaption);
	ASSERT_VALID(pOldFont);

	//pDC->SetTextColor(CBCGPVisualManager::GetInstance()->GetRibbonBackstageInfoTextColor());
	pDC->SetBkMode(TRANSPARENT);
	
	CString strInfo = _T("Information");
	pDC->DrawText(strInfo, rectInfo, DT_SINGLELINE | DT_END_ELLIPSIS);
	
	int nTextHeight = pDC->DrawText(m_strDocName, rectPath, DT_SINGLELINE | DT_END_ELLIPSIS);
	
	rectPath.top += nTextHeight + 5;
	
	pDC->SelectObject(&globalData.fontRegular);
	
	pDC->DrawText(m_strPath, rectPath, DT_SINGLELINE | DT_END_ELLIPSIS);
	
	pDC->SelectObject(pOldFont);
}

void CBackStagePageInfo::PreparePreviewBitmap()
{
	CRect rectPreview;
	m_btnPreview.GetClientRect(rectPreview);
	
	CBitmap& bmpAppPreview = ((CMainFrame*)AfxGetMainWnd())->GetAppPreview();
	
	BITMAP bmp;
	bmpAppPreview.GetBitmap(&bmp);
	
	double dblScaleRatio = std::min((double)rectPreview.Width() / bmp.bmWidth, (double)rectPreview.Height() / bmp.bmHeight);
	
	CSize szDst((int)(bmp.bmWidth * dblScaleRatio), (int)(bmp.bmHeight * dblScaleRatio));
	
	HBITMAP hBitmap = CBCGPDrawManager::CreateBitmap_24(szDst, NULL);
	
	{
		CDC dcDst;
		dcDst.CreateCompatibleDC(NULL);
		
		HBITMAP hBitmapOld = (HBITMAP)dcDst.SelectObject (hBitmap);
		
		CBCGPDrawManager dm(dcDst);
		int nShadowSize = 5;
		
		szDst.cx -= 2 * nShadowSize;
		szDst.cy -= 2 * nShadowSize;
		
		CBCGPVisualManager::GetInstance()->OnFillRibbonBackstageForm(&dcDst, this, rectPreview);
		dm.DrawShadow(CRect(CPoint(nShadowSize, nShadowSize), szDst), nShadowSize);
		
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(&dcDst);
		
		HBITMAP hBitmapViewOld = (HBITMAP)dcSrc.SelectObject(bmpAppPreview.GetSafeHandle());
		
		dcDst.SetStretchBltMode(HALFTONE);
		
		dcDst.StretchBlt(nShadowSize, nShadowSize, szDst.cx, szDst.cy, &dcSrc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		
		dcSrc.SelectObject (hBitmapViewOld);
		dcDst.SelectObject (hBitmapOld);
	}
	
	m_btnPreview.SetMouseCursorHand();
	m_btnPreview.SetDrawText(FALSE);
	m_btnPreview.m_bDrawFocus = FALSE;
	m_btnPreview.SetImage(hBitmap);
	m_btnPreview.m_bTransparent = TRUE;
	m_btnPreview.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_NOBORDERS;
	m_btnPreview.SizeToContent();
	
	bmpAppPreview.DeleteObject();
}

LRESULT CBackStagePageInfo::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if (lp & PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

void CBackStagePageInfo::OnPreview() 
{
	GetParent()->SendMessage(WM_CLOSE);
}



void CBackStagePageInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	m_pLinearGradientBrush->SetEndPoint(CPoint(cx, cy));
}


void CBackStagePageInfo::OnEnChangeUsernameEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CBackStagePageInfo::OnEnChangeUseremailEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
// 	CString windowText;
// 	user_email_edit_.GetWindowText(windowText);
// 	if (windowText.GetLength() > 0)
//	{
		timer_ID_ = SetTimer(1, 1000, nullptr);//this will call OnTimer every .5 sec
// 	}
// 	else
// 	{
// 		isEmailAdressValid_ = true;
// 		CRect rect;
// 		GetDlgItem(IDC_USEREMAIL_ERROR_STATIC)->GetWindowRect(rect);
// 		InvalidateRect(rect);
// 	}
}

void CBackStagePageInfo::validate_email_(CEdit* ceditCtrl)
{
	CString windowText;
	ceditCtrl->GetWindowText(windowText);
	
	try
	{
		EmailValidator email_validator(windowText);
		isEmailAdressValid_ = true;
	}
	catch (const wrong_email_address_format& e)
	{
		MessageBeep(0x00000030L);
		isEmailAdressValid_ = false;
	}
	//Make sure that the region gets repainted
	CRect rect;
	GetDlgItem(IDC_USEREMAIL_ERROR_STATIC)->GetWindowRect(rect);
	InvalidateRect(rect);
}

void CBackStagePageInfo::OnTimer(UINT nIDEvent)
{
	validate_email_(&user_email_edit_);
	KillTimer(timer_ID_);
}


void CBackStagePageInfo::OnEnKillfocusUseremailEdit()
{
	// TODO: Add your control notification handler code here
	KillTimer(timer_ID_);
}


void CBackStagePageInfo::OnBnClickedSaveconfig()
{
	// TODO: Add your control notification handler code here
	git_config* config{nullptr};
	CGit_05App* app = get_main_app_();

	git_repository_config(&config, app->repos_[0]);
}


void CBackStagePageInfo::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	get_login_credentials_();
}

void CBackStagePageInfo::get_login_credentials_()
{
	CString user_name;
	user_name_login_.GetWindowText(user_name);
	CString password;
	password_log_in_.GetWindowText(password);

	CT2CA c_str_username(user_name);
	CT2CA c_str_password(password);

	git_cred* credentials{ nullptr };
	int ret = git_cred_userpass_plaintext_new(&credentials, c_str_username, c_str_password);

}


void CBackStagePageInfo::OnBnClicked_AccountButton()
{
	// TODO: Add your control notification handler code here
	CGit_05App* app = get_main_app_();
	app->https_GIT_client_ptr_->current_user_avatar();
}
