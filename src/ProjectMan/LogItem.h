// LogItem.h: interface for the CLogItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGITEM_H__6D9B9A7E_EC05_468D_9DDD_A8A319F4D04B__INCLUDED_)
#define AFX_LOGITEM_H__6D9B9A7E_EC05_468D_9DDD_A8A319F4D04B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogItem  
{
public:
	typedef enum{
		LOG_INFO, LOG_ERROR, LOG_WARNING, LOG_ENGINE
	} TLogItemType;
	
	CString m_Filename;
	CString m_Text;
	CLogItem();
	CLogItem(TLogItemType Type, CString Text, CString Filename="");
	TLogItemType m_Type;
	virtual ~CLogItem();

};

#endif // !defined(AFX_LOGITEM_H__6D9B9A7E_EC05_468D_9DDD_A8A319F4D04B__INCLUDED_)
