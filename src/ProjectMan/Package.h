// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__6B94E5FC_0FF9_48C7_AE7C_C8D9A4D29F61__INCLUDED_)
#define AFX_PACKAGE_H__6B94E5FC_0FF9_48C7_AE7C_C8D9A4D29F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPackage  
{
public:
	HRESULT SaveSettings(FILE* File, int ID);
	HRESULT LoadSettings(CString Filename, int ID);
	CString m_Description;
	bool m_Primary;
	int m_CD;
	int m_Priority;
	CString m_Folder;
	CPackage();
	virtual ~CPackage();

};

#endif // !defined(AFX_PACKAGE_H__6B94E5FC_0FF9_48C7_AE7C_C8D9A4D29F61__INCLUDED_)
