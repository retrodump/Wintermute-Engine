// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSPRITESET_H__B6DFCD1F_F309_4AEF_A6B6_BCAABB9E0951__INCLUDED_)
#define AFX_ADSPRITESET_H__B6DFCD1F_F309_4AEF_A6B6_BCAABB9E0951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"
#include "BSprite.h"	// Added by ClassView

class CAdSpriteSet : public CBObject  
{
public:
	bool ContainsSprite(CBSprite* Sprite);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent=0);
	CBSprite* GetSprite(TDirection Direction);
	DECLARE_PERSISTENT(CAdSpriteSet, CBObject);
	CBObject* m_Owner;	
	CAdSpriteSet(CBGame* inGame, CBObject* Owner=NULL);
	virtual ~CAdSpriteSet();
	HRESULT LoadFile(char* Filename, int LifeTime=-1, TSpriteCacheType CacheType=CACHE_ALL);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true, int LifeTime=-1, TSpriteCacheType CacheType=CACHE_ALL);
	CBSprite* m_Sprites[NUM_DIRECTIONS];
};

#endif // !defined(AFX_ADSPRITESET_H__B6DFCD1F_F309_4AEF_A6B6_BCAABB9E0951__INCLUDED_)
