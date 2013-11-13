// SpriteEditDoc.cpp : implementation of the CSpriteEditDoc class
//

#include "stdafx.h"
#include "SpriteEdit.h"

#include "SpriteEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpriteEditDoc

IMPLEMENT_DYNCREATE(CSpriteEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpriteEditDoc, CDocument)
END_MESSAGE_MAP()


// CSpriteEditDoc construction/destruction

CSpriteEditDoc::CSpriteEditDoc()
{
	// TODO: add one-time construction code here

}

CSpriteEditDoc::~CSpriteEditDoc()
{
}

BOOL CSpriteEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSpriteEditDoc serialization

void CSpriteEditDoc::Serialize(CArchive& ar)
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


// CSpriteEditDoc diagnostics

#ifdef _DEBUG
void CSpriteEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpriteEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpriteEditDoc commands
