// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_HWDEVICEAUDIO_H__6E025DDC_8D50_416C_AA6A_44BA0B8DF2F8__INCLUDED_)
#define AFX_HWDEVICEAUDIO_H__6E025DDC_8D50_416C_AA6A_44BA0B8DF2F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HWDevice.h"

class CHWDeviceAudio : public CHWDevice  
{
public:
	bool m_NoSound;
	CHWDeviceAudio(CBGame* inGame);
	virtual ~CHWDeviceAudio();

};

#endif // !defined(AFX_HWDEVICEAUDIO_H__6E025DDC_8D50_416C_AA6A_44BA0B8DF2F8__INCLUDED_)
