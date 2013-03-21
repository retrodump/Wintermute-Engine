// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADTALKNODE_H__89A6BF25_8EF9_4930_86B3_C4A835AFC7F9__INCLUDED_)
#define AFX_ADTALKNODE_H__89A6BF25_8EF9_4930_86B3_C4A835AFC7F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdTalkNode : public CBBase  
{
public:
	char* m_SpriteSetFilename;
	CAdSpriteSet* m_SpriteSet;
	CBSprite* GetSprite(TDirection Dir);
	bool IsInTimeInterval(DWORD Time, TDirection Dir);
	HRESULT LoadSprite();
	DECLARE_PERSISTENT(CAdTalkNode, CBBase);

	CAdTalkNode(CBGame* inGame);
	virtual ~CAdTalkNode();
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent=0);
	char* m_SpriteFilename;
	CBSprite* m_Sprite;
	DWORD m_StartTime;
	DWORD m_EndTime;
	bool m_PlayToEnd;
	bool m_PreCache;
	char* m_Comment;

};

#endif // !defined(AFX_ADTALKNODE_H__89A6BF25_8EF9_4930_86B3_C4A835AFC7F9__INCLUDED_)
