// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once


class CAdActorDir : public CBBase
{
public:
	DECLARE_PERSISTENT(CAdActorDir, CBBase);
	CAdActorDir(CBGame* inGame);
	virtual ~CAdActorDir(void);
};
