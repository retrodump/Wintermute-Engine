// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UIWINDOW_H__51641E47_2A87_4144_866A_4C3F384A416C__INCLUDED_)
#define AFX_UIWINDOW_H__51641E47_2A87_4144_866A_4C3F384A416C__INCLUDED_

#include "dctypes.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIObject.h"

class CUIWindow : public CUIObject  
{
public:
	CBObject* GetPrevAccessObject(CBObject* CurrObject);
	CBObject* GetNextAccessObject(CBObject* CurrObject);
	HRESULT GetWindowObjects(CBArray<CUIObject*, CUIObject*>& Objects, bool InteractiveOnly);

	bool m_PauseMusic;
	void Cleanup();
	virtual void MakeFreezable(bool Freezable);
	CBViewport* m_Viewport;
	bool m_ClipContents;
	bool m_InGame;
	bool m_IsMenu;
	bool m_FadeBackground;
	DWORD m_FadeColor;
	virtual bool HandleMouseWheel(int Delta);
	CUIWindow* m_ShieldWindow;
	CUIButton* m_ShieldButton;
	HRESULT Close();
	HRESULT GoSystemExclusive();
	HRESULT GoExclusive();
	TWindowMode m_Mode;
	HRESULT MoveFocus(bool Forward=true);
	virtual HRESULT HandleMouse(TMouseEvent Event, TMouseButton Button);
	POINT m_DragFrom;
	bool m_Dragging;
	DECLARE_PERSISTENT(CUIWindow, CUIObject);
	bool m_Transparent;
	HRESULT ShowWidget(char* Name, bool Visible=true);
	HRESULT EnableWidget(char* Name, bool Enable=true);
	RECT m_TitleRect;
	RECT m_DragRect;
	virtual HRESULT Display(int OffsetX=0, int OffsetY=0);
	CUIWindow(CBGame* inGame);
	virtual ~CUIWindow();
	virtual bool HandleKeypress(bool Printable, DWORD CharCode, DWORD KeyData);
	CBArray<CUIObject*, CUIObject*> m_Widgets;
	TTextAlign m_TitleAlign;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	CUITiledImage* m_BackInactive;
	CBFont* m_FontInactive;
	CBSprite* m_ImageInactive;
	virtual HRESULT Listen(CBScriptHolder* param1, DWORD param2);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_UIWINDOW_H__51641E47_2A87_4144_866A_4C3F384A416C__INCLUDED_)
