// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFRAME_H__8888C3E0_13C0_11D4_9F37_E804E73C2277__INCLUDED_)
#define AFX_BFRAME_H__8888C3E0_13C0_11D4_9F37_E804E73C2277__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBFrame: public CBScriptable
{
public:
	bool m_KillSound;
	bool m_Keyframe;
	HRESULT OneTimeDisplay(CBObject* Owner, bool Muted=false);
	DECLARE_PERSISTENT(CBFrame, CBScriptable);
	CBSound* m_Sound;
	bool m_EditorExpanded;
	bool GetBoundingRect(LPRECT Rect, int X, int Y, float ScaleX=100, float ScaleY=100);
	HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
	int m_MoveY;
	int m_MoveX;
	DWORD m_Delay;
	CBArray<CBSubFrame*, CBSubFrame*> m_Subframes;
	HRESULT Draw(int X, int Y, CBObject* Register = NULL, float ZoomX=100, float ZoomY=100, bool Precise = true, DWORD Alpha=0xFFFFFFFF, bool AllFrames=false, float Rotate=0.0f, TSpriteBlendMode BlendMode=BLEND_NORMAL);
	HRESULT LoadBuffer(BYTE* Buffer, int LifeTime, bool KeepLoaded);

	CBFrame(CBGame* inGame);
	virtual ~CBFrame();

	CBArray<char*, char*> m_ApplyEvent;

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char* Name, CScValue* Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	virtual char* ScToString();

};

#endif // !defined(AFX_BFRAME_H__8888C3E0_13C0_11D4_9F37_E804E73C2277__INCLUDED_)
