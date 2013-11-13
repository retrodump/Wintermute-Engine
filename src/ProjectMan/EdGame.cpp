// EdGame.cpp: implementation of the CEdGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "EdGame.h"

#include "../engine_core/wme_base/BParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CEdGame::CEdGame(CBGame* inGame):CBObject(inGame)
{
	m_ResponseFile = NULL;
	m_InventoryFile = NULL;
	m_ItemsFile = NULL;

	m_PersonalizedSave = false;
	m_Subtitles = true;
	m_VideoSubtitles = true;
	m_SubtitlesSpeed = 70;
	m_SystemFont = NULL;
	m_VideoFont = NULL;
	m_Cursor = NULL;
	m_ActiveCursor = NULL;
	m_NoninteractiveCursor = NULL;

	m_ThumbnailWidth = m_ThumbnailHeight = 0;

	m_IndicatorX = m_IndicatorY = m_IndicatorWidth = -1;
	m_IndicatorHeight = 8;
	m_IndicatorColor = DRGBA(255, 0, 0, 128);

	m_TalkSkipButton = TALK_SKIP_LEFT;

	m_DebugStartupScene = NULL;
	m_StartupScene = NULL;

	m_LoadImageName = NULL;
	m_SaveImageName = NULL;

	m_SaveImageX = m_SaveImageY = 0;
	m_LoadImageX = m_LoadImageY = 0;

	m_LocalSaveDir = NULL;
	CBUtils::SetString(&m_LocalSaveDir, "saves");
	
	m_CompatKillMethodThreads = false;

	SetRectEmpty(&m_SceneViewport);
}


