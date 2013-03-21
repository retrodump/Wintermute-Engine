// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADTALKDEF_H__E407D730_CAA9_4324_900A_EE89322BDBBA__INCLUDED_)
#define AFX_ADTALKDEF_H__E407D730_CAA9_4324_900A_EE89322BDBBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdTalkDef : public CBObject  
{
public:
	char* m_DefaultSpriteSetFilename;
	CAdSpriteSet* m_DefaultSpriteSet;
	CBSprite* GetDefaultSprite(TDirection Dir);
	HRESULT LoadDefaultSprite();
	DECLARE_PERSISTENT(CAdTalkDef, CBObject);

	CAdTalkDef(CBGame* inGame);
	virtual ~CAdTalkDef();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	CBArray<CAdTalkNode*, CAdTalkNode*> m_Nodes;
	char* m_DefaultSpriteFilename;
	CBSprite* m_DefaultSprite;
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent=0);
};

#endif // !defined(AFX_ADTALKDEF_H__E407D730_CAA9_4324_900A_EE89322BDBBA__INCLUDED_)
