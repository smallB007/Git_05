// Git_05Doc.h : interface of the CGit_05Doc class
//


#pragma once


class CGit_05Doc : public CDocument
{
protected: // create from serialization only
	CGit_05Doc();
	DECLARE_DYNCREATE(CGit_05Doc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGit_05Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


