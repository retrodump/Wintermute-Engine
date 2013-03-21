// PropPair.h: interface for the CPropPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPPAIR_H__B55AF599_243B_44EF_BEF0_43B2B2D779B2__INCLUDED_)
#define AFX_PROPPAIR_H__B55AF599_243B_44EF_BEF0_43B2B2D779B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPropPair  
{
public:
	CString m_Value;
	CString m_Name;
	CPropPair();
	virtual ~CPropPair();

};

#endif // !defined(AFX_PROPPAIR_H__B55AF599_243B_44EF_BEF0_43B2B2D779B2__INCLUDED_)
