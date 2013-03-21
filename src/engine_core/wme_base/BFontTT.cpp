// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\bfonttt.h"

IMPLEMENT_PERSISTENT(CBFontTT, false);

//////////////////////////////////////////////////////////////////////////
CBFontTT::CBFontTT(CBGame* inGame):CBFont(inGame)
{
	m_HFont = NULL;

	m_FontHeight = 12;
	m_IsBold = m_IsItalic = m_IsUnderline = m_IsStriked = false;

	m_Typeface = NULL;
	CBUtils::SetString(&m_Typeface, "Arial");
	m_FontFile = NULL;

	m_Charset = 0;

	for(int i=0; i<NUM_CACHED_TEXTS; i++) m_CachedTexts[i] = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBFontTT::~CBFontTT(void)
{
	DeleteWinFont();
	ClearCache();

	int i;
	for(i=0; i<m_Layers.GetSize(); i++)
	{
		delete m_Layers[i];
	}
	m_Layers.RemoveAll();

	SAFE_DELETE_ARRAY(m_Typeface);
	SAFE_DELETE_ARRAY(m_FontFile);
}


//////////////////////////////////////////////////////////////////////////
void CBFontTT::ClearCache()
{
	for(int i=0; i<NUM_CACHED_TEXTS; i++)
	{
		if(m_CachedTexts[i]) delete m_CachedTexts[i];
		m_CachedTexts[i] = NULL;
	}

}

//////////////////////////////////////////////////////////////////////////
HRESULT CBFontTT::CreateWinFont()
{
	if(m_FontFile) Game->m_FontStorage->RegisterTTFont(m_FontFile);

	if(m_HFont) DeleteObject(m_HFont);

	HDC hDC = CreateCompatibleDC(NULL);
	SetMapMode(hDC, MM_TEXT);
	int Height = -MulDiv(m_FontHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	DeleteDC(hDC);

	m_HFont = ::CreateFont(Height, 0, 0, 0,
		m_IsBold?FW_BOLD:FW_NORMAL,
		m_IsItalic,
		m_IsUnderline,
		m_IsStriked,
		GetWindowsCharset(m_Charset),
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		m_Typeface);
	
	if(m_HFont) return S_OK;
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBFontTT::DeleteWinFont()
{
	if(m_HFont) DeleteObject(m_HFont);
	m_HFont = NULL;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CBFontTT::GetTextWidth(BYTE* Text, int MaxLength)
{
	int Ret = 0;

	if(FAILED(CreateWinFont())) return 0;

	HDC hDC = CreateCompatibleDC(NULL);
	SetMapMode(hDC, MM_TEXT);

	HGDIOBJ OldFont = SelectObject(hDC, m_HFont);

	BYTE* ConvertedText = CBTextUtils::ConvertToNative(Game->m_TextEncoding, Text);
	
	if(Game->m_TextEncoding==TEXT_ANSI)
	{
		SIZE Size;
		int Length = strlen((char*)ConvertedText);
		GetTextExtentPoint32(hDC, (char*)ConvertedText, Length, &Size);
		Ret = Size.cx;
	}
	else if(Game->m_TextEncoding==TEXT_UTF8)
	{
		SIZE Size;
		int Length = wcslen((wchar_t*)ConvertedText);
		GetTextExtentPoint32W(hDC, (wchar_t*)ConvertedText, Length, &Size);
		Ret = Size.cx;
	}
	
	delete [] ConvertedText;

	::SelectObject(hDC, OldFont);
	DeleteDC(hDC);

	DeleteWinFont();

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
int CBFontTT::GetTextHeight(BYTE* Text, int Width)
{
	if(FAILED(CreateWinFont())) return 0;

	HDC hDC = CreateCompatibleDC(NULL);
	SetMapMode(hDC, MM_TEXT);

	HGDIOBJ OldFont = SelectObject(hDC, m_HFont);

	BYTE* ConvertedText = CBTextUtils::ConvertToNative(Game->m_TextEncoding, Text);

	int Ret = FormatText(0, 0, ConvertedText, hDC, Width, -1);
	delete [] ConvertedText;

	::SelectObject(hDC, OldFont);
	DeleteDC(hDC);

	DeleteWinFont();

	return Ret;
}


//////////////////////////////////////////////////////////////////////////
void CBFontTT::DrawText(BYTE* Text, int X, int Y, int Width, TTextAlign Align, int MaxHeight, int MaxLength)
{
	if(Text==NULL || strcmp((char*)Text, "")==0) return;

	if(Game->m_UseD3D) DrawTextD3D(Text, X, Y, Width, Align, MaxHeight, MaxLength);
	else DrawTextDD(Text, X, Y, Width, Align, MaxHeight, MaxLength);
}

//////////////////////////////////////////////////////////////////////////
void CBFontTT::DrawTextDD(BYTE* Text, int X, int Y, int Width, TTextAlign Align, int MaxHeight, int MaxLength)
{
	CBRenderDD* m_Renderer = (CBRenderDD*)Game->m_Renderer;

	if(FAILED(CreateWinFont())) return;

	HDC hDC;
	m_Renderer->m_BackBuffer->GetDC(&hDC);
	
	HFONT OldFont = (HFONT)SelectObject(hDC, m_HFont);
		
	::SetBkMode(hDC, TRANSPARENT);
	
	int TextX = 0;
	SetTextAlign(hDC, Align, Width, &TextX);

	BYTE* ConvertedText = CBTextUtils::ConvertToNative(Game->m_TextEncoding, Text, MaxLength);

	for(int i=0; i<m_Layers.GetSize(); i++)
	{
		::SetTextColor(hDC, CBUtils::D3D2COLORREF(m_Layers[i]->m_Color));
		FormatText(X + TextX+m_Layers[i]->m_OffsetX, Y+m_Layers[i]->m_OffsetY, ConvertedText, hDC, Width, MaxHeight);
	}


	

	::SelectObject(hDC, OldFont);	
	m_Renderer->m_BackBuffer->ReleaseDC(hDC);

	DeleteWinFont();
	delete [] ConvertedText;
}


//////////////////////////////////////////////////////////////////////////
void CBFontTT::DrawTextD3D(BYTE* Text, int X, int Y, int Width, TTextAlign Align, int MaxHeight, int MaxLength)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	// find cached surface, if exists
	int MinPriority = INT_MAX;
	int MaxPriority = 0;
	int MinIndex = -1;	
	CBSurface* Surface = NULL;
	for(int i=0; i<NUM_CACHED_TEXTS; i++)
	{
		if(m_CachedTexts[i]==NULL)
		{
			MinPriority = 0;
			MinIndex = i;
		}
		else
		{
			MaxPriority = max(MaxPriority, m_CachedTexts[i]->m_Priority);

			if(strcmp(m_CachedTexts[i]->m_Text, (char*)Text)==0 && m_CachedTexts[i]->m_Align==Align && m_CachedTexts[i]->m_Width==Width && m_CachedTexts[i]->m_MaxHeight==MaxHeight && m_CachedTexts[i]->m_MaxLength==MaxLength)
			{
				Surface = m_CachedTexts[i]->m_Surface;
				m_CachedTexts[i]->m_Priority++;
				break;
			}
			else
			{
				if(m_CachedTexts[i]->m_Priority < MinPriority)
				{
					MinPriority = m_CachedTexts[i]->m_Priority;
					MinIndex = i;
				}
			}
		}
	}

	// not found, create one
	if(!Surface)
	{
		if(FAILED(CreateWinFont())) return;

		HDC hDC = ::CreateCompatibleDC(NULL);
		::SetMapMode(hDC, MM_TEXT);
		
		HFONT OldFont = (HFONT)::SelectObject(hDC, m_HFont);

		::SetTextColor(hDC, RGB(255,255,255));
		::SetBkColor(hDC, 0x00000000);
		
		int TextX = 0;
		SetTextAlign(hDC, Align, Width, &TextX);
		
		BYTE* ConvertedText = CBTextUtils::ConvertToNative(Game->m_TextEncoding, Text, MaxLength);

		//ExtTextOut(hDC, 0, 0, ETO_OPAQUE, NULL, (char*)Text, strlen((char*)Text), NULL);
		int Height = FormatText(TextX, 0, ConvertedText, hDC, Width, MaxHeight);
		Height = max(Height, 1);

		DWORD* BitmapBits;
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       =  (int)Width;
		bmi.bmiHeader.biHeight      = -(int)Height;
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biBitCount    = 32;

		HBITMAP hbmBitmap = ::CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (VOID**)&BitmapBits, NULL, 0);
		HBITMAP OldBitmap = (HBITMAP)::SelectObject(hDC, hbmBitmap);

		FormatText(TextX, 0, ConvertedText, hDC, Width, MaxHeight);

		CBImage* Image = new CBImage(Game);
		Image->CreateFromRaw(BitmapBits, Width, Height);
		Surface = Image->CreateSurface();

		delete Image;

		::SelectObject(hDC, OldBitmap);
		::SelectObject(hDC, OldFont);
		::DeleteObject(hbmBitmap);		
		::DeleteDC(hDC);
		
		DeleteWinFont();

		delete []  ConvertedText;

		// write surface to cache
		if(m_CachedTexts[MinIndex]!=NULL) delete m_CachedTexts[MinIndex];
		m_CachedTexts[MinIndex] = new CBCachedTTFontText;
		
		m_CachedTexts[MinIndex]->m_Surface = Surface;
		m_CachedTexts[MinIndex]->m_Align = Align;
		m_CachedTexts[MinIndex]->m_Width = Width;
		m_CachedTexts[MinIndex]->m_MaxHeight = MaxHeight;
		m_CachedTexts[MinIndex]->m_MaxLength = MaxLength;
		m_CachedTexts[MinIndex]->m_Priority = MaxPriority + 1;
		CBUtils::SetString(&m_CachedTexts[MinIndex]->m_Text, (char*)Text);
	}

	// and paint it
	if(Surface)
	{
		RECT rc;
		SetRect(&rc, 0, 0, Surface->GetWidth(), Surface->GetHeight());
		for(int i=0; i<m_Layers.GetSize(); i++)
		{
			DWORD Color = m_Layers[i]->m_Color;
			DWORD OrigForceAlpha = m_Renderer->m_ForceAlphaColor;
			if(m_Renderer->m_ForceAlphaColor!=0)
			{
				Color = DRGBA(D3DCOLGetR(Color), D3DCOLGetG(Color), D3DCOLGetB(Color), D3DCOLGetA(m_Renderer->m_ForceAlphaColor));
				m_Renderer->m_ForceAlphaColor = 0;
			}
			Surface->DisplayTrans(X+m_Layers[i]->m_OffsetX, Y+m_Layers[i]->m_OffsetY, rc, Color);

			m_Renderer->m_ForceAlphaColor = OrigForceAlpha;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CBFontTT::SetTextAlign(HDC hDC, TTextAlign Align, int Width, int* TextX)
{
	switch(Align)
	{
	case TAL_LEFT:
		::SetTextAlign(hDC, TA_TOP | TA_LEFT);
		*TextX = 0;
		break;
	case TAL_RIGHT:
		::SetTextAlign(hDC, TA_TOP | TA_RIGHT);
		*TextX = Width;
		break;
	case TAL_CENTER:
		::SetTextAlign(hDC, TA_TOP | TA_CENTER);
		*TextX = Width/2;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
int CBFontTT::FormatText(int X, int Y, BYTE* Text, HDC hDC, int Width, int MaxHeight)
{
	if(Game->m_TextEncoding==TEXT_UTF8) return FormatTextW(X, Y, Text, hDC, Width, MaxHeight);

	SIZE Size;
	GetTextExtentPoint32(hDC, "Ay", 2, &Size);
	int LineHeight = Size.cy;

	int TextLen = strlen((char*)Text);
	char* Str = (char*)Text;
	int LastSpace = -1;
	int CurrY = 0;
	
	if(TextLen==0) return 0;

	while(true)
	{
		if(MaxHeight >=0 && CurrY + LineHeight > MaxHeight) return CurrY;

		int i=0;
		LastSpace = -1;

		do
		{
			i++;
			if(Str[i-1]=='\n')
			{
				i--;
				break;
			}
			GetTextExtentPoint32(hDC, Str, i, &Size);
			if(Size.cx <= Width)
			{
				if(Str[i-1]==' ') LastSpace = i;				
			}
		}
		while(Str + i <(char*)Text+TextLen && Size.cx <= Width);

		// safety break
		if(i==1 && Size.cx > Width) return CurrY;
		
		// we encountered a newline
		if(Size.cx > Width)
		{
			if(LastSpace>=0) i = LastSpace-1;
			else i--;
		}

		UINT Flags = ETO_OPAQUE;
		if(Game->m_TextRTL) Flags |= ETO_RTLREADING;
		ExtTextOut(hDC, X, Y + CurrY, Flags, NULL, Str, i, NULL);
		if(LastSpace>=0) i++;
		Str+=i;
		if(Str>(char*)Text+TextLen)
		{
			CurrY+=LineHeight;
			break;
		}

		if(Str[0]=='\n') Str++;
		CurrY+=LineHeight;

		// we're at the end
		if(Str>=(char*)Text+TextLen) break;
	}
	return CurrY /*+ LineHeight*/;
}


//////////////////////////////////////////////////////////////////////////
int CBFontTT::FormatTextW(int X, int Y, BYTE* Text, HDC hDC, int Width, int MaxHeight)
{
	SIZE Size;
	GetTextExtentPoint32(hDC, "Ay", 2, &Size);
	int LineHeight = Size.cy;

	int TextLen = wcslen((wchar_t*)Text);
	wchar_t* Str = (wchar_t*)Text;
	int LastSpace = -1;
	int CurrY = 0;

	if(TextLen==0) return 0;

	while(true)
	{
		if(MaxHeight >=0 && CurrY + LineHeight > MaxHeight) return CurrY;

		int i=0;
		LastSpace = -1;

		do
		{
			i++;
			if(Str[i-1]=='\n')
			{
				i--;
				break;
			}
			GetTextExtentPoint32W(hDC, Str, i, &Size);
			if(Size.cx <= Width)
			{
				if(Str[i-1]==' ') LastSpace = i;				
			}
		}
		while(Str + i <(wchar_t*)Text+TextLen && Size.cx <= Width);

		// safety break
		if(i==1 && Size.cx > Width) return CurrY;

		// we encountered a newline
		if(Size.cx > Width)
		{
			if(LastSpace>=0) i = LastSpace-1;
			else i--;
		}

		UINT Flags = ETO_OPAQUE;
		if(Game->m_TextRTL) Flags |= ETO_RTLREADING;
		ExtTextOutW(hDC, X, Y + CurrY, Flags, NULL, Str, i, NULL);
		if(LastSpace>=0) i++;
		Str+=i;
		if(Str>(wchar_t*)Text+TextLen)
		{
			CurrY+=LineHeight;
			break;
		}

		if(Str[0]=='\n') Str++;
		CurrY+=LineHeight;

		// we're at the end
		if(Str>=(wchar_t*)Text+TextLen) break;
	}
	return CurrY /*+ LineHeight*/;
}

//////////////////////////////////////////////////////////////////////////
int CBFontTT::GetLetterHeight()
{
	if(FAILED(CreateWinFont())) return 0;
	
	HDC hDC = CreateCompatibleDC(NULL);
	SetMapMode(hDC, MM_TEXT);
	
	HGDIOBJ OldFont = SelectObject(hDC, m_HFont);
	SIZE Size;
	GetTextExtentPoint32(hDC, "Ay", 2, &Size);

	::SelectObject(hDC, OldFont);
	DeleteDC(hDC);

	DeleteWinFont();
	
	return Size.cy;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBFontTT::LoadFile(char* Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CBFontTT::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer))) Game->LOG(0, "Error parsing TTFONT file '%s'", Filename);

	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (TTFONT)
	TOKEN_DEF (SIZE)
	TOKEN_DEF (FACE)
	TOKEN_DEF (FILENAME)
	TOKEN_DEF (BOLD)
	TOKEN_DEF (ITALIC)
	TOKEN_DEF (UNDERLINE)
	TOKEN_DEF (STRIKE)
	TOKEN_DEF (CHARSET)
	TOKEN_DEF (COLOR)
	TOKEN_DEF (ALPHA)
	TOKEN_DEF (LAYER)
	TOKEN_DEF (OFFSET_X)
	TOKEN_DEF (OFFSET_Y)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////
HRESULT CBFontTT::LoadBuffer(BYTE * Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (TTFONT)
		TOKEN_TABLE (SIZE)
		TOKEN_TABLE (FACE)
		TOKEN_TABLE (FILENAME)
		TOKEN_TABLE (BOLD)
		TOKEN_TABLE (ITALIC)
		TOKEN_TABLE (UNDERLINE)
		TOKEN_TABLE (STRIKE)
		TOKEN_TABLE (CHARSET)
		TOKEN_TABLE (COLOR)
		TOKEN_TABLE (ALPHA)
		TOKEN_TABLE (LAYER)
	TOKEN_TABLE_END

	char* params;
	int cmd;
	CBParser parser(Game);

	if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_TTFONT){
		Game->LOG(0, "'TTFONT' keyword expected.");
		return E_FAIL;
	}
	Buffer = (BYTE*)params;

	DWORD BaseColor = 0x00000000;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
		case TOKEN_SIZE:
			parser.ScanStr(params, "%d", &m_FontHeight);
			break;
		
		case TOKEN_FACE:
			CBUtils::SetString(&m_Typeface, params);
			break;

		case TOKEN_FILENAME:
			CBUtils::SetString(&m_FontFile, params);
			break;

		case TOKEN_BOLD:
			parser.ScanStr(params, "%b", &m_IsBold);
			break;

		case TOKEN_ITALIC:
			parser.ScanStr(params, "%b", &m_IsItalic);
			break;

		case TOKEN_UNDERLINE:
			parser.ScanStr(params, "%b", &m_IsUnderline);
			break;

		case TOKEN_STRIKE:
			parser.ScanStr(params, "%b", &m_IsStriked);
			break;

		case TOKEN_CHARSET:
			parser.ScanStr(params, "%d", &m_Charset);
			break;

		case TOKEN_COLOR:
			{
				int r, g, b;
				parser.ScanStr(params, "%d,%d,%d", &r, &g, &b);
				BaseColor = DRGBA(r, g, b, D3DCOLGetA(BaseColor));				
			}
			break;

		case TOKEN_ALPHA:
			{
				int a;
				parser.ScanStr(params, "%d", &a);
				BaseColor = DRGBA(D3DCOLGetR(BaseColor), D3DCOLGetG(BaseColor), D3DCOLGetB(BaseColor), a);
			}
			break;

		case TOKEN_LAYER:
			{
				CBTTFontLayer* Layer = new CBTTFontLayer;
				if(Layer && SUCCEEDED(ParseLayer(Layer, (BYTE*)params))) m_Layers.Add(Layer);
				else
				{
					SAFE_DELETE(Layer);
					cmd = PARSERR_TOKENNOTFOUND;
				}
			}
			break;

		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in TTFONT definition");
		return E_FAIL;
	}

	// create at least one layer
	if(m_Layers.GetSize()==0)
	{
		CBTTFontLayer* Layer = new CBTTFontLayer;
		Layer->m_Color = BaseColor;
		m_Layers.Add(Layer);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBFontTT::ParseLayer(CBTTFontLayer* Layer, BYTE* Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (OFFSET_X)
		TOKEN_TABLE (OFFSET_Y)
		TOKEN_TABLE (COLOR)
		TOKEN_TABLE (ALPHA)
	TOKEN_TABLE_END

	char* params;
	int cmd;
	CBParser parser(Game);

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
		case TOKEN_OFFSET_X:
			parser.ScanStr(params, "%d", &Layer->m_OffsetX);
			break;

		case TOKEN_OFFSET_Y:
			parser.ScanStr(params, "%d", &Layer->m_OffsetY);
			break;

		case TOKEN_COLOR:
			{
				int r, g, b;
				parser.ScanStr(params, "%d,%d,%d", &r, &g, &b);
				Layer->m_Color = DRGBA(r, g, b, D3DCOLGetA(Layer->m_Color));
			}
			break;

		case TOKEN_ALPHA:
			{
				int a;
				parser.ScanStr(params, "%d", &a);
				Layer->m_Color = DRGBA(D3DCOLGetR(Layer->m_Color), D3DCOLGetG(Layer->m_Color), D3DCOLGetB(Layer->m_Color), a);
			}
			break;
		}
	}
	if (cmd != PARSERR_EOF) return E_FAIL;
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CBFontTT::GetWindowsCharset(int WMECharset)
{
	switch(WMECharset)
	{
		case 1:  return DEFAULT_CHARSET;
		case 2:  return OEM_CHARSET;
		case 3:  return BALTIC_CHARSET;
		case 4:  return CHINESEBIG5_CHARSET;
		case 5:  return EASTEUROPE_CHARSET;
		case 6:  return GB2312_CHARSET;
		case 7:  return GREEK_CHARSET;
		case 8:  return HANGUL_CHARSET;
		case 9:  return MAC_CHARSET;
		case 10: return RUSSIAN_CHARSET;
		case 11: return SHIFTJIS_CHARSET;
		case 12: return SYMBOL_CHARSET;
		case 13: return TURKISH_CHARSET;
		case 14: return VIETNAMESE_CHARSET;
		case 15: return JOHAB_CHARSET;
		case 16: return ARABIC_CHARSET;
		case 17: return HEBREW_CHARSET;
		case 18: return THAI_CHARSET;
		default: return ANSI_CHARSET;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBFontTT::Persist(CBPersistMgr* PersistMgr)
{
	CBFont::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_IsBold));
	PersistMgr->Transfer(TMEMBER(m_IsItalic));
	PersistMgr->Transfer(TMEMBER(m_IsUnderline));
	PersistMgr->Transfer(TMEMBER(m_IsStriked));
	PersistMgr->Transfer(TMEMBER(m_Typeface));
	PersistMgr->Transfer(TMEMBER(m_FontHeight));
	PersistMgr->Transfer(TMEMBER(m_FontFile));
	PersistMgr->Transfer(TMEMBER(m_Charset));

	// persist layers
	int NumLayers;
	if(PersistMgr->m_Saving)
	{
		NumLayers = m_Layers.GetSize();
		PersistMgr->Transfer(TMEMBER(NumLayers));
		for(int i=0; i<NumLayers; i++) m_Layers[i]->Persist(PersistMgr);
	}
	else
	{
		NumLayers = m_Layers.GetSize();
		PersistMgr->Transfer(TMEMBER(NumLayers));
		for(int i=0; i<NumLayers; i++)
		{
			CBTTFontLayer* Layer = new CBTTFontLayer;
			Layer->Persist(PersistMgr);
			m_Layers.Add(Layer);
		}
	}

	if(!PersistMgr->m_Saving)
	{
		m_HFont = NULL;
		for(int i=0; i<NUM_CACHED_TEXTS; i++) m_CachedTexts[i] = NULL;
	}

	return S_OK;
}
