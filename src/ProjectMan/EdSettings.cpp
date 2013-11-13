// EdSettings.cpp: implementation of the CEdSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "EdSettings.h"

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
CEdSettings::CEdSettings(CBGame* inGame):CBObject(inGame)
{
	m_EdGame = NULL;
	m_StringTableFilename = NULL;
	m_ResolutionWidth = m_ResolutionHeight = 0;
	m_RequireAcceleration = false;
	m_RequireSound = false;
	m_AllowWindowed = true;
	m_AllowDesktopRes = false;
	m_AllowAdvanced = false;
	m_AllowAccessTab = true;
	m_AllowAboutTab = true;
	m_TLMode = 0;
	m_RegPath = NULL;
	
	m_RichSavedGames = false;
	m_SavedGameExt = NULL;
	CBUtils::SetString(&m_SavedGameExt, "dsv");
	m_GameGUID = GUID_NULL;
}


//////////////////////////////////////////////////////////////////////////
CEdSettings::~CEdSettings()
{
	SAFE_DELETE(m_EdGame);
	SAFE_DELETE_ARRAY(m_StringTableFilename);
	SAFE_DELETE_ARRAY(m_RegPath);
	SAFE_DELETE_ARRAY(m_SavedGameExt);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CEdSettings::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CEdSettings::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing SETTINGS file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (SETTINGS)
	TOKEN_DEF (GAME)
	TOKEN_DEF (STRING_TABLE)
	TOKEN_DEF (RESOLUTION)
	TOKEN_DEF (REQUIRE_3D_ACCELERATION)
	TOKEN_DEF (REQUIRE_SOUND)
	TOKEN_DEF (ALLOW_WINDOWED_MODE)
	TOKEN_DEF (ALLOW_ADVANCED)
	TOKEN_DEF (ALLOW_ACCESSIBILITY_TAB)
	TOKEN_DEF (ALLOW_ABOUT_TAB)
	TOKEN_DEF (ALLOW_DESKTOP_RES)
	TOKEN_DEF (REGISTRY_PATH)
	TOKEN_DEF (HWTL_MODE)
	TOKEN_DEF (RICH_SAVED_GAMES)
	TOKEN_DEF (SAVED_GAME_EXT)
	TOKEN_DEF (GUID)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CEdSettings::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (SETTINGS)
		TOKEN_TABLE (GAME)
		TOKEN_TABLE (STRING_TABLE)
		TOKEN_TABLE (RESOLUTION)
		TOKEN_TABLE (REQUIRE_3D_ACCELERATION)
		TOKEN_TABLE (REQUIRE_SOUND)
		TOKEN_TABLE (ALLOW_WINDOWED_MODE)
		TOKEN_TABLE (ALLOW_ADVANCED)
		TOKEN_TABLE (ALLOW_ACCESSIBILITY_TAB)
		TOKEN_TABLE (ALLOW_ABOUT_TAB)
		TOKEN_TABLE (ALLOW_DESKTOP_RES)
		TOKEN_TABLE (REGISTRY_PATH)
		TOKEN_TABLE (HWTL_MODE)
		TOKEN_TABLE (RICH_SAVED_GAMES)
		TOKEN_TABLE (SAVED_GAME_EXT)
		TOKEN_TABLE (GUID)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_SETTINGS){
			Game->LOG(0, "'SETTINGS' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_GAME:
				SAFE_DELETE(m_EdGame);
				m_EdGame = ::new CEdGame(Game);
				if(!m_EdGame || FAILED(m_EdGame->LoadFile((char*)params))){
					SAFE_DELETE(m_EdGame);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_STRING_TABLE:
				if(FAILED(Game->m_StringTable->LoadFile((char*)params))) cmd = PARSERR_GENERIC;
				else{
					SAFE_DELETE_ARRAY(m_StringTableFilename);
					m_StringTableFilename = new char[strlen((char*)params)+1];
					strcpy(m_StringTableFilename, (char*)params);
				}
			break;

			case TOKEN_RESOLUTION:
				parser.ScanStr((char*)params, "%d,%d", &m_ResolutionWidth, &m_ResolutionHeight);
			break;

			case TOKEN_REQUIRE_3D_ACCELERATION:
				parser.ScanStr((char*)params, "%b", &m_RequireAcceleration);
			break;

			case TOKEN_REQUIRE_SOUND:
				parser.ScanStr((char*)params, "%b", &m_RequireSound);
			break;

			case TOKEN_ALLOW_WINDOWED_MODE:
				parser.ScanStr((char*)params, "%b", &m_AllowWindowed);
			break;

			case TOKEN_ALLOW_DESKTOP_RES:
				parser.ScanStr((char*)params, "%b", &m_AllowDesktopRes);
			break;

			case TOKEN_ALLOW_ADVANCED:
				parser.ScanStr((char*)params, "%b", &m_AllowAdvanced);
			break;

			case TOKEN_HWTL_MODE:
				parser.ScanStr((char*)params, "%d", &m_TLMode);
			break;

			case TOKEN_ALLOW_ACCESSIBILITY_TAB:
				parser.ScanStr((char*)params, "%b", &m_AllowAccessTab);
			break;

			case TOKEN_ALLOW_ABOUT_TAB:
				parser.ScanStr((char*)params, "%b", &m_AllowAboutTab);
			break;

			case TOKEN_REGISTRY_PATH:
				SAFE_DELETE_ARRAY(m_RegPath);
				m_RegPath = new char[strlen((char*)params)+1];
				strcpy(m_RegPath, (char*)params);
			break;

			case TOKEN_RICH_SAVED_GAMES:
				parser.ScanStr((char*)params, "%b", &m_RichSavedGames);
			break;

			case TOKEN_SAVED_GAME_EXT:
				SAFE_DELETE_ARRAY(m_SavedGameExt);
				CBUtils::SetString(&m_SavedGameExt, (char*)params);
			break;

			case TOKEN_GUID:
			{
				OLECHAR olestr[50];
				mbstowcs(olestr, (char*)params, 50);
				IIDFromString(olestr, &m_GameGUID);
			}
			break;
		}
	}

	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in SETTINGS definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading SETTINGS definition");
		return E_FAIL;
	}

	// get a new guid if none is set yet
	if(m_GameGUID==GUID_NULL) CoCreateGuid(&m_GameGUID);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CEdSettings::SaveAsText(CBDynBuffer *Buffer, int Indent)
{
	Buffer->PutText("; generated by ProjectMan\n\n");
	Buffer->PutTextIndent(Indent, "SETTINGS {\n");

	if(m_EdGame && m_EdGame->m_Filename)
		Buffer->PutTextIndent(Indent+2, "GAME=\"%s\"\n", m_EdGame->m_Filename);

	if(m_GameGUID != GUID_NULL)
	{
		LPOLESTR  olestr;
		char str[50];
		StringFromIID(m_GameGUID, &olestr);
		wcstombs(str, olestr, 50);
		CoTaskMemFree(olestr);

		Buffer->PutTextIndent(Indent+2, "GUID=\"%s\"\n", str);
	}
	
	if(m_StringTableFilename)
		Buffer->PutTextIndent(Indent+2, "STRING_TABLE=\"%s\"\n", m_StringTableFilename);

	Buffer->PutTextIndent(Indent+2, "RESOLUTION { %d,%d }\n", m_ResolutionWidth, m_ResolutionHeight);
	//Buffer->PutTextIndent(Indent+2, "REQUIRE_3D_ACCELERATION=%s\n", m_RequireAcceleration?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "REQUIRE_3D_ACCELERATION=TRUE\n");
	if(m_RequireSound)
		Buffer->PutTextIndent(Indent+2, "REQUIRE_SOUND=%s\n", m_RequireSound?"TRUE":"FALSE");

	if(m_TLMode > 0)
		Buffer->PutTextIndent(Indent+2, "HWTL_MODE=%d\n", m_TLMode);

	Buffer->PutTextIndent(Indent+2, "ALLOW_WINDOWED_MODE=%s\n", m_AllowWindowed?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "ALLOW_ADVANCED=%s\n", m_AllowAdvanced?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "ALLOW_ACCESSIBILITY_TAB=%s\n", m_AllowAccessTab?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "ALLOW_ABOUT_TAB=%s\n", m_AllowAboutTab?"TRUE":"FALSE");

	if(m_AllowDesktopRes)
		Buffer->PutTextIndent(Indent+2, "ALLOW_DESKTOP_RES=%s\n", m_AllowDesktopRes?"TRUE":"FALSE");
	
	if(m_RegPath)
		Buffer->PutTextIndent(Indent+2, "REGISTRY_PATH=\"%s\"\n", m_RegPath);

	if(m_RichSavedGames)
		Buffer->PutTextIndent(Indent+2, "RICH_SAVED_GAMES=%s\n", m_RichSavedGames?"TRUE":"FALSE");

	if(m_SavedGameExt && stricmp(m_SavedGameExt, "dsv") != 0)
		Buffer->PutTextIndent(Indent+2, "SAVED_GAME_EXT=\"%s\"\n", m_SavedGameExt);

	Buffer->PutTextIndent(Indent, "}\n\n");


	// save game
	char GameFullFilename[MAX_PATH];
	Game->m_FileManager->GetFullPath(m_EdGame->m_Filename, GameFullFilename);

	CBDynBuffer* buf = ::new CBDynBuffer(Game);
	m_EdGame->SaveAsText(buf, 0);

	FILE* f = fopen(GameFullFilename, "wt");
	if(!f){
		delete buf;
		return E_FAIL;
	}
		
	fwrite(buf->m_Buffer, buf->GetSize(), 1, f);
	delete buf;
	fclose(f);

	return S_OK;
}
