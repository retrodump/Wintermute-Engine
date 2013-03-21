// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADBLOCK_H__3F6EB3E6_5A62_41FC_B814_EC97EED78F5B__INCLUDED_)
#define AFX_ADBLOCK_H__3F6EB3E6_5A62_41FC_B814_EC97EED78F5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CAdBlock : public CBScriptable
{
public:
	bool m_ReceiveShadows;
	HRESULT Persist(CBPersistMgr* PersistMgr);
	bool m_Active;
	CAdBlock(CBGame* inGame);
	virtual ~CAdBlock();
	C3DMesh* m_Mesh;
};

#endif // !defined(AFX_ADBLOCK_H__3F6EB3E6_5A62_41FC_B814_EC97EED78F5B__INCLUDED_)
