// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UIOBJECT_H__4A6C78D2_2576_4F50_8960_63CCABBD45E6__INCLUDED_)
#define AFX_UIOBJECT_H__4A6C78D2_2576_4F50_8960_63CCABBD45E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"
#include "dctypes.h"	// Added by ClassView

class CUITiledImage;

class CUIObject : public CBObject
{
public:
	virtual char* GetAccessCaption();

	HRESULT GetTotalOffset(int* OffsetX, int* OffsetY);
	bool m_CanFocus;
	HRESULT Focus();
	virtual HRESULT HandleMouse(TMouseEvent Event, TMouseButton Button);	
	bool IsFocused();
	bool m_ParentNotify;
	DECLARE_PERSISTENT(CUIObject, CBObject);
	CUIObject* m_Parent;
	virtual HRESULT Display(int OffsetX=0, int OffsetY=0);
	virtual void CorrectSize();
	bool m_SharedFonts;
	bool m_SharedImages;
	void SetText(char* Text);
	char* m_Text;
	CBFont* m_Font;
	bool m_Visible;
	CUITiledImage* m_Back;
	bool m_Disable;
	CUIObject(CBGame* inGame=NULL);
	virtual ~CUIObject();
	int m_Width;
	int m_Height;
	TUIObjectType m_Type;
	CBSprite* m_Image;
	void SetListener(CBScriptHolder* Object, CBScriptHolder* ListenerObject, DWORD ListenerParam);
	CBScriptHolder* m_ListenerParamObject;
	DWORD m_ListenerParamDWORD;
	CBScriptHolder* m_ListenerObject;
	CUIObject* m_FocusedWidget;
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_UIOBJECT_H__4A6C78D2_2576_4F50_8960_63CCABBD45E6__INCLUDED_)
