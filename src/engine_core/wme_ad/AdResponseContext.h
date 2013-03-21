// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADRESPONSECONTEXT_H__ED6DC9FB_7B64_46EB_8010_2A13D03949B3__INCLUDED_)
#define AFX_ADRESPONSECONTEXT_H__ED6DC9FB_7B64_46EB_8010_2A13D03949B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdResponseContext : public CBBase  
{
public:
	void SetContext(char* Context);
	int m_ID;
	char* m_Context;
	DECLARE_PERSISTENT(CAdResponseContext, CBBase);
	CAdResponseContext(CBGame* inGame);
	virtual ~CAdResponseContext();

};

#endif // !defined(AFX_ADRESPONSECONTEXT_H__ED6DC9FB_7B64_46EB_8010_2A13D03949B3__INCLUDED_)
