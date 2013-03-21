// EdGame.h: interface for the CEdGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGAME_H__DDBCA7A5_A233_40C2_988F_996A2B093E9E__INCLUDED_)
#define AFX_EDGAME_H__DDBCA7A5_A233_40C2_988F_996A2B093E9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../engine_core/wme_ad/dcgf_ad.h"

class CEdGame : public CBObject  
{
public:
	int m_IndicatorX;
	int m_IndicatorY;
	int m_IndicatorWidth;
	int m_IndicatorHeight;
	DWORD m_IndicatorColor;

	char* m_LoadImageName;
	char* m_SaveImageName;
	int m_SaveImageX;
	int m_SaveImageY;
	int m_LoadImageX;
	int m_LoadImageY;
	char* m_LocalSaveDir;

	int m_ThumbnailWidth;
	int m_ThumbnailHeight;
	RECT m_SceneViewport;
	CEdGame(CBGame* inGame);
	virtual ~CEdGame();

	virtual HRESULT LoadFile(char * Filename);
	virtual HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);
	HRESULT LoadBufferGame(BYTE * Buffer, bool Complete=true);
	HRESULT LoadBufferAdGame(BYTE * Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);

	char* m_ResponseFile;
	char* m_InventoryFile;
	char* m_ItemsFile;

	char* m_DebugStartupScene;
	char* m_StartupScene;

	bool m_PersonalizedSave;
	bool m_Subtitles;
	bool m_VideoSubtitles;
	int m_SubtitlesSpeed;
	char* m_SystemFont;
	char* m_VideoFont;
	char* m_Cursor;
	char* m_ActiveCursor;
	char* m_NoninteractiveCursor;
	TTalkSkipButton m_TalkSkipButton;

	// compatibility bits
	bool m_CompatKillMethodThreads;
};

#endif // !defined(AFX_EDGAME_H__DDBCA7A5_A233_40C2_988F_996A2B093E9E__INCLUDED_)
