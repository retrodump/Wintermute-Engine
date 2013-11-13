// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include ".\AdGeomExt.h"


//////////////////////////////////////////////////////////////////////////
CAdGeomExt::CAdGeomExt(CBGame* inGame) : CBBase(inGame)
{
}

//////////////////////////////////////////////////////////////////////////
CAdGeomExt::~CAdGeomExt(void)
{
	for(int i=0; i<m_Nodes.GetSize(); i++)
	{
		SAFE_DELETE(m_Nodes[i]);
	}
	m_Nodes.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdGeomExt::LoadFile(char* Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL)
	{
		Game->LOG(0, "CAdGeomExt::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;
	if(FAILED(ret = LoadBuffer(Buffer))) Game->LOG(0, "Error parsing geometry description file '%s'", Filename);

	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (GEOMETRY)
	TOKEN_DEF (NODE)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdGeomExt::LoadBuffer(BYTE* Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (GEOMETRY)
		TOKEN_TABLE (NODE)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_GEOMETRY)
	{
		Game->LOG(0, "'GEOMETRY' keyword expected.");
		return E_FAIL;
	}
	Buffer = params;


	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_NODE:
			{
				CAdGeomExtNode* Node = new CAdGeomExtNode(Game);
				if(Node && !FAILED(Node->LoadBuffer(params, false)))
				{
					m_Nodes.Add(Node);
				}
				else
				{
					SAFE_DELETE(Node);
					cmd = PARSERR_GENERIC;
				}
			}
			break;
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

	AddStandardNodes();
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdGeomExt::AddStandardNodes()
{
	CAdGeomExtNode* Node;

	Node = new CAdGeomExtNode(Game);
	Node->SetupNode("walk_*", GEOM_WALKPLANE, true);
	m_Nodes.Add(Node);

	Node = new CAdGeomExtNode(Game);
	Node->SetupNode("blk_*", GEOM_BLOCKED, false);
	m_Nodes.Add(Node);

	Node = new CAdGeomExtNode(Game);
	Node->SetupNode("wpt_*", GEOM_WAYPOINT, false);
	m_Nodes.Add(Node);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CAdGeomExtNode* CAdGeomExt::MatchName(char* Name)
{
	if(!Name) return NULL;

	for(int i=0; i<m_Nodes.GetSize(); i++)
	{
		if(m_Nodes[i]->MatchesName(Name)) return m_Nodes[i];
	}
	return NULL;
}
