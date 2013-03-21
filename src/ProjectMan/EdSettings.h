// EdSettings.h: interface for the CEdSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDSETTINGS_H__93BF3690_817D_4E4B_9404_3AE55C6BE9D1__INCLUDED_)
#define AFX_EDSETTINGS_H__93BF3690_817D_4E4B_9404_3AE55C6BE9D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../engine_core/wme_ad/dcgf_ad.h"
#include "EdGame.h"

class CEdSettings : public CBObject  
{
public:
	bool m_AllowWindowed;
	bool m_AllowDesktopRes;
	bool m_AllowAdvanced;
	bool m_AllowAccessTab;
	bool m_AllowAboutTab;
	bool m_RequireAcceleration;
	bool m_RequireSound;
	int m_ResolutionHeight;
	int m_ResolutionWidth;
	int m_TLMode;
	bool m_RichSavedGames;
	char* m_SavedGameExt;
	GUID m_GameGUID;
	CEdGame* m_EdGame;
	CEdSettings(CBGame* inGame);
	virtual ~CEdSettings();
	char* m_StringTableFilename;
	char* m_RegPath;

	virtual HRESULT LoadFile(char * Filename);
	virtual HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
};

#endif // !defined(AFX_EDSETTINGS_H__93BF3690_817D_4E4B_9404_3AE55C6BE9D1__INCLUDED_)
