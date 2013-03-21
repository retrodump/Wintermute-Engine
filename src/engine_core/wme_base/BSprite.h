// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSprite_H__DAA97525_137F_11D4_9F37_8F90855A4202__INCLUDED_)
#define AFX_BSprite_H__DAA97525_137F_11D4_9F37_8F90855A4202__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "coll_templ.h"

class CBSprite: public CBScriptHolder
{
public:
	HRESULT KillAllSounds();
	CBSurface* GetSurface();
	char* m_EditorBgFile;
	int m_EditorBgOffsetX;
	int m_EditorBgOffsetY;
	int m_EditorBgAlpha;
	bool m_Streamed;
	bool m_StreamedKeepLoaded;
	void Cleanup();
	void SetDefaults();
	bool m_Precise;
	DECLARE_PERSISTENT(CBSprite, CBScriptHolder);

	bool m_EditorAllFrames;
	bool GetBoundingRect(LPRECT Rect, int X, int Y, float ScaleX=100, float ScaleY=100);
	int m_MoveY;
	int m_MoveX;
	HRESULT Display(int X, int Y, CBObject* Register=NULL, float ZoomX=100, float ZoomY=100, DWORD Alpha=0xFFFFFFFF, float Rotate=0.0f, TSpriteBlendMode BlendMode=BLEND_NORMAL);
	bool GetCurrentFrame(float ZoomX=100, float ZoomY=100);
	bool m_CanBreak;
	bool m_EditorMuted;
	bool m_Continuous;
	void Reset();
	CBObject* m_Owner;
	bool m_Changed;
	bool m_Paused;
	bool m_Finished;
	HRESULT LoadBuffer(BYTE* Buffer, bool Compete = true, int LifeTime=-1, TSpriteCacheType CacheType=CACHE_ALL);
	HRESULT LoadFile(char* Filename, int LifeTime=-1, TSpriteCacheType CacheType=CACHE_ALL);
	DWORD m_LastFrameTime;
	HRESULT Draw(int X, int Y, CBObject* Register = NULL, float ZoomX=100, float ZoomY=100, DWORD Alpha=0xFFFFFFFF);
	bool m_Looping;
	int m_CurrentFrame;
	HRESULT AddFrame(char* Filename, DWORD Delay=0, int HotspotX=0, int HotspotY=0, RECT* Rect=NULL);
	CBSprite(CBGame* inGame, CBObject* Owner=NULL);
	virtual ~CBSprite();
	CBArray<CBFrame*, CBFrame*> m_Frames;
	HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char* Name, CScValue* Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_BSprite_H__DAA97525_137F_11D4_9F37_8F90855A4202__INCLUDED_)
