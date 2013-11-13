// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BEVENTSET_H__61C7CC1F_1043_4ABB_BB45_B871E1C79FB0__INCLUDED_)
#define AFX_BEVENTSET_H__61C7CC1F_1043_4ABB_BB45_B871E1C79FB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CBEventSet : public CBObject  
{
public:
	DECLARE_PERSISTENT(CBEventSet, CBObject);
	HRESULT ApplyEvent(TEventType Event, CBObject* Owner);
	CBEventSet(CBGame* inGame);
	virtual ~CBEventSet();
	HRESULT LoadFile(char * Filename);
	HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);
	CBArray<CBEvent*, CBEvent*> m_Events;
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);

};

#endif // !defined(AFX_BEVENTSET_H__61C7CC1F_1043_4ABB_BB45_B871E1C79FB0__INCLUDED_)
