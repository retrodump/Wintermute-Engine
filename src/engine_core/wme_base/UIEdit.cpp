// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "UIEdit.h"



IMPLEMENT_PERSISTENT(CUIEdit, false);

//////////////////////////////////////////////////////////////////////////
CUIEdit::CUIEdit(CBGame* inGame):CUIObject(inGame)
{
	m_Type = UI_EDIT;

	m_FontSelected = NULL;

	m_SelStart = m_SelEnd = 10000;
	m_ScrollOffset = 0;
	
	m_CursorChar = NULL;
	SetCursorChar("|");

	m_CursorBlinkRate = GetCaretBlinkTime();
	m_FrameWidth = 0;

	SetText("");

	m_LastBlinkTime = 0;
	m_CursorVisible = true;

	m_MaxLength = -1;

	m_CanFocus = true;
}


//////////////////////////////////////////////////////////////////////////
CUIEdit::~CUIEdit()
{
	if(!m_SharedFonts)
	{
		if(m_FontSelected)   Game->m_FontStorage->RemoveFont(m_FontSelected);
	}

	SAFE_DELETE_ARRAY(m_CursorChar);

}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::LoadFile(char* Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL)
	{
		Game->LOG(0, "CUIEdit::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing EDIT file '%s'", Filename);

	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (DISABLED)
	TOKEN_DEF (VISIBLE)
	TOKEN_DEF (BACK)
	TOKEN_DEF (IMAGE)
	TOKEN_DEF (FONT_SELECTED)
	TOKEN_DEF (FONT)
	TOKEN_DEF (TEXT)
	TOKEN_DEF (X)
	TOKEN_DEF (Y)
	TOKEN_DEF (WIDTH)
	TOKEN_DEF (HEIGHT)
	TOKEN_DEF (CURSOR_BLINK_RATE)
	TOKEN_DEF (CURSOR)
	TOKEN_DEF (FRAME_WIDTH)
	TOKEN_DEF (NAME)
	TOKEN_DEF (SCRIPT)
	TOKEN_DEF (PARENT_NOTIFY)
	TOKEN_DEF (MAX_LENGTH)
	TOKEN_DEF (EDITOR_PROPERTY)
	TOKEN_DEF (EDIT)
	TOKEN_DEF (CAPTION)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::LoadBuffer(BYTE* Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (DISABLED)
		TOKEN_TABLE (VISIBLE)
		TOKEN_TABLE (BACK)
		TOKEN_TABLE (IMAGE)
		TOKEN_TABLE (FONT_SELECTED)
		TOKEN_TABLE (FONT)
		TOKEN_TABLE (TEXT)
		TOKEN_TABLE (X)
		TOKEN_TABLE (Y)
		TOKEN_TABLE (WIDTH)
		TOKEN_TABLE (HEIGHT)
		TOKEN_TABLE (CURSOR_BLINK_RATE)
		TOKEN_TABLE (CURSOR)
		TOKEN_TABLE (FRAME_WIDTH)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (SCRIPT)
		TOKEN_TABLE (PARENT_NOTIFY)
		TOKEN_TABLE (MAX_LENGTH)
		TOKEN_TABLE (EDITOR_PROPERTY)
		TOKEN_TABLE (EDIT)
		TOKEN_TABLE (CAPTION)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd=2;
	CBParser parser(Game);

	if(Complete)
	{
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_EDIT)
		{
			Game->LOG(0, "'EDIT' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while (cmd > 0 && (cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_TEMPLATE:
				if(FAILED(LoadFile((char*)params))) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_NAME:
				SetName((char*)params);
			break;

			case TOKEN_BACK:
				SAFE_DELETE(m_Back);
				m_Back = new CUITiledImage(Game);
				if(!m_Back || FAILED(m_Back->LoadFile((char*)params)))
				{
					SAFE_DELETE(m_Back);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_IMAGE:
				SAFE_DELETE(m_Image);
				m_Image = new CBSprite(Game);
				if(!m_Image || FAILED(m_Image->LoadFile((char*)params)))
				{
					SAFE_DELETE(m_Image);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_FONT:
				if(m_Font) Game->m_FontStorage->RemoveFont(m_Font);
				m_Font = Game->m_FontStorage->AddFont((char*)params);
				if(!m_Font) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_FONT_SELECTED:
				if(m_FontSelected) Game->m_FontStorage->RemoveFont(m_FontSelected);
				m_FontSelected = Game->m_FontStorage->AddFont((char*)params);
				if(!m_FontSelected) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_TEXT:
				SetText((char*)params);
				Game->m_StringTable->Expand(&m_Text);
			break;

			case TOKEN_X:
				parser.ScanStr((char*)params, "%d", &m_PosX);
			break;

			case TOKEN_Y:
				parser.ScanStr((char*)params, "%d", &m_PosY);
			break;

			case TOKEN_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_Width);
			break;

			case TOKEN_HEIGHT:
				parser.ScanStr((char*)params, "%d", &m_Height);
			break;

			case TOKEN_MAX_LENGTH:
				parser.ScanStr((char*)params, "%d", &m_MaxLength);
			break;

			case TOKEN_CAPTION:
				SetCaption((char*)params);
			break;

			case TOKEN_CURSOR:
				SAFE_DELETE(m_Cursor);
				m_Cursor = new CBSprite(Game);
				if(!m_Cursor || FAILED(m_Cursor->LoadFile((char*)params)))
				{
					SAFE_DELETE(m_Cursor);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_CURSOR_BLINK_RATE:
				parser.ScanStr((char*)params, "%d", &m_CursorBlinkRate);
			break;

			case TOKEN_FRAME_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_FrameWidth);
			break;

			case TOKEN_SCRIPT:
				AddScript((char*)params);
			break;

			case TOKEN_PARENT_NOTIFY:
				parser.ScanStr((char*)params, "%b", &m_ParentNotify);
			break;

			case TOKEN_DISABLED:
				parser.ScanStr((char*)params, "%b", &m_Disable);
			break;

			case TOKEN_VISIBLE:
				parser.ScanStr((char*)params, "%b", &m_Visible);
			break;
		
			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;		
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in EDIT definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC)
	{
		Game->LOG(0, "Error loading EDIT definition");
		return E_FAIL;
	}

	CorrectSize();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::SaveAsText(CBDynBuffer* Buffer, int Indent)
{
	Buffer->PutTextIndent(Indent, "EDIT\n");
	Buffer->PutTextIndent(Indent, "{\n");

	Buffer->PutTextIndent(Indent+2, "NAME=\"%s\"\n", m_Name);
	Buffer->PutTextIndent(Indent+2, "CAPTION=\"%s\"\n", GetCaption());

	Buffer->PutTextIndent(Indent+2, "\n");

	if(m_Back && m_Back->m_Filename)
		Buffer->PutTextIndent(Indent+2, "BACK=\"%s\"\n", m_Back->m_Filename);

	if(m_Image && m_Image->m_Filename)
		Buffer->PutTextIndent(Indent+2, "IMAGE=\"%s\"\n", m_Image->m_Filename);

	if(m_Font && m_Font->m_Filename)
		Buffer->PutTextIndent(Indent+2, "FONT=\"%s\"\n", m_Font->m_Filename);
	if(m_FontSelected && m_FontSelected->m_Filename)
		Buffer->PutTextIndent(Indent+2, "FONT_SELECTED=\"%s\"\n", m_FontSelected->m_Filename);

	if(m_Cursor && m_Cursor->m_Filename)
		Buffer->PutTextIndent(Indent+2, "CURSOR=\"%s\"\n", m_Cursor->m_Filename);

	Buffer->PutTextIndent(Indent+2, "\n");

	if(m_Text)
		Buffer->PutTextIndent(Indent+2, "TEXT=\"%s\"\n", m_Text);

	Buffer->PutTextIndent(Indent+2, "\n");

	Buffer->PutTextIndent(Indent+2, "X=%d\n", m_PosX);
	Buffer->PutTextIndent(Indent+2, "Y=%d\n", m_PosY);
	Buffer->PutTextIndent(Indent+2, "WIDTH=%d\n", m_Width);
	Buffer->PutTextIndent(Indent+2, "HEIGHT=%d\n", m_Height);
	Buffer->PutTextIndent(Indent+2, "MAX_LENGTH=%d\n", m_MaxLength);
	Buffer->PutTextIndent(Indent+2, "CURSOR_BLINK_RATE=%d\n", m_CursorBlinkRate);
	Buffer->PutTextIndent(Indent+2, "FRAME_WIDTH=%d\n", m_FrameWidth);

	Buffer->PutTextIndent(Indent+2, "DISABLED=%s\n", m_Disable?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "VISIBLE=%s\n", m_Visible?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "PARENT_NOTIFY=%s\n", m_ParentNotify?"TRUE":"FALSE");

	// scripts
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		Buffer->PutTextIndent(Indent+2, "SCRIPT=\"%s\"\n", m_Scripts[i]->m_Filename);
	}

	Buffer->PutTextIndent(Indent+2, "\n");

	// editor properties
	CBBase::SaveAsText(Buffer, Indent+2);

	Buffer->PutTextIndent(Indent, "}\n");
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// SetSelectedFont
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SetSelectedFont")==0)
	{
		Stack->CorrectParams(1);

		if(m_FontSelected) Game->m_FontStorage->RemoveFont(m_FontSelected);
		m_FontSelected = Game->m_FontStorage->AddFont(Stack->Pop()->GetString());
		Stack->PushBool(m_FontSelected!=NULL);

		return S_OK;
	}

	else return CUIObject::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CUIEdit::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0)
	{
		m_ScValue->SetString("editor");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SelStart
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SelStart")==0)
	{
		m_ScValue->SetInt(m_SelStart);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SelEnd
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SelEnd")==0)
	{
		m_ScValue->SetInt(m_SelEnd);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// CursorBlinkRate
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CursorBlinkRate")==0)
	{
		m_ScValue->SetInt(m_CursorBlinkRate);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// CursorChar
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CursorChar")==0)
	{
		m_ScValue->SetString(m_CursorChar);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// FrameWidth
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FrameWidth")==0)
	{
		m_ScValue->SetInt(m_FrameWidth);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MaxLength
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MaxLength")==0)
	{
		m_ScValue->SetInt(m_MaxLength);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Text
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Text")==0)
	{
		if(Game->m_TextEncoding==TEXT_UTF8)
		{
			char* Utf8Text = CBTextUtils::AnsiToUtf8(m_Text);
			m_ScValue->SetString(Utf8Text);
			delete [] Utf8Text;
		}
		else
		{
			m_ScValue->SetString(m_Text);
		}
		return m_ScValue;
	}

	else return CUIObject::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// SelStart
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SelStart")==0)
	{
		m_SelStart = Value->GetInt();
		m_SelStart = max(m_SelStart, 0);
		m_SelStart = min(m_SelStart, strlen(m_Text));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SelEnd
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SelEnd")==0)
	{
		m_SelEnd = Value->GetInt();
		m_SelEnd = max(m_SelEnd, 0);
		m_SelEnd = min(m_SelEnd, strlen(m_Text));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// CursorBlinkRate
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CursorBlinkRate")==0)
	{
		m_CursorBlinkRate = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// CursorChar
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CursorChar")==0)
	{
		SetCursorChar(Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// FrameWidth
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FrameWidth")==0)
	{
		m_FrameWidth = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MaxLength
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MaxLength")==0)
	{
		m_MaxLength = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Text
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Text")==0)
	{
		if(Game->m_TextEncoding)
		{
			char* Text = CBTextUtils::Utf8ToAnsi(Value->GetString());
			SetText(Text);
			delete [] Text;
		}
		else
		{
			SetText(Value->GetString());
		}
		return S_OK;
	}

	else return CUIObject::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CUIEdit::ScToString()
{
	return "[edit]";
}


//////////////////////////////////////////////////////////////////////////
void CUIEdit::SetCursorChar(char *Char)
{
	if(!Char) return;
	SAFE_DELETE_ARRAY(m_CursorChar);
	m_CursorChar = new char [strlen(Char)+1];
	if(m_CursorChar) strcpy(m_CursorChar, Char);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::Display(int OffsetX, int OffsetY)
{
	if(!m_Visible) return S_OK;


	// hack!
	TTextEncoding OrigEncoding = Game->m_TextEncoding;
	Game->m_TextEncoding = TEXT_ANSI;

	if(m_Back) m_Back->Display(OffsetX + m_PosX, OffsetY + m_PosY, m_Width, m_Height);
	if(m_Image) m_Image->Draw(OffsetX + m_PosX, OffsetY + m_PosY, NULL);

	// prepare fonts
	CBFont* font;
	CBFont* sfont;

	if(m_Font) font = m_Font;
	else font = Game->m_SystemFont;

	if(m_FontSelected) sfont = m_FontSelected;
	else sfont = font;

	bool focused = IsFocused();

	m_SelStart = max(m_SelStart, 0);
	m_SelEnd   = max(m_SelEnd, 0);

	m_SelStart = min(m_SelStart, strlen(m_Text));
	m_SelEnd   = min(m_SelEnd,   strlen(m_Text));

	//int CursorWidth = font->GetCharWidth(m_CursorChar[0]);
	int CursorWidth = font->GetTextWidth((BYTE*)m_CursorChar);

	int s1, s2;
	bool CurFirst;
	// modify scroll offset
	if(m_SelStart >= m_SelEnd)
	{
		while(font->GetTextWidth((BYTE*)m_Text+m_ScrollOffset, max(0, m_SelEnd - m_ScrollOffset))>m_Width-CursorWidth-2*m_FrameWidth)
		{
			m_ScrollOffset++;
			if(m_ScrollOffset >= strlen(m_Text)) break;
		}

		m_ScrollOffset = min(m_ScrollOffset, m_SelEnd);

		s1 = m_SelEnd;
		s2 = m_SelStart;
		CurFirst = true;
	}
	else
	{
		while(font->GetTextWidth((BYTE*)m_Text+m_ScrollOffset, max(0, m_SelStart-m_ScrollOffset)) + 
			  sfont->GetTextWidth((BYTE*)(m_Text + max(m_ScrollOffset, m_SelStart)), m_SelEnd - max(m_ScrollOffset, m_SelStart))
			  
		>m_Width-CursorWidth-2*m_FrameWidth)
		{
			m_ScrollOffset++;
			if(m_ScrollOffset >= strlen(m_Text)) break;
		}

		m_ScrollOffset = min(m_ScrollOffset, m_SelEnd);

		s1 = m_SelStart;
		s2 = m_SelEnd;
		CurFirst = false;
	}


	int AlignOffset = 0;

	for(int Count = 0; Count < 2; Count++)
	{
		// draw text
		int xxx, yyy, width, height;

		xxx = m_PosX+m_FrameWidth + OffsetX;
		yyy = m_PosY+m_FrameWidth + OffsetY;

		width = m_PosX + m_Width + OffsetX - m_FrameWidth;
		height = max(font->GetLetterHeight(), sfont->GetLetterHeight());

		if(Game->m_TextRTL) xxx += AlignOffset;

		TTextAlign Align = TAL_LEFT;


		// unselected 1
		if(s1>m_ScrollOffset)
		{
			if(Count) font->DrawText((BYTE*)m_Text+m_ScrollOffset, xxx, yyy, width-xxx, Align, height, s1 - m_ScrollOffset);
			xxx += font->GetTextWidth((BYTE*)m_Text+m_ScrollOffset, s1 - m_ScrollOffset);
			AlignOffset += font->GetTextWidth((BYTE*)m_Text+m_ScrollOffset, s1 - m_ScrollOffset);
		}

		// cursor
		if(focused && CurFirst)
		{
			if(Count) 
			{
				if(timeGetTime() - m_LastBlinkTime >= m_CursorBlinkRate)
				{
					m_LastBlinkTime = timeGetTime();
					m_CursorVisible = !m_CursorVisible;
				}
				if(m_CursorVisible)
					font->DrawText((BYTE*)m_CursorChar, xxx, yyy, width-xxx, Align, height, 1);		
			}
			xxx += CursorWidth;
			AlignOffset += CursorWidth;
		}

		// selected
		int s3 = max(s1, m_ScrollOffset);

		if(s2 - s3 > 0)
		{
			if(Count) sfont->DrawText((BYTE*)m_Text+s3, xxx, yyy, width-xxx, Align, height, s2 - s3);
			xxx += sfont->GetTextWidth((BYTE*)m_Text+s3, s2 - s3);
			AlignOffset += sfont->GetTextWidth((BYTE*)m_Text+s3, s2 - s3);
		}

		// cursor
		if(focused && !CurFirst)
		{
			if(Count)
			{
				if(timeGetTime() - m_LastBlinkTime >= m_CursorBlinkRate)
				{
					m_LastBlinkTime = timeGetTime();
					m_CursorVisible = !m_CursorVisible;
				}
				if(m_CursorVisible)
					font->DrawText((BYTE*)m_CursorChar, xxx, yyy, width-xxx, Align, height, 1);		
			}
			xxx += CursorWidth;
			AlignOffset += CursorWidth;
		}

		// unselected 2
		if(Count) font->DrawText((BYTE*)m_Text+s2, xxx, yyy, width-xxx, Align, height);
		AlignOffset += font->GetTextWidth((BYTE*)m_Text+s2);

		AlignOffset = (m_Width - 2 * m_FrameWidth) - AlignOffset;
		if(AlignOffset < 0) AlignOffset = 0;
	}


	Game->m_Renderer->m_RectList.Add(new CBActiveRect(Game, this, NULL, OffsetX + m_PosX, OffsetY + m_PosY, m_Width, m_Height, 100, 100, false));

	if(Game->m_AccessMgr->GetActiveObject()==this)
	{
		RECT rc;
		SetRect(&rc, OffsetX+m_PosX, OffsetY+m_PosY, OffsetX+m_PosX+m_Width, OffsetY+m_PosY+m_Height);
		Game->m_AccessMgr->SetHintRect(&rc, true);
	}


	Game->m_TextEncoding = OrigEncoding;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CUIEdit::HandleKeypress(bool Printable, DWORD CharCode, DWORD KeyData)
{
	bool Handled = false;

	if(CharCode==0x0A || CharCode==VK_ESCAPE || CharCode==VK_TAB || CharCode==VK_RETURN)
		return false;


	if(Printable)
	{
		bool IsControl = ((GetAsyncKeyState(VK_CONTROL) & 0x8000)!=0);
		bool IsAlt = ((GetAsyncKeyState(VK_MENU) & 0x8000)!=0);
		if(IsControl && !IsAlt)
		{
			switch(CharCode)
			{
				case 3: // CTRL+C
				case 24: // CTRL+X
					if(OpenClipboard(NULL))
					{
						int s, len;
						s = min(m_SelStart, m_SelEnd);
						len = abs(m_SelEnd-m_SelStart);

						EmptyClipboard();

						HGLOBAL Clipbuffer = GlobalAlloc(GMEM_DDESHARE, len+1);
						char* lpszBuffer = (char*)GlobalLock(Clipbuffer);
						strncpy(lpszBuffer, (const char*)m_Text+s, len);
						lpszBuffer[len] = '\0';
						GlobalUnlock(Clipbuffer);
						SetClipboardData(CF_TEXT, Clipbuffer);
						CloseClipboard();

						Handled = true;
					}
					if(CharCode==24)
					{
						DeleteChars(m_SelStart, m_SelEnd);
						if(m_SelEnd>m_SelStart) m_SelEnd -= (m_SelEnd - m_SelStart);
						m_SelStart = m_SelEnd;

						Handled = true;
					}
				break;

				case 22: // CTRL+V
					if(IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(NULL))
					{
						HGLOBAL Clipbuffer = GetClipboardData(CF_TEXT);
						if (Clipbuffer != NULL)
						{
							LPSTR lpszBuffer = (LPSTR)GlobalLock(Clipbuffer);
							if(lpszBuffer != NULL) {
								if(m_SelStart!=m_SelEnd) DeleteChars(m_SelStart, m_SelEnd);
								
								m_SelEnd += InsertChars(m_SelEnd, (BYTE*)lpszBuffer, strlen(lpszBuffer));
								if(Game->m_TextRTL)	m_SelEnd = m_SelStart;
								else m_SelStart = m_SelEnd;

								GlobalUnlock(Clipbuffer);
							}							
						}
						CloseClipboard();

						Handled = true;
					}
				break;

				case 1: // CTRL+A
					m_SelStart = 0;
					m_SelEnd = strlen(m_Text);

					Handled = true;
				break;
			}

			if(Handled) return true;
		}
		
		switch(CharCode)
		{
			case VK_BACK: // backspace				
				if(m_SelStart==m_SelEnd)
				{
					if(Game->m_TextRTL)	DeleteChars(m_SelStart, m_SelStart + 1);
					else DeleteChars(m_SelStart-1, m_SelStart);
				}
				else DeleteChars(m_SelStart, m_SelEnd);
				if(m_SelEnd>=m_SelStart) m_SelEnd -= max(1, m_SelEnd - m_SelStart);
				m_SelStart = m_SelEnd;
			break;

			default:
				if(m_SelStart!=m_SelEnd) DeleteChars(m_SelStart, m_SelEnd);
				m_SelEnd += InsertChars(m_SelEnd, (BYTE*)&CharCode, 1);

				if(Game->m_TextRTL)	m_SelEnd = m_SelStart;
				else m_SelStart = m_SelEnd;

				Game->m_AccessMgr->Speak((char*)&CharCode, TTS_KEYPRESS);

			break;
		}
	}
	else
	{
		bool IsShift = ((GetAsyncKeyState(VK_SHIFT) & 0x8000)!=0);
		switch(CharCode)
		{
			case VK_LEFT:
			case VK_UP:
				m_SelEnd--;
				if(!IsShift) m_SelStart = m_SelEnd;
			break;
			case VK_RIGHT:
			case VK_DOWN:
				m_SelEnd++;
				if(!IsShift) m_SelStart = m_SelEnd;
			break;
			case VK_HOME:
				if(Game->m_TextRTL)
				{
					m_SelEnd=strlen(m_Text);
					if(!IsShift) m_SelStart = m_SelEnd;
				}
				else
				{
					m_SelEnd=0;
					if(!IsShift) m_SelStart = m_SelEnd;
				}
			break;
			case VK_END:
				if(Game->m_TextRTL)
				{
					m_SelEnd=0;
					if(!IsShift) m_SelStart = m_SelEnd;
				}
				else
				{
					m_SelEnd=strlen(m_Text);
					if(!IsShift) m_SelStart = m_SelEnd;
				}
			break;
			case VK_DELETE:
				if(m_SelStart==m_SelEnd)
				{
					if(Game->m_TextRTL)
					{
						DeleteChars(m_SelStart-1, m_SelStart);
						m_SelEnd--;
						if(m_SelEnd < 0) m_SelEnd = 0;
					}
					else DeleteChars(m_SelStart, m_SelStart+1);
				}
				else DeleteChars(m_SelStart, m_SelEnd);
				if(m_SelEnd>m_SelStart) m_SelEnd -= (m_SelEnd - m_SelStart);

				m_SelStart = m_SelEnd;
			break;

			default:
				return false; // don't eat unused keys
		}
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////
int CUIEdit::DeleteChars(int Start, int End)
{
	if(Start>End) CBUtils::Swap(&Start, &End);
	
	Start = max(Start, 0);
	End = min(End, strlen(m_Text));

	char* str = new char[strlen(m_Text)-(End-Start)+1];
	if(str)
	{
		if(Start>0) memcpy(str, m_Text, Start);
		memcpy(str+max(0,Start), m_Text+End, strlen(m_Text)-End+1);

		SAFE_DELETE_ARRAY(m_Text);
		m_Text = str;
	}
	if(m_ParentNotify && m_Parent) m_Parent->ApplyEvent(m_Name);

	return End-Start;
}


//////////////////////////////////////////////////////////////////////////
int CUIEdit::InsertChars(int Pos, BYTE* Chars, int Num)
{
	if(strlen(m_Text)+Num > m_MaxLength)
	{
		Num -= (strlen(m_Text)+Num - m_MaxLength);
	}

	Pos = max(Pos, 0);
	Pos = min(Pos, strlen(m_Text));
	
	char* str = new char[strlen(m_Text) + Num + 1];
	if(str)
	{
		if(Pos>0) memcpy(str, m_Text, Pos);
		memcpy(str+Pos+Num, m_Text+Pos, strlen(m_Text)-Pos+1);
		
		memcpy(str+Pos, Chars, Num);

		SAFE_DELETE_ARRAY(m_Text);
		m_Text = str;
	}
	if(m_ParentNotify && m_Parent) m_Parent->ApplyEvent(m_Name);

	return Num;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CUIEdit::Persist(CBPersistMgr* PersistMgr)
{

	CUIObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_CursorBlinkRate));
	PersistMgr->Transfer(TMEMBER(m_CursorChar));
	PersistMgr->Transfer(TMEMBER(m_FontSelected));
	PersistMgr->Transfer(TMEMBER(m_FrameWidth));
	PersistMgr->Transfer(TMEMBER(m_MaxLength));
	PersistMgr->Transfer(TMEMBER(m_ScrollOffset));
	PersistMgr->Transfer(TMEMBER(m_SelEnd));
	PersistMgr->Transfer(TMEMBER(m_SelStart));
	
	if(!PersistMgr->m_Saving)
	{
		m_CursorVisible = false;
		m_LastBlinkTime = 0;
	}

	return S_OK;
}