// SpriteEditDoc.h : interface of the CSpriteEditDoc class
//


#pragma once


class CSpriteEditDoc : public CDocument
{
protected: // create from serialization only
	CSpriteEditDoc();
	DECLARE_DYNCREATE(CSpriteEditDoc)

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
	virtual ~CSpriteEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


