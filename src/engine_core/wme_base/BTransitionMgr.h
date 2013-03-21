// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BTRANSITIONMGR_H__ADDB666E_2DC5_4B91_85A4_3741A057C5FE__INCLUDED_)
#define AFX_BTRANSITIONMGR_H__ADDB666E_2DC5_4B91_85A4_3741A057C5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBTransitionMgr : public CBBase
{
public:
	bool m_Started;
	DWORD m_LastTime;
	bool m_OrigInteractive;
	bool m_PreserveInteractive;
	HRESULT Update();
	HRESULT Start(TTransitionType Type, bool NonInteractive=false);
	bool IsReady();
	TTransMgrState m_State;
	CBTransitionMgr(CBGame* inGame);
	virtual ~CBTransitionMgr();
	TTransitionType m_Type;

};

#endif // !defined(AFX_BTRANSITIONMGR_H__ADDB666E_2DC5_4B91_85A4_3741A057C5FE__INCLUDED_)
