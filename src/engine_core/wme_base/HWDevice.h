// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_HWDEVICE_H__CF9E7958_D8DA_4A79_B621_0774AE84985B__INCLUDED_)
#define AFX_HWDEVICE_H__CF9E7958_D8DA_4A79_B621_0774AE84985B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CHWDevice : public CBBase  
{
public:
	int m_AdapterID;
	int m_Height;
	int m_Width;
	void SetDescript(char* Descript);
	void SetDriver(char* Name, LARGE_INTEGER Version);
	bool m_Accelerated;
	GUID m_Guid;
	bool m_Supports32bit;
	bool m_Supports16bit;
	bool m_SupportsWindowed;
	char* m_Descript;
	char* m_Driver;
	CHWDevice(CBGame* inGame);
	virtual ~CHWDevice();

};

#endif // !defined(AFX_HWDEVICE_H__CF9E7958_D8DA_4A79_B621_0774AE84985B__INCLUDED_)
