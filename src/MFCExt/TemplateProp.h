// TemplateProp.h: interface for the CTemplateProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPLATEPROP_H__30B22A9E_D6E2_4931_A620_8971A309CA43__INCLUDED_)
#define AFX_TEMPLATEPROP_H__30B22A9E_D6E2_4931_A620_8971A309CA43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTemplateProp  
{
public:
	CString m_Type;
	int m_MaxValue;
	int m_MinValue;
	bool m_Validate;
	CString m_Value;
	CStringArray m_Choices;
	CString m_Caption;
	CString m_Name;
	CTemplateProp();
	virtual ~CTemplateProp();

};

#endif // !defined(AFX_TEMPLATEPROP_H__30B22A9E_D6E2_4931_A620_8971A309CA43__INCLUDED_)
