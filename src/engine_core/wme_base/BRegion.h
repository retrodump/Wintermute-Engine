// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BREGION_H__2802474C_5CE3_4B22_8794_402559E5D05B__INCLUDED_)
#define AFX_BREGION_H__2802474C_5CE3_4B22_8794_402559E5D05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBRegion : public CBObject
{
public:
	float m_LastMimicScale;
	int m_LastMimicX;
	int m_LastMimicY;
	void Cleanup();
	HRESULT Mimic(CBRegion* Region, float Scale=100.0f, int X=0, int Y=0);
	HRESULT GetBoundingRect(RECT* Rect);
	bool PtInPolygon(int X, int Y);
	DECLARE_PERSISTENT(CBRegion, CBObject);
	bool m_Active;
	int m_EditorSelectedPoint;
	CBRegion(CBGame* inGame);
	virtual ~CBRegion();
	bool PointInRegion(int X, int Y);
	bool CreateRegion();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	RECT m_Rect;
	//HRGN m_Region;
	CBArray<CBPoint*, CBPoint*> m_Points;
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent, char* NameOverride=NULL);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_BREGION_H__2802474C_5CE3_4B22_8794_402559E5D05B__INCLUDED_)
