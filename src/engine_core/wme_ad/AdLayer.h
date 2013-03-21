// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADLAYER_H__17939D14_3171_4317_9553_987624C4CE94__INCLUDED_)
#define AFX_ADLAYER_H__17939D14_3171_4317_9553_987624C4CE94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdLayer : public CBObject  
{
public:
	bool m_CloseUp;
	DECLARE_PERSISTENT(CAdLayer, CBObject);
	bool m_Active;
	int m_Height;
	int m_Width;
	bool m_Main;
	CAdLayer(CBGame* inGame);
	virtual ~CAdLayer();
	CBArray<CAdSceneNode*, CAdSceneNode*> m_Nodes;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_ADLAYER_H__17939D14_3171_4317_9553_987624C4CE94__INCLUDED_)
