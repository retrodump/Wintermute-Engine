// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "bfont.h"

#define NUM_CACHED_TEXTS 50

class CBFontTT : public CBFont
{
private:
	//////////////////////////////////////////////////////////////////////////
	class CBCachedTTFontText
	{
	public:
		char* m_Text;
		int m_Width;
		TTextAlign m_Align;
		int m_MaxHeight;
		int m_MaxLength;
		CBSurface* m_Surface;
		int m_Priority;

		CBCachedTTFontText()
		{
			m_Text = NULL;
			m_Width = m_MaxHeight = m_MaxLength = -1;
			m_Align = TAL_LEFT;
			m_Surface = NULL;
			m_Priority = -1;
		}

		virtual ~CBCachedTTFontText()
		{
			if(m_Text) delete [] m_Text;
			if(m_Surface) delete m_Surface;
		}
	};

public:
	//////////////////////////////////////////////////////////////////////////
	class CBTTFontLayer
	{
	public:
		CBTTFontLayer()
		{
			m_OffsetX = m_OffsetY = 0;
			m_Color = 0x00000000;
		}
		
		HRESULT Persist(CBPersistMgr* PersistMgr)
		{
			PersistMgr->Transfer(TMEMBER(m_OffsetX));
			PersistMgr->Transfer(TMEMBER(m_OffsetY));
			PersistMgr->Transfer(TMEMBER(m_Color));
			return S_OK;
		}
		
		int m_OffsetX;
		int m_OffsetY;
		DWORD m_Color;
	};


public:
	DECLARE_PERSISTENT(CBFontTT, CBFont);
	CBFontTT(CBGame* inGame);
	virtual ~CBFontTT(void);

	virtual int GetTextWidth(BYTE* text, int MaxLenght=-1);
	virtual int GetTextHeight(BYTE* text, int width);
	virtual void DrawText(BYTE* text, int x, int y, int width, TTextAlign align=TAL_LEFT, int max_height=-1, int MaxLenght=-1);
	virtual int GetLetterHeight();

	HRESULT LoadBuffer(BYTE* Buffer);
	HRESULT LoadFile(char* Filename);

private:
	//BYTE* GetConvertedText(BYTE* OrigText, int MaxLength=-1);
	HRESULT CreateWinFont();
	HRESULT DeleteWinFont();
	HFONT m_HFont;
	int FormatText(int X, int Y, BYTE* Text, HDC hdc, int Width, int MaxHeight);
	int FormatTextW(int X, int Y, BYTE* Text, HDC hDC, int Width, int MaxHeight);

	void DrawTextDD(BYTE* text, int x, int y, int width, TTextAlign align=TAL_LEFT, int max_height=-1, int MaxLenght=-1);
	void DrawTextD3D(BYTE* text, int x, int y, int width, TTextAlign align=TAL_LEFT, int max_height=-1, int MaxLenght=-1);
	void SetTextAlign(HDC hDC, TTextAlign Align, int Width, int* TextX);
	HRESULT ParseLayer(CBTTFontLayer* Layer, BYTE* Buffer);
	int GetWindowsCharset(int WMECharset);
	CBCachedTTFontText* m_CachedTexts[NUM_CACHED_TEXTS];
public:
	bool m_IsBold;
	bool m_IsItalic;
	bool m_IsUnderline;
	bool m_IsStriked;
	int m_FontHeight;
	char* m_Typeface;
	char* m_FontFile;
	int m_Charset;	
	CBArray<CBTTFontLayer*, CBTTFontLayer*> m_Layers;
	void ClearCache();
};
