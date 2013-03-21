// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFONTSTORAGE_H__0E1DF42D_F0B1_4A72_8D1A_AC5CFD8412D0__INCLUDED_)
#define AFX_BFONTSTORAGE_H__0E1DF42D_F0B1_4A72_8D1A_AC5CFD8412D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBFont;

class CBFontStorage : public CBBase  
{
public:
	HRESULT RegisterTTFont(char* FontPath);
	CBArray<char*, char*> m_RegTTFonts;
	DECLARE_PERSISTENT(CBFontStorage, CBBase);
	HRESULT Cleanup(bool Warn=false);	
	HRESULT RemoveFont(CBFont* Font);
	CBFont* AddFont(char* Filename);
	CBFontStorage(CBGame* inGame);
	virtual ~CBFontStorage();
	CBArray<CBFont*, CBFont*> m_Fonts;

private:
	void RegTTFontInternal(char* fileName);
};

#endif // !defined(AFX_BFONTSTORAGE_H__0E1DF42D_F0B1_4A72_8D1A_AC5CFD8412D0__INCLUDED_)
