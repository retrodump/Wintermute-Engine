// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADREGION_H__2802474C_5CE3_4B22_8794_402559E5D05B__INCLUDED_)
#define AFX_ADREGION_H__2802474C_5CE3_4B22_8794_402559E5D05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdRegion : public CBRegion
{
public:
	DECLARE_PERSISTENT(CAdRegion, CBRegion);
	DWORD m_Alpha;
	float m_Zoom;
	bool m_Blocked;
	bool m_Decoration;
	CAdRegion(CBGame* inGame);
	virtual ~CAdRegion();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_ADREGION_H__2802474C_5CE3_4B22_8794_402559E5D05B__INCLUDED_)
