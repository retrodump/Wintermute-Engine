// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFONT_H__F94BE126_3891_11D4_B319_88CB74523214__INCLUDED_)
#define AFX_BFONT_H__F94BE126_3891_11D4_B319_88CB74523214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define NUM_CHARACTERS 256

class CBFont: public CBObject
{
public:			
	DECLARE_PERSISTENT(CBFont, CBObject);
	virtual int GetTextWidth(BYTE* text, int MaxLenght=-1);
	virtual int GetTextHeight(BYTE* text, int width);
	virtual void DrawText(BYTE* text, int x, int y, int width, TTextAlign align=TAL_LEFT, int max_height=-1, int MaxLenght=-1);
	virtual int GetLetterHeight();

	CBFont(CBGame* inGame);
	virtual ~CBFont();

	static CBFont* CreateFromFile(CBGame* Game, char* Filename);

private:
	//HRESULT LoadBuffer(BYTE* Buffer);
	//HRESULT LoadFile(char* Filename);
	static bool IsTrueType(CBGame* Game, char* Filename);
};

#endif // !defined(AFX_BFONT_H__F94BE126_3891_11D4_B319_88CB74523214__INCLUDED_)
