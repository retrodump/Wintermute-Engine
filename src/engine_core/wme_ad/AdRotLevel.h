// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADROTLEVEL_H__6909606C_34F4_4740_A448_DA40C32A4E58__INCLUDED_)
#define AFX_ADROTLEVEL_H__6909606C_34F4_4740_A448_DA40C32A4E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdRotLevel : public CBObject  
{
public:
	DECLARE_PERSISTENT(CAdRotLevel, CBObject);
	CAdRotLevel(CBGame* inGame);
	virtual ~CAdRotLevel();
	float m_Rotation;
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
};

#endif // !defined(AFX_ADROTLEVEL_H__6909606C_34F4_4740_A448_DA40C32A4E58__INCLUDED_)
