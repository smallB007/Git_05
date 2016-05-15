// Git_05Doc.h : interface of the CGit_05Doc class
//


#pragma once


class CGit_05Doc : public CDocument
{
private:
	diffed_file_t diffed_file_;
protected: // create from serialization only
	CGit_05Doc();
	DECLARE_DYNCREATE(CGit_05Doc)
public:
	void set_file_for_display(const diffed_file_t& diffedFile);
	diffed_file_t get_diffed_file()const
	{
		return diffed_file_;
	}
	void reset_file_for_display()
	{
		diffed_file_ = diffed_file_t();
	}
// Attributes
public:
	static CGit_05Doc* GetDoc();
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


