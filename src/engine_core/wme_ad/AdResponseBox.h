// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADRESPONSEBOX_H__B6CA6F0D_1D30_459F_8063_5C1105471AAC__INCLUDED_)
#define AFX_ADRESPONSEBOX_H__B6CA6F0D_1D30_459F_8063_5C1105471AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CAdResponseBox : public CBObject  
{
public:
	CBObject* GetNextAccessObject(CBObject* CurrObject);
	CBObject* GetPrevAccessObject(CBObject* CurrObject);
	HRESULT GetObjects(CBArray<CUIObject*, CUIObject*>& Objects, bool InteractiveOnly);

	HRESULT HandleResponse(CAdResponse* Response);
	void SetLastResponseText(char* Text, char* TextOrig);
	char* m_LastResponseText;
	char* m_LastResponseTextOrig;
	DECLARE_PERSISTENT(CAdResponseBox, CBObject);
	CScScript* m_WaitingScript;
	virtual HRESULT Listen(CBScriptHolder* param1, DWORD param2);
	typedef enum{
		EVENT_PREV, EVENT_NEXT, EVENT_RESPONSE
	} TResponseEvent;

	HRESULT WeedResponses();
	HRESULT Display();
	int m_Spacing;
	int m_ScrollOffset;
	CBFont* m_FontHover;
	CBFont* m_Font;
	HRESULT CreateButtons();
	HRESULT InvalidateButtons();
	void ClearButtons();
	void ClearResponses();
	CAdResponseBox(CBGame* inGame);
	virtual ~CAdResponseBox();
	CBArray<CAdResponse*, CAdResponse*> m_Responses;
	CBArray<CUIButton*, CUIButton*> m_RespButtons;
	CUIWindow* m_Window;
	CUIWindow* m_ShieldWindow;
	bool m_Horizontal;
	RECT m_ResponseArea;
	int m_VerticalAlign;
	TTextAlign m_Align;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);
};

#endif // !defined(AFX_ADRESPONSEBOX_H__B6CA6F0D_1D30_459F_8063_5C1105471AAC__INCLUDED_)
