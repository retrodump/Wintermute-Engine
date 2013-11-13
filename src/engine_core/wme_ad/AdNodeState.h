// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADNODESTATE_H__5D8C533B_D277_498E_AEE6_841864E32B1B__INCLUDED_)
#define AFX_ADNODESTATE_H__5D8C533B_D277_498E_AEE6_841864E32B1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdEntity;

class CAdNodeState : public CBBase  
{
public:
	HRESULT TransferEntity(CAdEntity* Entity, bool IncludingSprites, bool Saving);
	void SetName(char* Name);
	void SetFilename(char* Filename);
	void SetCursor(char* Filename);
	DECLARE_PERSISTENT(CAdNodeState, CBBase);
	CAdNodeState(CBGame* inGame);
	virtual ~CAdNodeState();
	char* m_Name;
	bool m_Active;
	char* m_Caption[7];
	void SetCaption(char *Caption, int Case);
	char* GetCaption(int Case);
	DWORD m_AlphaColor;
	char* m_Filename;
	char* m_Cursor;
	
};

#endif // !defined(AFX_ADNODESTATE_H__5D8C533B_D277_498E_AEE6_841864E32B1B__INCLUDED_)
