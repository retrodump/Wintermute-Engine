// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CBSaveThumbHelper : public CBBase
{
public:
	CBSaveThumbHelper(CBGame* inGame);
	virtual ~CBSaveThumbHelper(void);
	HRESULT StoreThumbnail(bool DoFlip=false);

	CBImage* m_Thumbnail;
	CBImage* m_RichThumbnail;
};
