// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "HWDevice.h"


//////////////////////////////////////////////////////////////////////////
CHWDevice::CHWDevice(CBGame* inGame):CBBase(inGame)
{
	m_Descript = NULL;
	m_Driver = NULL;
	memset(&m_Guid, 0, sizeof(GUID));
	m_SupportsWindowed = false;
	m_Supports16bit = false;
	m_Supports32bit = false;
	m_Accelerated = false;
	m_AdapterID = -1;
}


//////////////////////////////////////////////////////////////////////////
CHWDevice::~CHWDevice()
{
	if(m_Descript) delete [] m_Descript;
	if(m_Driver) delete [] m_Driver;
}


//////////////////////////////////////////////////////////////////////////
void CHWDevice::SetDescript(char *Descript)
{
	if(m_Descript) delete [] m_Descript;
	m_Descript = new char[strlen(Descript)+1];
	if(m_Descript) strcpy(m_Descript, Descript);
}

//////////////////////////////////////////////////////////////////////////
void CHWDevice::SetDriver(char* Name, LARGE_INTEGER Version)
{
	if(!Name) return;

	WORD Product = HIWORD(Version.HighPart);
	WORD Ver     = LOWORD(Version.HighPart);
	WORD SubVer  = HIWORD(Version.LowPart);
	WORD Build   = LOWORD(Version.LowPart);


	if(m_Driver) delete [] m_Descript;
	m_Driver = new char[strlen(Name) + 100];
	sprintf(m_Driver, "%s %d.%d.%d.%d", Name, Product, Ver, SubVer, Build);
}