//////////////////////////////////////////////////////////////////////////
CEdGame::~CEdGame()
{
	SAFE_DELETE_ARRAY(m_ResponseFile);
	SAFE_DELETE_ARRAY(m_InventoryFile);
	SAFE_DELETE_ARRAY(m_ItemsFile);

	SAFE_DELETE_ARRAY(m_SystemFont);
	SAFE_DELETE_ARRAY(m_VideoFont);
	SAFE_DELETE_ARRAY(m_Cursor);
	SAFE_DELETE_ARRAY(m_ActiveCursor);
	SAFE_DELETE_ARRAY(m_NoninteractiveCursor);
	
	SAFE_DELETE_ARRAY(m_LocalSaveDir);

	SAFE_DELETE_ARRAY(m_DebugStartupScene);
	SAFE_DELETE_ARRAY(m_StartupScene);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CEdGame::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CEdGame::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing GAME file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (GAME)
	TOKEN_DEF (AD_GAME)
	TOKEN_DEF (RESPONSE_BOX)
	TOKEN_DEF (INVENTORY_BOX)
	TOKEN_DEF (ITEMS)
	TOKEN_DEF (ITEM)

	TOKEN_DEF (NAME)
	TOKEN_DEF (SYSTEM_FONT)
	TOKEN_DEF (VIDEO_FONT)
	TOKEN_DEF (EVENTS)
	TOKEN_DEF (CURSOR)
	TOKEN_DEF (ACTIVE_CURSOR)
	TOKEN_DEF (NONINTERACTIVE_CURSOR)
	TOKEN_DEF (PERSONAL_SAVEGAMES)
	TOKEN_DEF (SCRIPT)
	TOKEN_DEF (CAPTION)
	TOKEN_DEF (PROPERTY)
	TOKEN_DEF (SUBTITLES_SPEED)
	TOKEN_DEF (SUBTITLES)
	TOKEN_DEF (VIDEO_SUBTITLES)
	TOKEN_DEF (TALK_SKIP_BUTTON)
	TOKEN_DEF (SCENE_VIEWPORT)
	TOKEN_DEF (DEBUG_STARTUP_SCENE)
	TOKEN_DEF (STARTUP_SCENE)
	TOKEN_DEF (THUMBNAIL_WIDTH)
	TOKEN_DEF (THUMBNAIL_HEIGHT)
	TOKEN_DEF (INDICATOR_X)
	TOKEN_DEF (INDICATOR_Y)
	TOKEN_DEF (INDICATOR_WIDTH)
	TOKEN_DEF (INDICATOR_HEIGHT)
	TOKEN_DEF (INDICATOR_COLOR)
	TOKEN_DEF (SAVE_IMAGE_X)
	TOKEN_DEF (SAVE_IMAGE_Y)
	TOKEN_DEF (SAVE_IMAGE)
	TOKEN_DEF (LOAD_IMAGE_X)
	TOKEN_DEF (LOAD_IMAGE_Y)
	TOKEN_DEF (LOAD_IMAGE)
	TOKEN_DEF (LOCAL_SAVE_DIR)
	TOKEN_DEF (COMPAT_KILL_METHOD_THREADS)
TOKEN_DEF_END


//////////////////////////////////////////////////////////////////////////
HRESULT CEdGame::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (GAME)
		TOKEN_TABLE (AD_GAME)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_GAME:
				if(FAILED(LoadBufferGame(params, false))) cmd = PARSERR_GENERIC;
			break;
			
			case TOKEN_AD_GAME:
				if(FAILED(LoadBufferAdGame(params, false))) cmd = PARSERR_GENERIC;
			break;
		}
	}

	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in GAME definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading GAME definition");
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CEdGame::LoadBufferGame(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (GAME)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (SYSTEM_FONT)
		TOKEN_TABLE (VIDEO_FONT)
		TOKEN_TABLE (EVENTS)
		TOKEN_TABLE (CURSOR)
		TOKEN_TABLE (ACTIVE_CURSOR)
		TOKEN_TABLE (NONINTERACTIVE_CURSOR)
		TOKEN_TABLE (PERSONAL_SAVEGAMES)
		TOKEN_TABLE (SCRIPT)
		TOKEN_TABLE (CAPTION)
		TOKEN_TABLE (PROPERTY)
		TOKEN_TABLE (SUBTITLES_SPEED)
		TOKEN_TABLE (SUBTITLES)
		TOKEN_TABLE (VIDEO_SUBTITLES)		
		TOKEN_TABLE (THUMBNAIL_WIDTH)
		TOKEN_TABLE (THUMBNAIL_HEIGHT)
		TOKEN_TABLE (INDICATOR_X)
		TOKEN_TABLE (INDICATOR_Y)
		TOKEN_TABLE (INDICATOR_WIDTH)
		TOKEN_TABLE (INDICATOR_HEIGHT)
		TOKEN_TABLE (INDICATOR_COLOR)
		TOKEN_TABLE (SAVE_IMAGE_X)
		TOKEN_TABLE (SAVE_IMAGE_Y)
		TOKEN_TABLE (SAVE_IMAGE)
		TOKEN_TABLE (LOAD_IMAGE_X)
		TOKEN_TABLE (LOAD_IMAGE_Y)
		TOKEN_TABLE (LOAD_IMAGE)
		TOKEN_TABLE (LOCAL_SAVE_DIR)
		TOKEN_TABLE (COMPAT_KILL_METHOD_THREADS)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_GAME){
			Game->LOG(0, "'GAME' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_NAME:
				Game->SetName((char*)params);
			break;

			case TOKEN_CAPTION:
				Game->SetCaption((char*)params);
			break;
			
			case TOKEN_SCRIPT:
				Game->AddScript((char*)params);
			break;		

			case TOKEN_PERSONAL_SAVEGAMES:
				parser.ScanStr((char*)params, "%b", &m_PersonalizedSave);
			break;

			case TOKEN_SUBTITLES:
				parser.ScanStr((char*)params, "%b", &m_Subtitles);
			break;

			case TOKEN_SUBTITLES_SPEED:
				parser.ScanStr((char*)params, "%d", &m_SubtitlesSpeed);
			break;

			case TOKEN_VIDEO_SUBTITLES:
				parser.ScanStr((char*)params, "%b", &m_VideoSubtitles);
			break;

			case TOKEN_PROPERTY:
				Game->ParseProperty(params, false);
			break;

			case TOKEN_SYSTEM_FONT:
				SAFE_DELETE_ARRAY(m_SystemFont);
				m_SystemFont = new char[strlen((char*)params)+1];
				strcpy(m_SystemFont, (char*)params);
			break;

			case TOKEN_VIDEO_FONT:
				SAFE_DELETE_ARRAY(m_VideoFont);
				m_VideoFont = new char[strlen((char*)params)+1];
				strcpy(m_VideoFont, (char*)params);
			break;

			case TOKEN_CURSOR:
				SAFE_DELETE_ARRAY(m_Cursor);
				m_Cursor = new char[strlen((char*)params)+1];
				strcpy(m_Cursor, (char*)params);
			break;

			case TOKEN_ACTIVE_CURSOR:
				SAFE_DELETE_ARRAY(m_ActiveCursor);
				m_ActiveCursor = new char[strlen((char*)params)+1];
				strcpy(m_ActiveCursor, (char*)params);
			break;

			case TOKEN_NONINTERACTIVE_CURSOR:
				SAFE_DELETE_ARRAY(m_NoninteractiveCursor);
				m_NoninteractiveCursor = new char[strlen((char*)params)+1];
				strcpy(m_NoninteractiveCursor, (char*)params);
			break;

			case TOKEN_THUMBNAIL_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_ThumbnailWidth);
			break;

			case TOKEN_THUMBNAIL_HEIGHT:
				parser.ScanStr((char*)params, "%d", &m_ThumbnailHeight);
			break;
		
			case TOKEN_INDICATOR_X:
				parser.ScanStr((char*)params, "%d", &m_IndicatorX);
			break;

			case TOKEN_INDICATOR_Y:
				parser.ScanStr((char*)params, "%d", &m_IndicatorY);
			break;

			case TOKEN_INDICATOR_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_IndicatorWidth);
			break;

			case TOKEN_INDICATOR_HEIGHT:
				parser.ScanStr((char*)params, "%d", &m_IndicatorHeight);
			break;
			
			case TOKEN_INDICATOR_COLOR:
			{
				int r, g, b, a;
				parser.ScanStr((char*)params, "%d,%d,%d,%d", &r, &g, &b, &a);
				m_IndicatorColor = DRGBA(r, g, b, a);
			}
			break;

			case TOKEN_SAVE_IMAGE:
				CBUtils::SetString(&m_SaveImageName, (char*)params);
			break;

			case TOKEN_LOCAL_SAVE_DIR:
				CBUtils::SetString(&m_LocalSaveDir, (char*)params);
			break;

			case TOKEN_SAVE_IMAGE_X:
				parser.ScanStr((char*)params, "%d", &m_SaveImageX);
			break;

			case TOKEN_SAVE_IMAGE_Y:
				parser.ScanStr((char*)params, "%d", &m_SaveImageY);
			break;

			case TOKEN_LOAD_IMAGE:
				CBUtils::SetString(&m_LoadImageName, (char*)params);
			break;

			case TOKEN_LOAD_IMAGE_X:
				parser.ScanStr((char*)params, "%d", &m_LoadImageX);
			break;

			case TOKEN_LOAD_IMAGE_Y:
				parser.ScanStr((char*)params, "%d", &m_LoadImageY);
			break;

			case TOKEN_COMPAT_KILL_METHOD_THREADS:
				parser.ScanStr((char*)params, "%b", &m_CompatKillMethodThreads);
			break;
		}
	}

	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in GAME section");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading GAME section");
		return E_FAIL;
	}

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CEdGame::LoadBufferAdGame(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (AD_GAME)
		TOKEN_TABLE (RESPONSE_BOX)
		TOKEN_TABLE (INVENTORY_BOX)
		TOKEN_TABLE (ITEMS)
		TOKEN_TABLE (ITEM)
		TOKEN_TABLE (TALK_SKIP_BUTTON)
		TOKEN_TABLE (SCENE_VIEWPORT)
		TOKEN_TABLE (DEBUG_STARTUP_SCENE)
		TOKEN_TABLE (STARTUP_SCENE)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_AD_GAME){
			Game->LOG(0, "'AD_GAME' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_ITEM:
				// TODO: ignore for now				
			break;

			case TOKEN_RESPONSE_BOX:
				SAFE_DELETE_ARRAY(m_ResponseFile);
				m_ResponseFile = new char[strlen((char*)params)+1];
				strcpy(m_ResponseFile, (char*)params);
			break;

			case TOKEN_INVENTORY_BOX:
				SAFE_DELETE_ARRAY(m_InventoryFile);
				m_InventoryFile = new char[strlen((char*)params)+1];
				strcpy(m_InventoryFile, (char*)params);
			break;

			case TOKEN_ITEMS:
				SAFE_DELETE_ARRAY(m_ItemsFile);
				m_ItemsFile = new char[strlen((char*)params)+1];
				strcpy(m_ItemsFile, (char*)params);
			break;

			case TOKEN_TALK_SKIP_BUTTON:
				if(stricmp((char*)params, "right")==0) m_TalkSkipButton = TALK_SKIP_RIGHT;
				else if(stricmp((char*)params, "both")==0) m_TalkSkipButton = TALK_SKIP_BOTH;
				else m_TalkSkipButton = TALK_SKIP_LEFT;
			break;

			case TOKEN_SCENE_VIEWPORT:
				parser.ScanStr((char*)params, "%d,%d,%d,%d", &m_SceneViewport.left, &m_SceneViewport.top, &m_SceneViewport.right, &m_SceneViewport.bottom);
			break;

			case TOKEN_DEBUG_STARTUP_SCENE:
				CBUtils::SetString(&m_DebugStartupScene, (char*)params);
			break;

			case TOKEN_STARTUP_SCENE:
				CBUtils::SetString(&m_StartupScene, (char*)params);
			break;
		}
	}

	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in AD_GAME section");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading AD_GAME section");
		return E_FAIL;
	}

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CEdGame::SaveAsText(CBDynBuffer *Buffer, int Indent)
{
	Buffer->PutText("; generated by ProjectMan\n\n");
	Buffer->PutTextIndent(Indent, "GAME {\n");
	
	if(Game->m_Name)
		Buffer->PutTextIndent(Indent+2, "NAME=\"%s\"\n", Game->m_Name);
	if(Game->m_Caption[0])
		Buffer->PutTextIndent(Indent+2, "CAPTION=\"%s\"\n", Game->m_Caption[0]);
	
    // scripts
	for(int i=0; i<Game->m_Scripts.GetSize(); i++){
		Buffer->PutTextIndent(Indent+2, "SCRIPT=\"%s\"\n", Game->m_Scripts[i]->m_Filename);
	}

	Buffer->PutTextIndent(Indent+2, "PERSONAL_SAVEGAMES=%s\n", m_PersonalizedSave?"TRUE":"FALSE");
	if(m_ThumbnailWidth>0 && m_ThumbnailHeight>0){
		Buffer->PutTextIndent(Indent+2, "THUMBNAIL_WIDTH=%d\n", m_ThumbnailWidth);
		Buffer->PutTextIndent(Indent+2, "THUMBNAIL_HEIGHT=%d\n", m_ThumbnailHeight);
	}
	if(m_LocalSaveDir) Buffer->PutTextIndent(Indent+2, "LOCAL_SAVE_DIR=\"%s\"\n", m_LocalSaveDir);

	if(m_IndicatorX>=0) Buffer->PutTextIndent(Indent+2, "INDICATOR_X=%d\n", m_IndicatorX);
	if(m_IndicatorY>=0) Buffer->PutTextIndent(Indent+2, "INDICATOR_Y=%d\n", m_IndicatorY);
	if(m_IndicatorWidth>=0) Buffer->PutTextIndent(Indent+2, "INDICATOR_WIDTH=%d\n", m_IndicatorWidth);
	Buffer->PutTextIndent(Indent+2, "INDICATOR_HEIGHT=%d\n", m_IndicatorHeight);
	Buffer->PutTextIndent(Indent+2, "INDICATOR_COLOR { %d, %d, %d, %d }\n", D3DCOLGetR(m_IndicatorColor), D3DCOLGetG(m_IndicatorColor), D3DCOLGetB(m_IndicatorColor), D3DCOLGetA(m_IndicatorColor));

	if(m_SaveImageName) Buffer->PutTextIndent(Indent+2, "SAVE_IMAGE=\"%s\"\n", m_SaveImageName);
	if(m_SaveImageX!=0) Buffer->PutTextIndent(Indent+2, "SAVE_IMAGE_X=%d\n", m_SaveImageX);
	if(m_SaveImageY!=0) Buffer->PutTextIndent(Indent+2, "SAVE_IMAGE_Y=%d\n", m_SaveImageY);

	if(m_LoadImageName) Buffer->PutTextIndent(Indent+2, "LOAD_IMAGE=\"%s\"\n", m_LoadImageName);
	if(m_LoadImageX!=0) Buffer->PutTextIndent(Indent+2, "LOAD_IMAGE_X=%d\n", m_LoadImageX);
	if(m_LoadImageY!=0) Buffer->PutTextIndent(Indent+2, "LOAD_IMAGE_Y=%d\n", m_LoadImageY);


	// properties
	if(Game->m_ScProp) Game->m_ScProp->SaveAsText(Buffer, Indent+2);

	if(m_SystemFont)
		Buffer->PutTextIndent(Indent+2, "SYSTEM_FONT=\"%s\"\n", m_SystemFont);

	if(m_VideoFont)
		Buffer->PutTextIndent(Indent+2, "VIDEO_FONT=\"%s\"\n", m_VideoFont);

	if(m_Cursor)
		Buffer->PutTextIndent(Indent+2, "CURSOR=\"%s\"\n", m_Cursor);

	if(m_ActiveCursor)
		Buffer->PutTextIndent(Indent+2, "ACTIVE_CURSOR=\"%s\"\n", m_ActiveCursor);

	if(m_NoninteractiveCursor)
		Buffer->PutTextIndent(Indent+2, "NONINTERACTIVE_CURSOR=\"%s\"\n", m_NoninteractiveCursor);

	Buffer->PutTextIndent(Indent+2, "SUBTITLES=%s\n", m_Subtitles?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "VIDEO_SUBTITLES=%s\n", m_VideoSubtitles?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "SUBTITLES_SPEED=%d\n", m_SubtitlesSpeed);

	if(m_CompatKillMethodThreads)
	{
		Buffer->PutTextIndent(Indent+2, "; compatibility bits\n");
		Buffer->PutTextIndent(Indent+2, "COMPAT_KILL_METHOD_THREADS=TRUE\n");
	}

	Buffer->PutTextIndent(Indent, "}\n\n");


	
	Buffer->PutTextIndent(Indent, "AD_GAME {\n");

	if(m_ResponseFile)
		Buffer->PutTextIndent(Indent+2, "RESPONSE_BOX=\"%s\"\n", m_ResponseFile);

	if(m_InventoryFile)
		Buffer->PutTextIndent(Indent+2, "INVENTORY_BOX=\"%s\"\n", m_InventoryFile);

	if(m_ItemsFile)
		Buffer->PutTextIndent(Indent+2, "ITEMS=\"%s\"\n", m_ItemsFile);

	if(m_TalkSkipButton==TALK_SKIP_RIGHT)
		Buffer->PutTextIndent(Indent+2, "TALK_SKIP_BUTTON=\"right\"\n");
	else if(m_TalkSkipButton==TALK_SKIP_BOTH)
		Buffer->PutTextIndent(Indent+2, "TALK_SKIP_BUTTON=\"both\"\n");
	else
		Buffer->PutTextIndent(Indent+2, "TALK_SKIP_BUTTON=\"left\"\n");

	if(!IsRectEmpty(&m_SceneViewport)){
		Buffer->PutTextIndent(Indent+2, "SCENE_VIEWPORT { %d, %d, %d, %d }\n", m_SceneViewport.left, m_SceneViewport.top, m_SceneViewport.right, m_SceneViewport.bottom);
	}

	if(m_StartupScene)
		Buffer->PutTextIndent(Indent+2, "STARTUP_SCENE=\"%s\"\n", m_StartupScene);

	if(m_DebugStartupScene)
		Buffer->PutTextIndent(Indent+2, "DEBUG_STARTUP_SCENE=\"%s\"\n", m_DebugStartupScene);


	Buffer->PutTextIndent(Indent, "}\n\n");

	return S_OK;
}
