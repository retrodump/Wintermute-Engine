// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SXOBJECT_H__5B153D0E_B593_4694_8B68_DD13C711B05F__INCLUDED_)
#define AFX_SXOBJECT_H__5B153D0E_B593_4694_8B68_DD13C711B05F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CSXObject : public CBObject  
{
public:
	DECLARE_PERSISTENT(CSXObject, CBObject);
	CSXObject(CBGame* inGame, CScStack* Stack);
	virtual ~CSXObject();

};

#endif // !defined(AFX_SXOBJECT_H__5B153D0E_B593_4694_8B68_DD13C711B05F__INCLUDED_)
