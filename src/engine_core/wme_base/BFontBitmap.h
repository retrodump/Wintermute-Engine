// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "bfont.h"

class CBFontBitmap : public CBFont
{
public:			
	DECLARE_PERSISTENT(CBFontBitmap, CBFont);
	HRESULT LoadBuffer(BYTE* Buffer);
	HRESULT LoadFile(char* Filename);
	virtual int GetTextWidth(BYTE* text, int MaxLenght=-1);
	virtual int GetTextHeight(BYTE* text, int width);
	virtual void DrawText(BYTE* text, int x, int y, int width, TTextAlign align=TAL_LEFT, int max_height=-1, int MaxLenght=-1);
	virtual int GetLetterHeight();

	CBFontBitmap(CBGame* inGame);
	virtual ~CBFontBitmap();


	HRESULT GetWidths();
	CBSprite* m_Sprite;
	int m_WidthsFrame;
	bool m_FontextFix;
	int m_NumColumns;
	int m_TileHeight;
	int m_TileWidth;
	BYTE m_Widths[NUM_CHARACTERS];
	CBSubFrame* m_Subframe;
	bool m_WholeCell;

private:
	int GetCharWidth(BYTE Index);
	void DrawChar(BYTE c, int x, int y);

	int TextHeightDraw(BYTE* text, int x, int y, int width, TTextAlign align, bool draw, int max_height=-1, int MaxLenght=-1);

};
