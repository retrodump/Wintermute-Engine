// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSUBFRAME_H__2D46BB36_1D2D_47DC_B4F9_299D06BD8E92__INCLUDED_)
#define AFX_BSUBFRAME_H__2D46BB36_1D2D_47DC_B4F9_299D06BD8E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBSubFrame : public CBScriptable, public IWmeSubFrame
{
public:
	bool m_MirrorX;
	bool m_MirrorY;
	bool m_Decoration;
	HRESULT SetSurface (char* Filename, bool default_ck=true, BYTE ck_red=0, BYTE ck_green=0, BYTE ck_blue=0, int LifeTime=-1, bool KeepLoaded=false);
	HRESULT SetSurfaceSimple();
	DECLARE_PERSISTENT(CBSubFrame, CBScriptable);
	void SetDefaultRect();
	DWORD m_Transparent;
	HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent, bool Complete=true);
	bool m_EditorSelected;
	CBSubFrame(CBGame* inGame);
	virtual ~CBSubFrame();
	HRESULT LoadBuffer(BYTE* Buffer, int LifeTime, bool KeepLoaded);
	HRESULT Draw(int X, int Y, CBObject* Register = NULL, float ZoomX=100, float ZoomY=100, bool Precise = true, DWORD Alpha=0xFFFFFFFF, float Rotate=0.0f, TSpriteBlendMode BlendMode=BLEND_NORMAL);
	bool GetBoundingRect(LPRECT Rect, int X, int Y, float ScaleX=100, float ScaleY=100);

	int m_HotspotX;
	int m_HotspotY;
	DWORD m_Alpha;
	RECT m_Rect;	

	bool m_CKDefault;
	BYTE m_CKRed;
	BYTE m_CKGreen;
	BYTE m_CKBlue;
	int m_LifeTime;
	bool m_KeepLoaded;
	char* m_SurfaceFilename;

	bool m_2DOnly;
	bool m_3DOnly;

	CBSurface* m_Surface;

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char* Name, CScValue* Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	virtual char* ScToString();

// IWmeObject
public:
	bool SendEvent(const char* EventName);
	IWmeValue* CallMethod(const char* MethodName, IWmeParamSet* Params=0);
	bool SetProperty(const char* PropName, IWmeValue* Value);
	bool SetProperty(const char* PropName, int Value);
	bool SetProperty(const char* PropName, const char* Value);
	bool SetProperty(const char* PropName, double Value);
	bool SetProperty(const char* PropName, bool Value);
	bool SetProperty(const char* PropName, IWmeObject* Value);
	bool SetProperty(const char* PropName);
	IWmeValue* GetProperty(const char* PropName);
	void* GetInterface(const char* ClassName);

// IWmeSubFrame
public:
	bool StartPixelOperations();
	bool EndPixelOperations();
	unsigned long GetPixel(int X, int Y);
	bool PutPixel(int X, int Y, unsigned long Pixel);
	int GetWidth();
	int GetHeight();

};

#endif // !defined(AFX_BSUBFRAME_H__2D46BB36_1D2D_47DC_B4F9_299D06BD8E92__INCLUDED_)
