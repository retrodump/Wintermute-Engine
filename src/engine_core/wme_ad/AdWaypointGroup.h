// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADWAYPOINTGROUP_H__66C7E17D_BEFB_4DB1_BBBF_14286A90646E__INCLUDED_)
#define AFX_ADWAYPOINTGROUP_H__66C7E17D_BEFB_4DB1_BBBF_14286A90646E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdWaypointGroup : public CBObject
{
public:
	float m_LastMimicScale;
	int m_LastMimicX;
	int m_LastMimicY;
	void Cleanup();
	HRESULT Mimic(CAdWaypointGroup* Wpt, float Scale=100.0f, int X=0, int Y=0);
	DECLARE_PERSISTENT(CAdWaypointGroup, CBObject);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
	bool m_Active;
	CAdWaypointGroup(CBGame* inGame);
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual ~CAdWaypointGroup();
	CBArray<CBPoint*, CBPoint*> m_Points;
	int m_EditorSelectedPoint;
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
};

#endif // !defined(AFX_ADWAYPOINTGROUP_H__66C7E17D_BEFB_4DB1_BBBF_14286A90646E__INCLUDED_)
