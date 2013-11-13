// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BREGISTRY_H__9C5C4B4C_0400_436D_915A_D0956CC004C6__INCLUDED_)
#define AFX_BREGISTRY_H__9C5C4B4C_0400_436D_915A_D0956CC004C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBRegistry : public CBBase  
{
public:
	void SetIniName(char* Name);
	char* GetIniName();
	bool WriteBool(char* Subkey, char* Key, bool Value);
	bool ReadBool(char* Subkey, char* Key, bool Init=false);
	bool WriteInt(char* Subkey, char* Key, int Value);
	int ReadInt(char* Subkey, char* Key, int Init=0);
	bool WriteString(char* Subkey, char* Key, char* String);
	bool ReadString(char* Subkey, char* Key, char* Buffer, DWORD* BufferSize, char* Init = NULL);
	void SetBasePath(char* BasePath);
	const char* GetBasePath();
	CBRegistry(CBGame* inGame);
	virtual ~CBRegistry();
private:
	char* m_BasePath;
	char* m_IniName;

};

#endif // !defined(AFX_BREGISTRY_H__9C5C4B4C_0400_436D_915A_D0956CC004C6__INCLUDED_)
