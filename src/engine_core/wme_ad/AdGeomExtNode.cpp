// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include ".\AdGeomExtNode.h"


//////////////////////////////////////////////////////////////////////////
CAdGeomExtNode::CAdGeomExtNode(CBGame* inGame) : CBBase(inGame)
{
	m_NamePattern = NULL;
	m_ReceiveShadows = false;
	m_Type = GEOM_GENERIC;
}

//////////////////////////////////////////////////////////////////////////
CAdGeomExtNode::~CAdGeomExtNode(void)
{
	SAFE_DELETE_ARRAY(m_NamePattern);
}


TOKEN_DEF_START
	TOKEN_DEF (NODE)
	TOKEN_DEF (NAME)
	TOKEN_DEF (WALKPLANE)
	TOKEN_DEF (BLOCKED)
	TOKEN_DEF (WAYPOINT)
	TOKEN_DEF (RECEIVE_SHADOWS)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdGeomExtNode::LoadBuffer(BYTE* Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (NODE)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (WALKPLANE)
		TOKEN_TABLE (BLOCKED)
		TOKEN_TABLE (WAYPOINT)
		TOKEN_TABLE (RECEIVE_SHADOWS)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd=2;
	CBParser parser(Game);

	if(Complete)
	{
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_NODE)
		{
			Game->LOG(0, "'NODE' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_NAME:
				CBUtils::SetString(&m_NamePattern, (char*)params);
			break;

			case TOKEN_RECEIVE_SHADOWS:
				parser.ScanStr((char*)params, "%b", &m_ReceiveShadows);
			break;

			case TOKEN_WALKPLANE:
			{
				bool IsWalkplane = false;
				parser.ScanStr((char*)params, "%b", &IsWalkplane);
				if(IsWalkplane) m_Type = GEOM_WALKPLANE;
				break;
			}

			case TOKEN_BLOCKED:
			{
				bool IsBlocked = false;
				parser.ScanStr((char*)params, "%b", &IsBlocked);
				if(IsBlocked) m_Type = GEOM_BLOCKED;
				break;
			}

			case TOKEN_WAYPOINT:
			{
				bool IsWaypoint = false;
				parser.ScanStr((char*)params, "%b", &IsWaypoint);
				if(IsWaypoint) m_Type = GEOM_WAYPOINT;
				break;
			}			
		}
	}

	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in geometry description file");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC)
	{
		Game->LOG(0, "Error loading geometry description");
		return E_FAIL;
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdGeomExtNode::SetupNode(char* NamePattern, TGeomNodeType Type, bool ReceiveShadows)
{
	CBUtils::SetString(&m_NamePattern, NamePattern);
	m_Type = Type;
	m_ReceiveShadows = ReceiveShadows;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CAdGeomExtNode::MatchesName(char* Name)
{
	return CBUtils::MatchesPattern(m_NamePattern, Name);
}