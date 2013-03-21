// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BOBJECT_H__2B6A5040_3ED9_11D4_B319_BCE8B5119713__INCLUDED_)
#define AFX_BOBJECT_H__2B6A5040_3ED9_11D4_B319_BCE8B5119713__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "wme_plugin.h"

class CBSprite;
class CBSound;
class CBEventSet;
class C3DModel;
class CBSurface;
class CXModel;

class CBObject : public CBScriptHolder
{
public:
	char* m_AccessCaption;
	virtual char* GetAccessCaption();

	TSpriteBlendMode m_BlendMode;
	virtual HRESULT RenderModel();
	virtual HRESULT AfterMove();
	float m_RelativeRotate;
	bool m_RotateValid;
	float m_Rotate;
	void SetSoundEvent(char* EventName);
	bool m_Rotatable;
	DWORD m_AlphaColor;
	float m_Scale;
	float m_ScaleX;
	float m_ScaleY;
	float m_RelativeScale;
	virtual bool IsReady();
	virtual bool GetExtendedFlag(char* FlagName);
	virtual HRESULT ResetSoundPan();
	virtual HRESULT UpdateSounds();
	HRESULT UpdateOneSound(CBSound* Sound);
	bool m_AutoSoundPanning;
	DWORD m_SFXStart;
	int m_SFXVolume;
	HRESULT SetSFXTime(DWORD Time);
	HRESULT SetSFXVolume(int Volume);
	HRESULT ResumeSFX();
	HRESULT PauseSFX();
	HRESULT StopSFX(bool DeleteSound=true);
	HRESULT PlaySFX(char* Filename, bool Looping=false, bool PlayNow=true, char* EventName=NULL, DWORD LoopStart=0);
	CBSound* m_SFX;

	TSFXType m_SFXType;
	float m_SFXParam1;
	float m_SFXParam2;
	float m_SFXParam3;
	float m_SFXParam4;

	virtual bool HandleMouseWheel(int Delta);
	virtual HRESULT HandleMouse(TMouseEvent Event, TMouseButton Button);
	virtual bool HandleKeypress(bool Printable, DWORD CharCode, DWORD KeyData);
	virtual int GetHeight();
	HRESULT SetCursor(char* Filename);
	HRESULT SetActiveCursor(char* Filename);
	HRESULT Cleanup();
	char* GetCaption(int Case=1);
	void SetCaption(char* Caption, int Case=1);
	bool m_EditorSelected;
	bool m_EditorAlwaysRegister;
	bool m_EditorOnly;
	bool m_Is3D;
	DECLARE_PERSISTENT(CBObject, CBScriptHolder);
	virtual HRESULT ShowCursor();
	CBSprite* m_Cursor;
	bool m_SharedCursors;
	CBSprite* m_ActiveCursor;
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);
	virtual HRESULT Listen(CBScriptHolder* param1, DWORD param2);
	bool m_Ready;
	bool m_Registrable;
	bool m_Zoomable;
	bool m_Shadowable;
	RECT m_Rect;
	bool m_RectSet;
	int m_ID;
	bool m_Movable;
	CBObject(CBGame* inGame);
	virtual ~CBObject();
	char* m_Caption[7];
	char* m_SoundEvent;
	int m_PosY;
	int m_PosX;
	bool m_SaveState;

	// base
	virtual HRESULT Update()  { return E_FAIL; };
	virtual HRESULT Display() { return E_FAIL; };
	virtual HRESULT InvalidateDeviceObjects()  { return S_OK; };
	virtual HRESULT RestoreDeviceObjects()     { return S_OK; };
	bool m_NonIntMouseEvents;

	// 3d stuff
	float m_Angle;
	C3DModel* m_Model;
	CXModel* m_ModelX;
	CXModel* m_ShadowModel;
	D3DXMATRIX m_WorldMatrix;
	D3DXVECTOR3 m_PosVector;
	HRESULT GetMatrix(D3DXMATRIX* ModelMatrix, D3DXVECTOR3* PosVect=NULL);	
	DWORD m_ShadowColor;
	CBSurface* m_ShadowImage;
	float m_ShadowSize;
	float m_Scale3D;
	D3DXVECTOR3 m_ShadowLightPos;
	bool m_DrawBackfaces;
	TShadowType m_ShadowType;
	virtual DWORD GetAnimTransitionTime(char* From, char* To) { return 0; };

public:
	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_BOBJECT_H__2B6A5040_3ED9_11D4_B319_BCE8B5119713__INCLUDED_)
