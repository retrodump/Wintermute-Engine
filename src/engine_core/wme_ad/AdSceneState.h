// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSCENESTATE_H__1DCD8D9A_A950_4ED5_83EA_99F807B74BD2__INCLUDED_)
#define AFX_ADSCENESTATE_H__1DCD8D9A_A950_4ED5_83EA_99F807B74BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdSceneState : public CBBase  
{
public:
	CAdNodeState* GetNodeState(char* Name, bool Saving);
	void SetFilename(char* Filename);
	DECLARE_PERSISTENT(CAdSceneState, CBBase);
	CAdSceneState(CBGame* inGame);
	virtual ~CAdSceneState();
	char* m_Filename;
	CBArray<CAdNodeState*, CAdNodeState*> m_NodeStates;
};

#endif // !defined(AFX_ADSCENESTATE_H__1DCD8D9A_A950_4ED5_83EA_99F807B74BD2__INCLUDED_)
