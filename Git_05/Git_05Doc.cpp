// Git_05Doc.cpp : implementation of the CGit_05Doc class
//

#include "stdafx.h"
#include "Git_05.h"

#include "Git_05Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGit_05Doc

IMPLEMENT_DYNCREATE(CGit_05Doc, CDocument)

BEGIN_MESSAGE_MAP(CGit_05Doc, CDocument)
END_MESSAGE_MAP()


// CGit_05Doc construction/destruction

CGit_05Doc::CGit_05Doc()
{
	// TODO: add one-time construction code here

}

CGit_05Doc * CGit_05Doc::GetDoc()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (CGit_05Doc*)pFrame->GetActiveDocument();
}

void CGit_05Doc::set_file_for_display(const diffed_file_t& diffedFile)
{
	diffed_file_ = diffedFile;
	CGit_05App * app_p = static_cast<CGit_05App*>(AfxGetApp());
	app_p->invalidate_view();
}

CGit_05Doc::~CGit_05Doc()
{
}

BOOL CGit_05Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGit_05Doc serialization

void CGit_05Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CGit_05Doc diagnostics

#ifdef _DEBUG
void CGit_05Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGit_05Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGit_05Doc commands
